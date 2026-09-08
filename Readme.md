# Biomorph Generator

An interactive C++ and Qt Quick application for rendering and exploring complex [Pickover Biomorphs](https://en.wikipedia.org/wiki/Pickover_stalk#Biomorphs). Powered by an OpenCL backend using 64-bit double precision (`cl_khr_fp64`), it offloads complex fractal iterations to the GPU while maintaining a responsive Qt user interface.

> **Status:** Early Preview / Active Development. Currently implements a two-step cubic ($z^3$) biomorph scheme with alternating growth vectors. Support for additional algebraic and transcendental functions is planned.

<img width="1102" height="783" alt="image" src="https://github.com/user-attachments/assets/c46cc802-021b-46a5-b025-27c7d6369b32" />

## Mathematical Engine

Unlike standard Mandelbrot or Julia fractals, the biomorph kernel utilizes a two-stage relaxation/interpolation scheme combined with Pickover boundary conditions:

* **Iterative Scheme**: Relaxes two evaluation points $v$ and $z$ per step using weighting parameters $\alpha$ and $\beta$:
  $$v = \beta (f(z) + d) + (1 - \beta)z$$
  $$z = \alpha (f(v) + d) + (1 - \alpha)z$$
  where $f(z) = z^3$.
* **Growth Vector Switching**: An alternating modifier $d$ toggles between growth constants $c_1$ and $c_2$ using an inverse power transformation $c^{-i} - d$ on alternating iteration steps $i$.
* **Pickover Escape Test**: Pixels are categorized by testing whether the real or imaginary components remain bounded relative to escape threshold $R$:
  $$\lvert \text{Re}(z) \rvert < R \quad \text{or} \quad \lvert \text{Im}(z) \rvert < R$$
* **Smooth Palette Mapping**: Calculates continuous fractional iteration values $t = i + 1 - \nu$ (using logarithmic scaling) and maps them to a sinusoidal RGB color space.

## Key Features

* **GPU-Accelerated**: Native OpenCL compute kernel running on 64-bit floats for precision during deep zoom levels.
* **Interactive Viewport Controls**:
  * Pan in all four cardinal directions with automatic step scaling.
  * Zoom in/out anchored to current domain center.
  * **Fix 1:1 Ratio** action to immediately re-square distorted aspect ratios.
* **Non-Blocking Generation**: Multi-threaded QML/C++ bridge runs calculations asynchronously without locking the UI.
* **High-Resolution Export**: Native file dialog integration utilizing `stb_image_write.h` for PNG image saving.

## Prerequisites & Build Requirements

* **Compiler**: C++17 or C++20 compliant compiler (MSVC, GCC, Clang)
* **GUI Framework**: Qt 6.x (Qt Quick, Controls, Layouts, Dialogs)
* **Compute Drivers**: OpenCL GPU runtime drivers and development headers (`cl_khr_fp64` extension support required)
* **Build Tooling**: CMake 3.16+

## Planned Improvements
* Add selectable base formulas ($z^4$, $z^5$, $e^z$, sin(z)).
* Interactive canvas mouse dragging (click and drag to pan).
* Custom palette preset selection directly from the sidebar.
* OpenCL fallback rendering mode for hardware without double-precision support.

## Examples
<img width="4000" height="4000" alt="bio6" src="https://github.com/user-attachments/assets/7f2a0f96-b00d-48d1-bfd8-813d8dca35f4" />
<img width="4000" height="4000" alt="bio5" src="https://github.com/user-attachments/assets/6dc21ec4-1fff-4a83-9389-f8c020a35931" />
<img width="4000" height="4000" alt="bio4" src="https://github.com/user-attachments/assets/67176eb1-fbcf-474a-b916-5c6ec985d150" />

## Building and Running

Ensure you have your environment set up and run the following commands:
   ```bash
   git clone https://github.com/handlaez/OpenCLBiomorph.git
   cd OpenCLBiomorph
   cmake -B build -S . -DCMAKE_PREFIX_PATH="C:/YourQtPath/version/msvc2022_64"
   cmake --build build --config Release
   ./build/OpenCLBiomorph.exe
   ```

## License
This project is licensed under the MIT License - see the LICENSE file for details.
