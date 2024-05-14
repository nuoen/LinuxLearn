#include <linux/init.h>
#include <linux/module.h>

int testa = 20;
static int debug = 1;
#define print(args...) if(debug) {printk(KERN_DEBUG args);}

static int __init my_init(void){
	print("init export module\n");
	return 0;
}

static void __exit my_exit(void){
	print("exit export module\n");
}

static void test(int num){
	print("the testa is %d\n",testa);
	testa=num;
	print("change testa is %d\n",testa);
}

module_init(my_init);
module_exit(my_exit);
EXPORT_SYMBOL(testa);
EXPORT_SYMBOL(test);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("rlk");
MODULE_DESCRIPTION("my export test kernel module");
MODULE_ALIAS("exporttest");


