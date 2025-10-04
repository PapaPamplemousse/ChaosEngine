# ChaosEngine ⚙️

A lightweight experimental **physics and rendering engine** written in pure **C**,  
featuring a modular architecture and a minimal **software + SDL2 renderer**.

---

## 🚀 Overview

ChaosEngine is designed for **low-level learning**, **graphics experimentation**, and **physics simulation**.  
It’s written entirely in C, with only **SDL2** as an external dependency, and can run both **natively** or inside **Docker**.

---

## 🧩 Features

### 🪐 Physics Engine
- Unified 2D/3D vector math (`vec_t`)
- Gravity, velocity, and motion integration
- Simple **Euler integration**
- Lightweight data structures (`body_t`, `world_t`)

### 🎨 Rendering Engine
- **SDL2-based renderer** (no OpenGL)
- Real-time display window
- Extensible primitives (`primitive_t`)
- Example: **bouncing ball** on a green plane

---

## 🧱 Project Structure

```

ChaosEngine/
├── inc/          # Engine headers
│   ├── chaos_engine.h
│   ├── physics.h
│   ├── renderer.h
│   └── types.h
├── src/          # Engine sources
│   ├── physics.c
│   └── renderer.c
├── examples/     # Demos
│   └── demo.c
├── Dockerfile
├──  chaosbuild.sh   # Docker helper script 🐳
└── Makefile

````

---

## ⚙️ Build Instructions

### 🧰 Requirements
- GCC or Clang (C99)
- Make
- SDL2 development library

Install on Debian/Ubuntu:
```bash
sudo apt update
sudo apt install build-essential libsdl2-dev
````

---

### 🔧 Local Build

```bash
make         # Build the library and demo
make run     # Run the bouncing ball demo
make clean   # Clean build artifacts
```

---

## 🐳 Docker Build & Run

ChaosEngine can be built and run inside a **Docker container**,
perfect for consistent development environments or WSL setups.

### 🧱 Build the Docker Image

```bash
make docker-build
```

This creates an image named `chaos-engine-dev`.

### 🧑‍💻 Open a Shell in Docker

```bash
make docker-shell
```

You’ll get an interactive terminal inside `/workspace`
with `gcc`, `make`, and `SDL2` already installed.

---

## ⚡ Automated Build Script (` chaosbuild.sh`)

You can also use the helper script provided in the repo:

```bash
./ chaosbuild.sh
```

This script will:

1. Automatically rebuild the Docker image if necessary
2. Compile the project inside the container
3. Run the SDL demo with **X11 forwarding** for GUI display (works with WSL + VcXsrv or X410)

If you’re using **Windows + WSL**, make sure:

* You have an **X server** running (VcXsrv or X410)
* The `DISPLAY` variable is set (usually `:0`)

Example manual launch:

```bash
export DISPLAY=:0
./ chaosbuild.sh run
```


## 🧾 License

MIT License © 2025 — Developed by **PapaPamplemousse**

---

## 🌍 Roadmap

* ✅ Modular physics + renderer subsystems
* ✅ SDL2 rendering with software projection
* 🔄 Collision detection system
* 🌈 Lighting and shading (software-based)
* 🧩 Scene graph and scripting
* 🧠 OpenGL/Vulkan backend option

---

## 💬 Contributing

Contributions, bug reports, and ideas are welcome!
Open an issue or pull request on [GitHub](https://github.com/yourusername/ChaosEngine).

Let’s build Chaos together 🌀
