all: infones.z64
.PHONY: all

BUILD_DIR = build
include $(N64_INST)/include/n64.mk

# add current folder and infones subfolder to include path
INCDIR = -I. -Iinfones
# add INCDIR to CFLAGS
CFLAGS += $(INCDIR)
# add INCDIR to CXXFLAGS
CXXFLAGS += $(INCDIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/infones/%.cpp $(SOURCE_DIR)/infones/mapper/%.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<


# Find all .o files in the build directory and put in OBJS


OBJS = $(BUILD_DIR)/main.o \
       $(BUILD_DIR)/InfoNES.o \
	   $(BUILD_DIR)/K6502.o \
	   $(BUILD_DIR)/InfoNES_pAPU.o \
	   $(BUILD_DIR)/InfoNES_Mapper.o

vtest.z64: N64_ROM_TITLE = "InfoNES Nes Emulator"

$(BUILD_DIR)/infones.elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
