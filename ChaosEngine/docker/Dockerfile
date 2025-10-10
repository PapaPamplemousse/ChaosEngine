# ==========================================================
# ChaosEngine — Minimal Devcontainer / Build Environment
# Base: Debian (stable) — C-only, SDL2, Debug tools
# ==========================================================

FROM debian:stable-slim

# --- System Update & Essentials ---
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    pkg-config \
    python3 \
    python3-pip \
    python3-venv \
    \
    # Core runtime libs (needed by SDL2)
    libx11-dev \
    libxext-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    \
    # SDL2 (window, input, audio, timer)
    libsdl2-dev \
    \
    # Debugging tools
    gdb \
    valgrind \
 && apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# --- OPTIONAL: OpenGL Dependencies (comment if unused) ---
# RUN apt-get update && \
#     DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
#     libgl1-mesa-dev && \
#     apt-get clean && rm -rf /var/lib/apt/lists/*

# --- Environment Setup ---
WORKDIR /workspace
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8
ENV SDL_VIDEODRIVER=x11
ENV DISPLAY=:99

# --- Recommended for dev UX ---
RUN python3 -m venv /opt/venv && \
    /opt/venv/bin/pip install --no-cache-dir colorama rich

# Add venv to PATH so it's active by default
ENV PATH="/opt/venv/bin:$PATH"



# --- Default Command ---
CMD ["/bin/bash"]
