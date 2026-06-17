# FluidSim

This is my real-time 2D Eulerian fluid simulation on the GPU using the Navier-Stokes equations, rendered with OpenGL 4.6.

## Tools & Libraries

- **OpenGL 4.6** — rendering and compute shaders
- **GLFW** — window creation and input handling
- **GLAD** — OpenGL function loader
- **GLM** — math (vectors)
- **CMake** — build system
- **vcpkg** — package manager

## Concepts

- Navier-Stokes fluid simulation (velocity, pressure, density, vorticity)
- Vector Calculus (divergence, curl, grad)
- GPU compute shaders (advection, diffusion, Jacobi pressure solve, vorticity confinement)
- Direct State Access (DSA) OpenGL API
- Ping-pong texture buffering
- Custom class for RAII resource management for OpenGL textures
- Shader class hierarchy with shared base (`BaseShader` → `VertFragShader`, `ComputeShader`)
- HSV color cycling with Reinhard tone mapping and gamma correction

## Build

Dependencies (`glfw3`, `glad`, `glm`) are declared in `vcpkg.json` and installed automatically by vcpkg during CMake configuration — no manual `vcpkg install` needed.

### Visual Studio

1. Install [vcpkg](https://vcpkg.io) and integrate it: `vcpkg integrate install`
2. Clone the repo using Visual Studio
3. Select a build preset (e.g. `x64-debug` or `x64-release`)
4. **Build → Build All** (`Ctrl+Shift+B`)
5. Run via **Debug → Start Debugging** (`F5`)

## Performance

Averages ~70 FPS (Debug Build) and ~80 FPS (Release Build) at 800×800 with 40 Jacobi iterations on an NVIDIA GPU measured with RenderDoc.

## Configurables

All simulation parameters are defined at the top of `main.cpp`. Static asserts enforce valid ranges at compile time.

| Parameter                    | Default      | Valid Range                         | Description                                             |
| ---------------------------- | ------------ | ----------------------------------- | ------------------------------------------------------- |
| `colorMode`                  | `MultiColor` | `SingleColor`, `MultiColor`, `Fire` | Rendering mode                                          |
| `jacobiIterations`           | `40`         | `1 – 40`                            | Jacobi solver accuracy (higher = more accurate, slower) |
| `diffusionCoeff`             | `1.0`        | `0.0 – 1.0`                         | How fast density spreads                                |
| `viscosityCoeff`             | `0.1`        | `0.0 – 1.0`                         | How fast velocity diffuses                              |
| `densityIncrement`           | `0.1`        | `0.0 – 1.0`                         | Base density added per splat                            |
| `densityIncrementMultiplier` | `100.0`      | `0.0 – 100.0`                       | Scales density injected per frame                       |
| `cursorRadius`               | `20.0`       | `0.0 – width/10`                    | Gaussian splat radius in pixels                         |
| `forceMultiplier`            | `1000.0`     | `0.0 – 1000.0`                      | Scales force applied by cursor drag                     |
| `vorticityCoeff`             | `10.0`       | `0.0 – 10.0`                        | Vorticity confinement strength (adds swirl)             |

## Future Improvements

- Diffusion step for `MultiColor` texture (currently only advected, not diffused)
- Boundary conditions (solid walls, no-slip borders)
- Mouse drag direction → hue so each stroke has a unique color
- Fine-tuning workgroup size for maximum GPU throughput
- Runtime parameter adjustment via ImGui overlay
