#include <linux/init.h>
#include <linux/module.h>

extern int testa;
extern void test(int);
static int debug = 1;

static int input =200;
module_param(input,int,0644);
MODULE_PARM_DESC(input,"intput num");
#define print(args...) if(debug) {printk(KERN_DEBUG args);}


static int __init my_init(void){
	print("init import module\n");
            test(input);
	return 0;
}

static void __exit my_exit(void){
	print("exit import module\n");
}


module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rlk");
MODULE_DESCRIPTION("my import test kernel module");
MODULE_ALIAS("importtest");


