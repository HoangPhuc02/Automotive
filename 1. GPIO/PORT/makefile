# Makefile để build dự án Blink LED cho STM32F103 (no HAL/SPL)

# Tên file đầu ra
TARGET = PORT
BUILD_DIR = build


# Thư mục chứa file nguồn
SPL_DIR = SPL
BSW_DIR = Automotive/BSW
MCAL_DIR = $(BSW_DIR)/MCAL
COMM_DIR = Automotive/Common
CONF_DIR = Automotive/Config

# Include Paths
INCLUDES = -I$(MCAL_DIR)/DIO/Inc \
		   -I$(MCAL_DIR)/PORT/Inc \
           -I$(SPL_DIR)/Inc \
		   -ICore \
           -I$(COMM_DIR)/Inc \
           -I$(CONF_DIR)

# Danh sách file nguồn
DIO_SOURCES = $(wildcard $(MCAL_DIR)/DIO/Src/*.c)
PORT_SOURCES = $(wildcard $(MCAL_DIR)/PORT/Src/*.c)
BSW_SOURCES = $(DIO_SOURCES) $(PORT_SOURCES)

SPL_SOURCES = $(wildcard $(SPL_DIR)/Src/*.c)
SRCS_C  = main.c $(BSW_SOURCES) $(SPL_SOURCES) 
SRCS_S  = startup_stm32f103.s
# OBJS    = $(SRCS_C:.c=.o) $(SRCS_S:.s=.o)

OBJS    = $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SRCS_C))) \
          $(patsubst %.s,$(BUILD_DIR)/%.o,$(notdir $(SRCS_S)))

# Trình biên dịch và các flags
CC      = arm-none-eabi-gcc
CFLAGS  = -mcpu=cortex-m3 -mthumb -O0 -g -Wall -ffreestanding -nostdlib \
			$(INCLUDES) \
			-DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
LDFLAGS = -T stm32f103.ld -nostdlib -Wl,--gc-sections

# ✅ CHANGE: Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


# Mục tiêu mặc định
all: $(BUILD_DIR)/$(TARGET).bin

# ✅ CHANGE: Compile file C into build directory
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ✅ CHANGE: Handle sources in subdirectories
$(BUILD_DIR)/%.o: $(MCAL_DIR)/DIO/Src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(MCAL_DIR)/PORT/Src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SPL_DIR)/Src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ✅ CHANGE: Assemble file ASM into build directory
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ✅ CHANGE: Link thành ELF in build directory
$(BUILD_DIR)/$(TARGET).elf: $(OBJS) stm32f103.ld
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

# ✅ CHANGE: Tạo file .bin từ .elf in build directory
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

# ✅ CHANGE: Tạo file .hex từ .elf (bonus)
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	arm-none-eabi-objcopy -O ihex $< $@

# ✅ CHANGE: Update flash target
flash: $(BUILD_DIR)/$(TARGET).bin
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(BUILD_DIR)/$(TARGET).bin 0x08000000 verify reset exit"

# ✅ CHANGE: Clean build directory
clean:
	rm -rf $(BUILD_DIR)/*

# Add hex target
hex: $(BUILD_DIR)/$(TARGET).hex

.PHONY: all clean flash hex
