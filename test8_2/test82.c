#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>

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
static struct task_struct *tsk[NR_CPUS];

static void show_reg(void){
    unsigned int spsr,sp,el;
    asm("mrs %0,spsr_el1":"=r"(spsr) : : "cc");
    asm("mov %0,sp":"=r"(sp)::"cc");
    asm("mrs %0,CurrentEL":"=r"(el)::"cc");
    printk("spsr:0x%x, sp:0x%x,el=%d\n",spsr,sp,el>>2);
}



static void show_prio(void){
    struct task_struct *task = current;
    printk("%s pid:%d, nice: %d prio:%d static_prio:%d normal_prio:%d\n",
        task->comm,task->pid,PRIO_TO_NICE(task->static_prio),
        task->prio,task->static_prio,task->normal_prio);
}

static void print_cpu(char *s){
    preempt_disable();
    pr_info("%s cpu=%d.\n",s,smp_processor_id());
    preempt_enable();
} 

static int thread_fun(void *t){
    int time =3;
    do{
       print_cpu("SLEEP in Thread Function ");
       msleep_interruptible(2000);
       print_cpu("msleep over in Thread Function");
       print_cpu("running time");
       show_reg();
       show_prio();
       time--;
    }while(!kthread_should_stop() && time>0);
    return 0;
}

static int __init my_test_init(void){
    int i;
	printk("my first kernel module init \n");
	dprintk("debug my first kernel module init \n");
	dprintk("module parameter=%d\n",mytest);
    print_cpu("Loading module");
    for_each_online_cpu(i){
        tsk[i] = kthread_create(thread_fun,NULL,"kdemo/%d",i);
        if(!tsk[i]){
            pr_info("Failed to generate a kernel thread\n");
            return -1;
        }
        kthread_bind(tsk[i],i);
        pr_info("About to wake up and run the thread for cpu=%d\n",i);
        wake_up_process(tsk[i]);
        pr_info("Staring thread for cpu %d",i);
        print_cpu("on");
    } 
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


