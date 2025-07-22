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
BUILD_DIR = build

# Source files
SOURCES = main.c \
          $(IOHWAB_DIR)/IoHwAb.c \
          $(MCAL_DIR)/Port/Port.c \
          $(MCAL_DIR)/Dio/Dio.c \
          $(MCAL_DIR)/Adc/Adc.c \
          $(MCAL_DIR)/Pwm/Pwm.c \
          $(CONFIG_DIR)/Port_Cfg.c \
          $(CONFIG_DIR)/Dio_Cfg.c \
          $(CONFIG_DIR)/Adc_Cfg.c \
          $(CONFIG_DIR)/Pwm_Cfg.c

# Include directories
INCLUDES = -I. \
           -I$(IOHWAB_DIR) \
           -I$(MCAL_DIR)/Port \
           -I$(MCAL_DIR)/Dio \
           -I$(MCAL_DIR)/Adc \
           -I$(MCAL_DIR)/Pwm \
           -I$(CONFIG_DIR)

# Compiler flags
CFLAGS = -mcpu=cortex-m3 \
         -mthumb \
         -Wall \
         -Wextra \
         -Werror \
         -std=c99 \
         -O2 \
         -g \
         -DSTM32F103xB \
         $(INCLUDES)

# Linker flags
LDFLAGS = -mcpu=cortex-m3 \
          -mthumb \
          -specs=nano.specs \
          -specs=nosys.specs \
          -static \
          -Wl,--gc-sections

# Generate object file names
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)

# Default target
all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).bin $(BUILD_DIR)/$(PROJECT).hex

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(IOHWAB_DIR)
	mkdir -p $(BUILD_DIR)/$(MCAL_DIR)/Port
	mkdir -p $(BUILD_DIR)/$(MCAL_DIR)/Dio
	mkdir -p $(BUILD_DIR)/$(MCAL_DIR)/Adc
	mkdir -p $(BUILD_DIR)/$(MCAL_DIR)/Pwm
	mkdir -p $(BUILD_DIR)/$(CONFIG_DIR)

# Compile source files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling $<"
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
	st-flash write $(BUILD_DIR)/$(PROJECT).bin 0x8000000

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
