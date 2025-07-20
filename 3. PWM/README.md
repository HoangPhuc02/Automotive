# Tài Liệu PWM AUTOSAR

## Tổng Quan
PWM (Pulse Width Modulation - Điều Chế Độ Rộng Xung) là một kỹ thuật điều khiển tín hiệu số để tạo ra các tín hiệu analog. Driver PWM AUTOSAR cung cấp các API chuẩn để điều khiển các kênh PWM trên vi điều khiển STM32F103C8T6.

## Kiến Trúc Hệ Thống

### Cấu Trúc Phân Lớp AUTOSAR
```
┌─────────────────────────────────────┐
│          Application Layer          │ ← Ứng dụng người dùng
├─────────────────────────────────────┤
│             PWM APIs                │ ← Các hàm API chuẩn AUTOSAR
├─────────────────────────────────────┤
│         PWM Driver (MCAL)           │ ← Driver tầng thấp
├─────────────────────────────────────┤
│        Hardware (STM32F103)        │ ← Phần cứng vi điều khiển
└─────────────────────────────────────┘
```

### Ánh Xạ Kênh PWM
| Kênh PWM | Timer | Kênh Timer | Chân GPIO | Chức Năng | Trạng Thái |
|----------|-------|------------|-----------|-----------|-----------|
| Channel 0 | TIM1 | CH1 | PA8 | Variable Period | Hoạt động |
| Channel 1 | TIM1 | CH2 | PA9 | Variable Period | Hoạt động |
| Channel 2 | TIM1 | CH3 | PA10 | Variable Period | Hoạt động |
| Channel 3 | TIM1 | CH4 | PA11 | Variable Period | Hoạt động |
| Channel 4 | TIM2 | CH1 | PA0 | Fixed Period | Hoạt động |
| Channel 5 | TIM2 | CH2 | PA1 | Fixed Period | Hoạt động |
| Channel 6 | TIM2 | CH3 | PA2 | Fixed Period | Hoạt động |
| Channel 7 | TIM2 | CH4 | PA3 | Fixed Period | Hoạt động |

## Các Hàm API PWM AUTOSAR

### 1. `Pwm_Init()`
**Mục đích**: Khởi tạo driver PWM và tất cả các kênh PWM
**Cú pháp**: 
```c
void Pwm_Init(const Pwm_ConfigType* ConfigPtr)
```

**Tham số**:
- `ConfigPtr`: Con trở tới cấu trúc cấu hình PWM chứa thông tin về tất cả các kênh

**Chức năng chi tiết**:
1. **Khởi tạo clock**: Bật clock cho các timer TIM1, TIM2
2. **Cấu hình GPIO**: Thiết lập các chân PA0-PA3, PA8-PA11 làm chức năng thay thế (Alternate Function)
3. **Cấu hình Timer Base**: Thiết lập prescaler, period, counter mode cho từng timer
4. **Cấu hình PWM Channels**: Thiết lập chế độ PWM, polarity, preload cho từng kênh
5. **Bật Timer**: Kích hoạt các timer để bắt đầu tạo tín hiệu PWM

**Ví dụ sử dụng**:
```c
extern const Pwm_ConfigType Pwm_Config;
Pwm_Init(&Pwm_Config);  // Khởi tạo tất cả 8 kênh PWM
```

**Lưu ý**: Hàm này phải được gọi trước tất cả các hàm PWM khác.

---

### 2. `Pwm_SetDutyCycle()`
**Mục đích**: Thiết lập tỷ lệ độ rộng xung (duty cycle) cho một kênh PWM cụ thể
**Cú pháp**: 
```c
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, uint16 DutyCycle)
```

**Tham số**:
- `ChannelNumber`: Số kênh PWM (0-7)
- `DutyCycle`: Giá trị duty cycle (0x0000 đến 0x8000)
  - `0x0000` = 0% (luôn LOW)
  - `0x4000` = 50% (HIGH 50% thời gian)
  - `0x8000` = 100% (luôn HIGH)

**Chức năng chi tiết**:
1. **Kiểm tra tính hợp lệ**: Xác nhận ChannelNumber và DutyCycle hợp lệ
2. **Tính toán Compare Value**: Chuyển đổi DutyCycle thành giá trị so sánh cho timer
3. **Kiểm tra trạng thái Idle**: Nếu kênh đang ở trạng thái idle, thực hiện khôi phục PWM
4. **Cập nhật thanh ghi**: Ghi giá trị compare vào thanh ghi CCRx của timer
5. **Kích hoạt Preload**: Đảm bảo giá trị mới được áp dụng an toàn

**Công thức tính toán**:
```c
CompareValue = (DutyCycle * TimerPeriod) / 0x8000
Phần trăm = (DutyCycle * 100) / 0x8000
```

**Ví dụ sử dụng**:
```c
Pwm_SetDutyCycle(4, 0x2000);  // Kênh 4 (PA0) = 25% duty cycle
Pwm_SetDutyCycle(5, 0x6000);  // Kênh 5 (PA1) = 75% duty cycle
```

**Tín hiệu mong đợi**:
- 25% duty cycle: 250μs HIGH, 750μs LOW (chu kỳ 1ms)
- 75% duty cycle: 750μs HIGH, 250μs LOW (chu kỳ 1ms)

---

### 3. `Pwm_SetPeriodAndDuty()`
**Mục đích**: Thiết lập đồng thời cả chu kỳ (period) và duty cycle cho kênh PWM
**Cú pháp**: 
```c
void Pwm_SetPeriodAndDuty(Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, uint16 DutyCycle)
```

**Tham số**:
- `ChannelNumber`: Số kênh PWM
- `Period`: Chu kỳ PWM mới (đơn vị timer ticks)
- `DutyCycle`: Duty cycle (0x0000 đến 0x8000)

**Chức năng chi tiết**:
1. **Kiểm tra loại kênh**: Chỉ áp dụng cho kênh Variable Period (TIM1)
2. **Dừng timer tạm thời**: Ngăn glitch khi thay đổi period
3. **Cập nhật Period**: Ghi giá trị mới vào thanh ghi ARR
4. **Cập nhật Duty Cycle**: Tính toán và ghi giá trị compare
5. **Khởi động lại timer**: Tiếp tục tạo PWM với thông số mới

**Ví dụ sử dụng**:
```c
Pwm_SetPeriodAndDuty(0, 2000, 0x4000);  // Kênh 0: Period 2ms, 50% duty
```

**Lưu ý**: Chỉ dành cho các kênh TIM1 (Channel 0-3). Các kênh TIM2 có period cố định.

---

### 4. `Pwm_SetOutputToIdle()`
**Mục đích**: Buộc kênh PWM về trạng thái nghỉ (idle state) được cấu hình
**Cú pháp**: 
```c
void Pwm_SetOutputToIdle(Pwm_ChannelType ChannelNumber)
```

**Tham số**:
- `ChannelNumber`: Số kênh PWM cần thiết lập idle

**Chức năng chi tiết**:
1. **Lấy cấu hình Idle**: Đọc trạng thái idle từ config (PWM_HIGH hoặc PWM_LOW)
2. **Áp dụng Forced Output**: Sử dụng `TIM_ForcedOCxConfig()` để buộc output
   - `TIM_ForcedAction_Active`: Buộc output HIGH
   - `TIM_ForcedAction_InActive`: Buộc output LOW
3. **Đánh dấu trạng thái**: Thiết lập cờ IdleStateSet để theo dõi
4. **Vô hiệu hóa PWM**: Chuyển từ chế độ PWM sang Forced Output

**Cấu hình Idle State**:
- Channel 6 (PA2): PWM_HIGH → Output HIGH khi idle
- Channel 7 (PA3): PWM_HIGH → Output HIGH khi idle
- Channel 4,5: PWM_LOW → Output LOW khi idle

**Ví dụ sử dụng**:
```c
Pwm_SetOutputToIdle(6);  // PA2 sẽ ở mức HIGH
Pwm_SetOutputToIdle(7);  // PA3 sẽ ở mức HIGH
```

**Ứng dụng**: An toàn hệ thống, tắt PWM trong trường hợp khẩn cấp.

---

### 5. `Pwm_GetOutputState()`
**Mục đích**: Đọc trạng thái hiện tại của đầu ra PWM
**Cú pháp**: 
```c
Pwm_OutputStateType Pwm_GetOutputState(Pwm_ChannelType ChannelNumber)
```

**Tham số**:
- `ChannelNumber`: Số kênh PWM cần đọc

**Giá trị trả về**:
- `PWM_HIGH`: Đầu ra đang ở mức cao (3.3V)
- `PWM_LOW`: Đầu ra đang ở mức thấp (0V)

**Chức năng chi tiết**:
1. **Đọc thanh ghi GPIO**: Kiểm tra trạng thái thực tế của chân GPIO
2. **Xác định logic level**: So sánh với ngưỡng điện áp
3. **Trả về kết quả**: PWM_HIGH hoặc PWM_LOW

**Ví dụ sử dụng**:
```c
Pwm_OutputStateType state = Pwm_GetOutputState(4);
if (state == PWM_HIGH) {
    // PA0 đang ở mức cao
} else {
    // PA0 đang ở mức thấp
}
```

**Ứng dụng**: Kiểm tra trạng thái, debug, feedback cho ứng dụng.

---

### 6. `Pwm_EnableNotification()`
**Mục đích**: Bật thông báo ngắt (interrupt) cho các sự kiện cạnh của PWM
**Cú pháp**: 
```c
void Pwm_EnableNotification(Pwm_ChannelType ChannelNumber, Pwm_EdgeNotificationType Notification)
```

**Tham số**:
- `ChannelNumber`: Số kênh PWM
- `Notification`: Loại cạnh kích hoạt ngắt
  - `PWM_RISING_EDGE`: Cạnh lên (LOW → HIGH)
  - `PWM_FALLING_EDGE`: Cạnh xuống (HIGH → LOW)  
  - `PWM_BOTH_EDGES`: Cả hai cạnh

**Tham số**:
- `ChannelNumber`: Số kênh PWM
- `Notification`: Loại cạnh kích hoạt ngắt
    - `PWM_RISING_EDGE`: Cạnh lên (LOW → HIGH)
    - `PWM_FALLING_EDGE`: Cạnh xuống (HIGH → LOW)  
    - `PWM_BOTH_EDGES`: Cả hai cạnh

**Cơ chế ngắt cạnh**:
- **PWM_RISING_EDGE**: 
    - Kích hoạt khi tín hiệu PWM chuyển từ LOW sang HIGH
    - Sử dụng ngắt TIM_IT_Update (khi bộ đếm timer quay về 0)
    - Thanh ghi CNT của timer reset về 0 tạo ra xung rising edge

- **PWM_FALLING_EDGE**:
    - Kích hoạt khi tín hiệu PWM chuyển từ HIGH sang LOW
    - Sử dụng ngắt TIM_IT_CCx (x là số kênh timer 1-4)
    - Xảy ra khi CNT = CCRx, tạo ra cạnh xuống của xung PWM

- **PWM_BOTH_EDGES**:
    - Kết hợp cả hai loại ngắt trên
    - Kích hoạt TIM_IT_Update và TIM_IT_CCx
    - Hai ngắt này sẽ xảy ra ở cả hai cạnh của xung PWM

**Chức năng chi tiết**:
1. **Thiết lập trigger source**: Cấu hình timer để phát hiện cạnh đúng như yêu cầu
    - Cạnh lên: Cấu hình `TIM_IT_Update` để phát hiện khi counter reset
    - Cạnh xuống: Cấu hình `TIM_IT_CCx` để phát hiện khi counter = CCRx
2. **Kích hoạt NVIC**: Bật vector ngắt tương ứng với timer trong NVIC controller
    - Kiểm tra xem ngắt timer đã được kích hoạt chưa bằng bảng `Pwm_UpdateInterruptUsers` trong `Pwm_Hw.c` cho ngắt `TIM_IT_Update`
    - Ghi vào bảng `Pwm_UpdateInterruptUsers`
    - Cho phép ngắt timer trong NVIC sử dụng `NVIC_EnableIRQ()`
3. **Lưu trữ callback**: Lưu lại trigger source
**Hàm Callback được gọi**:
```c
// Khi có cạnh lên/xuống, các hàm này sẽ được gọi:
void Pwm_Tim2_Channel1_Notification(void);  // Kênh 4 (PA0)
void Pwm_Tim2_Channel2_Notification(void);  // Kênh 5 (PA1)
void Pwm_Tim2_Channel3_Notification(void);  // Kênh 6 (PA2)
```

**Ví dụ sử dụng**:
```c
Pwm_EnableNotification(4, PWM_RISING_EDGE);   // PA0: Ngắt cạnh lên
Pwm_EnableNotification(5, PWM_FALLING_EDGE);  // PA1: Ngắt cạnh xuống
Pwm_EnableNotification(6, PWM_BOTH_EDGES);    // PA2: Ngắt cả hai cạnh
```

**Ứng dụng**: Đo tần số, đếm xung, đồng bộ hóa, feedback.

---

### 7. `Pwm_DisableNotification()`
**Mục đích**: Tắt thông báo ngắt cho kênh PWM
**Cú pháp**: 
```c
void Pwm_DisableNotification(Pwm_ChannelType ChannelNumber)
```

**Tham số**:
- `ChannelNumber`: Số kênh PWM cần tắt notification (0-7)

**Cơ chế tắt ngắt**:
- **Update interrupt**: Tắt `TIM_IT_Update` chỉ khi không còn kênh nào sử dụng (quản lý qua bảng `Pwm_UpdateInterruptUsers`)
- **CC interrupt**: Tắt `TIM_IT_CCx` ngay lập tức cho kênh cụ thể
- **Cập nhật trạng thái**: Đặt `NotificationEnabled = FALSE`

**Ví dụ sử dụng**:
```c
Pwm_DisableNotification(4);  // Tắt notification cho PA0
Pwm_DisableNotification(5);  // Tắt notification cho PA1
Pwm_DisableNotification(6);  // Tắt notification cho PA2
```

---

## Cấu Trúc Dữ Liệu

### `Pwm_ChannelConfigType`
Cấu trúc cấu hình cho từng kênh PWM:
```c
typedef struct {
    Pwm_ChannelType ChannelId;           // ID kênh (0-7)
    Pwm_HwUnitType HwUnit;               // Timer sử dụng (TIM1/TIM2)
    Pwm_ChannelClassType ChannelClass;   // Loại kênh (Fixed/Variable Period)
    Pwm_PeriodType Period;               // Chu kỳ PWM
    uint16 DutyCycle;                    // Duty cycle mặc định
    Pwm_OutputStateType Polarity;        // Phân cực (HIGH/LOW)
    Pwm_OutputStateType IdleState;       // Trạng thái idle
    Pwm_NotificationPtrType NotificationPtr; // Con trỏ hàm callback
    Pwm_EdgeNotificationType NotificationEdge; // Loại cạnh notification
} Pwm_ChannelConfigType;
```

### `Pwm_ConfigType`
Cấu trúc chính chứa tất cả cấu hình PWM:
```c
typedef struct {
    Pwm_ChannelConfigType* ChannelConfig;    // Mảng cấu hình kênh
    Pwm_HwUnitConfigType* HwUnitConfig;      // Cấu hình timer
    uint8 MaxChannels;                       // Số kênh tối đa
    uint8 MaxHwUnits;                        // Số timer tối đa
} Pwm_ConfigType;
```

## Thông Số Kỹ Thuật

### Thông Số Timer
| Timer | Clock Source | Prescaler | Max Period | Frequency | Resolution |
|-------|--------------|-----------|------------|-----------|------------|
| TIM1 | 72MHz | 72 | 65535 | 1kHz | 1μs |
| TIM2 | 72MHz | 72 | 65535 | 1kHz | 1μs |

### Thông Số PWM
- **Điện áp logic**: 0V (LOW), 3.3V (HIGH)
- **Tần số mặc định**: 1kHz
- **Độ phân giải duty cycle**: 16-bit (0x0000 - 0x8000)
- **Độ chính xác**: ±1 timer tick (±1μs)
- **Thời gian thiết lập**: <10μs

## Lưu Đồ Hoạt Động

### Khởi Tạo PWM
```
[Start] → [Pwm_Init()] → [Clock Enable] → [GPIO Config] 
     ↓
[Timer Base Setup] → [PWM Channel Config] → [Timer Enable] → [Ready]
```

### Thiết Lập Duty Cycle
```
[Pwm_SetDutyCycle()] → [Validate Input] → [Check Idle State]
     ↓
[Calculate Compare] → [Update CCR Register] → [Enable Preload] → [Done]
```

### Thiết Lập Idle State
```
[Pwm_SetOutputToIdle()] → [Get Idle Config] → [Apply Forced Output]
     ↓
[Set Idle Flag] → [Disable PWM Mode] → [Output at Idle Level]
```


## Kết Luận

Driver PWM AUTOSAR cung cấp một interface chuẩn và mạnh mẽ để điều khiển các tín hiệu PWM trên STM32F103C8T6. Với 8 kênh PWM được hỗ trợ, hệ thống có thể đáp ứng nhiều ứng dụng khác nhau từ điều khiển motor, LED đến tạo tín hiệu analog.

### Ưu Điểm
- **Chuẩn AUTOSAR**: Tuân thủ tiêu chuẩn công nghiệp
- **Linh hoạt**: Hỗ trợ nhiều chế độ và cấu hình
- **Hiệu suất cao**: Độ phân giải 16-bit, tần số lên đến 1MHz
- **An toàn**: Chế độ idle state cho các tình huống khẩn cấp

### Hạn Chế Hiện Tại
- Một số enumeration constants chưa được định nghĩa

