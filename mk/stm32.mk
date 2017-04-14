# stm32
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -g -Wall -O3 -T ./stm32_flash_gcc.ld
CFLAGS += -mthumb -mcpu=cortex-m4 
# hardware fpu
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += --specs=nosys.specs

CFLAGS += -D USE_STDPERIPH_DRIVER

