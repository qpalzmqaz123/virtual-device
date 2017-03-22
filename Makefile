CC = gcc

# 工程名,影响输出的文件名
PROJ_NAME = simulater
# 编译输出的目录，防止文件过多看起来混乱，包括.d文件
OBJS_OUTPUT_DIR = ./obj_output
# 宏定义
DEFINES = 

# 所有头文件的目录
INCLUDE_DIRS = ./vdev/inc \
			   ./vdev/posix/inc \
			   ./third_party/ucgui/GUI/Core/ ./third_party/ucgui/Config

# 所有源文件的目录
SRC_DIRS =	   ./vdev/posix/src

# 依赖的目标文件
OBJS = main.o \
	   posix_queue.o \
	   posix_vdev_lcd.o posix_vdev.o posix_vdev_os.o


# 链接时的lib参数
LIBS_OPTION = -L ./third_party/ucgui/GUI
LIBS_OPTION += -lpthread \
			   -lucgui
LIBS_OPTION += $(shell sdl2-config --libs)

CFLAGS = -g -Wall -O3
CFLAGS += $(shell sdl2-config --cflags)

# includes
CFLAGS += $(foreach dir, $(INCLUDE_DIRS), -I $(dir) )
# defines
CFLAGS += $(foreach def, $(DEFINES), -D $(def))

#####################################################################
vpath %.s $(SRC_DIRS)
vpath %.c $(SRC_DIRS)
vpath %.h $(INCLUDE_DIRS)

# 所有源文件，去除了startup
SRCS = $(subst startup_stm32f4xx_gcc.c, , $(OBJS:.o=.c))
# 给所有obj加上真实的路径
OBJS_REAL = $(foreach obj, $(OBJS), $(OBJS_OUTPUT_DIR)/$(obj))

# 输出
.PHONY: all

all: project

project: $(OBJS_REAL)
	$(CC) $(CFLAGS) -o $(PROJ_NAME) $(OBJS_REAL) $(LIBS_OPTION)
	
# 默认规则
$(OBJS_OUTPUT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJS_OUTPUT_DIR)/%.o: %.s
	$(CC) $(CFLAGS) -c -o $@ $<

# 头文件依赖
include $(foreach d, $(SRCS:.c=.d), $(OBJS_OUTPUT_DIR)/$(d))

$(OBJS_OUTPUT_DIR)/%.d: %.c
	[ -d $(OBJS_OUTPUT_DIR) ] || mkdir $(OBJS_OUTPUT_DIR)
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJS_OUTPUT_DIR)\/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# 自定义依赖

#####################################################################
clean:
	rm -rvf $(OBJS_OUTPUT_DIR)
	rm -vf $(PROJ_NAME)
