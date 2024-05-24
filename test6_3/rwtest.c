#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define MODULE_NAME "idemo"
#define MAX_BUFFER_SIZE 64


static char* device_buffer;

static struct device *my_device;

static int dev_open(struct inode *inode, struct file *file){
    int major = MAJOR(inode->i_rdev);
    int minor = MINOR(inode->i_rdev);
    printk("%s: major=%d, minor=%d\n", __func__, major, minor);
    return 0;
}

static ssize_t dev_read(struct file *file, char * user_buffer, size_t user_size, loff_t *op){
    int max_buffer;
    int need_read;
    int ret;
    int actual_readed;
    printk("%s call",__func__);
    max_buffer = MAX_BUFFER_SIZE - *op;
    need_read = max_buffer > user_size ?user_size:max_buffer;
    if(need_read == 0){
        dev_warn(my_device,"no space to read");
    }
    ret = copy_to_user(user_buffer,device_buffer+*op,need_read);
    if(ret == need_read){
        return -EFAULT;
    }
    actual_readed = need_read-ret;
    *op +=actual_readed;
    printk("%s,actual_readed =%d,pos=%lld\n",__func__,actual_readed,*op);
    return actual_readed;
}

static ssize_t dev_write(struct file *file, const char __user *user_buffer, size_t user_size, loff_t *op){
    int max_buffer;
    int need_write;
    int ret;
    int actual_writed;
    max_buffer = MAX_BUFFER_SIZE - *op;
    need_write = user_size > max_buffer ? max_buffer:user_size;
    if(need_write == 0){
        dev_warn(my_device,"no space to write");
    }
    ret = copy_from_user(device_buffer+*op,user_buffer,need_write);
    if(ret == need_write){
        return -EFAULT;
    }
    actual_writed = need_write -ret;
    *op = actual_writed;
    printk("%s,actual_writed =%d,pos=%lld\n",__func__,actual_writed,*op);
    return actual_writed;
}

static int dev_release(struct inode *inode, struct file *file){
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

	device_buffer = kmalloc(MAX_BUFFER_SIZE, GFP_KERNEL);
	if (!device_buffer)
		return -ENOMEM;

	ret = misc_register(&misc_device);
	if (ret) {
		printk("failed register misc device\n");
		kfree(device_buffer);
		return ret;
	}

	my_device = misc_device.this_device;

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