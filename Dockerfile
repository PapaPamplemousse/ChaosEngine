FROM mcr.microsoft.com/devcontainers/base:debian


# Pré-installation des outils de build
# RUN apt-get update && apt-get install -y \
#     build-essential \
#     cmake \
#     git \
#     pkg-config \
#     libsdl2-dev \
#     libgl1-mesa-dev \
#     libglfw3-dev \
#     libglew-dev \
#     libxrandr-dev \
#     libxi-dev \
#     libxxf86vm-dev \
#     libxcursor-dev \
#  && rm -rf /var/lib/apt/lists/*
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    libsdl2-dev \
 && rm -rf /var/lib/apt/lists/*
 
WORKDIR /workspace


# Par défaut, on exécute un shell sh (utile pour entrer dans le conteneur)
CMD ["/bin/sh"]