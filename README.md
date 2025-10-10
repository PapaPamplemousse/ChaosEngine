# ChaosEngine ⚙️

![Made with C](https://img.shields.io/badge/Made%20with-C-blue.svg?style=for-the-badge\&logo=c)
![Docker Ready](https://img.shields.io/badge/Docker-ready-2496ED.svg?style=for-the-badge\&logo=docker)

An experimental **C game engine** built from scratch — no Unity, no Unreal, no C++… just **pure C**, **SDL2**, and **chaotic determination**.
Designed for **learning**, **experimentation**, and ultimately… **creating full games**.

---

## 🎮 What is ChaosEngine?

ChaosEngine is a **modular and minimalist game engine** written in **C11**,
focused on full control over **rendering**, **physics**, **audio**, and **systems** — without relying on the standard library more than necessary.

It’s a learning sandbox for anyone who wants to understand how a real engine is built from the ground up.

---

## 🧩 Core Features

### 🪐 Physics Engine

* Unified 2D/3D math with `vec_t`
* Gravity, motion, and velocity integration
* Simple **Euler integrator**
* Lightweight physical bodies (`body_t`, `world_t`)
* Deterministic, fixed timestep updates

### 🎨 Rendering Engine

* **SDL2-based renderer** (software or OpenGL-ready backend)
* Window creation, input, and timing
* Color fill, sprite drawing, and debug primitives
* Designed for **pluggable rendering backends** (GL3.3+ later)

### 🔊 Audio Engine

* **miniaudio** (single-header) for mixing and playback
* WAV/OGG loading, volume, and panning
* Modular sound channels

### ⚙️ Engine Core

* Custom allocators (arena/pool)
* Minimal libc replacement (`memcpy`, `strlen`, `itoa`, …)
* Logging, error handling, and profiling utilities
* Portable platform layer (SDL2 + optional native Linux/Win32)
* Clear separation of runtime, physics, audio, resources, and graphics

---

## 🧱 Project Architecture

```
ChaosEngine/
├── cmake/                # Build system (Makefile)
│   └── Makefile
├── docker/               # Docker environment
│   └── Dockerfile
├── scripts/              # Utility scripts
│   └── chaosbuild.sh
├── inc/                  # Public headers (API)
│   ├── core/
│   ├── platform/
│   ├── gfx/
│   ├── audio/
│   ├── physics/
│   ├── resources/
│   └── runtime/
│   └── utility/
├── src/                  # Engine implementations
│   ├── core/
│   ├── platform/
│   ├── gfx/
│   ├── audio/
│   ├── physics/
│   ├── resources/
│   └── runtime/
│   └── utility/
├── examples/             # Playable demos
│   ├── 00_boot/
│   ├── 01_sprites/
│   ├── 02_audio/
│   ├── 03_physics2d/
│   └── 04_scene3d/
├── tests/                # Unit and performance tests
│   ├── unit/
│   └── perf/
└── docs/                 # Documentation & roadmap
    └── ROADMAP.md
```

---

## 🧰 Requirements

ChaosEngine is **written in pure C**, with **minimal dependencies**.

| Component      | Purpose                     | Status     |
| -------------- | --------------------------- | ---------- |
| GCC / Clang    | Compilation                 | ✅ Required |
| SDL2           | Window, input, timer, audio | ✅ Required |
| make / cmake   | Build system                | ✅ Required |
| Python 3       | For tools/scripts           | ✅ Optional |
| miniaudio, stb | Bundled single-header libs  | ✅ Included |

Install essentials on Debian/Ubuntu :

```bash
sudo apt update
sudo apt install build-essential libsdl2-dev make git python3
```

---

## ⚙️ Build Instructions

### 🧱 Build the Library

```bash
make -f cmake/Makefile
```

Builds every `.c` in `ChaosEngine/src` into `lib/libChaosEngine.a`.

### 🧪 Build & Run a Demo

```bash
make -f cmake/Makefile run EXAMPLE=00_boot
```

| Example         | Description           |
| --------------- | --------------------- |
| `00_boot/`      | Engine boot test      |
| `01_sprites/`   | Sprite rendering      |
| `02_audio/`     | Simple audio playback |
| `03_physics2d/` | Physics world         |
| `04_scene3d/`   | Early 3D prototype    |

### 🧹 Clean

```bash
make -f cmake/Makefile clean
make -f cmake/Makefile clean-examples
```

---

## 🐳 Docker Development Environment

A ready-to-use **Docker setup** lives in `docker/Dockerfile`.

### 🧱 Build the Image

```bash
docker build -t chaosengine:latest -f docker/Dockerfile .
```

### 🧑‍💻 Launch a Shell

```bash
docker run --rm -it -v "$(pwd)":/workspace -w /workspace chaosengine:latest /bin/bash
```

Includes :

* GCC, GDB, Valgrind
* SDL2 dev headers
* Python (`rich`, `colorama`)

Perfect for **WSL + VcXsrv** or Linux.

---

## ⚡ Interactive Build Tool (`scripts/chaosbuild.sh`)

```bash
./scripts/chaosbuild.sh
```

### 🧭 Menu

1. 🐳 Rebuild Docker image
2. ⚙️ Build/Rebuild ChaosEngine library
3. 🎮 Compile & run a demo
4. ❌ Exit

💡 On WSL :

```bash
export DISPLAY=:0
```

Launch **VcXsrv** with “Disable access control”.

---

## 🖥️ Local Development Setup

Run ChaosEngine natively on Linux / WSL :

```bash
sudo apt install build-essential libsdl2-dev
export DISPLAY=:0
make -f cmake/Makefile run EXAMPLE=00_boot
```

---

## 🧠 Developer Notes

| Namespace           | Purpose                      |
| ------------------- | ---------------------------- |
| `chaos_core_*`      | Types, memory, logs          |
| `chaos_platform_*`  | OS abstraction, input        |
| `chaos_gfx_*`       | Rendering backend            |
| `chaos_audio_*`     | Audio system                 |
| `chaos_physics_*`   | Physics & collisions         |
| `chaos_resources_*` | Assets & handles             |
| `chaos_runtime_*`   | Engine loop, jobs, profiling |

### 💡 Add a Demo

```bash
mkdir -p examples/my_demo
echo '#include <chaos_engine.h>\nint main(){return 0;}' > examples/my_demo/main.c
make -f cmake/Makefile run EXAMPLE=my_demo
```

---

## 🌍 Roadmap

| Status | Feature                 |
| :----: | :---------------------- |
|    ✅   | Core + logging + memory |
|    ✅   | SDL2 window/input       |
|    ✅   | Software renderer       |
|   🔄   | Audio (miniaudio)       |
|   🔄   | Physics 2D              |
|   🧱   | ECS/Scene graph         |
|   🧩   | Resource system         |
|   🧠   | Job system              |
|   🧾   | Profiling               |
|   🌈   | Optional OpenGL backend |

---

## 🧙 Developer Motto

ChaosEngine is an exploration of what happens
when you build everything yourself — with precision, style, and a bit of madness.

> **Let’s build Chaos together 🌀**
