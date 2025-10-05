# ==========================================================
# ChaosEngine — Devcontainer / Build environment
# Base: Debian from Microsoft Devcontainers
# Provides: SDL2 + OpenGL 4.5 + GLAD + Debug tools
# ==========================================================

FROM mcr.microsoft.com/devcontainers/base:debian

# --- Dependencies Installation ---
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    # Essential build tools
    build-essential \
    cmake \
    git \
    pkg-config \
    python3 \
    python3-pip \
    python3-venv \
    \
    # Primary Dependency: SDL2
    libsdl2-dev \
    \
    # OpenGL Dependencies (Crucial for GLAD and GLSL 450)
    libgl1-mesa-dev \
    \
    # X11 dependencies for SDL2 / OpenGL
    libx11-dev \
    libxext-dev \
    libxrandr-dev \
    \
    # Optional runtime GL utilities (for debugging)
    mesa-utils \
    \
    # Debugging and CI tools
    gdb \
    valgrind \
    xvfb \
 && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Installs glad2 generator to create OpenGL 4.5 headers
# --- GLAD Setup (in isolated virtualenv) ---
RUN python3 -m venv /opt/gladenv && \
    /opt/gladenv/bin/pip install --no-cache-dir glad2 && \
    ln -s /opt/gladenv/bin/glad /usr/local/bin/glad


# --- Workspace Configuration ---
WORKDIR /workspace

# --- Environment variables ---
# Needed to make SDL2 work inside Docker (X11 forwarding)
ENV SDL_VIDEODRIVER=x11
ENV DISPLAY=:99

# --- Default Command ---
CMD ["/bin/bash"]
