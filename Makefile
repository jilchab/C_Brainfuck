include components.mk

# Params
TARGET_NAME?= target
SRCS_DIR?= src
BUILD_DIR?= .build
CC:=gcc

# Utils
CFLAGS= -Wall \
		$(addprefix -D,$(DEFINES)) \
		$(addprefix -I$(SRCS_DIR)/,$(INCLUDES))

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
SRCS:= $(call rwildcard,$(SRCS_DIR)/,*.c)
OBJS:= $(addprefix $(BUILD_DIR)/,$(SRCS:.c=.o))
OBJS_DIR:=$(dir $(OBJS))
TARGET:= $(BUILD_DIR)/$(TARGET_NAME)

ifeq ($(V), 1)
PREF:=
else
PREF:=@
endif

.PHONY: all

all: build
	@echo "$(TARGET_NAME) succesfully compiled. Run make start or ./$(TARGET) to start the program"

build: $(OBJS_DIR) $(TARGET)

print_vars:
	@echo "TARGET=$(TARGET)"
	@echo "SRCS=$(SRCS)"
	@echo "OBJS=$(OBJS)"

$(TARGET): $(OBJS)
	@echo "Linking $@"
	$(PREF)$(CC) $^ -o $@

$(OBJS_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o: %.c
	@echo "Compiling $@"
	$(PREF)$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	@echo "Cleaning"
	$(PREF)rm -rf $(BUILD_DIR)

start: build
	@echo "Program start...\n"
	$(PREF)./$(TARGET)