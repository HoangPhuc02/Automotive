# =====================================================
#  Makefile for Fan Control AUTOSAR Project
# =====================================================
#  Project: Temperature-Controlled Fan System
#  Target: STM32F103C8T6
#  Toolchain: ARM GCC
# =====================================================

# Project name
PROJECT = FanControl

# Target MCU
MCU = STM32F103C8T6

# Toolchain
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE = $(PREFIX)size

# Directories
SRC_DIR = .
MCAL_DIR = MCAL
IOHWAB_DIR = IoHwAb
CONFIG_DIR = Config
COMM_DIR  = Common
BUILD_DIR = build
SPL_DIR = SPL

DIO_SOURCES = $(wildcard $(MCAL_DIR)/Dio/Src/*.c)
PORT_SOURCES = $(wildcard $(MCAL_DIR)/Port/Src/*.c)
ADC_SOURCES = $(wildcard $(MCAL_DIR)/Adc/Src/*.c)
PWM_SOURCES = $(wildcard $(MCAL_DIR)/Pwm/Src/*.c)
BSW_SOURCES = $(DIO_SOURCES) $(PORT_SOURCES) $(ADC_SOURCES) $(PWM_SOURCES)
CFG_SOURCES = $(wildcard $(CONFIG_DIR)/Src/*.c)
SPL_SOURCES = $(wildcard $(SPL_DIR)/Src/*.c)
# Source files
SOURCES = main.c \
		isr.c\
		 startup_stm32f103.s \
		 $(BSW_SOURCES) \
		 $(CFG_SOURCES) \
		 $(SPL_SOURCES) \
        $(IOHWAB_DIR)/IoHwAb.c


# Include directories
INCLUDES = -I. \
		   -ICore \
           -I$(SPL_DIR)/Inc \
           -I$(IOHWAB_DIR) \
		   -I$(MCAL_DIR)/Dio/Inc \
		   -I$(MCAL_DIR)/Port/Inc \
		   -I$(MCAL_DIR)/Adc/Inc \
		   -I$(MCAL_DIR)/Pwm/Inc \
		   -I$(MCAL_DIR)/Det/Inc \
		   -I$(COMM_DIR)/Inc \
           -I$(CONFIG_DIR)/Inc

# Compiler flags
CFLAGS  = -mcpu=cortex-m3 -mthumb -O0 -g -Wall -ffreestanding -nostdlib \
			$(INCLUDES) \
			-DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER

# Linker flags
LDFLAGS = -T stm32f103.ld -nostdlib -Wl,--gc-sections

# Generate object file names
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)
OBJECTS := $(OBJECTS:%.s=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).bin $(BUILD_DIR)/$(PROJECT).hex

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(IOHWAB_DIR)
	mkdir -p $(BUILD_DIR)/$(MCAL_DIR)/Port/Src
	mkdir -p $(BUILD_DIR)/$(MCAL_DIR)/Dio/Src
	mkdir -p $(BUILD_DIR)/$(MCAL_DIR)/Adc/Src
	mkdir -p $(BUILD_DIR)/$(MCAL_DIR)/Pwm/Src
	mkdir -p $(BUILD_DIR)/$(CONFIG_DIR)/Src
	mkdir -p $(BUILD_DIR)/$(SPL_DIR)/Src
# Compile source files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble assembly files
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	@echo "Assembling $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files
$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS)
	@echo "Linking $(PROJECT).elf"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

# Generate binary file
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	@echo "Creating $(PROJECT).bin"
	$(OBJCOPY) -O binary $< $@

# Generate hex file
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	@echo "Creating $(PROJECT).hex"
	$(OBJCOPY) -O ihex $< $@

# Flash to target (requires st-link)
flash: $(BUILD_DIR)/$(PROJECT).bin
	@echo "Flashing to STM32F103C8T6"
	openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "program $(BUILD_DIR)/$(PROJECT).bin 0x08000000 verify reset exit"


# Clean build files
clean:
	@echo "Cleaning build files"
	rm -rf $(BUILD_DIR)

# Show disassembly
disasm: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJDUMP) -d $< | less

# Show memory usage
size: $(BUILD_DIR)/$(PROJECT).elf
	$(SIZE) -A $<

# Debug with GDB (requires st-link and openocd)
debug: $(BUILD_DIR)/$(PROJECT).elf
	@echo "Starting debug session"
	arm-none-eabi-gdb $< -ex "target remote localhost:3333"

# Help
help:
	@echo "Available targets:"
	@echo "  all      - Build all files (default)"
	@echo "  clean    - Remove build files"
	@echo "  flash    - Flash binary to target"
	@echo "  size     - Show memory usage"
	@echo "  disasm   - Show disassembly"
	@echo "  debug    - Start GDB debug session"
	@echo "  help     - Show this help"

# Phony targets
.PHONY: all clean flash size disasm debug help

# =====================================================
#  Build Instructions:
# =====================================================
# 
# Prerequisites:
# 1. Install ARM GCC toolchain
# 2. Install st-link tools for flashing
# 3. Install make utility
# 
# Build Commands:
# make all       - Compile and link all files
# make clean     - Clean build directory
# make flash     - Flash to STM32F103C8T6
# make size      - Show memory usage
# 
# Hardware Setup:
# 1. Connect ST-Link programmer to STM32F103C8T6
# 2. Connect temperature sensor to PA0
# 3. Connect PWM-controlled fan to PA8
# 4. Connect status LED to PC13
# 
# =====================================================
