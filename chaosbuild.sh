#!/bin/bash
# ------------------------------------------------------------------------------
# @file chaosbuild.sh
# @brief ChaosEngine Interactive Builder & Demo Runner
# 
# Features:
# - Build/Rebuild Docker image
# - Build ChaosEngine library
# - Automatically list, compile, and run demos
# - Colorful interactive terminal menu
# 
# @author PapaPamplemousse
# @date October 2025
# @version 1.0
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# ChaosEngine Interactive Builder & Demo Runner
# ------------------------------------------------------------------------------

IMAGE_NAME="chaos-engine:latest"
DOCKERFILE_PATH="Dockerfile"
HOST_DIR=$(pwd)
CONTAINER_DIR="/workspace"
DEMO_ROOT="examples"
LIB_DIR="lib"
LIB_NAME="libChaosEngine.a"

# === Colors ===
RED="\033[1;31m"; GREEN="\033[1;32m"; YELLOW="\033[1;33m"
BLUE="\033[1;34m"; CYAN="\033[1;36m"; RESET="\033[0m"

section() { echo -e "\n${BLUE}=== $1 ===${RESET}\n"; }

# --- Docker build ---
build_docker() {
  section "🐳 Rebuilding Docker image: ${IMAGE_NAME}"
  docker build -t "$IMAGE_NAME" -f "$DOCKERFILE_PATH" "$HOST_DIR" || {
    echo -e "${RED}❌ Docker build failed.${RESET}"; exit 1; }
  echo -e "${GREEN}✅ Docker image built successfully.${RESET}"
}

# --- Build ChaosEngine lib ---
build_library() {
  section "⚙️ Building ChaosEngine library"

  # 🌀 Generate GLAD before compilation (ensures GL 4.5 support)
  echo -e "${YELLOW}🌀 Generating GLAD (OpenGL 4.5)...${RESET}"
  docker run --rm -v "$HOST_DIR:$CONTAINER_DIR" -w "$CONTAINER_DIR" "$IMAGE_NAME" \
    bash -c "
      mkdir -p ChaosEngine/glad && \
      glad --api gl:core=4.5 --out-path ChaosEngine/glad c && \
      if [ ! -f ChaosEngine/glad/include/glad/glad.h ]; then
        echo '[WARN] GLAD header not found after generation — check structure';
        find ChaosEngine/glad -type f;
      fi
    "

  docker run --rm -v "$HOST_DIR:$CONTAINER_DIR" -w "$CONTAINER_DIR" "$IMAGE_NAME" make clean all
  if [ $? -eq 0 ]; then
    echo -e \"${GREEN}✅ ChaosEngine library built successfully.${RESET}\"
  else
    echo -e \"${RED}❌ Library build failed.${RESET}\"
  fi
}



# --- Gather demos ---
DEMO_LIST=()
list_demos() {
  DEMO_LIST=()
  for d in "$DEMO_ROOT"/*/; do
    [ -d "$d" ] || continue
    DEMO_LIST+=("$(basename "$d")")
  done
}

# --- Print demos (to screen only) ---
print_demos() {
  list_demos
  if [ ${#DEMO_LIST[@]} -eq 0 ]; then
    echo -e "${RED}❌ No demos found in ${DEMO_ROOT}.${RESET}"
    return 1
  fi
  echo -e "${YELLOW}📂 Available demos:${RESET}"
  for i in "${!DEMO_LIST[@]}"; do
    printf "   ${CYAN}[%d]${RESET} %s\n" $((i+1)) "${DEMO_LIST[$i]}"
  done
  echo
}

# --- Select demo ---
select_demo() {
  print_demos || return 1
  echo -ne "${YELLOW}Select a demo number: ${RESET}" >&2
  read -r choice < /dev/tty
  [[ "$choice" =~ ^[0-9]+$ ]] || { echo -e "${RED}Invalid input.${RESET}" >&2; return 1; }
  local idx=$((choice-1))
  if [ "$idx" -lt 0 ] || [ "$idx" -ge "${#DEMO_LIST[@]}" ]; then
    echo -e "${RED}Invalid selection.${RESET}" >&2; return 1
  fi
  SELECTED_DEMO="${DEMO_LIST[$idx]}"
}

# --- Build & run demo ---
run_demo() {
  local demo_name="$1"
  local demo_dir="$DEMO_ROOT/$demo_name"
  local src_file="$demo_dir/src/demo.c"

  if [ ! -f "$src_file" ]; then
    echo -e "${RED}❌ Source file not found: $src_file${RESET}"
    exit 1
  fi

  section "🚀 Building & running demo: ${demo_name}"


  if [ ! -f "$HOST_DIR/$LIB_DIR/$LIB_NAME" ]; then
    echo -e "${RED}🚨 Error: ChaosEngine library ($LIB_NAME) not found.${RESET}"
    echo -e "${YELLOW}Please ensure the libChaosEngine.a library is built (Menu Option 2) before attempting to compile and run any demo.${RESET}"
    return 1 # Retourne au menu
  fi

  # **Modification ici : on appelle 'make run-only' qui n'a pas la dépendance 'all'**
  docker run --rm -it \
    -e DISPLAY=$DISPLAY \
    -v "$HOST_DIR:$CONTAINER_DIR" \
    -w "$CONTAINER_DIR" \
    "$IMAGE_NAME" \
    bash -c "make clean-example && make run-only EXAMPLES_SRC=$demo_dir/src EXAMPLE_DIR=$demo_dir"
    # Note : 'make run-only' contient déjà l'exécution de $demo_dir/demo
}

main_menu() {
  clear
  echo -e "${CYAN}"
  echo "======================================="
  echo "      CHAOS ENGINE BUILD SYSTEM"
  echo "======================================="
  echo -e "${RESET}"
  echo "1) 🐳 Rebuild Docker image"
  echo "2) ⚙️  Build/Rebuild ChaosEngine library"
  echo "3) 🎮 Compile and run a demo"
  echo "4) ❌ Exit"
  echo
  print_demos
  echo -ne "${YELLOW}Choose an option: ${RESET}"
  read -r opt < /dev/tty

  case "$opt" in
    1) build_docker ;;
    2) build_library ;;
    3) select_demo && run_demo "$SELECTED_DEMO" ;;
    4) echo -e "${CYAN}Bye 👋${RESET}"; exit 0 ;;
    *) echo -e "${RED}Invalid choice.${RESET}" ;;
  esac
}

while true; do
  main_menu
  echo
  read -p "Press ENTER to return to menu..." < /dev/tty
done
