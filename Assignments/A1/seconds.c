/**
 * seconds.c
 *
 * Kernel module that creates a /proc file that reports the number of elapsed seconds since the module was loaded
 *
 * Kernel version: 6.17.0-8-generic *KRISHNA CHECK ITS THE SAME FOR YOURS* *Krishna- IT IS FOR MINE*
 * Ubuntu version: 25.10
 * 
 * Fiona Douglas, January 28th, 2026
 * Krishna Chauhan
 * SFWRENG 3SH3, Winter 2026
 
 * Reference:  hello.c Operating System Concepts - 10th Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128

#define PROC_NAME "seconds"
static unsigned long start_jiffies;


/**
 * Function prototypes
 */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);
int proc_init(void);
void proc_exit(void);


static const struct proc_ops my_proc_ops = {
        // Defines the operations that can be performed on a /proc file
        .proc_read = proc_read, // replaces the builtin .proc_read method with the proc_read function defined in this file
};

/* This function is called when the module is loaded. */
int proc_init(void)
{

        // creates the /proc/hello entry
        // the following function call is a wrapper for
        // proc_create_data() passing NULL as the last argument
        proc_create(PROC_NAME, 0, NULL, &my_proc_ops);
        start_jiffies = jiffies;
        // Prints name of /proc file when loaded
        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

/* This function is called when the module is removed. */
void proc_exit(void) {

        // removes the /proc/seconds entry
        remove_proc_entry(PROC_NAME, NULL);

        printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

/**
 * This function is called each time the /proc/seconds is read.
 * 
 * This function is called repeatedly until it returns 0, so
 * there must be logic that ensures it ultimately returns 0
 * once it has collected the data that is to go into the 
 * corresponding /proc file.
 *
 * params:
 *
 * file:
 * buf: buffer in user space
 * count:
 * pos:
 */
 
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;

        if (completed) {
                completed = 0;
                return 0;
        }

        completed = 1;


        rv = sprintf(buffer, "%lu\n", (jiffies - start_jiffies) / HZ);

        // copies the contents of buffer to userspace usr_buf
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("Fiona Douglas and Krishna Chauhan");
