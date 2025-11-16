# Snake Game – Bài tập nhóm SS004.Q12

Dự án là một trò chơi **Snake** dạng console, chạy trên cả Windows và Linux/macOS, được viết bằng C++. Người chơi điều khiển con rắn di chuyển trên màn hình, ăn thức ăn để tăng điểm và tránh va vào tường hoặc thân mình.

![alt text](demo_game_picture.png)
---

## 1. Thông tin nhóm

- Môn học:g**SS004.Q12 – Kỹ năng nghề nghiệp**  
- Đề tài: **Snake Game**
- Giảng viên hướng dẫn: Thầy **Nguyễn Văn Toàn**

| STT | Họ và tên        | MSSV      | Nhiệm vụ chính                  |
|-----|------------------|-----------|---------------------------------|
| 1   | Nguyễn Việt Anh                | 24520121         | Vẽ giao diện console và combine code (hàm main)       |
| 2   | Nguyễn Thị Mỹ Duyên                | 24520408         | Thiết kế logic và code game loop  |
| 3   | Lê Minh Hiếu                | 24520494         | Xử lý hệ thống, xử lý nhập xuất, test chương trình     |

---

## 2. Mô tả trò chơi

- Trò chơi chạy trong **console**, sử dụng ký tự ASCII để vẽ:
  - `#` : tường/biên màn chơi  
  - `@` : đầu rắn  
  - `o` : thân rắn  
  - `O` : thức ăn
- Mỗi lần ăn thức ăn, rắn dài ra và **+10 điểm**.
- Trò chơi kết thúc khi:
  - Đầu rắn chạm tường, **hoặc**
  - Đầu rắn chạm vào chính thân rắn.

Kích thước mặc định của bảng chơi (có thể chỉnh trong code):

```cpp
int WIDTH = 40;
int HEIGHT = 20;
````

---

## 3. Điều khiển

* `W` – đi **lên**
* `S` – đi **xuống**
* `A` – rẽ **trái**
* `D` – rẽ **phải**
* `Q` – thoát game trong khi đang chơi
* Khi **GAME OVER**:

  * `R` – chơi lại
  * Phím bất kỳ khác – thoát chương trình

Game có kiểm tra để **không cho quay 180° ngay lập tức** (ví dụ đang đi sang trái thì không cho bấm sang phải liền).

---

## 4. Cách biên dịch & chạy

### 4.1. Linux / macOS

```bash
g++ -std=c++17 main.cpp -o snake
./snake
```
---

### 4.2. Windows – MinGW (g++)

```bash
g++ -std=c++17 main.cpp -o snake.exe
snake
```

Sau khi build xong, chạy file `main.exe`.

---

## 5. Cách chơi (Gameplay)

1. Chạy chương trình, rắn sẽ xuất hiện giữa màn hình, thức ăn xuất hiện ngẫu nhiên.
2. Dùng các phím **W/A/S/D** để điều khiển hướng đi.
3. Mỗi lần ăn được `O`:

   * Rắn dài thêm 1 đốt.
   * Điểm số tăng thêm **10**.
4. Nếu rắn đâm vào tường hoặc thân mình:

   * Màn hình in ra **“GAME OVER! Final score: …”**.
   * Bấm **`R`** để bắt đầu lại với điểm = 0, rắn về vị trí ban đầu.
   * Hoặc bấm phím khác để thoát.

---

## 6. Cấu trúc & kỹ thuật lập trình

Một số thành phần chính trong code:

* **Xử lý terminal/console**:

  * Trên **Windows**: dùng `windows.h`, `conio.h`, `_kbhit()`, `_getch()` và bật cờ `ENABLE_VIRTUAL_TERMINAL_PROCESSING` để dùng màu/ký tự điều khiển ANSI.
  * Trên **Linux/macOS**: dùng `termios`, `fcntl`, `read()` để chuyển stdin sang chế độ **non-blocking** (không chặn khi chờ phím).

* **Ẩn/hiện con trỏ & vẽ lại màn hình**:

  * Hàm `hideCursor()`, `showCursor()`, `moveCursorHome()`, `clearScreenOnce()` sử dụng escape sequence `\x1B[...`.
  * Mỗi frame, game vẽ toàn bộ bảng ra một `stringstream` rồi in ra một lần để tránh nháy màn hình.

* **Lớp Game**:

  * Thuộc tính:

    * `deque<pii> snake;` – lưu các đốt rắn (đầu ở front).
    * `pii food;` – vị trí thức ăn.
    * `Direction dir;` – hướng hiện tại (LEFT, RIGHT, UP, DOWN).
    * `int width, height; bool over;`
  * Một số hàm quan trọng:

    * `reset()` – đặt lại game về trạng thái ban đầu.
    * `placeFood()` – random vị trí thức ăn không trùng thân rắn.
    * `changeDirFromKey(int key)` – đổi hướng dựa vào phím W/A/S/D.
    * `update()` – cập nhật vị trí rắn, kiểm tra va chạm, ăn thức ăn.

* **Vòng lặp trò chơi (game loop)** trong `main()`:

  * Dùng `getCharNonBlocking()` để đọc phím mà **không chặn** chương trình.
  * Dùng `std::this_thread::sleep_for(std::chrono::milliseconds(100))` làm delay giữa 2 frame → điều khiển tốc độ rắn.