# ChaosEngine ⚙️

An experimental **C game engine** built from scratch — no Unity, no Unreal, just **SDL2 + OpenGL 4.5** and pure chaos.
Designed for **learning**, **experimentation**, and eventually… **making real games**.

---

## 🎮 What is ChaosEngine?

ChaosEngine is a **minimalist game engine** written in **C99**,
built around the idea that you can create a complete 2D/3D rendering and physics stack from the ground up.

It’s lightweight, modular, and designed for **game development experiments** — from particle systems to physics-driven worlds, with full control over the math, rendering, and memory layout.

---

## 🧩 Core Features

### 🪐 Physics Engine

* Unified 2D/3D vector math (`vec_t`)
* Gravity, motion, and velocity integration
* Simple **Euler integration**
* Lightweight entities (`body_t`, `world_t`)
* Deterministic stepping for fixed-timestep simulation

### 🎨 Rendering Engine

* Modern **OpenGL 4.5 Core Profile**
* Auto-generated **GLAD 2** OpenGL loader
* **SDL2** for context, windowing, and input
* Uniform Buffer Objects for camera management
* Real-time unlit & textured shader pipelines
* Debug helpers: lines, grids, wireframes

### 🧠 Architecture

* Fully modular C codebase
* Independent modules: `ChaosMath`, `ChaosRenderer`, `ChaosPhysics`
* Built to evolve toward an **ECS** + **scene graph** system

---

## 🕹️ Available Demos

ChaosEngine comes with small, playable demos for testing core systems:

| Demo                           | Description                          |
| ------------------------------ | ------------------------------------ |
| 🟢 **Bouncing Ball**           | Basic physics, gravity & restitution |
| 🌧️ **Rain Particles**         | GPU particle system demo             |

---

## 🧱 Project Structure

```
ChaosEngine/
├── inc/                  # Engine headers
│   ├── chaos_math.h
│   ├── chaosrenderer.h
│   ├── chaos_physics.h
│   └── chaosengine.h
├── src/                  # Engine sources
│   ├── chaosmath.c
│   ├── chaosrenderer.c
│   └── chaosphysics.c
├── glad/                 # Auto-generated OpenGL loader (GLAD 2)
│   ├── include/glad/gl.h
│   └── src/gl.c
├── examples/             # Playable demos
│   ├── bouncing_ball/
│   └── rain_particles/
├── Dockerfile
├── chaosbuild.sh         # Interactive Docker build & run tool 🐳
└── Makefile
```

---

## ⚙️ Build Instructions

### 🧰 Requirements

* GCC or Clang (C99)
* Make
* SDL2 development libraries
* Python ≥ 3.10 (for GLAD 2 generation)

On Debian/Ubuntu:

```bash
sudo apt update
sudo apt install build-essential libsdl2-dev python3-pip
pip install glad2
```

---

### 🔧 Local Build

```bash
make         # Build the ChaosEngine library and demos
make run     # Run the default demo
make clean   # Clean build artifacts
```

---

## 🐳 Docker Build & Run

Prefer a clean, reproducible environment?
ChaosEngine includes a **Docker-based build system** for development, testing, or WSL setups.

### 🧱 Build the Docker Image

```bash
make docker-build
```

### 🧑‍💻 Run Inside Docker

```bash
make docker-shell
```

You’ll get a terminal with all dependencies (GCC, SDL2, GLAD 2) preinstalled.
Perfect for WSL + VcXsrv or Linux.

---

## ⚡ Interactive Build Tool (`chaosbuild.sh`)

A colorful, interactive terminal interface for managing builds and demos:

```bash
./chaosbuild.sh
```

🧭 Menu options:

1. 🐳 Rebuild Docker image
2. ⚙️ Build/Rebuild ChaosEngine library
3. 🎮 Compile and run available demos
4. ❌ Exit

💡 On **Windows/WSL**, make sure:

```bash
export DISPLAY=:0
```

and an X server (VcXsrv / X410) is running.

---

## 🧾 License

MIT License © 2025 — Developed by **PapaPamplemousse**

---

## 🌍 Roadmap

| Status | Feature                        |
| :----: | ------------------------------ |
|    ✅   | Physics + Rendering Core       |
|    ✅   | OpenGL 4.5 + SDL2 backend      |
|   🔄   | Collision system & rigidbodies |
|   🌈   | Lighting & material system     |
|   🧩   | ECS + scene graph              |
|   🧠   | Lua scripting support          |
|   🧱   | Asset loader (OBJ, PNG, etc.)  |
|   🪄   | Editor with live scene preview |

---

## 🧙 Developer Notes

ChaosEngine is still in active development —
below are some tips and conventions to keep your chaos organized 🌀

### 💡 Adding a New Demo

1. Create a new folder under `examples/your_demo_name/`
2. Add a `src/demo.c` file using the template from `examples/bouncing_ball/`
3. Your demo automatically appears in the menu when running:

   ```bash
   ./chaosbuild.sh
   ```
4. Include the core headers you need:

   ```c
   #include <chaosrenderer.h>
   #include <chaos_physics.h>
   #include <chaos_math.h>
   ```

### 🧱 Adding a New Module

* Create your `.c` and `.h` under `ChaosEngine/src` and `ChaosEngine/inc`
* Add the `.o` file to the Makefile’s `OBJS` section
* Rebuild with:

  ```bash
  ./chaosbuild.sh
  ```

### 🧩 Code Style

* C99 standard, strict `-Wall -Wextra -Wpedantic`
* No C++ features (for now)
* Each subsystem has its own namespace prefix:

  * `chaos_` for global engine
  * `chaos_renderer_` for rendering code
  * `chaos_math_` for math helpers

---

## 💬 Contributing

ChaosEngine is still experimental — but contributions are welcome.
Whether you want to fix bugs, add new demos, or propose engine architecture improvements:

👉 Open a PR or issue on [GitHub](https://github.com/PapaPamplemousse/ChaosEngine).

Let’s build Chaos together 🌀



