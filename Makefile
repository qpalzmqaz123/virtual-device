ROOT_DIR = $(shell pwd)
include mk/select_platform.mk

###########   Configure   #############

PROJECT_NAME=simulator

MAIN_LIBRARY = demo

ALL_LIBRARYS += demo
ALL_LIBRARYS += ucgui
ALL_LIBRARYS += vdev

LIBS += -lucgui -lvdev

########### End configure #############


# generate object output dir
OUT_DIR = $(ROOT_DIR)/out.$(PLATFORM)

# export values
export PLATFORM ROOT_DIR OUT_DIR

# generate LD_FLAGS
LD_FLAGS += $(foreach lib, $(ALL_LIBRARYS), -L $(OUT_DIR)/$(lib))


.PHONY: all $(ALL_LIBRARYS)
ifeq ($(PLATFORM), posix)
.PHONY: special
all: $(ALL_LIBRARYS) special
	$(CC) $(CFLAGS) -o $(OUT_DIR)/$(PROJECT_NAME) $(OUT_DIR)/$(MAIN_LIBRARY)/lib$(MAIN_LIBRARY).a $(LD_FLAGS) $(LIBS)
	@echo 'Compile successful!'
else
all: $(ALL_LIBRARYS)
	$(CC) $(CFLAGS) -o $(OUT_DIR)/$(PROJECT_NAME).elf $(OUT_DIR)/$(MAIN_LIBRARY)/lib$(MAIN_LIBRARY).a $(LD_FLAGS) $(LIBS)
	$(OBJCOPY) -O ihex $(OUT_DIR)/$(PROJECT_NAME).elf $(OUT_DIR)/$(PROJECT_NAME).hex
	$(OBJCOPY) -O binary $(OUT_DIR)/$(PROJECT_NAME).elf $(OUT_DIR)/$(PROJECT_NAME).bin
	@echo 'Compile successful!'
endif

vdev:
	$(MAKE) -C vdev/

ucgui:
	$(MAKE) -C third_party/ucgui/

demo:
	$(MAKE) -C demo/

special:
	$(MAKE) -C special/

.PHONY: clean
clean:
	rm -rvf $(OUT_DIR)

ifeq ($(PLATFORM), stm32)
.PHONY: burn
burn:
	st-flash write $(OUT_DIR)/$(PROJECT_NAME).bin 0x8000000
endif
