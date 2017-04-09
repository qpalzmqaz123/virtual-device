include $(ROOT_DIR)/mk/$(PLATFORM).mk

LIB_NAME = vdev

CFLAGS += $(shell sdl2-config --cflags)

INCLUDE_DIRS = ../inc third_party/uthash
INCLUDE_DIRS += $(shell ls -l | grep '^d' | awk '{print $$9}')

SOURCE_FILES = $(shell find . -name '*.c')

include $(ROOT_DIR)/mk/lib.mk
