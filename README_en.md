<div align="center">
    <img src="https://github.com/knightfox75/ngine/blob/master/Media/ngn_logo.png" alt="N'gine Logo" width="400"/>
</div>

<br/>

<div align="center">

[![Website](https://img.shields.io/badge/Website-ngine.nightfoxandco.com-9cf?style=for-the-badge&logo=)](https://ngine.nightfoxandco.com/)
[![Latest Release](https://img.shields.io/github/v/release/knightfox75/ngine?style=for-the-badge&logo=github)](https://github.com/knightfox75/ngine/releases/latest)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://github.com/knightfox75/ngine/blob/main/LICENSE)

</div>

**[Versión en Español Disponible Aquí](README.md)**

---

**N’gine** is an open-source engine for 2D game development, programmed in **C++** and inspired by classic console techniques. It is specially designed to create games with a *pixel art* aesthetic, utilizing tile-based backgrounds, *sprite-sheets*, and efficient resource management.

With over 10 years of development, it is currently in a stable phase, heavily focused on performance improvements and the addition of new features.

## ✨ Main Features

<details>
<summary><strong>🖱️ Input Methods</strong></summary>
<ul>
    <li>Keyboard</li>
    <li>Mouse</li>
    <li>Gamepads (X-Input standard)</li>
</ul>
</details>

<details>
<summary><strong>🎨 Graphics</strong></summary>
<ul>
    <li>Textures up to 8192×8192 pixels</li>
    <li>Large tile-based backgrounds</li>
    <li>Sprites with and without animation</li>
    <li>2D virtual camera</li>
    <li>Text layers and canvas for primitives</li>
    <li>Blend modes, masks, and up to 8 simultaneous <i>viewports</i></li>
    <li>Full capacity to move, rotate, and scale all graphical elements</li>
</ul>
</details>

<details>
<summary><strong>🔊 Sound</strong></summary>
<ul>
    <li>Up to 4 simultaneous music/dialogue streams (OGG format)</li>
    <li>Up to 64 simultaneous sound effects (WAV format recommended)</li>
    <li>5-channel mixer with volume, pan, pitch, and loop control</li>
</ul>
</details>

<details>
<summary><strong>💥 Collisions</strong></summary>
<ul>
    <li>Bounding box detection with multiple colliders per sprite</li>
    <li>Pixel-perfect detection</li>
    <li><i>Raycasting</i> from a point to a sprite</li>
    <li>Collision maps supporting up to 255 colors</li>
</ul>
</details>

<details>
<summary><strong>📁 File System</strong></summary>
<ul>
    <li>Asset loading from the local file system</li>
    <li>Creation of asset packaging files with optional encryption</li>
    <li>Management of up to 127 resource repositories defined via a text file</li>
</ul>
</details>

## 🚀 Who is N'gine for?

N'gine is intended for anyone with object-oriented programming knowledge who wants to develop their own 2D video game. It was originally born as a custom-made tool for personal projects and serves as the foundational framework for first-year students' projects at the educational center where I teach.

## 💻 Supported Platforms

Currently, the engine works and compiles natively on:
*   **Windows**
*   **Linux** (tested on Linux Mint)
*   **Raspberry Pi OS**
*   **Anbernic RG35XX** (using a Docker image)

## 📸 Screenshots

| The Lost Mines | ADV Game Demo | Idunn's Grace |
|:---:|:---:|:---:|
| ![Screenshot 1](https://github.com/knightfox75/ngine/blob/master/Media/scr01.png) | ![Screenshot 2](https://github.com/knightfox75/ngine/blob/master/Media/scr02.png) | ![Screenshot 3](https://github.com/knightfox75/ngine/blob/master/Media/scr03.png) |

---

## 🛠️ SDK Installation Manual

N'gine has evolved. The SDK has migrated to a modern workflow using **Visual Studio Code** and **CMake**. This manual covers the setup for Windows, Linux, and Raspberry Pi.

> **Note:** The root folder of your distribution is usually named `ngn.X.X.X-stable`.

### 📂 Distribution Package Structure

~~~text
ngn.x.x.x-stable
 ├── Demo                  - Compiled demos for all examples (Win/Linux/RPi)
 ├── Docs                  - Documentation & Manuals
 ├── Examples              - Source code & assets covering all engine features
 ├── LibBuild              - Scripts for compiling/installing the SDK
 │    ├── BuildNgineSDK    - Windows Build Scripts (.bat)
 │    ├── Dockers          - Docker images for Cross-Compile (RG35XX, RPi4)
 │    └── InstallNgineSDK  - Linux/RPi Installation Scripts (.sh)
 ├── LibRelease            - The N'gine library binaries ready to use
 ├── Ngine_SDK             - The N'gine SDK for Windows ready to use
 ├── Source                - N'gine Core Source Code
 ├── Templates             - Project Templates
 │    ├── cmake               - Start Here: Templates for VS Code + CMake
 │    ├── docker_Raspberry_Pi - Templates for Docker compilation (RPi)
 │    └── docker_RG35XX       - Templates for Docker compilation (Anbernic)
 └── Tools                 - Asset conversion tools sources
      ├── NGN_CollisionMap - PNG to Collision Map
      ├── NGN_FileSystem   - Asset Packer
      ├── NGN_Sprite       - PNG to Sprite
      ├── NGN_TileMap      - PNG to Tilemap
      └── ToolSet          - Compiled tools ready to use
~~~

### 🌍 Global Prerequisites (All OS)

Before specific configurations, ensure you have the editor installed:

1. **Visual Studio Code:** [Download here](https://code.visualstudio.com/).
2. **Extensions:** Open VS Code and install these official Microsoft extensions:
   * `C/C++` (IntelliSense, debugging)
   * `CMake Tools` (Project configuration)

![VS Code Extensions Marketplace](https://github.com/knightfox75/ngine/blob/master/Media/marketplace_de_extensiones_vs_code.png)

---

### 🪟 Windows Setup

#### 1. Toolchain Setup
* **Step A:** Download and install [CMake](https://cmake.org/download/). (Make sure to select *"Add CMake to system PATH"* during install).
* **Step B:** Download and install [MSYS2](https://www.msys2.org/).

Open the MSYS2 terminal (MINGW64) and run the following commands to install the compiler:

~~~bash
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain
~~~

#### 2. Environment Variables
Windows needs to know where the compiler and the SDK are located.

* **Add Compiler to Path:**
  Edit the System Environment Variables. Inside the **Path** variable, add the path to your MinGW64 `bin` folder. If you used the default MSYS2 installation path, it will be:
  `C:\msys64\mingw64\bin`
  *(If you installed MSYS2 in a custom location, for example `C:\Code\msys64`, adjust the path accordingly).*

  ![Windows Path Variable Edit](https://github.com/knightfox75/ngine/blob/master/Media/windows_path_variable_edit.png)

* **Define SDK Path:**
  1. Copy the `Ngine_SDK` folder (containing the libs) to a safe place (e.g., `C:\Code\Ngine_SDK`).
  2. Create a **NEW system variable** named `NGINE_SDK_PATH` pointing to that folder.

  ![New System Variable NGINE_SDK_PATH](https://github.com/knightfox75/ngine/blob/master/Media/new_system_variable_ngine_sdk_path.png)

---

### 🐧 Linux / Raspberry Pi Setup

The SDK includes scripts to handle dependencies and library compilation automatically. Navigate to the `LibBuild/InstallNgineSDK/` folder inside the release and run:

~~~bash
chmod +x install_ngine_sdk.sh
./install_ngine_sdk.sh
~~~

![Terminal running install script](https://github.com/knightfox75/ngine/blob/master/Media/terminal_running_install_script.png)

This will install the necessary dependencies (SDL2, SFML) and the N'gine core into `/usr/local/`.

---

### 🎮 Your First Project

1. Navigate to the `Templates/cmake` folder in the SDK.
2. Copy the `basic_template` folder to your workspace (e.g. `C:\Projects\MyGame` or `~/Projects/MyGame`).
3. Open that folder in **Visual Studio Code**.
4. Wait for **CMake Tools** to configure the project. If prompted, select the Kit (`GCC x86_64`).
5. Press **F7** or click "Build" in the bottom status bar to compile.

![VS Code: Project Running](https://github.com/knightfox75/ngine/blob/master/Media/vscode_project_running.png)

---

## 📥 Download

The distribution package contains the **source code**, **compiled libraries**, **documentation**, **commented examples**, and **functional demos**.

You can download the latest version from the **[GitHub Releases Page](https://github.com/knightfox75/ngine/releases/latest)**.

## 🎬 Demo Reel

[![N'gine Demo Reel](https://img.youtube.com/vi/kIcG9lp1oI0/hqdefault.jpg)](https://www.youtube.com/watch?v=kIcG9lp1oI0)

*(Click the image to watch the video)*

## 📜 License

N'gine is distributed under the **MIT License**. This means you can freely use, modify, and distribute it for your projects (commercial or non-commercial), as long as the corresponding attribution is included.

## 📧 Contact

For any questions or suggestions, you can contact me at **contact@nightfoxandco.com**.