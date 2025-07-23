# Hệ Thống Điều Khiển Quạt Làm Mát Theo Nhiệt Độ
**AUTOSAR Classic Mini-Project**

## 📖 Mô Tả Dự Án

Dự án này triển khai một hệ thống điều khiển quạt DC thông minh sử dụng kiến trúc AUTOSAR Classic. Hệ thống đọc nhiệt độ từ cảm biến analog và tự động điều chỉnh tốc độ quạt theo các ngưỡng nhiệt độ đã định.

### 🎯 Mục Tiêu Học Tập
- Hiểu và triển khai thực tế các module AUTOSAR cơ bản (MCAL và IoHwAb)
- Làm quen cách cấu hình, sử dụng các MCAL Driver: Port, Dio, Adc, Pwm
- Thiết kế và phát triển module I/O Hardware Abstraction (IoHwAb)
- Thực hành triển khai hệ thống nhúng đơn giản với STM32F103C8T6

## 🔧 Chức Năng Hệ Thống

### Logic Điều Khiển
- **< 30°C**: Quạt tắt (0% PWM)
- **30°C - 40°C**: Quạt chạy 50% (50% PWM)  
- **≥ 40°C**: Quạt chạy 100% (100% PWM)
- **LED Status**: Sáng khi quạt hoạt động, tắt khi quạt ngừng

### Ánh Xạ Phần Cứng
| Chân STM32 | Chức Năng | Cấu Hình |
|------------|-----------|----------|
| PA0 | ADC Input (Cảm biến nhiệt độ) | Analog Input |
| PA8 | PWM Output (Điều khiển quạt) | Timer1 CH1, 10kHz |
| PC13 | GPIO Output (LED status) | Digital Output |

## 🛠️ Danh Sách Linh Kiện

| Linh Kiện | Số Lượng | Chức Năng |
|-----------|----------|-----------|
| STM32F103C8T6 "Blue Pill" | 1 | Vi điều khiển chính |
| LM35DZ hoặc NTC10K | 1 | Cảm biến nhiệt độ |
| MOSFET IRLZ44N/IRFZ44N | 1 | Điều khiển PWM quạt |
| Quạt DC 12V | 1 | Thiết bị làm mát |
| LED 5mm + R220Ω | 1 | Báo trạng thái |
| Breadboard + Jumper wires | 1 set | Lắp ráp mạch |

## 📁 Cấu Trúc Dự Án

```
Project_FanControl_AUTOSAR/
├── main.c                    # Ứng dụng chính
├── Makefile                  # Build configuration
├── README.md                 # Tài liệu dự án
├── schematic.png            # Sơ đồ mạch (cần vẽ)
│
├── IoHwAb/                  # ECU Abstraction Layer
│   ├── IoHwAb.c            # Triển khai IoHwAb API
│   └── IoHwAb.h            # Interface IoHwAb
│
├── MCAL/                   # Microcontroller Abstraction Layer
│   ├── Port/
│   │   ├── Port.c          # Port Driver implementation
│   │   └── Port.h          # Port Driver interface
│   ├── Dio/
│   │   ├── Dio.c           # Digital I/O Driver
│   │   └── Dio.h           # Digital I/O interface
│   ├── Adc/
│   │   ├── Adc.c           # ADC Driver implementation
│   │   └── Adc.h           # ADC Driver interface
│   └── Pwm/
│       ├── Pwm.c           # PWM Driver implementation
│       └── Pwm.h           # PWM Driver interface
│
└── Config/                 # Configuration Files
    ├── Port_Cfg.c          # Port configuration
    ├── Dio_Cfg.c           # DIO configuration
    ├── Adc_Cfg.c           # ADC configuration
    └── Pwm_Cfg.c           # PWM configuration
```

```
/* Hàm khởi tạo IoHwAb module */
void IoHwAb_Init(void);

/* Đọc nhiệt độ hiện tại (trả về giá trị nhiệt độ theo °C) */
uint16 IoHwAb_ReadTemperature(void);

/* Thiết lập tốc độ quạt (duty cycle %) */
void IoHwAb_SetFanDuty(uint8 percent);

/* Điều khiển trạng thái LED báo */
void IoHwAb_SetLed(boolean state);
```

## 🚀 Hướng Dẫn Triển Khai

### Bước 1: Thiết Kế Sơ Đồ Mạch
- [ ] Vẽ sơ đồ kết nối STM32 với LM35/NTC
- [ ] Thiết kế mạch điều khiển MOSFET cho quạt
- [ ] Kết nối LED báo trạng thái
- [ ] Lưu sơ đồ vào file `schematic.png`

### Bước 2: Cấu Hình MCAL Drivers

#### 2.1 Port Driver
- [ ] Cấu hình PA0 là Analog Input (ADC)
- [ ] Cấu hình PA8 là PWM Output (Timer1 CH1)
- [ ] Cấu hình PC13 là GPIO Output (LED)

#### 2.2 ADC Driver  
- [ ] Thiết lập ADC1 single-channel mode
- [ ] Cấu hình DMA hoặc Interrupt cho việc đọc liên tục
- [ ] Viết hàm chuyển đổi giá trị ADC sang nhiệt độ (°C)

#### 2.3 PWM Driver
- [ ] Cấu hình Timer1 Channel 1, tần số 10kHz
- [ ] API điều chỉnh duty cycle (0-100%)
- [ ] Đảm bảo PWM hoạt động ổn định

#### 2.4 DIO Driver
- [ ] API điều khiển LED (On/Off)
- [ ] Hỗ trợ read/write GPIO pins

### Bước 3: Triển Khai IoHwAb Module
- [ ] Triển khai `IoHwAb_Init()`
- [ ] Triển khai `IoHwAb_ReadTemperature()`
- [ ] Triển khai `IoHwAb_SetFanDuty()`
- [ ] Triển khai `IoHwAb_SetLed()`
- [ ] Đảm bảo chỉ gọi MCAL APIs, không có logic ứng dụng

### Bước 4: Viết Ứng Dụng
- [ ] Khởi tạo hệ thống trong `main()`
- [ ] Vòng lặp chính đọc nhiệt độ
- [ ] Logic điều khiển quạt theo ngưỡng nhiệt độ
- [ ] Cập nhật trạng thái LED

## 🧪 Test Cases

### Test Case 1: Nhiệt độ thấp (< 30°C)
- **Input**: Nhiệt độ = 25°C
- **Expected**: Quạt tắt (0% PWM), LED tắt

### Test Case 2: Nhiệt độ trung bình (30-40°C)
- **Input**: Nhiệt độ = 35°C  
- **Expected**: Quạt 50% PWM, LED sáng

### Test Case 3: Nhiệt độ cao (≥ 40°C)
- **Input**: Nhiệt độ = 45°C
- **Expected**: Quạt 100% PWM, LED sáng

### Test Case 4: Biến đổi nhiệt độ
- **Input**: Nhiệt độ thay đổi 25°C → 35°C → 45°C → 25°C
- **Expected**: Quạt thay đổi tương ứng 0% → 50% → 100% → 0%

## 📚 APIs Tham Khảo

### IoHwAb APIs
```c
/* Khởi tạo IoHwAb module */
void IoHwAb_Init(void);

/* Đọc nhiệt độ hiện tại (°C) */
uint16 IoHwAb_ReadTemperature(void);

/* Thiết lập tốc độ quạt (0-100%) */
void IoHwAb_SetFanDuty(uint8 percent);

/* Điều khiển LED báo trạng thái */
void IoHwAb_SetLed(boolean state);
```

### Công Thức Chuyển Đổi Nhiệt Độ

#### LM35 Sensor
```c
// LM35: 10mV/°C, Vref = 3.3V, ADC 12-bit (0-4095)
// Temperature (°C) = (ADC_Value * 3.3 * 100) / 4095
uint16 temperature = (adc_value * 330) / 4095;
```

#### NTC 10K Sensor
```c
// NTC: Sử dụng bảng lookup hoặc công thức Steinhart-Hart
// Cần điện trở pull-up và tính toán phức tạp hơn
```

## 🔨 Build Instructions

```bash
# Build project
make all

# Flash to STM32
make flash

# Clean build files  
make clean
```

## 📝 Lưu Ý Quan Trọng

### AUTOSAR Compliance
- **Application** chỉ gọi IoHwAb APIs, không trực tiếp gọi MCAL
- **IoHwAb** chỉ gọi MCAL APIs, không chứa logic ứng dụng
- **MCAL** chỉ thao tác với hardware registers

### Safety Considerations
- Kiểm tra giá trị ADC hợp lệ trước khi chuyển đổi
- Giới hạn duty cycle PWM trong phạm vi an toàn
- Thêm timeout cho các thao tác hardware
- Xử lý lỗi khi cảm biến không hoạt động

### Performance Tips
- Sử dụng DMA cho ADC để giảm tải CPU
- Implement moving average filter cho giá trị nhiệt độ
- Thêm hysteresis để tránh oscillation tại ngưỡng chuyển đổi

## 🐛 Known Issues & Troubleshooting

### Issue 1: ADC Noise
**Triệu chứng**: Giá trị nhiệt độ nhảy liên tục
**Giải pháp**: Thêm capacitor filter, implement software filter

### Issue 2: PWM không hoạt động
**Triệu chứng**: Quạt không quay hoặc quay liên tục
**Giải pháp**: Kiểm tra cấu hình Timer, GPIO alternate function

### Issue 3: LED không sáng
**Triệu chứng**: LED không phản ứng với trạng thái quạt
**Giải pháp**: Kiểm tra cấu hình GPIO, logic điều khiển

## 📖 Tài Liệu Tham Khảo

- [AUTOSAR Classic Platform Specification](https://www.autosar.org/)
- [STM32F103C8T6 Reference Manual](https://www.st.com/resource/en/reference_manual/rm0008-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [LM35 Temperature Sensor Datasheet](https://www.ti.com/lit/ds/symlink/lm35.pdf)

## 👥 Contributors

- **Tên học viên**: [Điền tên]
- **Ngày bắt đầu**: [Điền ngày]
- **Ngày hoàn thành**: [Điền ngày]

---
**Chúc bạn học tập hiệu quả và triển khai thành công dự án! 🚀**
