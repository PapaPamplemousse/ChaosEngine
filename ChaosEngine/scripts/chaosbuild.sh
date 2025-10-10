#!/bin/bash
# ===========================================================
# ChaosEngine — Interactive Builder & Runner
# ===========================================================

# --- Resolve script directory ---
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"


IMAGE_NAME="chaosengine:latest"
DOCKERFILE_PATH="$PROJECT_ROOT/docker/Dockerfile"
HOST_DIR="$PROJECT_ROOT"
CONTAINER_DIR="/workspace"
LIB_DIR="lib"
LIB_NAME="libChaosEngine.a"
EXAMPLES_DIR="$PROJECT_ROOT/examples"

# === Colors ===
RED="\033[1;31m"; GREEN="\033[1;32m"; YELLOW="\033[1;33m"
BLUE="\033[1;34m"; CYAN="\033[1;36m"; RESET="\033[0m"

section() { echo -e "\n${BLUE}=== $1 ===${RESET}\n"; }

# ===========================================================
# === Check Docker availability
# ===========================================================
check_docker() {
  if ! command -v docker &>/dev/null; then
    echo -e "${RED}❌ Docker is not installed or not in PATH.${RESET}"
    exit 1
  fi
  if ! docker info &>/dev/null; then
    echo -e "${RED}❌ Docker daemon not running.${RESET}"
    exit 1
  fi
}

# ===========================================================
# === Docker build
# ===========================================================
build_docker() {
  section "🐳 Rebuilding Docker image: ${IMAGE_NAME}"
  check_docker
  docker build -t "$IMAGE_NAME" -f "$DOCKERFILE_PATH" "$HOST_DIR"
  if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Docker build failed.${RESET}"
    read -p "Press ENTER to return to menu..." < /dev/tty
    return 1
  fi
  echo -e "${GREEN}✅ Docker image built successfully.${RESET}"
}

# ===========================================================
# === Build ChaosEngine Library
# ===========================================================
build_library() {
  section "⚙️ Building ChaosEngine library"
  check_docker
  docker run --rm -v "$HOST_DIR:$CONTAINER_DIR" -w "$CONTAINER_DIR" "$IMAGE_NAME" \
    bash -c "make -f cmake/Makefile clean all"
  if [ $? -eq 0 ]; then
    echo -e "${GREEN}✅ Library built successfully.${RESET}"
  else
    echo -e "${RED}❌ Library build failed.${RESET}"
  fi
}

# ===========================================================
# === Demo listing and selection
# ===========================================================
list_demos() {
  DEMO_LIST=()
  for dir in "$EXAMPLES_DIR"/*/; do
    [ -d "$dir" ] || continue
    DEMO_LIST+=("$(basename "$dir")")
  done
  if [ ${#DEMO_LIST[@]} -eq 0 ]; then
    echo -e "${RED}❌ No demos found in ${EXAMPLES_DIR}.${RESET}"
    return 1
  fi

  echo -e "${YELLOW}📂 Available demos:${RESET}"
  for i in "${!DEMO_LIST[@]}"; do
    printf "   ${CYAN}[%d]${RESET} %s\n" $((i+1)) "${DEMO_LIST[$i]}"
  done
}


select_demo() {
  list_demos || return 1
  echo -ne "${YELLOW}Select a demo number: ${RESET}" >&2
  read -r choice < /dev/tty
  [[ "$choice" =~ ^[0-9]+$ ]] || { echo -e "${RED}Invalid input.${RESET}" >&2; return 1; }
  local idx=$((choice-1))
  if [ "$idx" -lt 0 ] || [ "$idx" -ge "${#DEMO_LIST[@]}" ]; then
    echo -e "${RED}Invalid selection.${RESET}" >&2; return 1
  fi
  SELECTED_DEMO="${DEMO_LIST[$idx]}"
}

# ===========================================================
# === Build and Run a demo
# ===========================================================
run_demo() {
  local demo="$1"
  section "🚀 Running demo: ${demo}"

  if [ ! -f "$HOST_DIR/$LIB_DIR/$LIB_NAME" ]; then
    echo -e "${RED}❌ Library not built. Run option 2 first.${RESET}"
    return 1
  fi

  check_docker
  docker run --rm -it \
    -e DISPLAY=$DISPLAY \
    -v "$HOST_DIR:$CONTAINER_DIR" \
    -w "$CONTAINER_DIR" \
    "$IMAGE_NAME" \
    bash -c "make -f cmake/Makefile run EXAMPLE=$demo"
}

# ===========================================================
# === Menu
# ===========================================================
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

# ===========================================================
# === Main Loop
# ===========================================================
while true; do
  main_menu
  echo
  read -p "Press ENTER to return to menu..." < /dev/tty
done
