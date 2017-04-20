LIB_OUT_DIR = $(OUT_DIR)/$(LIB_NAME)

OUTPUT = $(LIB_OUT_DIR)/$(LIB_NAME).out

OBJ_FILES = $(foreach obj, $(SOURCE_FILES), $(LIB_OUT_DIR)/$(obj:.c=.o))
OBJ_FILES += $(foreach obj, $(STARTUP_FILES), $(LIB_OUT_DIR)/$(obj:.s=.o))

# includes
CFLAGS += $(foreach dir, $(INCLUDE_DIRS), -I $(dir))

# compile
$(LIB_OUT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB_OUT_DIR)/%.o: %.s
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all
all: $(OUTPUT)

# include dependents
include $(foreach d, $(SOURCE_FILES:.c=.d), $(LIB_OUT_DIR)/$(d))

$(LIB_OUT_DIR)/%.d: %.c
	@dir=`dirname $@` && test -d $$dir || mkdir -p $$dir; \
	set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\(.*\)\.o[ :]*,$(@:.d=.o) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$;

$(OUTPUT): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(LD_FLAGS) $(LIBS)
