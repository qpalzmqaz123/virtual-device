include $(ROOT_DIR)/mk/$(PLATFORM).mk

LIB_NAME = special_lcd

INCLUDE_DIRS = \
	$(ROOT_DIR)/vdev/inc \
	$(ROOT_DIR)/vdev/posix/common

INCLUDE_DIRS += $(shell ls -l | grep '^d' | awk '{print $$9}')

SOURCE_FILES = $(shell find . -name '*.c')
SOURCE_FILES += $(ROOT_DIR)/vdev/posix/common/posix_socket.c

CFLAGS += $(shell sdl2-config --cflags)
LIBS += $(shell sdl2-config --libs)

include $(ROOT_DIR)/mk/build.mk
