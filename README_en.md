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

With **over 10 years of development**, it is currently in a stable phase, heavily focused on performance improvements and the addition of new features.

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
<summary><strong>🎬 Video</strong></summary>
<ul>
    <li>Support for OGV (Theora) video file playback</li>
    <li>Up to 4 simultaneous video streams with audio/video synchronization</li>
    <li>Full playback control: Start, pause, stop, and loop mode</li>
    <li>Complete video render transformation: Position, rotation, and scaling</li>
    <li>Visual effects applicable to the stream: Transparency (Alpha), RGB tinting, and blend modes</li>
</ul>
</details>

<details>
<summary><strong>🔊 Sound</strong></summary>
<ul>
    <li>Up to 4 simultaneous music/dialogue streams (OGG format)</li>
    <li>Up to 64 simultaneous sound effects (WAV format recommended)</li>
    <li><strong>6-channel mixer</strong> with volume, pan, pitch, and loop control</li>
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
    <li>Load and save functions for binary or text files</li>
    <li>Management of up to 127 resource repositories defined via a plain text file</li>
</ul>
</details>

## 🚀 Who is N'gine for?

N'gine is intended for anyone with object-oriented programming knowledge who wants to develop their own 2D video game. It was originally born as a custom-made tool for personal projects and serves as the foundational framework for first-year students' projects at the educational center where I teach Game Design and Programming.

## 💻 Supported Platforms

Currently, the engine runs and allows exporting for:
*   **Windows**
*   **Linux** (tested on Linux Mint / Ubuntu)
*   **Raspberry Pi OS**
*   **Anbernic RG35XX** (using the provided Docker images)
*   **Evercade***

> <small>* Evercade export requires an official developer license from Blaze Entertainment Ltd.</small>

## 📸 Screenshots

| The Lost Mines | ADV Game Demo | Idunn's Grace |
|:---:|:---:|:---:|
| ![Screenshot 1](https://github.com/knightfox75/ngine/blob/master/Media/scr01.png) | ![Screenshot 2](https://github.com/knightfox75/ngine/blob/master/Media/scr02.png) | ![Screenshot 3](https://github.com/knightfox75/ngine/blob/master/Media/scr03.png) |

---

## 🛠️ SDK Installation Manual

N'gine utilizes a modern workflow based on **Visual Studio Code** and **CMake**.

### 📂 Distribution Package Structure

The package includes the engine's source code, compiled libraries for Windows, automation scripts for Linux, and Docker container support.

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
 ├── Templates             - Project Templates (CMake, Docker)
 └── Tools                 - Asset conversion tools (Sprites, Tilemaps, FileSystem)
~~~

### 🌍 Global Prerequisites

1. **Visual Studio Code:** [Download here](https://code.visualstudio.com/).
2. **Extensions:** Install `C/C++` and `CMake Tools` from the Marketplace.

---

### 🪟 Windows Setup

1. Install [CMake](https://cmake.org/download/) (add it to the system PATH).
2. Install [MSYS2](https://www.msys2.org/) and run in its terminal:
   `pacman -S mingw-w64-x86_64-toolchain`
3. Add the MinGW64 `bin` path to your Windows **Path**.
4. Create a system environment variable named `NGINE_SDK_PATH` pointing to your `Ngine_SDK` folder.

---

### 🐧 Linux / Raspberry Pi Setup

The SDK simplifies dependency management (SDL2, SFML, etc.) via scripts:
1. Navigate to `LibBuild/InstallNgineSDK/`.
2. Run:
   ~~~bash
   chmod +x install_ngine_sdk.sh
   ./install_ngine_sdk.sh
   ~~~

---

## 📥 Download

You can obtain the source code, documentation, and binaries from the **[GitHub Releases Page](https://github.com/knightfox75/ngine/releases/latest)**.

## 📜 License

N'gine is distributed under the **MIT License**. You are free to use, modify, and distribute it (including commercial projects) as long as the corresponding attribution is included.

## 📧 Contact

For questions, suggestions, or support: **contact@nightfoxandco.com**.