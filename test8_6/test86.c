#include <linux/module.h>
#include <linux/init.h>
#include <linux/percpu.h>
#include <linux/cpumask.h>


static DEFINE_PER_CPU(long,cpuvar) = 10;
static long __percpu *cpualloc;

static int __init my_init(void){
    int cpu;
    /**
     * 	获取当前正在执行这段代码的 CPU 编号，存放于 cur_cpu。
	•	raw_smp_processor_id() 相当于获取 “当前CPU ID”，但不做其他额外检查。
     */
    int cur_cpu = raw_smp_processor_id();
    pr_info("current cpu %d\n",cur_cpu);
    /**打印cpuvar在所有CPU上的初始值 */
    for_each_possible_cpu(cpu){
        pr_info("init:cpuvar on cpu%d = %1d\n",cpu,per_cpu(cpuvar,cpu));
    }

    //使用 __this_cpu_write 修改当前CPU上的cpuvar值为22
    __this_cpu_write(cpuvar,22);

    //使用 get_cpu_var(cpuvar) 来获取当前CPU上的cpuvar值
    pr_info("cpu%d has motify to %ld\n",cur_cpu,get_cpu_var(cpuvar));
    put_cpu_var(cpuvar);

	/* 分配一块新的per-CPU区域给cpualloc，每个CPU上都有一个long类型空间 */
    cpualloc = alloc_percpu(long);

    for_each_possible_cpu(cpu){
        *per_cpu_ptr(cpualloc,cpu) = 100;
        pr_info("init : cpu:%d cpualloc = %ld\n",cpu,*per_cpu_ptr(cpualloc,cpu));   
    }
    return 0;
}

static void __exit my_exit(void){
    int cpu;
    pr_info("exit module...\n");

    for_each_possible_cpu(cpu){
        pr_info("cpuvar cpu%d= %ld\n",cpu,per_cpu(cpuvar,cpu));
        pr_info("exit: cpualloc%d = %ld\n",cpu,*per_cpu_ptr(cpualloc,cpu));
    }

    free_percpu(cpualloc);
}


module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("nuoen");
MODULE_DESCRIPTION("my test kernel module");
MODULE_ALIAS("mytest");