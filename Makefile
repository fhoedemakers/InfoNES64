all: infones64.z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

# add current folder and infones subfolder to include path
INCDIR = -I. -Iinfones
# add INCDIR to CFLAGS
CFLAGS += $(INCDIR)
# add INCDIR to CXXFLAGS
CXXFLAGS += $(INCDIR)

SUBDIRS = $(SOURCE_DIR) $(SOURCE_DIR)/infones
$(BUILD_DIR)/%.o: %.c 
# 	@mkdir -p $(dir $@)
# 	@echo "    [CC] $<"
# 	$(CC) -c $(CFLAGS) -o $@ $<

# $(BUILD_DIR)/%.o: %.cpp
# 	@mkdir -p $(dir $@)
# 	@echo "    [CXX] $<"
# 	$(CXX) -c $(CXXFLAGS) -o $@ $<vpath %.c $(SUBDIRS)
vpath %.cpp $(SUBDIRS)

# 

OBJS = $(BUILD_DIR)/infones64.o $(BUILD_DIR)/InfoNES.o

infones64.z64: N64_ROM_TITLE = "Video Res Test"

$(BUILD_DIR)/infones64.elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
