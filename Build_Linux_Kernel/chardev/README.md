#generatorDev.c 
Đây là chương trình Kernel Module dùng để sinh ra một số ngẫu nhiên, cho phép các tiến trình ở userspacecos thể open và read các số ngẫu nhiê
##static int __init generDev_init(void)
Nhiệm vụ: Hàm này dùng để khởi tạo khi xây dựng một driver. Hàm được gọi khi module được load lên.  
Quy trình: 
- Đầu tiên hàm này sẽ đăng kí với hệ thống để hệ thống cấp cho một majorNumber. Nếu thành công (majorNumber < 0) thì đăng kí thất bại. 
- Sau khi đăng kí thành công. Tiếp theo ta tiến hành khởi tạo một lớp thiết bị (device class) với câu lện
     genClass = class_create(THIS_MODULE, CLASS_NAME);
    Hàm này sẽ trả về một con trỏ hàm đến class device được khởi tạo nếu thành công.
    Ngược lại chúng ta phải hủy bỏ đi số majorNumber đã đăng kí.
    Sau đó thông báo ra màn hình việc đăng kí thất bại. 
- Cuối cùng sẽ đăng kí một driver qua lệnh:
    genDevice = device_create(genClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    Hàm này sẽ trả về con trỏ kiểu device-driver device struct được định nghĩa ở "driver.h" nếu thành công.
    Ngược lại chúng ta sẽ hủy bỏ genClass và majorNumber vừa mới đăng kí. 
##static void __exit generDev_exit(void)
Nhiệm vụ: Hàm này sẽ "dọn dẹp" những gì chúng ta đã làm trước khi unload module. Hàm được gọi trong quá trình unload module. 
Quy trình:
- Chúng ta chỉ cần lần lượt hủy đi những con trỏ được cấp phát và hủy đăng kí majorNumber. 
##static int dev_open(struct inode *inodep, struct file *filep)
Nhiệm vụ: Hàm sẽ được gọi khi nhận yêu cầu mở từ các câu lệnh ở userspace. 
Quy trình:
- Hàm sẽ có một biến toàn cục numberOpens để đếm số lần module này được yêu cầu mở và xuất ra thông tin này khi nhận được yêu cầu open. 
##static ssize_t dev_read(struct file *filep, char* buffer, size_t len, loff_t *offset)
Nhiệm vụ: Hàm này được gọi khi userspace muốn truy cập dữ liệu vào từ device. 
Quy trình:
- Ta dùng lệnh get_random_bytes(numberGen, sizeof(int)) để random một số ngẫu nhiên. 
- Sau đó, ta gửi dữ liệu này đến userspace thông qua lệnh: copy_to_user(buffer, numberGen, sizeof(int)). Nếu thành công, hàm copy_to_user sẽ trả về 0.
##static int dev_release(struct inode *inodep, struct file *filep)
Nhiệm vụ: Hàm này được khỏi khi đóng một device. 
Quy trình:
- Trong hàm này ta chỉ xuất thông báo ra màn hình thiết bị đã đóng bằng printk. 

#testGenerator.c
Đây là chương trình C bình thường (thuôc userspace). 
Chương trình này sẽ gọi lệnh open("/dev/generDev", O_RDWR) để yêu cầu mở một device để random số ngẫu nhiên. 
Nếu mở thành công, tiến hành đọc số ngẫu nhiên mà device gửi. 
Nếu thành công, xuất ra số nhận được. 

#Chạy và kiểm tra driver 
1. Dùng lệnh "make" để build chương trình 
2. Chạy file sudo insmod generatorDev.ko
3. Chạy sudo ./test để chạy file test 
4. Ngắt kết nối: sudo rmmod generatorDev
Ta có thể xem các tiến trình chạy ở module kernel bằng lệnh: 
sudo tail -f /var/log/kern.log

