# Hướng Dẫn Hoàn Chỉnh PWM cho STM32F103

*Tài liệu tổng hợp từ Bài 1 và Bài 2*

---

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

## ✅ Ưu điểm của PWM

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

## 📚 Câu hỏi ôn tập Bài 1

1. **Timer STM32 tạo PWM như thế nào?**
2. **Vai trò của PSC, ARR, CCR trong PWM?**
3. **Tính PWM 2kHz với system clock 72MHz?**
4. **PWM Mode 1 và Mode 2 khác nhau gì?**
5. **Tại sao PWM hiệu quả hơn điều chỉnh điện áp?**

---

---

# Bài 2: Thanh Ghi PWM và Lập Trình với SPL

## 🎯 Mục tiêu bài học
- Hiểu rõ các thanh ghi quan trọng trong PWM
- Nắm vững cách cấu hình PWM với STM32 SPL
- Biết cách tính toán các parameter PWM
- Xử lý các trường hợp đặc biệt trong PWM
- Hiểu mối quan hệ giữa các thanh ghi

---

## 🔧 Các Thanh Ghi PWM Quan Trọng

### **1. PSC (Prescaler Register) - Thanh ghi chia tần số**

#### Chức năng:
```
PSC = Bộ chia tần số của Timer
Mục đích: Giảm tần số clock để timer đếm chậm hơn
```

#### Công thức:
```
Timer_Clock = System_Clock / (PSC + 1)

Ví dụ:
System_Clock = 72MHz
PSC = 71
Timer_Clock = 72MHz / (71 + 1) = 72MHz / 72 = 1MHz
```

#### Minh họa:
```
System Clock (72MHz):  |‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|‾|_|
                       13.9ns mỗi xung

                           ↓ PSC = 71 (chia cho 72)

Timer Clock (1MHz):    |‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|________________
                       1μs mỗi tick timer
```

#### **Bảng PSC thông dụng:**
| PSC | Timer Clock | Độ phân giải | Ứng dụng |
|-----|-------------|--------------|----------|
| 71 | 1MHz (1μs) | Cao | Servo motor (1-2ms pulse) |
| 719 | 100kHz (10μs) | Trung bình | LED control |
| 7199 | 10kHz (100μs) | Thấp | Motor control |

---

### **2. ARR (Auto Reload Register) - Thanh ghi tự động nạp lại**

#### Chức năng:
```
ARR = Giá trị tối đa mà timer đếm đến
Timer đếm: 0 → 1 → 2 → ... → ARR → 0 (reset)
```

#### Công thức:
```
PWM_Frequency = Timer_Clock / (ARR + 1)

Ví dụ:
Timer_Clock = 1MHz
ARR = 19999
PWM_Frequency = 1MHz / (19999 + 1) = 1MHz / 20000 = 50Hz
```

#### Minh họa chu kỳ đếm:
```
Timer Counter:
     0 ──→ 1 ──→ 2 ──→ ... ──→ 19998 ──→ 19999 ──→ 0
     |                                            ↑
     |                                            |
     └────────────── Reset khi đạt ARR ──────────┘
     
     Tổng: 20000 bước = 20ms ở 1MHz = 50Hz
```

#### **Bảng ARR cho các tần số phổ biến:**
| Tần số PWM | Timer Clock | ARR | Ứng dụng |
|------------|-------------|-----|----------|
| 50Hz | 1MHz | 19999 | Servo motor |
| 1kHz | 1MHz | 999 | LED brightness |
| 2kHz | 1MHz | 499 | Audio PWM |
| 10kHz | 1MHz | 99 | Motor control |

---

### **3. CCR (Capture/Compare Register) - Thanh ghi so sánh**

#### Chức năng:
```
CCR = Giá trị để so sánh với timer counter
Quyết định thời điểm chuyển đổi PWM từ HIGH → LOW
```

#### Công thức:
```
Duty_Cycle (%) = (CCR / (ARR + 1)) × 100%

Pulse_Width = CCR × Timer_Tick
Timer_Tick = 1 / Timer_Clock
```

#### Minh họa hoạt động:
```
Timer Counter vs CCR:

Counter: 0──1──2──3──4──5──6──7──8──9──10 (ARR=9)
CCR = 3:         ↑
PWM:     ┌───────┐
HIGH     │       │                    
         │       │                    
LOW    ──┘       └────────────────────
         │<─HIGH─>│<─── LOW ──────────>
         0   1   2   3   4   5   6   7   8   9   0

Duty Cycle = 3/10 × 100% = 30%
```

#### **Bảng CCR cho Servo Motor (ARR=19999, 50Hz):**
| Góc | Pulse Width | CCR | Tính toán |
|-----|-------------|-----|-----------|
| 0° | 1ms | 1000 | 1ms × 1MHz = 1000 |
| 45° | 1.25ms | 1250 | 1.25ms × 1MHz = 1250 |
| 90° | 1.5ms | 1500 | 1.5ms × 1MHz = 1500 |
| 135° | 1.75ms | 1750 | 1.75ms × 1MHz = 1750 |
| 180° | 2ms | 2000 | 2ms × 1MHz = 2000 |

---

## ⚙️ Các Thanh Ghi Phụ Trợ

### **4. CR1 (Control Register 1)**
```
Bit quan trọng:
- CEN (bit 0): Counter Enable (Bật/tắt timer)
- DIR (bit 4): Direction (Hướng đếm: lên/xuống)
- CMS (bit 5-6): Center-aligned Mode Selection
```

### **5. CCMR (Capture/Compare Mode Register)**
```
Cấu hình chế độ PWM:
- OCxM (bit 4-6): Output Compare Mode
  * 110: PWM Mode 1
  * 111: PWM Mode 2
- OCxPE (bit 3): Output Compare Preload Enable
```

### **6. CCER (Capture/Compare Enable Register)**
```
Điều khiển output:
- CCxE (bit 0): Capture/Compare output enable
- CCxP (bit 1): Capture/Compare output polarity
```

---

## 🛠️ Hướng Dẫn Lập Trình PWM với SPL

### **Bước 1: Cấu hình GPIO**

#### Mục đích:
```
Thiết lập chân GPIO để xuất tín hiệu PWM
Chế độ: Alternate Function Push-Pull
```

#### Code pattern:
```c
// Khởi tạo cấu trúc GPIO
GPIO_InitTypeDef GPIO_InitStruct;

// Bật clock cho GPIO port
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

// Cấu hình chân PWM
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;        // Chọn chân
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;  // Alternate Function Push-Pull
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // Tốc độ cao
GPIO_Init(GPIOA, &GPIO_InitStruct);
```

#### **Bảng mapping Timer-GPIO:**
| Timer | Channel | GPIO Pin | Alternate Function |
|-------|---------|----------|--------------------|
| TIM2 | CH1 | PA0 | TIM2_CH1 |
| TIM2 | CH2 | PA1 | TIM2_CH2 |
| TIM3 | CH1 | PA6 | TIM3_CH1 |
| TIM3 | CH2 | PA7 | TIM3_CH2 |
| TIM3 | CH3 | PB0 | TIM3_CH3 |
| TIM3 | CH4 | PB1 | TIM3_CH4 |

---

### **Bước 2: Cấu hình Timer Base**

#### Mục đích:
```
Thiết lập tần số PWM và chu kỳ đếm
Quyết định PSC và ARR
```

#### Code pattern:
```c
// Khởi tạo cấu trúc Timer Base
TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

// Bật clock cho Timer
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

// Cấu hình Timer Base
TIM_TimeBaseStruct.TIM_Prescaler = 71;           // PSC = 71 (72MHz/72 = 1MHz)
TIM_TimeBaseStruct.TIM_Period = 19999;           // ARR = 19999 (50Hz)
TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up; // Đếm lên
TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;    // Không chia thêm
TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct);
```

#### **Công thức tính toán:**
```
Muốn PWM frequency = F Hz với System Clock = 72MHz:

1. Chọn Timer Clock mong muốn (thường 1MHz cho dễ tính)
   PSC = (72MHz / 1MHz) - 1 = 71

2. Tính ARR từ frequency
   ARR = (Timer_Clock / PWM_Frequency) - 1
   ARR = (1MHz / F) - 1

Ví dụ cho 50Hz:
   ARR = (1MHz / 50Hz) - 1 = 20000 - 1 = 19999
```

---

### **Bước 3: Cấu hình PWM Channel**

#### Mục đích:
```
Thiết lập chế độ PWM và duty cycle ban đầu
Cấu hình polarity và output enable
```

#### Code pattern:
```c
// Khởi tạo cấu trúc Output Compare
TIM_OCInitTypeDef TIM_OCStruct;

// Cấu hình PWM Mode 1
TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;          // PWM Mode 1
TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable; // Enable output
TIM_OCStruct.TIM_Pulse = 1500;                      // CCR = 1500 (90°)
TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;  // Active High

// Áp dụng cấu hình cho Channel 1
TIM_OC1Init(TIM2, &TIM_OCStruct);

// Enable Preload (quan trọng!)
TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
```

#### **Chi tiết các tham số PWM Channel:**

### **1. TIM_OCMode - Chế độ Output Compare**

#### **Các chế độ available:**
```c
TIM_OCMode_Timing          // Không có output, chỉ để timing
TIM_OCMode_Active          // Set active khi match
TIM_OCMode_Inactive        // Set inactive khi match  
TIM_OCMode_Toggle          // Toggle output khi match
TIM_OCMode_PWM1            // PWM Mode 1 (thường dùng)
TIM_OCMode_PWM2            // PWM Mode 2 (đảo ngược)
```

#### **So sánh PWM Mode 1 vs Mode 2:**
```
PWM Mode 1: TIM_OCMode_PWM1
Counter < CCR: OUTPUT = HIGH (Active)
Counter ≥ CCR: OUTPUT = LOW (Inactive)

Counter: 0─1─2─3─4─5─6─7─8─9 (ARR=9, CCR=3)
PWM1:    ┌─────┐
HIGH     │     │              
LOW    ──┘     └──────────────
         │ 30% │   70%   │

PWM Mode 2: TIM_OCMode_PWM2 (ngược lại)
Counter < CCR: OUTPUT = LOW (Inactive)  
Counter ≥ CCR: OUTPUT = HIGH (Active)

PWM2:  ──┐     ┌──────────────
LOW      │     │              
HIGH     └─────┘              
         │ 30% │   70%   │
```

#### **Ví dụ các chế độ khác:**
```
Toggle Mode: TIM_OCMode_Toggle
Mỗi lần CNT = CCR → Output đảo trạng thái

Counter: 0─1─2─3─4─5─6─7─8─9 (CCR=3)
Output:  ──┐   └─┐   └─┐   └─
LOW        │     │     │     
HIGH       └─────┘     └─────

Active Mode: TIM_OCMode_Active  
Khi CNT = CCR → Output = HIGH và giữ nguyên

Counter: 0─1─2─3─4─5─6─7─8─9 (CCR=3)
Output:  ───┐     ┌─────────
LOW        │     │         
HIGH       └─────┘         

Inactive Mode: TIM_OCMode_Inactive
Khi CNT = CCR → Output = LOW và giữ nguyên

Counter: 0─1─2─3─4─5─6─7─8─9 (CCR=3)
Output:  ┌───┐   ┌─────────
HIGH     │   │   │         
LOW      └───┘   └─────────
```

---

### **2. TIM_OutputState - Trạng thái đầu ra**

#### **Các giá trị:**
```c
TIM_OutputState_Enable     // Cho phép output ra pin
TIM_OutputState_Disable    // Không cho output ra pin
```

#### **Ý nghĩa và ứng dụng:**
```
ENABLE: Tín hiệu PWM được xuất ra pin GPIO
- Dùng khi muốn điều khiển thiết bị ngoài
- Pin GPIO sẽ thay đổi theo tín hiệu PWM

DISABLE: Tín hiệu PWM không xuất ra pin
- Timer vẫn chạy bình thường
- Có thể dùng để đồng bộ nội bộ
- Pin GPIO ở trạng thái idle
```

#### **Ví dụ thực tế:**
```c
// Kích hoạt PWM output - LED sẽ nhấp nháy
TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;

// Tắt PWM output - LED không sáng, timer vẫn chạy
TIM_OCStruct.TIM_OutputState = TIM_OutputState_Disable;

// Ứng dụng: Tạm thời tắt PWM mà không stop timer
if (emergency_stop) {
    TIM_CCxCmd(TIM2, TIM_Channel_1, TIM_CCx_Disable);
    // Timer vẫn chạy, có thể enable lại ngay
}
```

---

### **3. TIM_Pulse (CCR) - Giá trị so sánh**

#### **Ý nghĩa:**
```c
TIM_Pulse = CCR = Capture/Compare Register value
```

#### **Tác động đến PWM:**
```
CCR quyết định thời điểm chuyển đổi PWM:
- PWM Mode 1: HIGH khi CNT < CCR, LOW khi CNT ≥ CCR
- PWM Mode 2: LOW khi CNT < CCR, HIGH khi CNT ≥ CCR

Duty Cycle = CCR / (ARR + 1) × 100%
```

#### **Ví dụ với ARR = 999 (1kHz PWM):**
```c
// 0% duty cycle - Luôn LOW (PWM Mode 1)
TIM_OCStruct.TIM_Pulse = 0;

// 25% duty cycle - HIGH 250μs, LOW 750μs  
TIM_OCStruct.TIM_Pulse = 250;

// 50% duty cycle - HIGH 500μs, LOW 500μs
TIM_OCStruct.TIM_Pulse = 500;

// 75% duty cycle - HIGH 750μs, LOW 250μs
TIM_OCStruct.TIM_Pulse = 750;

// 100% duty cycle - Luôn HIGH
TIM_OCStruct.TIM_Pulse = 1000; // = ARR + 1
```

#### **Trường hợp đặc biệt:**
```c
// CCR = 0: 
PWM Mode 1 → Luôn LOW (0% duty)
PWM Mode 2 → Luôn HIGH (100% duty)

// CCR > ARR:
PWM Mode 1 → Luôn HIGH (100% duty) 
PWM Mode 2 → Luôn LOW (0% duty)

// CCR = ARR:
PWM Mode 1 → HIGH gần 100% (1 tick LOW)
PWM Mode 2 → LOW gần 100% (1 tick HIGH)
```

---

### **4. TIM_OCPolarity - Cực tính đầu ra**

#### **Các giá trị:**
```c
TIM_OCPolarity_High        // Active High (mặc định)
TIM_OCPolarity_Low         // Active Low (đảo ngược)
```

#### **Ý nghĩa:**
```
Polarity quyết định ý nghĩa của "Active" và "Inactive":

HIGH Polarity:
- Active = HIGH (3.3V)
- Inactive = LOW (0V)

LOW Polarity:  
- Active = LOW (0V)
- Inactive = HIGH (3.3V)
```

#### **Minh họa tác động:**
```
PWM Mode 1 + High Polarity (thường dùng):
Counter: 0─1─2─3─4─5─6─7─8─9 (CCR=3)
Output:  ┌─────┐     
3.3V     │     │     
0V     ──┘     └─────────────

PWM Mode 1 + Low Polarity (đảo ngược):
Counter: 0─1─2─3─4─5─6─7─8─9 (CCR=3)  
Output:  ──┐     ┌─────────────
0V         │     │     
3.3V       └─────┘     

→ Cùng duty cycle nhưng ngược pha!
```

#### **Ứng dụng thực tế:**

##### **LED Control với Common Anode:**
```c
// LED Common Cathode: Sáng khi HIGH
TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;
// CCR tăng → LED sáng hơn

// LED Common Anode: Sáng khi LOW  
TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
// CCR tăng → LED tối hơn
```

##### **Motor H-Bridge Control:**
```c
// Motor Direction A
TIM_OC1Init(TIM2, &TIM_OCStruct_A);
TIM_OCStruct_A.TIM_OCPolarity = TIM_OCPolarity_High;

// Motor Direction B (ngược chiều)
TIM_OC2Init(TIM2, &TIM_OCStruct_B);  
TIM_OCStruct_B.TIM_OCPolarity = TIM_OCPolarity_Low;

// Điều khiển:
// Tiến: PWM_A = duty%, PWM_B = 0%
// Lùi:  PWM_A = 0%, PWM_B = duty%
```

---

### **5. Kết hợp các tham số - Ví dụ thực tế**

#### **Ví dụ 1: Servo Motor Standard**
```c
TIM_OCInitTypeDef TIM_OCStruct;

// Servo cần PWM Mode 1, Active High
TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1;      // Mode 1
TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable; // Enable output
TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;     // Active High
TIM_OCStruct.TIM_Pulse = 1500;  // 1.5ms = 90 degrees

// Kết quả: 1.5ms HIGH, 18.5ms LOW mỗi 20ms
```

#### **Ví dụ 2: LED RGB với Common Anode**
```c
// LED Common Anode: Cần LOW để sáng
TIM_OCInitTypeDef Red_Config;
Red_Config.TIM_OCMode = TIM_OCMode_PWM1;
Red_Config.TIM_OutputState = TIM_OutputState_Enable;
Red_Config.TIM_OCPolarity = TIM_OCPolarity_Low;    // Active LOW!
Red_Config.TIM_Pulse = 200;  // CCR = 200 → 80% sáng (vì đảo)

// Duty cycle thực = (1000-200)/1000 = 80% sáng
```

#### **Ví dụ 3: Buzzer với Toggle Mode**
```c
// Tạo tần số cố định cho buzzer
TIM_OCInitTypeDef Buzzer_Config;
Buzzer_Config.TIM_OCMode = TIM_OCMode_Toggle;      // Toggle mode
Buzzer_Config.TIM_OutputState = TIM_OutputState_Enable;
Buzzer_Config.TIM_OCPolarity = TIM_OCPolarity_High;
Buzzer_Config.TIM_Pulse = 500;  // Toggle mỗi 500 ticks

// Kết quả: Square wave với freq = Timer_freq/(2×CCR)
```

#### **Ví dụ 4: Complementary PWM cho Motor**
```c
// Channel 1: PWM bình thường
TIM_OCStruct_1.TIM_OCMode = TIM_OCMode_PWM1;
TIM_OCStruct_1.TIM_OCPolarity = TIM_OCPolarity_High;
TIM_OCStruct_1.TIM_Pulse = duty_cycle;

// Channel 2: PWM ngược pha (để điều khiển H-bridge)  
TIM_OCStruct_2.TIM_OCMode = TIM_OCMode_PWM2;       // Mode 2!
TIM_OCStruct_2.TIM_OCPolarity = TIM_OCPolarity_High;
TIM_OCStruct_2.TIM_Pulse = duty_cycle;

// Kết quả: 2 PWM ngược pha nhau hoàn toàn
```

---

### **6. Bảng tổng hợp tham số**

| Tham số | Giá trị | Ý nghĩa | Ứng dụng |
|---------|---------|---------|----------|
| **OCMode** | PWM1 | CNT<CCR: HIGH | Standard PWM |
| | PWM2 | CNT<CCR: LOW | Inverted PWM |
| | Toggle | Đảo khi CNT=CCR | Square wave |
| | Active | HIGH khi CNT=CCR | Trigger |
| | Inactive | LOW khi CNT=CCR | Reset signal |
| **OutputState** | Enable | Output ra pin | Normal operation |
| | Disable | Không output | Disable PWM |
| **Pulse (CCR)** | 0 | 0% hoặc 100% | Min/Max duty |
| | 0<CCR<ARR | Variable duty | Normal PWM |
| | ≥ARR | 100% hoặc 0% | Max/Min duty |
| **Polarity** | High | Active = HIGH | Common cathode |
| | Low | Active = LOW | Common anode |

---

### **7. Debug các tham số PWM**

#### **Kiểm tra tín hiệu không đúng:**
```c
// Problem: LED không sáng với common anode
// Nguyên nhân: Dùng nhầm polarity

// SAI:
TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High; // LED không sáng
TIM_OCStruct.TIM_Pulse = 800; // 80% duty → 80% HIGH → LED tối

// ĐÚNG:  
TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;  // LED sáng
TIM_OCStruct.TIM_Pulse = 800; // 80% duty → 20% HIGH → LED sáng
```

#### **Kiểm tra duty cycle không như mong đợi:**
```c
// Problem: Servo không quay đúng góc
// Nguyên nhân: Dùng nhầm PWM Mode

// SAI:
TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2; // Đảo ngược
TIM_OCStruct.TIM_Pulse = 1500; // Expecting 1.5ms → Actually 18.5ms!

// ĐÚNG:
TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM1; // Standard
TIM_OCStruct.TIM_Pulse = 1500; // Correct 1.5ms pulse
```

---

### **Bước 4: Khởi động Timer**

#### Code pattern:
```c
// Bật Timer để bắt đầu tạo PWM
TIM_Cmd(TIM2, ENABLE);
```

---

## 📊 Ví Dụ Cụ Thể

### **Ví dụ 1: LED Breathing Effect (1kHz PWM)**

#### Yêu cầu:
```
- PWM 1kHz cho LED fade in/out mượt
- Duty cycle từ 0% đến 100%
- Độ phân giải cao (1000 bước)
```

#### Tính toán:
```
Target: 1kHz PWM với 1000 bước điều chỉnh

PSC = 72 - 1 = 71    (Timer clock = 1MHz)
ARR = 1000 - 1 = 999 (PWM freq = 1MHz/1000 = 1kHz)

Duty cycle control:
CCR = 0    → 0%   duty cycle
CCR = 250  → 25%  duty cycle  
CCR = 500  → 50%  duty cycle
CCR = 999  → 99.9% duty cycle
```

#### Minh họa breathing effect:
```
Duty Cycle theo thời gian:
100% ┌─────┐         ┌─────┐
     │     │         │     │
 50% │     │    ┌────┤     ├────┐
     │     │   ╱     │     │     ╲
  0% ┘     ╲__╱      │     │      ╲__╱
     0    1    2    3    4    5    6    (giây)
     
Fade In → Max → Fade Out → Min → Repeat
```

---

### **Ví dụ 2: Servo Motor Control (50Hz PWM)**

#### Yêu cầu:
```
- PWM 50Hz cho servo standard
- Pulse width: 1ms-2ms (0°-180°)
- Độ chính xác: 1μs
```

#### Tính toán:
```
Target: 50Hz PWM với độ phân giải 1μs

PSC = 72 - 1 = 71     (Timer clock = 1MHz = 1μs resolution)
ARR = 20000 - 1 = 19999 (PWM freq = 1MHz/20000 = 50Hz)

Servo position control:
0°:   CCR = 1000  (1ms pulse)
45°:  CCR = 1250  (1.25ms pulse)
90°:  CCR = 1500  (1.5ms pulse)  
135°: CCR = 1750  (1.75ms pulse)
180°: CCR = 2000  (2ms pulse)
```

#### Minh họa servo timing:
```
50Hz PWM (20ms period):

0° Position:
┌─┐                 ┌─┐                 
│ │                 │ │                 
┘ └─────────────────┘ └─────────────────
1ms    19ms         1ms    19ms         

90° Position:  
┌───┐               ┌───┐               
│   │               │   │               
┘   └───────────────┘   └───────────────
1.5ms  18.5ms       1.5ms  18.5ms       

180° Position:
┌────┐              ┌────┐              
│    │              │    │              
┘    └──────────────┘    └──────────────
 2ms    18ms         2ms    18ms        
```

---

### **Ví dụ 3: Multi-Channel RGB LED (2kHz PWM)**

#### Yêu cầu:
```
- 3 channel PWM cho RGB LED
- PWM 2kHz (tai người không nghe thấy)
- Synchronized timing cho tất cả channels
```

#### Tính toán:
```
Target: 2kHz PWM cho 3 channels

PSC = 36 - 1 = 35     (Timer clock = 2MHz)
ARR = 1000 - 1 = 999  (PWM freq = 2MHz/1000 = 2kHz)

Color mixing examples:
Red:   CCR1 = 800  (80% brightness)
Green: CCR2 = 400  (40% brightness)  
Blue:  CCR3 = 200  (20% brightness)
→ Result: Warm white color
```

#### Minh họa RGB timing:
```
Tất cả channels đồng bộ với nhau:

Timer:   0────250────500────750────999────0
         │    │      │      │      │      │
Red:     ┌──────────────────────────────┐  
Green:   ┌──────────────┐                  
Blue:    ┌──────┐                          

Kết quả: Màu trộn theo tỷ lệ R:G:B = 80:40:20
```

---

## ⚠️ Trường Hợp Đặc Biệt

### **1. Center-Aligned PWM**

#### Khi nào sử dụng:
```
- Motor control chính xác cao
- Giảm harmonics trong tín hiệu
- Đồng bộ hóa nhiều PWM channels
```

#### Hoạt động:
```
Up Mode (thường):       Center-Aligned Mode:
0→1→2→3→4→5→0          0→1→2→3→4→5→4→3→2→1→0

PWM Up Mode:           PWM Center-Aligned:
┌───┐     ┌───┐        ┌─────┐   ┌─────┐
│   │     │   │        │     │   │     │
┘   └─────┘   └─       ┘     └───┘     └─
CCR=2, ARR=5           CCR=2, ARR=5
Duty = 2/6 = 33%       Duty = 4/10 = 40%
```

#### Cấu hình:
```c
TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
```

---

### **2. Dead Time cho Motor Control**

#### Mục đích:
```
Tránh short-circuit trong H-bridge
Tạo khoảng trống giữa HIGH và LOW
```

#### Minh họa:
```
Không có Dead Time (NGUY HIỂM):
PWM_A: ┌─────┐     ┌─────┐
       │     │     │     │
       ┘     └─────┘     └─

PWM_B: ─┐     ┌─────┐     ┌─
        │     │     │     │
        └─────┘     └─────┘
        ↑ Nguy cơ short-circuit

Có Dead Time (AN TOÀN):
PWM_A: ┌───┐       ┌───┐  
       │   │       │   │  
       ┘   └───────┘   └──

PWM_B: ────┐   ┌───────┐  
           │   │       │  
           └───┘       └──
           ↑ Dead time
```

#### Cấu hình:
```c
// Chỉ có advanced timers (TIM1, TIM8)
TIM_BDTRInitTypeDef TIM_BDTRStruct;
TIM_BDTRStruct.TIM_DeadTime = 50; // 50 timer ticks dead time
TIM_BDTRConfig(TIM1, &TIM_BDTRStruct);
```

---

### **3. PWM Input Capture**

#### Mục đích:
```
Đo duty cycle và frequency của tín hiệu PWM đầu vào
Ví dụ: Đọc tín hiệu từ RC receiver
```

#### Hoạt động:
```
Input PWM:  ┌─────┐     ┌─────┐     ┌─────┐
            │     │     │     │     │     │
          ──┘     └─────┘     └─────┘     └──
            ↑     ↑     ↑     ↑     ↑     ↑
            │     │     │     │     │     │
Capture:   T1    T2    T3    T4    T5    T6

Period = T3 - T1 = T5 - T3
Pulse Width = T2 - T1 = T4 - T3  
Duty Cycle = Pulse Width / Period
```

#### Cấu hình:
```c
// Cấu hình Input Capture
TIM_ICInitTypeDef TIM_ICStruct;
TIM_ICStruct.TIM_Channel = TIM_Channel_1;
TIM_ICStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
TIM_ICStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
TIM_ICStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
TIM_ICStruct.TIM_ICFilter = 0;
TIM_PWMIConfig(TIM2, &TIM_ICStruct);
```

---

### **4. One-Pulse Mode**

#### Mục đích:
```
Tạo một xung duy nhất với độ rộng xác định
Ứng dụng: Trigger, delay timing
```

#### Hoạt động:
```
Trigger ──┐
          │
          └────────────────────────

Output:     ┌──────────┐
            │          │
         ───┘          └─────────
            │<-Pulse->│
            CCR width  
```

#### Cấu hình:
```c
// Enable One Pulse Mode
TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);

// Trigger
TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
```

---

## 📐 Công Thức Tính Toán Nhanh

### **Bảng tham khảo nhanh:**

| Mục đích | System Clock | PSC | Timer Clock | ARR | PWM Freq |
|----------|--------------|-----|-------------|-----|----------|
| Servo Control | 72MHz | 71 | 1MHz | 19999 | 50Hz |
| LED Control | 72MHz | 71 | 1MHz | 999 | 1kHz |
| Audio PWM | 72MHz | 35 | 2MHz | 99 | 20kHz |
| Motor Control | 72MHz | 7199 | 10kHz | 9 | 1kHz |

### **Công thức tổng quát:**
```
1. Chọn PWM frequency mong muốn: F (Hz)
2. Chọn độ phân giải mong muốn: N (steps)
3. Tính Timer Clock: T_clk = F × N
4. Tính PSC: PSC = (System_Clock / T_clk) - 1
5. Tính ARR: ARR = N - 1
6. Duty cycle: CCR = (Duty% × N) / 100
```

---

## 🔍 Debug và Troubleshooting

### **Vấn đề thường gặp:**

#### **1. PWM không có tín hiệu:**
```
Kiểm tra:
☐ GPIO cấu hình đúng chế độ AF_PP
☐ Timer clock được enable
☐ TIM_Cmd(TIMx, ENABLE) đã gọi
☐ Pin mapping đúng timer channel
```

#### **2. Tần số PWM không đúng:**
```
Kiểm tra:
☐ PSC và ARR tính toán đúng
☐ System clock đúng (72MHz cho STM32F103)
☐ Timer clock source (APB1 hay APB2)
```

#### **3. Duty cycle không chính xác:**
```
Kiểm tra:
☐ CCR ≤ ARR
☐ OCPreload được enable
☐ PWM Mode 1 hay 2
☐ Polarity setting
```

---

## 🎯 Bài Tập Thực Hành

### **Bài tập 1:** Tính toán PWM parameters
```
Yêu cầu: PWM 500Hz với 2000 steps resolution
System clock: 72MHz
→ Tính PSC và ARR?
```

### **Bài tập 2:** Servo sweep
```
Tạo PWM điều khiển servo quét từ 0° đến 180° và về lại
Thời gian quét: 2 giây mỗi chiều
→ Tính CCR cho mỗi bước?
```

### **Bài tập 3:** RGB LED color mixing
```
Tạo hiệu ứng rainbow cho RGB LED
6 màu cơ bản trong 3 giây
→ Thiết kế sequence cho 3 channels?
```

---

## 📚 Câu hỏi ôn tập tổng hợp

### **Lý thuyết:**
1. So sánh PWM Mode 1 và Mode 2, khi nào dùng mỗi mode?
2. Giải thích ý nghĩa của TIM_OCPolarity và ứng dụng thực tế?
3. Tại sao cần enable TIM_OCPreload trong cấu hình PWM?
4. Sự khác biệt giữa TIM_OutputState_Enable và Disable?

### **Tính toán:**
1. Tính PSC và ARR để có PWM 2.5kHz với độ phân giải 800 steps
2. Với servo motor, tính CCR để có pulse width 1.25ms (góc 45°)
3. RGB LED cần duty cycle 30%, 60%, 90% cho R-G-B, tính CCR?

### **Thực hành:**
1. Debug: PWM có tần số đúng nhưng duty cycle luôn 0%?
2. Debug: Servo không quay đến đúng góc yêu cầu?
3. Debug: LED RGB không hiển thị màu đúng với common anode?

---

*Hết phần tổng hợp Bài 1 và Bài 2 - PWM cho STM32F103*
