```markdown
# TI Platform - Threat Intelligence & Data Leak Monitoring System

TI Platform là nền tảng giám sát, phát hiện và cảnh báo sớm các nguy cơ an ninh mạng liên quan đến rò rỉ dữ liệu (Data Leakage). Hệ thống chủ động thu thập dữ liệu từ các nguồn lộ lọt (Dark Web, diễn đàn ngầm, kênh Telegram...), phân tích tệp thô, đối soát theo bộ luật (Rule-based) cấu hình bởi người dùng và tự động phát cảnh báo tới các kênh quản trị.

Hệ thống giúp tối ưu hóa quy trình vận hành của đội ngũ **SOC/Blue Team** trong việc phát hiện sớm và ứng phó kịp thời với các sự cố lộ lọt thông tin định danh cá nhân (PII) và tài sản số của tổ chức.

---

## 🛠️ Tính Năng Cốt Lõi (Core Features)

* **Xử lý & Giải mã Dữ liệu Thô:** Tự động quản lý, giải nén và xử lý các tệp tin có cấu trúc phức tạp, dung lượng lớn (`.zip`, `.rar`, `.7z`) thu thập từ các nguồn lộ lọt. Hỗ trợ khôi phục/bẻ khóa mật khẩu lưu trữ cơ bản.
* **Giám sát Linh hoạt theo Luật (Rule-based):** Cho phép cấu hình linh hoạt qua file `YAML` để rà soát và bảo vệ tài sản số:
    * *Giám sát liên kết (URL):* Phát hiện tên miền, đường dẫn nội bộ hoặc đối tác xuất hiện trong dữ liệu rao bán.
    * *Giám sát tài khoản (Email/Username):* Định danh tài khoản nhân viên hoặc khách hàng bị rò rỉ.
* **Xác thực Trạng thái Tài khoản (Auth Verification):** Tự động kiểm tra tính hợp lệ (Valid/Invalid/Unknown) của tài khoản bị lộ trên hệ thống **Microsoft Online** (`login.microsoftonline.com`).
* **Cảnh báo & Báo cáo Tự động:**
    * Tổng hợp số liệu biến động trong 24 giờ và gửi báo cáo tóm tắt qua **Telegram Channel**.
    * Tự động đóng gói và đính kèm danh sách bản ghi rò rỉ chi tiết dưới dạng file `.csv` phục vụ quá trình điều tra số (Digital Forensics) và ứng cứu sự cố.

---

## 🔄 Quy Trình Hoạt Động (Workflow)

<img width="1408" height="768" alt="TI-workflow" src="https://github.com/user-attachments/assets/081be37d-4a66-4601-8049-248374c01cc0" />

---

## 🚀 Cài Đặt Môi Trường (Installation)

Yêu cầu hệ thống sử dụng hệ điều hành Ubuntu/Debian. Chạy các lệnh sau để cài đặt các gói phụ thuộc và công cụ cần thiết:

```bash
# Cập nhật danh sách gói dịch vụ
sudo apt-get update

# Cài đặt các thư viện xử lý và giải nén tệp tin chuyên dụng
sudo apt-get install -y libarchive-dev p7zip-full unrar libcompress-raw-lzma-perl

# Cài đặt công cụ kiểm tra/bẻ khóa mật khẩu lưu trữ
sudo apt install -y john

# Cài đặt trình duyệt và driver phục vụ cơ chế tự động hóa (Automation/Scraping)
sudo apt-get install -y chromium chromium-driver

# Cập nhật cấu hình lịch trình tác vụ (Cron job) thông qua Whenever
whenever --update-crontab

```

---

## 🔮 Định Hướng Phát Triển (Future Roadmap)

* [ ] **Giám sát Dark Web tự động:** Tích hợp bot tự động cào dữ liệu (Scraper) và chủ động giám sát (Monitor) các diễn đàn ngầm, chợ đen phổ biến.
* [ ] **Xác thực đa nền tảng:** Mở rộng khả năng kiểm tra tài khoản, hỗ trợ đăng nhập và xác thực tự động trên nhiều dịch vụ SaaS và hệ thống lớn khác (Google Workspace, AWS, IDP nội bộ...).
* [ ] **Bảo mật hệ thống:** Tích hợp cơ chế Xác thực & Phân quyền dựa trên vai trò (RBAC) cho bảng điều khiển quản trị của hệ thống TI.
* [ ] **Làm giàu dữ liệu (Data Enrichment):** Tích hợp thêm các nguồn Threat Intel mở (OSINT) để tối ưu hóa bộ rule cấu hình và giảm thiểu tỷ lệ cảnh báo giả (False Positive).

```

```
