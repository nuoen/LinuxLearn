#include <linux/init.h>
#include <linux/module.h>

static int debug =1;
module_param(debug,int,0644);
MODULE_PARM_DESC(debug,"enable debugging information");
#define dprintk(args...) \
	if(debug){ \
		printk(KERN_DEBUG args);\
	}

static int mytest =100;
module_param(mytest,int,0644);
MODULE_PARM_DESC(mytest,"test for module parameter");


static int __init my_test_init(void){
	printk("my first kernel module init \n");
	dprintk("debug my first kernel module init \n");
	dprintk("module parameter=%d\n",mytest);
	return 0;
}

static void __exit my_test_exit(void){
	printk("goodbye\n");

}

module_init(my_test_init);
module_exit(my_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rlk");
MODULE_DESCRIPTION("my test kernel module");
MODULE_ALIAS("mytest");


