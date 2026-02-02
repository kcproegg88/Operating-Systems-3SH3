/**
 * Lab_1.c
 *
 * A simple kernel module. 
 * 
 * To compile, run makefile by entering "make"
 *
 * Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>

/* This function is called when the module is loaded. */
static int Lab_1_init(void)
{
       printk(KERN_INFO "Loading Lab_1 Module\n");
       struct task_struct *task = &init_task; // Get the current task (process)
       printk(KERN_INFO "Process List:\n");
       printk(KERN_INFO "PID: %d\n", task->pid);
       printk(KERN_INFO "State: %ld\n", task->__state);
       printk(KERN_INFO "Flags: %lu\n", task->flags);
       printk(KERN_INFO "rt_priority: %u\n", task->rt_priority);
       printk(KERN_INFO "policy: %u\n", task->policy);
       printk(KERN_INFO "tgid: %d\n", task->tgid);
       return 0;
}

/* This function is called when the module is removed. */
static void Lab_1_exit(void) {
	printk(KERN_INFO "Removing Lab_1 Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( Lab_1_init );
module_exit( Lab_1_exit );