# Makefile để build dự án Blink LED cho STM32F103 (no HAL/SPL)

# Tên file đầu ra
TARGET = DIO



# Thư mục chứa file nguồn
SPL_DIR = SPL
BSW_DIR = Automotive/BSW
MCAL_DIR = $(BSW_DIR)/MCAL
COMM_DIR = Automotive/Common
CONF_DIR = Automotive/Config

# Include Paths
INCLUDES = -I$(MCAL_DIR)/Dio/Inc \
           -I$(SPL_DIR)/Inc \
		   -ICore \
           -I$(COMM_DIR)/Inc \
           -I$(CONF_DIR)

# Danh sách file nguồn
DIO_SOURCES = $(wildcard $(MCAL_DIR)/Dio/Src/*.c)
SPL_SOURCES = $(wildcard $(SPL_DIR)/Src/*.c)
SRCS_C  = main.c $(DIO_SOURCES) $(SPL_SOURCES) 
SRCS_S  = startup_stm32f103.s
OBJS    = $(SRCS_C:.c=.o) $(SRCS_S:.s=.o)


# Trình biên dịch và các flags
CC      = arm-none-eabi-gcc
CFLAGS  = -mcpu=cortex-m3 -mthumb -O0 -g -Wall -ffreestanding -nostdlib \
			$(INCLUDES) \
			-DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
LDFLAGS = -T stm32f103.ld -nostdlib -Wl,--gc-sections
# Mục tiêu mặc định
all: $(TARGET).bin

# Compile file C
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble file ASM
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

# Link thành ELF
$(TARGET).elf: $(OBJS) stm32f103.ld
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

# Tạo file .bin từ .elf
$(TARGET).bin: $(TARGET).elf
	arm-none-eabi-objcopy -O binary $< $@

# Nạp firmware vào Blue Pill (dùng file .bin)
flash: $(TARGET).bin
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(TARGET).bin 0x08000000 verify reset exit"

# Xóa file tạm
clean:
	rm -f *.o *.elf *.bin SPL/src/*.o

.PHONY: all clean flash
