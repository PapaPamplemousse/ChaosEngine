# ===============================================================
# === ChaosEngine Unified Build System (with GLAD Integration) ===
# ===============================================================

# === Toolchain ===
CC        ?= gcc
AR        ?= ar
RANLIB    ?= ranlib
CFLAGS    ?= -Wall -Wextra -Wpedantic -O2 -fPIC
DEBUG_FLAGS ?= -g -O0
LDFLAGS   ?=

# === Directories ===
ENGINE_DIR    := ChaosEngine
ENGINE_INC    := $(ENGINE_DIR)/inc
ENGINE_SRC    := $(ENGINE_DIR)/src
GLAD_SRC      := $(ENGINE_DIR)/glad/src/gl.c
GLAD_INCLUDE  := $(ENGINE_DIR)/glad/include

EXAMPLES_DIR  := examples
# These variables are overridden dynamically by chaosbuild.sh
EXAMPLES_SRC  ?= $(EXAMPLES_DIR)/src
EXAMPLE_DIR   ?= $(patsubst %/src,%,$(EXAMPLES_SRC))
EXAMPLE_INC   ?= $(EXAMPLE_DIR)/inc
EXAMPLE_BIN   ?= $(EXAMPLE_DIR)/demo

BUILD_DIR     := build
LIB_DIR       := lib
LIB_NAME      := libChaosEngine.a
LIB_PATH      := $(LIB_DIR)/$(LIB_NAME)

# === Includes ===
INCLUDE_FLAGS := -I$(ENGINE_INC) -I$(ENGINE_INC)/ChaosEngine -I$(EXAMPLE_INC) -I$(GLAD_INCLUDE)

# === Source files ===
ENGINE_SRCS := $(wildcard $(ENGINE_SRC)/*.c) $(GLAD_SRC)
ENGINE_OBJS := $(patsubst $(ENGINE_SRC)/%.c,$(BUILD_DIR)/%.o,$(wildcard $(ENGINE_SRC)/*.c))
ENGINE_OBJS += $(BUILD_DIR)/glad.o

# === Phony targets ===
.PHONY: all clean distclean run example debug install run-headless docker-build docker-shell build-lib

# ===============================================================
# === Build ChaosEngine Static Library
# ===============================================================
all: $(LIB_PATH)

$(LIB_PATH): $(ENGINE_OBJS) | $(LIB_DIR)
	@echo "📦 Creating static library $@"
	$(AR) rcs $@ $(ENGINE_OBJS)
	$(RANLIB) $@

# --- Engine core sources ---
$(BUILD_DIR)/%.o: $(ENGINE_SRC)/%.c | $(BUILD_DIR)
	@echo "🧱 Compiling $<"
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

# --- GLAD integration ---
$(BUILD_DIR)/glad.o: $(GLAD_SRC) | $(BUILD_DIR)
	@echo "🔧 Compiling GLAD (OpenGL loader)"
	$(CC) $(CFLAGS) -I$(GLAD_INCLUDE) -c $< -o $@

# --- Ensure build directories exist ---
$(LIB_DIR) $(BUILD_DIR):
	@mkdir -p $@

# ===============================================================
# === Example Demos
# ===============================================================
example: $(EXAMPLES_SRC)/demo.c $(LIB_PATH)
	@echo "🚀 Compiling demo at $(EXAMPLES_SRC)"
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(EXAMPLES_SRC)/demo.c \
		-L$(LIB_DIR) -lChaosEngine -lm -lSDL2 -lGL -ldl \
		-o $(EXAMPLE_BIN)

run: example
	@echo "🎮 Running demo:"
	$(EXAMPLE_BIN)

# ===============================================================
# === Example Demos 
# ===============================================================
example-only: $(EXAMPLES_SRC)/demo.c
	@echo "🚀 Compiling demo at $(EXAMPLES_SRC) (Lib assumed to be built)"
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(EXAMPLES_SRC)/demo.c \
		-L$(LIB_DIR) -lChaosEngine -lm -lSDL2 -lGL -ldl \
		-o $(EXAMPLE_BIN)

run-only: example-only
	@echo "🎮 Running demo:"
	$(EXAMPLE_BIN)

# ===============================================================
# === Cleaning
# ===============================================================
clean:
	@echo "🧹 Cleaning compiled files"
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d
	rm -f $(LIB_PATH)
	find $(EXAMPLES_DIR) -type f -name "demo" -delete

clean-example:
	@echo "🧹 Cleaning demo executable"
	# Supprime l'exécutable spécifique à cette démo
	rm -f $(EXAMPLE_BIN)
	
distclean: clean
	@echo "🔥 Full cleanup"
	rm -rf .cache

build-lib: $(LIB_PATH)

# ===============================================================
# === Debug & Install
# ===============================================================
debug: CFLAGS += $(DEBUG_FLAGS)
debug: all
	@echo "🐞 Built ChaosEngine in debug mode."

install: $(LIB_PATH)
	@echo "📥 Installing ChaosEngine headers and library..."
	mkdir -p /usr/local/lib /usr/local/include/ChaosEngine
	cp $(LIB_PATH) /usr/local/lib/
	cp -r $(ENGINE_INC)/* /usr/local/include/ChaosEngine/

# ===============================================================
# === Headless (CI / Docker)
# ===============================================================
run-headless: example
	@echo "🧠 Running demo in headless mode (xvfb)..."
	xvfb-run $(EXAMPLE_BIN)

# ===============================================================
# === Docker Helpers
# ===============================================================
docker-build:
	docker build -t chaos-engine-dev .

docker-shell:
	docker run --rm -it -v "$(pwd)":/workspace -w /workspace chaos-engine-dev /bin/sh

# ===============================================================
# === Dependency Tracking
# ===============================================================
CFLAGS += -MMD -MP
-include $(ENGINE_OBJS:.o=.d)
