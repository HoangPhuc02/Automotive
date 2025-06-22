# Makefile để build dự án Blink LED cho STM32F103 (no HAL/SPL)

# Tên file đầu ra
TARGET = blinkled

# Thư mục chứa các file biên dịch(build)
BUILD_DIR = build

$(BUILD_DIR): mkdir -p $(BUILD_DIR)
# SRC_DIR

# Trình biên dịch và các flags
# CC chỉ định biến biên dịch
CC      = arm-none-eabi-gcc
CFLAGS  = -mcpu=cortex-m3 -mthumb -O0 -g -Wall -ffreestanding -nostdlib

LD = stm32f103.ld

# Các flags liên quan đến liên kết
LDFLAGS = -T$(LD) -nostdlib -Wl,--gc-sections
# , -Map=$(BUILD_DIR)/$(TARGET).map
# LDFLAGS = -T stm32f103.ld -nostdlib -Wl,--gc-sections -Map $(TARGET).map

# Danh sách file nguồn

# Danh sách file C
SRCS_C  = \
Core/Src/main.c \
$(wildcard Driver/SPL/Src/*.c)  
# Driver/SPL/Src/*.c

# Danh sách file ASM
SRCS_S  = startup_stm32f103.s


OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS_C)) $(patsubst %.s,$(BUILD_DIR)/%.o,$(SRCS_S))
# OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS_C:.c=.o))) $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS_S:.s=.o)))
# OBJS = $(patsubst %.c,build/%.o,$(notdir $(C_SOURCES)))
# AS includes
AS_INCLUDES = 

# C defines
C_DEFS =  \
-DSTM32F10X_LD\
-DUSE_FULL_ASSERT
# -DUSE_HAL_DRIVER 


# C includes
C_INCLUDES =  \
-ICore/Inc \
-IDriver/SPL/inc \
-IDriver/CMSIS/Include 

CFLAGS += -mcpu=cortex-m3 $(C_DEFS) $(C_INCLUDES) -O0 -Wall 
# -fdata-sections -ffunction-sections

# Mục tiêu mặc định: biên dịch tất cả
# all: $(TARGET).bin
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

# # Compile file C
# %.o: %.c
# $(CC) $(CFLAGS) -c $< -o $@
# # $(BUILD_DIR)/%.o: %.c
#$(CC) $(CFLAGS) -c -o $@ $<


# # Assemble file ASM
# %.o: %.s
# $(CC) $(CFLAGS) -c $< -o $@

# Compile C files vào build folder
$(BUILD_DIR)/%.o: %.c 
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble Assembly files vào build folder  
$(BUILD_DIR)/%.o: */%.s 
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# Link thành ELF
$(BUILD_DIR)/$(TARGET).elf: $(OBJS) stm32f103.ld
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

# Tạo file .bin từ .elf
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

# Nạp firmware vào Blue Pill (dùng file .bin)
flash: $(BUILD_DIR)/$(TARGET).bin
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(BUILD_DIR)/$(TARGET).bin 0x08000000 verify reset exit"

# Xóa file tạm
clean:
	-rm -fR $(BUILD_DIR)/
	rm -f *.o 

.PHONY: all clean flash
