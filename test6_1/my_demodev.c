#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>


#define DEMO_NAME "my_demo_dev"

static dev_t dev;
static struct cdev *demo_cdev;
static signed count =1;


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
static ssize_t demodrv_write(struct file *file,const char __usr *buf,size_t count,loff_t *f_pos){
    printk("%s enter\n",__func__);
}


static const struct file_operations demodrc_fops ={
    .owner = THIS_MODULE,
    .open = demodrv_open,
    .read =demodrv_read,
    .release = demodrv_release,
    .write = demodrv_write
}


static void __init simple_char_init(void){
    int ret;
    ret = alloc_chrdev_region(&dev,0,count,DEMO_NAME);
    if(ret){
        printk("failed to allocate char device region");
        return ret;
    }
}

static void __exit simple_char_exit(void){

}



module_init();
module_exit();

MODULE_AUTHOR("nuoen");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("simple character device");