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

SRCS_C = $(wildcard *.c)
SRCS_CPP = $(wildcard *.cpp)
SRCS_SUBDIR = $(wildcard infones/*.c) $(wildcard infones/*.cpp) $(wildcard infones/mapper/*.c) $(wildcard infones/mapper/*.cpp)

OBJS_C = $(SRCS_C:.c=.o)
OBJS_CPP = $(SRCS_CPP:.cpp=.o)
OBJS_SUBDIR_C = $(SRCS_SUBDIR:.c=.o)
OBJS_SUBDIR_CPP = $(OBJS_SUBDIR:.cpp=.o)


OBJS = $(BUILD_DIR)/vtest.o

infones.z64: N64_ROM_TITLE = "Video Res Test"

$(BUILD_DIR)/vtest.elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)