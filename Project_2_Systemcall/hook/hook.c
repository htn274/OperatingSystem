#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/pgtable_types.h>
#include <linux/highmem.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
MODULE_LICENSE("GPL");
/*MY sys_call_table address*/
//ffffffff81601680
void **system_call_table_addr;

unsigned long **find_sys_call_table() {
    unsigned long **sctable;
    unsigned long int i = START_MEM;
    while ( i < END_MEM) {
        sctable = (unsigned long **)i;
        if ( sctable[__NR_close] == (unsigned long *) sys_close) {
            return &sctable[0];
        }
        i += sizeof(void *);
    }
    return NULL;
}
/*my custom syscall that takes process name*/
asmlinkage long (*temp_open) (const char*, int, umode_t);
asmlinkage long (*temp_write) (unsigned int, const char*, size_t);
///*hook*/
asmlinkage long hook_open(const char* filename, int flags, umode_t mode) {
    char buff[100];
    copy_from_user(buff, filename, 100);
    printk(KERN_INFO "process name opens file: %s", current->comm);
    printk(KERN_INFO "hooked open: filename = %s\n", buff);
    return temp_open(filename, flags, mode);
}

asmlinkage long hook_write(unsigned int fd, const char* buf, size_t len) {
    printk(KERN_INFO "process name writes file: %s", current->comm);
    printk(KERN_INFO "hooked write: fd = %u, len = %d\n", fd, (int)len);
    return temp_write(fd, buf, len);
}

/*Make page writeable*/
int make_rw(unsigned long address){
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    if(pte->pte &~_PAGE_RW){
        pte->pte |=_PAGE_RW;
    }
    return 0;
}

/* Make the page write protected */
int make_ro(unsigned long address){
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    pte->pte = pte->pte & ~_PAGE_RW;
    return 0;
}
static int __init entry_point(void){
    printk(KERN_INFO "Hook loaded successfully..\n");
    /*MY sys_call_table address*/
    system_call_table_addr = (void*)0xffffffff81801440;
    /* Replace custom syscall with the correct system call name (write,open,etc) to hook*/
    temp_open = system_call_table_addr[__NR_open];
    temp_write = system_call_table_addr[__NR_write];
    /*Disable page protection*/
    make_rw((unsigned long)system_call_table_addr);
    /*Change syscall to our syscall function*/
    system_call_table_addr[__NR_open] = hook_open;
    system_call_table_addr[__NR_write] = hook_write;
    return 0;
}

static int __exit exit_point(void){
    printk(KERN_INFO "Unloaded Captain Hook successfully\n");
    /*Restore original system call */
    system_call_table_addr[__NR_open] = temp_open;
    system_call_table_addr[__NR_write] = temp_write;
    /*Renable page protection*/
    make_ro((unsigned long)system_call_table_addr);
    return 0;
}
module_init(entry_point);
module_exit(exit_point);
