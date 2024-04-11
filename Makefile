all: vtest.z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

# add current folder and infones subfolder to include path
INCDIR = -I. -Iinfones
# add INCDIR to CFLAGS
CFLAGS += $(INCDIR)
# add INCDIR to CXXFLAGS
CXXFLAGS += $(INCDIR)

OBJS = $(BUILD_DIR)/vtest.o

vtest.z64: N64_ROM_TITLE = "Video Res Test"

$(BUILD_DIR)/vtest.elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)