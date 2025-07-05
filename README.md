# E-WALLET POINT SYSTEM (C++)

## Mô tả
Ứng dụng quản lý ví điểm thưởng, hỗ trợ phân quyền người dùng, bảo mật OTP, giao dịch chuyển điểm an toàn và quản lý bởi người quản lý.

##  Thành viên và phân công
| Họ tên           | MSSV       | Vai trò            | Công việc được giao                    |
|----------------  |---------   |------------------- |----------------------------------------|
| Trần Huy Linh   | K24DTCN298 | Trưởng nhóm        |- Thiết kế kiến trúc hệ thống, xử lý OTP, xử lý ví điểm và lịch sử giao dịch, - Xử lý ví điểm và lịch sử giao dịch, - Đăng nhập, user đổi thông tin cá nhân, - Xem thông tin user, admin tạo account cho user, chỉnh sửa thông tin user|
| Trần Văn Phát  |  | D24TXCN12-K         | Thành viên      | - Xử lý Đăng nhập, đăng ký

## Tính năng
- Đăng ký / đăng nhập (mật khẩu tự động / thủ công)
- OTP bảo vệ đổi mật khẩu / thông tin cá nhân
- Phân quyền người dùng / quản lý
- Ví tổng cấp phát điểm
- Giao dịch chuyển điểm giữa người dùng
- Ghi log và tra cứu lịch sử giao dịch

## ▶️ Cách build và run
```sh
g++ main.cpp Account.cpp Transaction.cpp TransactionManager.cpp UserInterface.cpp utils.cpp -o wallet_app -lssl -lcrypto
```

## Cách chạy chương trình
- Đăng nhập với tài khoản manager có sẵn: admin/123123

##  Các tập tin kèm theo
- `main.cpp`: mã nguồn chính
- `user.txt`: dữ liệu người dùng, dữ liệu ví điểm
- `transactions.txt`: lịch sử giao dịch
- `users.txt.bak`: dữ liệu backup

##  Sao lưu
- Dữ liệu được đọc và ghi dưới file `user.txt` và `transactions.txt`
- Tự động backup dữ liệu dưới dạng file `users.txt.bak`
