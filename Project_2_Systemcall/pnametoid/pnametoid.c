#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/string.h>
//#include "pname.h"
asmlinkage long sys_pnametoid(char* process_name){
    struct task_struct *task;
    char name[32];

    for_each_process(task){
        /*compares the current process name (defined in task->comm) to the passed in name*/
        if(strcmp(task->comm,process_name) == 0){
            return task_pid_nr(task);
        }
    }
    return -1;
}

