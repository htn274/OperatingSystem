#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main() {
    char name[32], process_name[32];
    printf("Test pnametoid\nEnter process_name: ");
    scanf("%s", name);
    int s = syscall(350, name);
    printf("PName: %s - PID: %d\n", name, s);

    int id;
    printf("Test pidtoname\nEnter id: ");
    scanf("%d", &id);
    int ans = syscall(355, id, process_name, 32);
    printf("%d %s\n", ans, process_name);
    }
