# AUTOSAR Classic Software Modules

Dự án mô phỏng và triển khai các module AUTOSAR Classic trên vi điều khiển STM32F103, bao gồm các driver MCAL cơ bản và ví dụ ứng dụng thực tế.

## 📁 Cấu Trúc Dự Án

- **0. Setup Makefile** - Template Makefile và cấu hình cơ bản
- **1. GPIO** - Triển khai Digital Input/Output
  - **DIO** - Digital I/O driver (AUTOSAR)
  - **PORT** - Port configuration driver (AUTOSAR)
- **2. ADC** - Analog to Digital Converter ⭐ **Version 3.0**
  - **Examples** - Các ví dụ SPL và AUTOSAR
  - **Automotive** - AUTOSAR ADC driver đầy đủ
  - **Documentation** - Tài liệu và update logs
- **3. PWM** - Pulse Width Modulation implementations
- **4. MiniProject_FanControl_AUTOSAR** - Dự án hoàn chỉnh điều khiển quạt 🆕
- **AutosarLib** - Thư viện AUTOSAR chung

## � Updates Gần Nhất (Tháng 7/2025)

- **ADC Version 3.0**: Sửa lỗi OneShot streaming, auto-notification, cải thiện state management
- **Testing hoàn chỉnh**: 100% pass rate cho tất cả interrupt modes
- **Mini-Project mới**: Hệ thống điều khiển quạt theo nhiệt độ với kiến trúc AUTOSAR
- **Documentation**: Cập nhật TodoList và Update logs chi tiết

## 🎯 Về Dự Án AUTOSAR

### Mục Tiêu
- **Học tập AUTOSAR Classic**: Triển khai các module MCAL cơ bản theo chuẩn AUTOSAR
- **Thực hành STM32**: Làm việc với vi điều khiển STM32F103C8T6
- **Ứng dụng thực tế**: Phát triển các dự án nhúng hoàn chỉnh

### Kiến Trúc AUTOSAR
```
Application Layer
     ↓
IoHwAb (I/O Hardware Abstraction)
     ↓
MCAL (Microcontroller Abstraction Layer)
  ├── ADC Driver
  ├── DIO Driver  
  ├── PORT Driver
  └── PWM Driver
     ↓
STM32F103 Hardware
```

## 📚 Các Module Chính

### ADC (Analog to Digital Converter)
- **AUTOSAR Driver**: Đầy đủ theo spec AUTOSAR Classic
- **SPL Examples**: Ví dụ sử dụng STM32 Standard Peripheral Library
- **Features**: OneShot, Continuous, Streaming, DMA, Interrupt modes

### Mini-Project: Fan Control
- **Chức năng**: Điều khiển quạt theo nhiệt độ (< 30°C: 0%, 30-40°C: 50%, ≥ 40°C: 100%)
- **Hardware**: STM32F103 + LM35/NTC + MOSFET + DC Fan + LED
- **Architecture**: AUTOSAR compliant với IoHwAb layer

## 🛠️ Hướng Dẫn Sử Dụng

### Build và Flash
```bash
# ADC examples
cd "2. ADC"
make example-win EXAMPLE=adc_interrupt  # Build SPL example
make all                                # Build AUTOSAR driver

# Mini-project
cd "4. MiniProject_FanControl_AUTOSAR"
make all    # Build project
make flash  # Flash to STM32
```

### VS Code Tasks
1. Ctrl+Shift+P → "Tasks: Run Task"
2. Chọn task:
   - "Build and Flash" - Build và flash ADC driver
   - "Build Example" - Build example cụ thể
   - "Clean" - Xóa build files

## � Môi Trường Phát Triển

### Tools Cần Thiết
- **ARM GCC Toolchain** - Compiler cho STM32
- **Make** - Build automation
- **OpenOCD** - Flash và debug
- **VS Code** - IDE (khuyến nghị)

### Hardware
- **STM32F103C8T6** "Blue Pill"
- **ST-Link V2** - Programmer
- **USB-Serial** converter (debug UART)

## � Tài Liệu Tham Khảo

- `2. ADC/TodoList.md` - Roadmap và task tracking
- `2. ADC/Update.md` - Version 3.0 release notes
- `4. MiniProject_FanControl_AUTOSAR/README.md` - Hướng dẫn project hoàn chỉnh

## 🚀 Bắt Đầu

1. Clone repository
2. Cài đặt ARM GCC toolchain
3. Thử ADC examples đơn giản
4. Triển khai mini-project fan control
5. Tùy chỉnh và phát triển thêm

---
**Chúc bạn học tập hiệu quả với AUTOSAR! 🚗⚡**
