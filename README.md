# Flappy Bird – STM32 Nucleo-F401RE

Đây là game Flappy Bird viết bằng C cho board STM32 Nucleo-F401RE, dùng
thư viện SPL/CMSIS và Ucglib để điều khiển phần cứng và vẽ lên LCD.
Trong cấu hình hiện tại, SDK dùng màn hình ST7735 128×128. Màn hình được
xoay 180° và phần chơi chiếm một vùng 128×80 pixel.

## Điều khiển

- **SW1 (PB5):** đưa con trỏ về dòng LEVEL trong menu.
- **SW5 (PB4):** đưa con trỏ xuống dòng START.
- **SW3 (PA4):** nhấn ở dòng LEVEL để đổi độ khó, nhấn ở dòng START để chơi.
  Khi đang chơi, mỗi lần nhấn là một lần chim vỗ cánh.
- Giữ SW3 sẽ không làm chim vỗ cánh liên tục. Sau khi nhấn START, bạn cần
  nhả nút rồi nhấn lại để điều khiển chim.
- Khi thua, game dừng lại, buzzer tắt và màn hình hiện điểm vừa đạt được.
  LED ở chân PA11 nháy 5 lần trong khoảng 1,2 giây, sau đó game trở về menu.

Buzzer nối với PC9. Code hiện chỉ bật/tắt chân GPIO nên dùng được với module
buzzer có sẵn bộ dao động. Nếu bạn dùng buzzer thụ động thì cần thêm phần
phát PWM để tạo âm thanh.

## Phân chia source

File header nằm trong `Inc/`, còn phần cài đặt nằm trong `Src/`. Hai thư mục
được chia giống nhau để dễ tìm: chẳng hạn, `Inc/game/bird.h` đi cùng với
`Src/game/bird.c`.

| Module | Dùng để làm gì? |
|---|---|
| app | Chuyển giữa menu, đang chơi và game over; xử lý thao tác nút và hiệu ứng |
| game/config.h, level_config.c | Đặt kích thước vùng chơi, chim, ống và thông số của từng level |
| game/bird | Tính vị trí, vận tốc của chim và xử lý vỗ cánh |
| game/pipe | Tạo ống mới và cho ống di chuyển |
| game/collision | Kiểm tra chim có chạm ống, trần hoặc sàn không |
| game/game | Ghép các bước cập nhật game, cộng điểm và xác định lúc thua |
| gfx/render | Vẽ chim, ống và khung; khi ống di chuyển chỉ vẽ lại những dải thay đổi |
| gfx/ui | Vẽ menu và màn hình kết quả |
| periph/input | Lọc rung nút trong 15 ms và ghi nhận mỗi lần nhấn |
| periph/buzzer, led | Bật/tắt LED và buzzer; tiếng vỗ cánh kéo dài khoảng 60 ms |
| drivers/timebase | Lấy thời gian theo mili-giây và gọi bộ lập lịch của SDK |
| board_config.h | Khai báo chân GPIO và thứ tự màu LCD |

Nếu muốn xem game hoạt động thế nào, bạn có thể bắt đầu từ `app.c`, rồi
đọc tiếp `game/game.c`. Cứ mỗi 20 ms, game cập nhật chim và ống, kiểm tra
va chạm rồi mới tính điểm. Khi vẽ LCD mất nhiều thời gian, game sẽ chạy bù
những bước còn thiếu trước khi vẽ khung hình tiếp theo, để chim và ống
vẫn di chuyển theo cùng một nhịp.

Phần `game/` chỉ xử lý dữ liệu, không gọi trực tiếp STM32 hay Ucglib.
Việc đọc nút và cập nhật game đều nằm trong vòng lặp chính. Thời gian được
lấy từ SysTick do `TimerInit()` của SDK khởi tạo, không dùng TIM2.

Phần vẽ ống và kiểm tra va chạm dùng chung `PIPE_GAP_HALF`, nên khe nhìn
thấy trên màn hình cũng là khe chim có thể bay qua. Trong code, hình chữ
nhật được tính theo khoảng `[x, x + width)`: có tính biên đầu, không tính
biên cuối. Chim được phép vừa khít biên khe; vượt ra ngoài thì thua.
Khi cả ống đã đi qua chim, điểm tăng một lần.

## Build bằng PowerShell

Bạn cần có SDK `ThuVien_SDK_1.0.3_NUCLEO-F401RE-master` và ARM GCC đi kèm
STM32CubeIDE. Mở PowerShell tại thư mục chứa `build.ps1`, thay đường dẫn
compiler bên dưới bằng đường dẫn trên máy bạn rồi chạy:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\build.ps1 -SdkRoot "C:\ThuVien_SDK_1.0.3_NUCLEO-F401RE-master" -ToolchainBin "DUONG_DAN_ARM_GCC\bin" -Configuration Debug
```

Muốn build bản tối ưu kích thước, đổi `Debug` thành `Release`. Nếu không muốn
nhập lại đường dẫn mỗi lần, bạn có thể đặt hai biến môi trường
`FLAPPY_SDK_ROOT` và `ARM_GCC_BIN`, rồi bỏ các tham số đường dẫn khỏi lệnh.

Build xong, các file ELF, HEX và map sẽ nằm trong `build/Debug` hoặc
`build/Release`. Script bật `-Wall -Wextra -Werror` cho code ứng dụng, nên
cần sửa hết cảnh báo ở phần này mới build được. Cảnh báo từ SDK vẫn được
hiện ra nhưng không bị coi là lỗi.

## STM32CubeIDE

Sau khi import project, vào **Properties → Resource → Linked Resources**
và kiểm tra liên kết `ThuVien_SDK_1.0.3_NUCLEO-F401RE-master`.
Project đang trỏ đến `C:/ThuVien_SDK_1.0.3_NUCLEO-F401RE-master`.
Nếu bạn để SDK ở chỗ khác, sửa lại liên kết này. Cả Debug và Release đều
đã được cấu hình đường dẫn header và source của SDK giống nhau.

Với lần build đầu sau khi cập nhật source, hãy **Refresh → Project → Clean →
Build**. Bước này giúp CubeIDE tạo lại makefile theo danh sách file mới,
thay cho các file cũ như `entities.c`, `levels.c`, `loop.c` và `game/ui.c`.
Các makefile trong `Debug/` do IDE tự tạo, nên bạn không cần sửa chúng bằng tay.

## Kiểm thử

Bản source này đã build và link được cả Debug lẫn Release bằng ARM GCC
13.3.1, có đầy đủ file ELF và HEX. Code ứng dụng không còn cảnh báo với
`-Wall -Wextra -Werror`; SDK vẫn còn một số cảnh báo về tham số không dùng.

Phần test C mới được kiểm tra cú pháp, chưa chạy các lệnh `assert` vì máy
dùng để sửa source chưa có compiler C chạy trên Windows. Firmware cũng
chưa được nạp thử trên board thật.

File `tests/game_test.c` có các trường hợp kiểm tra cho cả hai level:
bay sát biên khe, chạm trần/sàn, chiều rộng ống, cộng điểm một lần,
tạo lại ống, dừng khi thua và bắt đầu ván mới. Để chạy, bạn cần GCC cho
Windows, chẳng hạn MinGW. Thay đường dẫn bên dưới cho đúng với máy bạn
(đây là GCC chạy trên máy tính, không phải ARM GCC):

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\tests\run.ps1 -Compiler "DUONG_DAN_MINGW\gcc.exe"
```

Sau khi nạp firmware lên board, bạn có thể thử lần lượt những thao tác sau:

1. Bấm SW3 ở menu ngay khi bật nguồn, rồi thử lại sau khi chơi thua. Cả hai
   lần đều phải nhận nút bình thường.
2. Giữ SW3 khi chơi: chim chỉ vỗ cánh một lần. Nhả rồi nhấn lại mới vỗ tiếp.
3. Chơi cả hai level và thử bay sát mép khe. Ống rộng lần lượt 8 và 12 pixel;
   chim không được thua khi vẫn còn nằm trong khe.
4. Đi qua một ống rồi kiểm tra điểm cuối ván. Mỗi ống chỉ được cộng một điểm,
   và ván mới phải bắt đầu từ 0.
5. Khi thua, kiểm tra buzzer đã tắt, chim đã dừng, LED nháy rồi game về menu.
6. Quan sát xem chim và ống có di chuyển mượt không. Phần này cần thử trên
   LCD thật vì tốc độ truyền SPI còn phụ thuộc SDK và phần cứng.
