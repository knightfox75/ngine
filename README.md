<div align="center">
    <img src="https://github.com/knightfox75/ngine/blob/master/Media/ngn_logo.png" alt="Logo de N'gine" width="400"/>
</div>

<br/>

<div align="center">

[![Website](https://img.shields.io/badge/Website-ngine.nightfoxandco.com-9cf?style=for-the-badge&logo=)](https://ngine.nightfoxandco.com/)
[![Latest Release](https://img.shields.io/github/v/release/knightfox75/ngine?style=for-the-badge&logo=github)](https://github.com/knightfox75/ngine/releases/latest)
[![License: MIT](https://img.shields.io/badge/Licencia-MIT-yellow.svg?style=for-the-badge)](https://github.com/knightfox75/ngine/blob/main/LICENSE)

</div>

**[English Version Available Here](README_en.md)**

---

**N’gine** es un motor de código abierto para el desarrollo de juegos 2D, programado en **C++** e inspirado en las técnicas de las consolas clásicas. Está especialmente diseñado para crear juegos con estética *pixel art*, utilizando fondos de tiles, *sprite-sheets* y una gestión de recursos eficiente.

Con más de **10 años de desarrollo**, se encuentra en una fase estable, con el foco puesto en la mejora de rendimiento y la incorporación de nuevas funcionalidades.

## ✨ Características Principales

<details>
<summary><strong>🖱️ Métodos de Entrada</strong></summary>
<ul>
    <li>Teclado</li>
    <li>Ratón</li>
    <li>Gamepads (estándar X-Input)</li>
</ul>
</details>

<details>
<summary><strong>🎨 Gráficos</strong></summary>
<ul>
    <li>Texturas de hasta 8192×8192 píxeles</li>
    <li>Fondos basados en tiles de gran tamaño</li>
    <li>Sprites con y sin animación</li>
    <li>Cámara virtual 2D</li>
    <li>Capas de texto y lienzo para primitivas</li>
    <li>Modos de mezcla, máscaras y hasta 8 <i>viewports</i> simultáneos</li>
    <li>Capacidad para mover, rotar y escalar todos los elementos gráficos</li>
</ul>
</details>

<details>
<summary><strong>🎬 Vídeo</strong></summary>
<ul>
    <li>Soporte para reproducción de archivos en formato OGV (Theora)</li>
    <li>Hasta 4 streams de vídeo simultáneos con sincronización de audio y vídeo</li>
    <li>Control total de reproducción: Iniciar, pausar, detener y modo bucle (loop)</li>
    <li>Transformación completa del render de vídeo: Posición, rotación y escalado</li>
    <li>Efectos visuales aplicables al stream: Transparencia (Alpha), tintado RGB y modos de mezcla</li>
</ul>
</details>

<details>
<summary><strong>🔊 Sonido</strong></summary>
<ul>
    <li>Hasta 4 streams de música/diálogos simultáneos (formato OGG)</li>
    <li>Hasta 64 efectos de sonido simultáneos (formato WAV recomendado)</li>
    <li>Mezclador de <strong>6 canales</strong> con control de volumen, paneo, pitch y loop</li>
</ul>
</details>

<details>
<summary><strong>💥 Colisiones</strong></summary>
<ul>
    <li>Detección por cajas (<i>bounding box</i>) con múltiples colisionadores por sprite</li>
    <li>Detección a nivel de píxel</li>
    <li><i>Raycasting</i> desde un punto a un sprite</li>
    <li>Mapas de colisión de hasta 255 colores</li>
</ul>
</details>

<details>
<summary><strong>📁 Sistema de Archivos</strong></summary>
<ul>
    <li>Carga de recursos desde el sistema de archivos local</li>
    <li>Creación de archivos de empaquetado de recursos con opción de cifrado</li>
    <li>Funciones de carga y grabación de archivos en formato binario o texto</li>
    <li>Gestión de hasta 127 repositorios de recursos definidos en un fichero de texto plano</li>
</ul>
</details>

## 🚀 ¿Para Quién es N'gine?

N'gine está destinado a cualquier persona con nociones de programación orientada a objetos que quiera desarrollar su propio videojuego 2D. Nació como una herramienta a medida para proyectos personales y como base para los proyectos de los estudiantes de primer curso en el centro donde imparto docencia como profesor de diseño y programación de videojuegos.

## 💻 Plataformas Soportadas

Actualmente, el motor funciona y permite exportar para:
*   **Windows**
*   **Linux** (probado en Linux Mint / Ubuntu)
*   **Raspberry Pi OS**
*   **Anbernic RG35XX** (usando imágenes de Docker proporcionadas)
*   **Evercade***

> <small>* La exportación a Evercade requiere de la licencia oficial de desarrollador por parte de Blaze Entertainment Ltd.</small>

## 📸 Capturas de Pantalla

| The Lost Mines | ADV Game Demo | Idunn's Grace |
|:---:|:---:|:---:|
| ![Screenshot 1](https://github.com/knightfox75/ngine/blob/master/Media/scr01.png) | ![Screenshot 2](https://github.com/knightfox75/ngine/blob/master/Media/scr02.png) | ![Screenshot 3](https://github.com/knightfox75/ngine/blob/master/Media/scr03.png) |

---

## 🛠️ Manual de Instalación del SDK

N'gine utiliza un flujo de trabajo moderno basado en **Visual Studio Code** y **CMake**.

### 📂 Estructura del paquete de distribución

El paquete incluye el código fuente, librerías compiladas para Windows, scripts de automatización para Linux y soporte para contenedores Docker.

~~~text
ngn.x.x.x-stable
 ├── Demo                  - Demos compiladas de todos los ejemplos (Win/Linux/RPi)
 ├── Docs                  - Documentación y manuales
 ├── Examples              - Código fuente y assets cubriendo todas las funciones
 ├── LibBuild              - Scripts y código para compilar/instalar el SDK
 │    ├── BuildNgineSDK    - Windows Build Scripts (.bat)
 │    ├── Dockers          - Imágenes Docker para compilación cruzada (RG35XX, RPi4)
 │    └── InstallNgineSDK  - Scripts de instalación Linux/RPi (.sh)
 ├── LibRelease            - Binarios de la librería N'gine listos para usar
 ├── Ngine_SDK             - El SDK N'gine para Windows listo para usar
 ├── Source                - Código fuente del núcleo de N'gine
 ├── Templates             - Plantillas de Proyectos (CMake, Docker)
 └── Tools                 - Herramientas de conversión (Sprites, Tilemaps, FileSystem)
~~~

### 🌍 Prerrequisitos Globales

1. **Visual Studio Code:** [Descargar aquí](https://code.visualstudio.com/).
2. **Extensiones:** Instalar `C/C++` y `CMake Tools` desde el Marketplace.

---

### 🪟 Configuración para Windows

1. Instalar [CMake](https://cmake.org/download/) (añadir al PATH).
2. Instalar [MSYS2](https://www.msys2.org/) y ejecutar en su terminal:
   `pacman -S mingw-w64-x86_64-toolchain`
3. Añadir la ruta `bin` de MinGW64 al **Path** de Windows.
4. Crear la variable de entorno de sistema `NGINE_SDK_PATH` apuntando a tu carpeta `Ngine_SDK`.

---

### 🐧 Configuración para Linux / Raspberry Pi

El SDK facilita la gestión de dependencias (SDL2, SFML, etc.) mediante scripts:
1. Navega a `LibBuild/InstallNgineSDK/`.
2. Ejecuta:
   ~~~bash
   chmod +x install_ngine_sdk.sh
   ./install_ngine_sdk.sh
   ~~~

---

## 📥 Descargar

Puedes obtener el código fuente, la documentación y los binarios desde la **[página de Releases de GitHub](https://github.com/knightfox75/ngine/releases/latest)**.

## 📜 Licencia

N'gine se distribuye bajo la **Licencia MIT**. Puedes usarlo, modificarlo y distribuirlo libremente (proyectos comerciales incluidos) siempre que mantengas la atribución correspondiente.

## 📧 Contacto

Para dudas, sugerencias o soporte: **contact@nightfoxandco.com**.