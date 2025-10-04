# === Toolchain ===
CC       ?= gcc
AR       ?= ar
RANLIB   ?= ranlib
CFLAGS   ?= -Wall -Wextra -Wpedantic -O2 -fPIC
DEBUG_FLAGS ?= -g -O0
LDFLAGS  ?=

# === Directories ===
ENGINE_DIR    := ChaosEngine
ENGINE_INC    := $(ENGINE_DIR)/inc
ENGINE_SRC    := $(ENGINE_DIR)/src

EXAMPLES_DIR  := examples
EXAMPLES_INC  := $(EXAMPLES_DIR)/inc
EXAMPLES_SRC  := $(EXAMPLES_DIR)/src

BUILD_DIR     := build
LIB_DIR       := lib
LIB_NAME      := libChaosEngine.a
LIB_PATH      := $(LIB_DIR)/$(LIB_NAME)

# === Includes ===
# We add both inc/ and inc/ChaosEngine so we can simply include "renderer.h" internally
INCLUDE_FLAGS := -I$(ENGINE_INC) -I$(ENGINE_INC)/ChaosEngine -I$(EXAMPLES_INC)

# === Source files ===
ENGINE_SRCS := $(wildcard $(ENGINE_SRC)/*.c)
ENGINE_OBJS := $(patsubst $(ENGINE_SRC)/%.c,$(BUILD_DIR)/%.o,$(ENGINE_SRCS))

# === Phony targets ===
.PHONY: all clean distclean run docker-build docker-shell

# === General build ===
all: $(LIB_PATH) example

# === Build the ChaosEngine static library ===
$(LIB_PATH): $(ENGINE_OBJS) | $(LIB_DIR)
	@echo "📦 Creating static library $@"
	$(AR) rcs $@ $(ENGINE_OBJS)
	$(RANLIB) $@

$(BUILD_DIR)/%.o: $(ENGINE_SRC)/%.c | $(BUILD_DIR)
	@echo "🧱 Compiling $<"
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(LIB_DIR) $(BUILD_DIR):
	mkdir -p $@

# === Example demo ===
example: $(EXAMPLES_SRC)/demo.c $(LIB_PATH)
	@echo "🚀 Compiling demo"
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(EXAMPLES_SRC)/demo.c -L$(LIB_DIR) -lChaosEngine -lm -lSDL2 -o $(EXAMPLES_DIR)/demo

# === Run the demo ===
run: example
	@echo "🎮 Running demo:"
	./$(EXAMPLES_DIR)/demo

# === Cleaning ===
clean:
	@echo "🧹 Cleaning compiled files"
	rm -rf $(BUILD_DIR) $(LIB_DIR) $(EXAMPLES_DIR)/demo

distclean: clean
	@echo "🔥 Full cleanup"
	rm -rf .cache

# === Docker helpers ===
docker-build:
	docker build -t chaos-engine-dev .

docker-shell:
	docker run --rm -it -v "$(pwd)":/workspace -w /workspace chaos-engine-dev /bin/sh
