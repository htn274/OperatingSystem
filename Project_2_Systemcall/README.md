Syscall được build trên kernel 3.16.6. Do vậy cần tải bản kernel này về.
```
wget https://www.kernel.org/pub/linux/kernel/v3.x/linux-3.16.36.tar.xz
tar -xvf linux-3.16.36.tar.xz -C /usr/src/
```
Chuyển đường dẫn vào thư mục kernel:
```
cd /usr/src/linux-3.16.36
```
Copy thư mục pnametoid và pidtoname vào kernel:
```
cp /pnametoid /usr/src/linux-3.16.36
cp /pnametoid /usr/src/linux-3.16.36
```
Sửa Makefile của kernel:
```
/usr/src/linux-3.16.36
nano Makefile
```
Tìm dòng bên dưới và thêm pnametoid/ và pidtoname/ 
core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/ pnametoid/ pidtoname/
Thêm vào syscall table 
```
nano arch/x86/syscalls/syscall_64.tbl
```
Thêm vào file syscall_64.tbl
350 common pname sys_pnamtoid
355 common pname sys_pidtoname
Thêm vào dưới file syscall.h 2 dòng sau:
- asmlinkage long sys_pnametoid(char* process_name);
- asmlinkage long sys_pidtoname(int pid, char* buf, int len);
```
nano include/linux/syscalls.h
```
Build
```
sudo make  
make modules_install install
reboot
```
Sau khi khởi động lại có thể chạy file test code:
```
gcc test.c -o test.o
./test
```



