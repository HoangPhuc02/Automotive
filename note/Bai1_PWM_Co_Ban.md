# Bài 1: PWM và Timer STM32

## 🎯 Mục tiêu bài học
- Hiểu PWM và nguyên lý hoạt động
- Nắm vững cách Timer STM32 tạo xung PWM
- Hiểu mối quan hệ giữa Counter, CCR và output PWM
- Biết các khái niệm: Duty Cycle, Frequency, Period

---

## 🔍 PWM (Pulse Width Modulation)

### Định nghĩa
**PWM** là kỹ thuật điều khiển công suất bằng cách thay đổi độ rộng xung trong chu kỳ cố định.

### Nguyên lý
Thay vì thay đổi điện áp, PWM duy trì điện áp không đổi nhưng thay đổi thời gian bật/tắt để điều chỉnh công suất trung bình.

### Tín hiệu PWM cơ bản:
```
Điện áp
3.3V ┌─────┐       ┌─────┐       ┌─────┐       
     │     │       │     │       │     │       
  0V ┘     └───────┘     └───────┘     └───────
     │<-T_on->│<-T_off->│
     │<------- Period (T) ------->│
```

### Các tham số quan trọng:
```
Duty Cycle (%) = (T_on / T) × 100%
Frequency (Hz) = 1 / T
Điện áp trung bình = VCC × Duty Cycle
```

---

## ⚙️ Timer STM32 và tạo xung PWM

### Cấu trúc Timer STM32
STM32F103 có các Timer với khả năng tạo PWM:
- **TIM1**: Advanced Timer (8 channels, dead-time)
- **TIM2, TIM3, TIM4**: General Purpose Timer (4 channels)

### Kiến trúc Timer tạo PWM:
```
System Clock (72MHz)
         ↓
    ┌─────────┐
    │ Prescaler│  PSC: Chia tần số
    │   (PSC)  │
    └─────────┘
         ↓
    Timer Clock
         ↓
    ┌─────────┐
    │ Counter │  CNT: Đếm từ 0 đến ARR
    │  (CNT)  │
    └─────────┘
         ↓
    ┌─────────┐
    │Compare  │  CCR: So sánh với CNT
    │  (CCR)  │
    └─────────┘
         ↓
    PWM Output
```

### Thanh ghi chính:
- **PSC (Prescaler)**: Chia tần số clock
- **ARR (Auto Reload)**: Giá trị tối đa của counter
- **CNT (Counter)**: Bộ đếm hiện tại  
- **CCR (Capture Compare)**: Giá trị so sánh để tạo PWM

---

## 🔄 Cách STM32 tạo xung PWM

### Quy trình tạo PWM:

#### Bước 1: Thiết lập tần số PWM
```
Timer Clock = System Clock / (PSC + 1)
PWM Frequency = Timer Clock / (ARR + 1)

Ví dụ:
System Clock = 72MHz
PSC = 71 → Timer Clock = 72MHz/72 = 1MHz
ARR = 999 → PWM Freq = 1MHz/1000 = 1kHz
```

#### Bước 2: Counter đếm và so sánh
```
Counter hoạt động:
CNT: 0 → 1 → 2 → ... → ARR → 0 (reset)

So sánh với CCR:
- Khi CNT < CCR: PWM output = HIGH
- Khi CNT ≥ CCR: PWM output = LOW
```

#### Bước 3: Tạo tín hiệu PWM
```
Ví dụ: ARR = 9, CCR = 3

Counter: 0─1─2─3─4─5─6─7─8─9─0─1─2─3─4─5─6─7─8─9
PWM:     ┌─────┐         ┌─────┐
HIGH     │     │         │     │
LOW    ──┘     └─────────┘     └─────────

Duty Cycle = CCR/(ARR+1) = 3/10 = 30%
```

### Timing Diagram chi tiết:
```
System Clock:    |‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|
                 72MHz (13.9ns per cycle)

Timer Clock:     |‾‾‾‾‾‾‾|_______|‾‾‾‾‾‾‾|_______|‾‾‾‾‾‾‾|
                 1MHz (1μs per tick)

Counter (CNT):   0   1   2   3   4   5   6   7   8   9   0
                 │   │   │   │   │   │   │   │   │   │   │
CCR = 3:         │   │   │   ↑   │   │   │   │   │   │   │
                 │   │   │   │   │   │   │   │   │   │   │
PWM Output:      ┌───────────┐   │   │   │   │   │   │   │
                 │           │   │   │   │   │   │   │   │
                 ┘           └───┴───┴───┴───┴───┴───┴───┘
                 │<--HIGH--->│<--------LOW----------->│
                 30% duty cycle
```

---

## 📊 PWM Mode trong STM32

### PWM Mode 1 (thường dùng):
```
CNT < CCR: Output = HIGH
CNT ≥ CCR: Output = LOW

Minh họa (CCR = 3, ARR = 7):
Counter: 0─1─2─3─4─5─6─7─0─1─2─3─4─5─6─7
PWM:     ┌─────┐     ┌─────┐
HIGH     │     │     │     │
LOW    ──┘     └─────┘     └─────
```

### PWM Mode 2 (ngược lại):
```
CNT < CCR: Output = LOW  
CNT ≥ CCR: Output = HIGH

PWM:   ──┐     ┌─────┐     ┌─────
LOW      │     │     │     │
HIGH     └─────┘     └─────┘
```

---

## ⚡ Công thức tính toán PWM

### Tần số PWM:
```
f_PWM = f_timer / (ARR + 1)
f_timer = f_system / (PSC + 1)

→ f_PWM = f_system / [(PSC + 1) × (ARR + 1)]
```

### Duty Cycle:
```
Duty Cycle (%) = [CCR / (ARR + 1)] × 100%

Pulse Width = CCR × (1 / f_timer)
```

### Ví dụ tính toán:
```
Yêu cầu: PWM 50Hz cho servo motor
System Clock: 72MHz

Bước 1: Chọn Timer Clock = 1MHz (dễ tính)
PSC = (72MHz / 1MHz) - 1 = 71

Bước 2: Tính ARR cho 50Hz
ARR = (1MHz / 50Hz) - 1 = 19999

Bước 3: Tính CCR cho các góc servo
0°:   CCR = 1ms × 1MHz = 1000
90°:  CCR = 1.5ms × 1MHz = 1500  
180°: CCR = 2ms × 1MHz = 2000
```

---

## 🎯 Ứng dụng PWM cơ bản

### 1. Điều khiển độ sáng LED
```
Duty Cycle → Độ sáng cảm nhận:
0%   → Tắt
25%  → Mờ
50%  → Vừa
75%  → Sáng
100% → Sáng tối đa
```

### 2. Điều khiển servo motor
```
50Hz PWM:
1ms pulse  → 0°
1.5ms pulse → 90°
2ms pulse  → 180°
```

### 3. Điều khiển tốc độ motor DC
```
Duty Cycle → Tốc độ:
0%   → Dừng
30%  → Chậm
70%  → Nhanh
100% → Tốc độ tối đa
```

---

## � Ưu điểm của PWM

### 1. Hiệu quả năng lượng cao
```
Điều chỉnh tuyến tính: V_out = k × V_in (lãng phí năng lượng)
PWM: Chỉ bật/tắt → ít tổn hao, hiệu quả cao
```

### 2. Điều khiển chính xác
```
Độ phân giải PWM = ARR + 1 steps
ARR = 999 → 1000 bước điều chỉnh (0.1% precision)
```

### 3. Đơn giản và ổn định
```
Chỉ có 2 trạng thái: HIGH/LOW
→ Ít nhiễu, dễ implement, tin cậy cao
```

---

## 🔍 Tóm tắt hoạt động Timer PWM

### Chu trình hoạt động:
```
1. Clock system → PSC → Timer Clock
2. Timer Clock → Counter (CNT) đếm 0→ARR→0
3. CNT so sánh với CCR → Tạo PWM output
4. PWM output → GPIO pin → Thiết bị ngoài
```

### Công thức chính:
```
┌─────────────────────────────────────┐
│ PWM_Freq = f_sys/[(PSC+1)×(ARR+1)]  │
│ Duty = CCR/(ARR+1) × 100%           │
│ Pulse_Width = CCR/f_timer           │
└─────────────────────────────────────┘
```

---

## 📚 Câu hỏi ôn tập

1. **Timer STM32 tạo PWM như thế nào?**
2. **Vai trò của PSC, ARR, CCR trong PWM?**
3. **Tính PWM 2kHz với system clock 72MHz?**
4. **PWM Mode 1 và Mode 2 khác nhau gì?**
5. **Tại sao PWM hiệu quả hơn điều chỉnh điện áp?**

---

*Bài học tiếp theo: **Bài 2 - Thanh ghi PWM và Lập trình SPL***
