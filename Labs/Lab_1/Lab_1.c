#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

static int print_init_pcb(void){
       struct task_struct *task = &init_task;
       
       printk(KERN_INFO "init_task fields:\n");
       printk(KERN_INFO "PID: %d\n", task->pid);
       printk(KERN_INFO "State: %u\n", task->__state);
       printk(KERN_INFO "Flags: %u\n", task->flags);
       printk(KERN_INFO "rt_priority: %d\n", task->rt_priority);
       printk(KERN_INFO "policy: %u\n", task->policy);
       printk(KERN_INFO "tgid: %d\n", task->tgid);
       
       return 0;
}

static int Lab_1_init(void)
{

    printk(KERN_INFO "Loading Lab_1 Module\n");
    print_init_pcb();

    return 0;
}

static void Lab_1_exit(void)
{
    printk(KERN_INFO "Removing Lab_1 Module\n");
}

module_init(Lab_1_init);
module_exit(Lab_1_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");