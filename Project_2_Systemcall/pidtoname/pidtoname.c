#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/string.h>
#include<linux/slab.h>

#define MAX_LEN 50
asmlinkage long sys_pidtoname(int pid, char* buf, int len){
    struct task_struct *task;

    char *process_name = kmalloc(MAX_LEN, GFP_KERNEL);

    for_each_process(task){
	if (task_pid_nr(task) == pid){
  		strcpy(process_name, task->comm);
		printk("%s\n", process_name);
		copy_to_user(buf, process_name, len - 1);
		if (strlen(process_name) > len - 1)
			return strlen(process_name);
		else 	return 0;
	}
    }
    return -1;
}

