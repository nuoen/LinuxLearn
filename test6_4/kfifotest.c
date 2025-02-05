#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/kfifo.h>

static MODUEL_NAME = "kfifodemo";
DEFINE_KFIFO(mydemo_fifo, char, 64);



static struct mydemo_device{
    const char *name;
    struct device *dev;
    struct miscdevice *miscdev;
    wait_queue_head_t read_wait;
    wait_queue_head_t write_wait;
}

struct mydemo_private_data {
	struct mydemo_device *device;
};

static struct mydemo_device *mydemo_device_one; 

#define MAX_DEVICE_BUFFER_SIZE 64

static int dev_open(struct inode *inode, struct file *file){
    struct mydemo_private_data *my_priv_data;
    struct mydemo_device *mydev;
    printk("%s call\n",__func__);
    int major = MAJOR(inode->i_rdev);
    int minor = MINOR(inode->i_rdev);
    printk("%s: major=%d, minor=%d\n", __func__, major, minor);
    my_priv_data = kmalloc(sizeof(*my_priv_data), GFP_KERNEL);
    if (!my_priv_data){
        return -ENOMEM;
    }
    my_priv_data->device = mydemo_device_one;
    file->private_data = my_priv_data;
    return 0;
}


static ssize_t dev_read(struct file *file, char __user * user_buffer, size_t user_size, loff_t *op){
    int actual_readed;
    int ret;
    struct mydemo_private_data *my_priv_data = file->private_data;
    struct mydemo_device *mydev = my_priv_data->device;

    if(kfifo_is_empty(&mydemo_fifo)){
        if(file->f_flags & O_NONBLOCK){
            return -EAGAIN;
        }
        printk("%s: pid=%d,going to sleep\n",__func__,current->pid);
        ret = wait_event_interruptible(mydev->read_queue, !kfifo_is_empty(&mydemo_fifo));
        if (ret){
            return ret;
        }
    }

    ret = kfifo_to_user(&mydemo_fifo, user_buffer, user_size, &actual_readed);
    if (ret){
        return -EIO;
    }
    if(!kfifo_is_full(&mydemo_fifo)){
        wake_up_interruptible(&mydev->write_queue);
    }
    printk("%s, actual_readed=%d, pos=%lld\n",__func__, actual_readed, *op);
    return actual_readed;
}

static ssize_t dev_write(struct file *file, const char __user *user_buffer, size_t user_size, loff_t *op){
    int actual_written;
    int ret;
    struct mydemo_private_data *my_priv_data = file->private_data;
    struct mydemo_device *mydev = my_priv_data->device;
    if(kfifo_is_full(&mydemo_fifo)){
        if(file->f_flags & O_NONBLOCK){
            return -EAGAIN;
        }
        printk("%s: pid=%d,going to sleep\n",__func__,current->pid);
        ret = wait_event_interruptible(mydev->write_queue, !kfifo_is_full(&mydemo_fifo));
        if (ret){
            return ret;
        }
    }
    ret = kfifo_from_user(&mydemo_fifo, user_buffer, user_size, &actual_written);
    if (ret){
        return -EIO;
    }
    if(!kfifo_is_empty(&mydemo_fifo)){
        wake_up_interruptible(&mydev->read_queue);
    }
    printk("%s, actual_written=%d\n",__func__,actual_written);
    return actual_written;
}


static int dev_release(struct inode *inode, struct file *file){
    struct mydemo_private_data *my_priv_data = file->private_data;
    kfree(my_priv_data);
    printk("%s call",__func__);
    return 0;
}



static const struct file_operations rw_file_op={
    .owner = THIS_MODULE,
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release
};

static struct miscdevice misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = MODULE_NAME,
    .fops = &rw_file_op
};

static int __init simple_char_init(void)
{
	int ret;

	struct mydemo_device *my_demo_device = kmolloc(sizeof(struct mydemo_device),GFP_KERNEL);
	if (!my_demo_device)
		return -ENOMEM;

	ret = misc_register(&misc_device);
	if (ret) {
		printk("failed register misc device\n");
		kfree(my_demo_device);
		return ret;
	}

	my_demo_device->dev = misc_device.this_device;
    my_demo_device->miscdev = &misc_device;

    init_waitqueue_head(&my_demo_device->read_wait);
    init_waitqueue_head(&my_demo_device->write_wait);

    mydemo_device_one= my_demo_device;
	printk("succeeded register char device: %s\n", MODULE_NAME);

	return 0;
}

static void __exit simple_char_exit(void)
{
	printk("removing device\n");

	kfree(device_buffer);
	misc_deregister(&misc_device);
}

module_init(simple_char_init);
module_exit(simple_char_exit);

MODULE_AUTHOR("nuoen");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("simpe character device");