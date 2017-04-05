include $(ROOT_DIR)/mk/$(PLATFORM).mk

LIB_NAME = demo

INCLUDE_DIRS = \
	$(ROOT_DIR)/vdev/inc \
	$(ROOT_DIR)/third_party/ucgui/Config \
	$(ROOT_DIR)/third_party/ucgui/GUI/Core \
	$(ROOT_DIR)/third_party/ucgui/GUI/WM \
	$(ROOT_DIR)/third_party/ucgui/GUI/Widget

INCLUDE_DIRS += $(shell ls -l | grep '^d' | awk '{print $$9}')

SOURCE_FILES = $(shell find -name '*.c')

include $(ROOT_DIR)/mk/lib.mk
