#include <linux/miscdevice.h>
#include <linux/fs.h>


#define DEMO_NAME "my_misc_demo_dev"
static struct device *mydemodrv_device;


static int demodrv_open(struct inode *inode,struct file *file){
    int major = MAJOR(inode->i_rdev);
    int minor = MINOR(inode->i_rdev);
    printk("%s: major=%d, minor=%d\n", __func__, major, minor);
    return 0;
}
//	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
static ssize_t demodrv_read(struct file *file, char __user *buf, size_t lbuf, loff_t * ppos){
    printk("%s enter\n",__func__);
    return 0;
}
//int (*file_operations::release)(struct inode *, struct file *)
static int demodrv_release(struct inode * inode,struct file *file){
    printk("%s enter\n",__func__);
    return 0;
}
//ssize_t (*file_operations::write)(struct file *, const char *, size_t, loff_t *)
static ssize_t demodrv_write(struct file *file,const char *buf,size_t count,loff_t *f_pos){
    printk("%s enter\n",__func__);
    return 0;
}


static const struct file_operations demodrv_fops ={
    .owner = DEMO_NAME,
    .open = demodrv_open,
    .read =demodrv_read,
    .release = demodrv_release,
    .write = demodrv_write
};

static struct miscdevice miscdev ={
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEMO_NAME,
    .fops = &demodrv_fops
};


static int __init simple_char_init(void){
    int ret;
    printk("%s enter\n",__func__);
    ret = misc_register(&miscdev);
    if(ret){
        printk("failed register misc device\n");
        return ret;
    }
    mydemodrv_device = miscdev.this_device;
    printk("register misc device success\n");
    retrn 0;
}

static void __exit simple_char_exit(void){
    printk("removing device\n");
    misc_deregister(&miscdev);
}


module_init(simple_char_init);
module_exit(simple_char_exit);

MODULE_AUTHOR("nuoen");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("This is a demo for misc device");

