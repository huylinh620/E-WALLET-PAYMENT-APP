# E-WALLET POINT SYSTEM (C++)

## 🧩 Mô tả
Ứng dụng quản lý ví điểm thưởng, hỗ trợ phân quyền người dùng, bảo mật OTP, giao dịch chuyển điểm an toàn và quản lý bởi người quản lý.

## ✅ Tính năng
- Đăng ký / đăng nhập (mật khẩu tự động / thủ công)
- OTP bảo vệ đổi mật khẩu / thông tin cá nhân
- Phân quyền người dùng / quản lý
- Ví tổng cấp phát điểm
- Giao dịch chuyển điểm giữa người dùng
- Ghi log và tra cứu lịch sử giao dịch

## ▶️ Cách build và chạy
```sh
g++ *.cpp -o wallet_system.exe -lssl -lcrypto-lcrypto




b/ Người dùng quản lý: Ngoài chức năng quản lý thông tin cá nhân, người quản lý có thể:

  b.1 - Theo dõi danh sách nhóm

  b.2 - Tạo thêm tài khoản mới

  b.3 - Điều chỉnh thông tin của tài khoản khi có yêu cầu từ chủ tài khoản (làm hộ). Khi điều chỉnh xong, hệ thống gửi một mã OTP đến chủ tài khoản cùng thông báo các nội dung sẽ thay đổi để chủ tài khoản xác nhận thao tác cập nhật.

Nếu mã OTP do chủ tài khoản nhập vào hợp lệ, hệ thống tiến hành cập nhật thông tin của người dùng.



Chú ý: Không được phép thay đổi tên tài khoản đăng nhập.



C. Quản lý hoạt động ví:

Mỗi người dùng có một bộ dữ liệu về số điểm (ví - wallet) và dữ liệu các giao dịch trao đổi, chuyển điểm từ một ví sang một ví khác.

Một ví có mã số định danh duy nhất phân biệt với tất cả các ví còn lại.

Ví tổng: nguồn duy nhất sinh ra tổng số điểm sẽ lưu chuyển trong toàn bộ hệ thống.



Giao dịch chuyển d điểm từ ví A sang ví B sẽ gồm các thao tác thành phần sau

    1_ Tìm, mở ví A. Đây là ví chủ của giao dịch, là ví của chủ tài khoản muốn thực hiện chuyển điểm đi.

    2_ Tìm, mở ví B. Đây là ví đích của giao dịch. Nếu mã ví B tồn tại, tiếp tục đến 3_Giao dịch

    3_ Giao dịch: Hai tác vụ dưới đây là không tách rời (atomic). Bất kỳ có một bất thường (exception) nào xảy ra trong một tác vụ đều dẫn đến hủy toàn bộ giao dịch, phục hồi lại trạng thái số dư của hai ví A và B trước 3_ 

        3_1 if (A.balance >=d) 

                   A.balance = A.balance - d

               else

                   "low balance. Cannot proceed.”. Đến 4_

        3_2 B.balance = B.balance + d

    4_ Kết thúc 

