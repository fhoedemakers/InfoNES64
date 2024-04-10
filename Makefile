all: infones.z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

OBJS = $(BUILD_DIR)/infones.o

vtest.z64: N64_ROM_TITLE = "InfoNES Nes Emulator"

$(BUILD_DIR)/infones.elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
