include $(ROOT_DIR)/mk/$(PLATFORM).mk

LIB_NAME = vdev

INCLUDE_DIRS = ../inc
INCLUDE_DIRS += ./lib/inc
INCLUDE_DIRS += ./third_party/FreeRTOS/conf_gcc ./third_party/FreeRTOS/inc
INCLUDE_DIRS += $(shell ls -l | grep '^d' | awk '{print $$9}')

SOURCE_FILES = $(shell find -name '*.c')
STARTUP_FILES = ./sys/startup_stm32f4xx_gcc.s

include $(ROOT_DIR)/mk/lib.mk
