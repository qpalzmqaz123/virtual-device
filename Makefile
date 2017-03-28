########### Configure #############

PROJ_NAME = simulator
OBJS_OUTPUT_DIR = ./obj_output

CC = gcc

CFLAGS = -g -Wall -O3
CFLAGS += $(shell sdl2-config --cflags)

LIBS = -lpthread -lucgui
LIBS += $(shell sdl2-config --libs)

DEFINES = 

INCLUDE_DIRS = ./vdev/inc \
			   ./vdev/posix/common ./vdev/posix/lcd ./vdev/posix/os \
			   ./third_party/ucgui/GUI/Core/ ./third_party/ucgui/Config

LIBRARY_DIRS = ./third_party/ucgui/GUI

SOURCE_DIRS = ./vdev/posix ./vdev/posix/common ./vdev/posix/lcd ./vdev/posix/os

SRC_FILES = \
	main.c \
	posix_queue.c \
	posix_vdev_lcd.c posix_vdev.c posix_vdev_os.c posix_vdev_os_queue.c

########### End configure #############

# includes
CFLAGS += $(foreach dir, $(INCLUDE_DIRS), -I $(dir))
# defines
CFLAGS += $(foreach def, $(DEFINES), -D $(def))
# librarys
LDFLAGS += $(foreach lib, $(LIBRARY_DIRS), -L $(lib))

vpath %.s $(SOURCE_DIRS)
vpath %.c $(SOURCE_DIRS)
vpath %.h $(INCLUDE_DIRS)
vpath %.d $(OBJS_OUTPUT_DIR)

# generate objects
OBJS = $(SRC_FILES:.c=.o)
OBJS_REAL = $(foreach obj, $(OBJS), $(OBJS_OUTPUT_DIR)/$(obj))


.PHONY: all
all: setup project

setup:
	test -d $(OBJS_OUTPUT_DIR) || mkdir $(OBJS_OUTPUT_DIR)

project: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROJ_NAME) $(OBJS_REAL) $(LDFLAGS) $(LIBS)
	
# compile
%.o: %.c
	$(CC) $(CFLAGS) -c -o $(OBJS_OUTPUT_DIR)/$@ $<

# include files
include $(foreach d, $(SRCS:.c=.d), $(d))

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJS_OUTPUT_DIR)\/\1.o $@ : ,g' < $@.$$$$ > $(OBJS_OUTPUT_DIR)/$@; \
	rm -f $@.$$$$

clean:
	rm -rvf $(OBJS_OUTPUT_DIR)
	rm -vf $(PROJ_NAME)
