ROOT_DIR = $(shell pwd)
include mk/select_platform.mk

###########   Configure   #############

PROJECT_NAME=simulator

CFLAGS += -I ./demo/main

LIBS += -ldemo -lucgui -lvdev

ALL_LIBRARYS = demo
ALL_LIBRARYS += ucgui
ALL_LIBRARYS += vdev

########### End configure #############


# generate object output dir
OUT_DIR = $(ROOT_DIR)/out.$(PLATFORM)

# export values
export PLATFORM ROOT_DIR OUT_DIR

# generate LD_FLAGS
LD_FLAGS += $(foreach lib, $(ALL_LIBRARYS), -L $(OUT_DIR)/$(lib))


.PHONY: all $(ALL_LIBRARYS)
all: $(ALL_LIBRARYS)
	$(CC) $(CFLAGS) main.c -o $(OUT_DIR)/$(PROJECT_NAME) $(LD_FLAGS) $(LIBS)
	@echo 'Compile successful!'

vdev:
	$(MAKE) -C vdev/

ucgui:
	$(MAKE) -C third_party/ucgui/

demo:
	$(MAKE) -C demo/

.PHONY: clean
clean:
	rm -rvf $(OUT_DIR)
