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
# These variables will be overridden by chaosbuild.sh dynamically
EXAMPLES_SRC  ?= $(EXAMPLES_DIR)/src
EXAMPLE_DIR   ?= $(patsubst %/src,%,$(EXAMPLES_SRC))
EXAMPLE_INC   ?= $(EXAMPLE_DIR)/inc
EXAMPLE_BIN   ?= $(EXAMPLE_DIR)/demo

BUILD_DIR     := build
LIB_DIR       := lib
LIB_NAME      := libChaosEngine.a
LIB_PATH      := $(LIB_DIR)/$(LIB_NAME)

# === Includes ===
INCLUDE_FLAGS := -I$(ENGINE_INC) -I$(ENGINE_INC)/ChaosEngine -I$(EXAMPLE_INC)

# === Source files ===
ENGINE_SRCS := $(wildcard $(ENGINE_SRC)/*.c)
ENGINE_OBJS := $(patsubst $(ENGINE_SRC)/%.c,$(BUILD_DIR)/%.o,$(ENGINE_SRCS))

# === Phony targets ===
.PHONY: all clean distclean run docker-build docker-shell example

# === General build ===
all: $(LIB_PATH) 

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

# === Example demo (multi-demos) ===
example: $(EXAMPLES_SRC)/demo.c $(LIB_PATH)
	@echo "🚀 Compiling demo at $(EXAMPLES_SRC)"
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(EXAMPLES_SRC)/demo.c -L$(LIB_DIR) -lChaosEngine -lm -lSDL2 -o $(EXAMPLE_BIN)

# === Run the default demo ===
run: example
	@echo "🎮 Running demo:"
	$(EXAMPLE_BIN)

# === Cleaning ===
clean:
	@echo "🧹 Cleaning compiled files"
	rm -rf $(BUILD_DIR) $(LIB_DIR)
	find $(EXAMPLES_DIR) -type f -name "demo" -delete

distclean: clean
	@echo "🔥 Full cleanup"
	rm -rf .cache

build-lib: $(LIB_PATH)

# === Docker helpers ===
docker-build:
	docker build -t chaos-engine-dev .

docker-shell:
	docker run --rm -it -v "$(pwd)":/workspace -w /workspace chaos-engine-dev /bin/sh
