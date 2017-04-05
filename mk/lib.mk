LIB_OUT_DIR = $(OUT_DIR)/$(LIB_NAME)

LIB_FILE = $(LIB_OUT_DIR)/lib$(LIB_NAME).a

OBJ_FILES = $(foreach obj, $(SOURCE_FILES), $(LIB_OUT_DIR)/$(obj:.c=.o))

# includes
CFLAGS += $(foreach dir, $(INCLUDE_DIRS), -I $(dir))

# compile
$(LIB_OUT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: all
all: $(LIB_FILE)

# include dependents
include $(foreach d, $(SOURCE_FILES:.c=.d), $(LIB_OUT_DIR)/$(d))

$(LIB_OUT_DIR)/%.d: %.c
	@dir=`dirname $@` && test -d $$dir || mkdir -p $$dir; \
	set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\(.*\)\.o[ :]*,$(@:.d=.o) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$;

$(LIB_FILE): $(OBJ_FILES)
	$(AR) -r $@ $^
