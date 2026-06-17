# FluidSim

This is my real-time 2D Eulerian fluid simulation on the GPU using the Navier-Stokes equations, rendered with OpenGL 4.6.

## Demo

https://github.com/user-attachments/assets/21c90832-7f13-413e-9409-9707d96097e6
- Limited to the red channel

https://github.com/user-attachments/assets/f74cb5bf-234a-4d32-a436-dc0620b626d7
- Uses smoothstep in `fire.frag` to give glowing flame effect

https://github.com/user-attachments/assets/b360a101-24a8-4ed3-a097-940428d939d8
- Hue cycling for different colors

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

1. Install [vcpkg](https://vcpkg.io):

```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg; .\bootstrap-vcpkg.bat
```

2. Integrate it so that it is available to Visual Studio 

```powershell
.\vcpkg integrate install
```

3. Clone the repo using Visual Studio
4. Select a build preset (e.g. `x64-debug` or `x64-release`)

<img width="272" height="113" alt="image" src="https://github.com/user-attachments/assets/2c151702-3069-4b5e-a84d-003232d27629" />

6. Build: `Ctrl+Shift+B`

7. Run: `Ctrl + F5` or select:

<img width="370" height="70" alt="image" src="https://github.com/user-attachments/assets/b598475e-20f7-46a8-84e8-ce663e1e2b67" />

## Download Executables from Release
1. Go to [Release](https://github.com/rsoo23/FluidSim/releases/tag/main)
2. Download and unzip the latest `FluidSims.zip`
3. Click on each .exe file to run (Currently it has 3 executables for each color mode since runtime color mode changing is currently not supported)

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
