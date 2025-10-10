# ===========================================================
# ChaosEngine — Modular Build System (C-only)
# Compatible with Docker + chaosbuild.sh
# ===========================================================

# === Toolchain ===
CC       ?= gcc
AR       ?= ar
RANLIB   ?= ranlib
CFLAGS   ?= -Wall -Wextra -Wpedantic -std=c11 -O2 -fPIC
DEBUG_FLAGS ?= -g -O0
LDFLAGS  ?=
LIBS     := -lm -lSDL2

# === Directories ===
INC_DIR      := inc
SRC_DIR      := src
BUILD_DIR    := build
LIB_DIR      := lib
LIB_NAME     := libChaosEngine.a
LIB_PATH     := $(LIB_DIR)/$(LIB_NAME)
EXAMPLES_DIR := examples

# === Include Flags ===
INCLUDE_FLAGS := -I$(INC_DIR)

# === Engine Sources ===
ENGINE_SRCS := $(shell find $(SRC_DIR) -type f -name "*.c")
ENGINE_OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(ENGINE_SRCS))

# === Phony targets ===
.PHONY: all clean distclean debug example run install

# ===========================================================
# === Build ChaosEngine Static Library
# ===========================================================
all: $(LIB_PATH)

$(LIB_PATH): $(ENGINE_OBJS) | $(LIB_DIR)
	@echo "📦 Archiving static library $@"
	$(AR) rcs $@ $(ENGINE_OBJS)
	$(RANLIB) $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "🧱 Compiling $<"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(LIB_DIR) $(BUILD_DIR):
	@mkdir -p $@

# ===========================================================
# === Example Compilation / Execution
# ===========================================================
# Usage:
#   make example EXAMPLE=00_boot
#   make run EXAMPLE=00_boot

example:
	@if [ -z "$(EXAMPLE)" ]; then \
		echo "⚠️  Usage: make example EXAMPLE=<demo_folder>"; \
		exit 1; \
	fi
	@if [ ! -f "$(EXAMPLES_DIR)/$(EXAMPLE)/main.c" ]; then \
		echo "❌ No main.c found in $(EXAMPLES_DIR)/$(EXAMPLE)"; \
		exit 1; \
	fi
	@echo "🚀 Building example: $(EXAMPLE)"
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) \
		$(EXAMPLES_DIR)/$(EXAMPLE)/main.c \
		-L$(LIB_DIR) -lChaosEngine $(LIBS) \
		-o $(EXAMPLES_DIR)/$(EXAMPLE)/demo

run: example
	@echo "🎮 Running example $(EXAMPLE)"
	$(EXAMPLES_DIR)/$(EXAMPLE)/demo

# ===========================================================
# === Cleaning & Debug
# ===========================================================
clean:
	@echo "🧹 Cleaning build files"
	rm -rf $(BUILD_DIR) $(LIB_DIR)

clean-examples:
	@echo "🧹 Cleaning demos"
	find $(EXAMPLES_DIR) -type f -name "demo" -delete

distclean: clean clean-examples

debug: CFLAGS += $(DEBUG_FLAGS)
debug: all
	@echo "🐞 Built ChaosEngine in debug mode."

# ===========================================================
# === Install (Optional)
# ===========================================================
install: $(LIB_PATH)
	@echo "📥 Installing ChaosEngine headers and library..."
	mkdir -p /usr/local/lib /usr/local/include/ChaosEngine
	cp $(LIB_PATH) /usr/local/lib/
	cp -r $(INC_DIR)/* /usr/local/include/ChaosEngine/

# ===========================================================
# === Dependency Tracking
# ===========================================================
CFLAGS += -MMD -MP
-include $(ENGINE_OBJS:.o=.d)
