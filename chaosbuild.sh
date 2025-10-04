#!/bin/bash
# ------------------------------------------------------------------------------
# @file chaosbuild.sh
# @brief Script to build the Docker image, compile the project, and run the demo.
# 
# This script automates the compilation of the ChaosEngine project inside a Docker 
# container to ensure a consistent build environment. It then attempts to run 
# the compiled GUI executable, forwarding the display using X11/Wayland 
# capabilities (commonly used in WSL/Linux environments).
# 
# @author PapaPamplemousse
# @date October 2025
# @version 1.0
# ------------------------------------------------------------------------------

# --- Configuration Variables ---

# @var IMAGE_NAME
# @brief The name and tag for the Docker image.
IMAGE_NAME="chaos-engine:latest"

# @var DOCKERFILE_PATH
# @brief The path to the Dockerfile to use for the build.
# We default to the standard name 'Dockerfile' in the current directory.
DOCKERFILE_PATH="Dockerfile"

# @var HOST_DIR
# @brief The absolute path to the project directory on the host (WSL/Linux).
HOST_DIR=$(pwd)

# @var CONTAINER_DIR
# @brief The path where host files will be mounted inside the container.
CONTAINER_DIR="/workspace"

# @var TARGET_EXECUTABLE
# @brief Path to the compiled binary relative to HOST_DIR and CONTAINER_DIR.
TARGET_EXECUTABLE="examples/demo"

# --- Step 1: Build Docker Image ---
echo "--- 🔨 Checking and Rebuilding Docker Image ($IMAGE_NAME) ---"

# Build the Docker image.
# The build context is set to the current directory ($HOST_DIR).
docker build -t "$IMAGE_NAME" -f "$DOCKERFILE_PATH" "$HOST_DIR"
BUILD_STATUS=$?

if [ $BUILD_STATUS -ne 0 ]; then
    echo "--- ❌ Docker image build failed (code $BUILD_STATUS) ---"
    exit 1
fi
echo "--- ✅ Docker image built successfully ---"

# --- Step 2: Compile Project using 'make' ---
echo "--- 🚀 Compiling project inside Docker ---"

# Run the container to execute 'make'.
# --rm: Automatically remove the container when it exits.
# -v: Mount the current host directory into the container.
# -w: Set the working directory inside the container.
# "$@": Pass all command-line arguments (e.g., 'make clean', 'make install') to 'make'.
docker run \
    --rm \
    -v "$HOST_DIR:$CONTAINER_DIR" \
    -w "$CONTAINER_DIR" \
    "$IMAGE_NAME" \
    make "$@"

EXIT_CODE=$?
if [ $EXIT_CODE -ne 0 ]; then
    echo "--- ❌ Compilation failed (exit code $EXIT_CODE) ---"
    exit $EXIT_CODE
fi
echo "--- ✅ Compilation succeeded ---"

# --- Step 3: Run Executable (if compiled) ---

HOST_EXE="$HOST_DIR/$TARGET_EXECUTABLE"
CONTAINER_EXE="$CONTAINER_DIR/$TARGET_EXECUTABLE"

if [ -f "$HOST_EXE" ]; then
    echo "--- 🎮 Launching binary in Docker with X display forwarding ---"

    # Check if DISPLAY environment variable is set (critical for GUI forwarding)
    if [ -z "$DISPLAY" ]; then
        echo "⚠️ DISPLAY variable is not set. Defaulting to :0"
        export DISPLAY=:0
    fi

    # Run the GUI executable.
    # -it: Interactive (to handle input/output)
    # -e DISPLAY: Passes the host's DISPLAY environment variable to the container
    docker run --rm -it \
        -e DISPLAY=$DISPLAY \
        -v "$HOST_DIR:$CONTAINER_DIR" \
        -w "$CONTAINER_DIR" \
        "$IMAGE_NAME" \
        "$CONTAINER_EXE"
else
    echo "--- ⚠️ Executable $TARGET_EXECUTABLE not found at $HOST_EXE ---"
fi