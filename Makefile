##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.15.2] date: [Wed Apr 27 23:33:06 EEST 2022] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = sample-app


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Core/Src/main.c \
Core/Src/gpio.c \
Core/Src/usart.c \
Core/Src/stm32wbxx_it.c \
Core/Src/stm32wbxx_hal_msp.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_tim.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_tim_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_uart.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_uart_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rcc.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_rcc_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_flash.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_flash_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_gpio.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_hsem.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_dma.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_dma_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pwr.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_pwr_ex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_cortex.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal.c \
Drivers/STM32WBxx_HAL_Driver/Src/stm32wbxx_hal_exti.c \
Core/Src/system_stm32wbxx.c \
Core/Src/syscalls.c \
../../lib/sample-lib/sample-lib.c \
../../lib/led/led.c \
../../lib/log/log.c \
../../lib/storage/storage.c \
Core/Src/tim.c

# ASM sources
ASM_SOURCES =  \
startup_stm32wb55xx_cm4.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32WB55xx


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-ICore/Inc \
-IDrivers/STM32WBxx_HAL_Driver/Inc \
-IDrivers/STM32WBxx_HAL_Driver/Inc/Legacy \
-IDrivers/CMSIS/Device/ST/STM32WBxx/Include \
-IDrivers/CMSIS/Include \
-I../../lib/sample-lib \
-I../../lib/led \
-I../../lib/log \
-I../../lib/storage


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = stm32wb55xx_flash_cm4.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# custom commands
#######################################
flash: # programs your board with the project’s binary
	st-flash  write $(BUILD_DIR)/$(TARGET).bin 0x8000000
reset: # causes a board reset
	st-flash reset
gdb-server: # opens gdb server at port 4242
	st-util -p 4242
gdb: # opens a gdb session using the running gdb server
	gdb-multiarch $(BUILD_DIR)/$(TARGET).elf
console1: # opens the port connected to STM32 board’s st-link UART, default 115200 8N1 settings with Hardware Flow control disabled
	minicom -D /dev/ttyACM0
console2: # opens the port connected to STM32 board’s st-link UART, default 115200 8N1 settings with Hardware Flow control disabled
	minicom -D /dev/ttyACM1
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
