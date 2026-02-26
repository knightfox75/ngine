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

Con más de 10 años de desarrollo, se encuentra en una fase estable, con el foco puesto en la mejora de rendimiento y la incorporación de nuevas funcionalidades.

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
<summary><strong>🔊 Sonido</strong></summary>
<ul>
    <li>Hasta 4 streams de música/diálogos simultáneos (formato OGG)</li>
    <li>Hasta 64 efectos de sonido simultáneos (formato WAV recomendado)</li>
    <li>Mezclador de 5 canales con control de volumen, paneo, pitch y loop</li>
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
    <li>Gestión de hasta 127 repositorios de recursos definidos en un fichero de texto</li>
</ul>
</details>

## 🚀 ¿Para Quién es N'gine?

N'gine está destinado a cualquier persona con nociones de programación orientada a objetos que quiera desarrollar su propio videojuego 2D. Nació como una herramienta a medida para proyectos personales y como base para los proyectos de los estudiantes de primer curso en el centro donde imparto docencia.

## 💻 Plataformas Soportadas

Actualmente, el motor funciona y se compila en:
*   **Windows**
*   **Linux** (probado en Linux Mint)
*   **Raspberry Pi OS**
*   **Anbernic RG35XX** (usando una imagen de Docker)

## 📸 Capturas de Pantalla

| The Lost Mines | ADV Game Demo | Idunn's Grace |
|:---:|:---:|:---:|
| ![Screenshot 1](https://github.com/knightfox75/ngine/blob/master/Media/scr01.png) | ![Screenshot 2](https://github.com/knightfox75/ngine/blob/master/Media/scr02.png) | ![Screenshot 3](https://github.com/knightfox75/ngine/blob/master/Media/scr03.png) |

---

## 🛠️ Manual de Instalación del SDK

N'gine ha evolucionado. El SDK ha migrado a un flujo de trabajo moderno usando **Visual Studio Code** y **CMake**. A continuación, se detalla la configuración para Windows, Linux y Raspberry Pi.

> **Nota:** La carpeta raíz de tu distribución suele llamarse `ngn.X.X.X-stable`.

### 📂 Estructura del paquete de distribución

~~~text
ngn.x.x.x-stable
 ├── Demo                  - Demos compiladas de todos los ejemplos (Win/Linux/RPi)
 ├── Docs                  - Documentación y manuales
 ├── Examples              - Código fuente y assets cubriendo todas las funciones
 ├── LibBuild              - Scripts y código para compilar/instalar el SDK
 │    ├── BuildNgineSDK    - Windows Build Scripts (.bat)
 │    ├── Dockers          - Imágenes Docker para Cross-Compile (RG35XX, RPi4)
 │    └── InstallNgineSDK  - Scripts de instalación Linux/RPi (.sh)
 ├── LibRelease            - Binarios de la librería N'gine listos para usar
 ├── Ngine_SDK             - El SDK N'gine para Windows listo para usar
 ├── Source                - Código fuente del núcleo de N'gine
 ├── Templates             - Plantillas de Proyectos
 │    ├── cmake               - Empieza Aquí: Plantillas para VS Code + CMake
 │    ├── docker_Raspberry_Pi - Plantillas para compilación con Docker (RPi)
 │    └── docker_RG35XX       - Plantillas para compilación con Docker (Anbernic)
 └── Tools                 - Fuentes de las herramientas de conversión
      ├── NGN_CollisionMap - PNG to Collision Map
      ├── NGN_FileSystem   - Asset Packer
      ├── NGN_Sprite       - PNG to Sprite
      ├── NGN_TileMap      - PNG to Tilemap
      └── ToolSet          - Herramientas compiladas listas para usar
~~~

### 🌍 Prerrequisitos Globales (Todos los Sistemas Operativos)

Antes de las configuraciones específicas, asegúrate de tener el editor instalado:

1. **Visual Studio Code:** [Descargar aquí](https://code.visualstudio.com/).
2. **Extensiones:** Abre VS Code e instala estas extensiones oficiales de Microsoft:
   * `C/C++` (IntelliSense, depuración)
   * `CMake Tools` (Configuración de proyectos)

![VS Code Extensions Marketplace](https://github.com/knightfox75/ngine/blob/master/Media/marketplace_de_extensiones_vs_code.png)

---

### 🪟 Configuración para Windows

#### 1. Configuración de Herramientas
* **Paso A:** Descarga e instala [CMake](https://cmake.org/download/). (Asegúrate de seleccionar *"Add CMake to system PATH"* durante la instalación).
* **Paso B:** Descarga e instala [MSYS2](https://www.msys2.org/).

Abre la terminal de MSYS2 (MINGW64) y ejecuta los siguientes comandos para instalar el compilador:

~~~bash
pacman -Syu
pacman -S mingw-w64-x86_64-toolchain
~~~

#### 2. Variables de Entorno
Windows necesita saber dónde están el compilador y el SDK.

* **Añadir Compilador al Path:**
  Edita las Variables de Entorno del Sistema. Dentro de la variable **Path**, añade la ruta a tu carpeta `bin` de MinGW64. Si usaste la ruta de instalación por defecto, será:
  `C:\msys64\mingw64\bin`
  *(Si instalaste MSYS2 en otro directorio, por ejemplo en `C:\Code\msys64`, ajusta la ruta en consecuencia).*

  ![Windows Path Variable Edit](https://github.com/knightfox75/ngine/blob/master/Media/windows_path_variable_edit.png)

* **Definir Ruta del SDK:**
  1. Copia la carpeta `Ngine_SDK` (que contiene las librerías) a un lugar seguro (ej. `C:\Code\Ngine_SDK`).
  2. Crea una **NUEVA variable de entorno del sistema** llamada `NGINE_SDK_PATH` que apunte a esa carpeta.

  ![Nueva Variable de Sistema NGINE_SDK_PATH](https://github.com/knightfox75/ngine/blob/master/Media/new_system_variable_ngine_sdk_path.png)

---

### 🐧 Configuración para Linux / Raspberry Pi

El SDK incluye scripts para gestionar las dependencias y la compilación de librerías automáticamente. Navega a la carpeta `LibBuild/InstallNgineSDK/` dentro de la distribución y ejecuta:

~~~bash
chmod +x install_ngine_sdk.sh
./install_ngine_sdk.sh
~~~

![Terminal ejecutando script de instalación](https://github.com/knightfox75/ngine/blob/master/Media/terminal_running_install_script.png)

Esto instalará las dependencias necesarias (SDL2, SFML) y el propio núcleo de N'gine en `/usr/local/`.

---

### 🎮 Tu Primer Proyecto

1. Navega a la carpeta `Templates/cmake` en el directorio de tu SDK.
2. Copia la carpeta `basic_template` a tu zona de trabajo (ej. `C:\Proyectos\MiJuego` o `~/Proyectos/MiJuego`).
3. Abre esa carpeta en **Visual Studio Code**.
4. Espera a que **CMake Tools** configure el proyecto. Si te pide seleccionar un *Kit* de compilación, selecciona `GCC x86_64`.
5. Pulsa **F7** o haz clic en "Build" en la barra de estado inferior para compilar.

![VS Code: Ejecutando un proyecto](https://github.com/knightfox75/ngine/blob/master/Media/vscode_project_running.png)

---

## 📥 Descargar

El paquete de distribución contiene el **código fuente**, **bibliotecas compiladas**, **documentación**, **ejemplos comentados** y **demos funcionales**.

Puedes descargar la última versión desde la **[página de Releases de GitHub](https://github.com/knightfox75/ngine/releases/latest)**.

## 🎬 Demo Reel

[![N'gine Demo Reel](https://img.youtube.com/vi/kIcG9lp1oI0/hqdefault.jpg)](https://www.youtube.com/watch?v=kIcG9lp1oI0)

*(Haz clic en la imagen para ver el vídeo)*

## 📜 Licencia

N'gine se distribuye bajo la **Licencia MIT**. Esto significa que puedes usarlo, modificarlo y distribuirlo libremente para tus proyectos (comerciales o no), siempre que se incluya la atribución correspondiente.

## 📧 Contacto

Para cualquier duda o sugerencia, puedes contactar a través de **contact@nightfoxandco.com**.