# NightFox's Game Engine
https://ngine.nightfoxandco.com/
<br>

**[Español]**

- **¿Qué es N’gine?**<br>
  N’gine es un motor de código abierto para el desarrollo de juegos en 2D, especialmente diseñado para juegos de píxeles, inspirado en las técnicas utilizadas por las consolas clásicas (fondos de tiles, sprite-sheets, etc.).<br>

- **¿Qué lenguaje de programación se ha usado?**<br>
  Todo el motor está programado completamente en C++, utilizando bibliotecas de apoyo para acelerar el proceso de creación del mismo, como SDL2, SFML y LodePng, entre otras.<br>

- **¿A quién va destinado?**<br>
  A cualquier persona que quiera desarrollar su videojuego y tenga nociones de programación orientada a objetos.<br>

- **¿Bajo qué sistemas funciona?**<br>
  Actualmente, el motor funciona en sistemas operativos Windows, Linux y Raspberry PI OS.<br>

- **¿Bajo qué licencia se distribuye?**<br>
  El motor se distribuye bajo la licencia MIT. Eso significa que puedes usar, modificar, crear y distribuir contenido utilizando el motor, siempre y cuando lo acredites correctamente.<br>

- **¿Cómo nació este proyecto?**<br>
  El proyecto nació por la necesidad de tener una herramienta 2D a medida para el desarrollo de proyectos personales y, en mayor medida, como base para la realización de proyectos de los estudiantes de primer curso en el centro donde imparto clases como profesor de diseño y programación de videojuegos.<br>

- **¿En qué estado de desarrollo se encuentra el motor?**<br>
  Con más de 6 años de desarrollo, ya se encuentra en una fase estable y sin errores graves conocidos. Actualmente, se trabaja en la mejora del rendimiento y la incorporación de nuevas funcionalidades.<br>

- **¿Cuáles son las principales características del motor?**<br>

  **Métodos de entrada:**<br>
   - Teclado<br>
   - Ratón<br>
   - Gamepads norma x-input<br>
   
  **Gráficos:**<br>
   - Texturas de hasta 8192x8192 píxeles<br>
   - Fondos de gran tamaño usando tiles<br>
   - Sprites con o sin animación<br>
   - Cámara virtual 2D<br>
   - Capas de texto<br>
   - Lienzo para el dibujado de primitivas<br>
   - Diferentes modos de mezcla y uso de máscaras<br>
   - Hasta 8 viewports simultáneos y ajustables<br>
   - Capacidad de mover, rotar y escalar todos los elementos gráficos<br>
   
  **Sonido:**<br>
   - Hasta 4 streams de música/diálogos simultáneos (formato ogg)<br>
   - Hasta 64 efectos de sonido simultáneos (formato wav recomendado)<br>
   - Mezclador de 5 canales<br>
   - Posibilidad de cambiar el volumen, pan (en los efectos de sonido), pitch y loop<br>
   
  **Colisiones:**<br>
   - Detección de colisión entre 2 sprites por cajas, con la posibilidad de añadir múltiples colisionadores en cada sprite<br>
   - Detección de colisión entre 2 sprites a nivel de píxel<br>
   - Raycast de 1 punto de una coordenada a un sprite<br>
   - Mapas de colisión de hasta 255 colores<br>
   
  **Sistema de archivos:**<br>
   - Funciones de carga de los recursos desde el sistema de archivos<br>
   - Posibilidad de crear archivos de empaquetado de recursos con la opción de encriptación<br>
   - Funciones de carga y grabación de archivos en formato binario o texto<br>
   - Posibilidad de gestionar hasta 127 repositorios de recursos, indicando la lista de archivos a cargar desde un archivo de texto plano<br>

- **¿Qué contiene el paquete de distribución del motor?**<br>
  El archivo contiene el código fuente del motor, así como las librerías del mismo compiladas para Windows, Linux Mint y Raspberry PI OS (Raspbian), documentación de instalación y uso, ejemplos comentados y un paquete con demos ya compiladas que muestran las principales funcionalidades del motor.<br>

- **¿Cómo puedo contactar con el autor?**<br>
  Puedes enviar un correo electrónico a la dirección de contacto: contact@nightfoxandco.com.<br>

Aquí tienes una demo reel con la mayoría de características del motor:<br>
[Demo Reel](https://www.youtube.com/watch?v=kIcG9lp1oI0)
<br>

<br><br><br>

**[English]**

- **What is N’gine?**<br>
  N’gine is an open-source engine for 2D game development, especially tailored for pixel art games. It is inspired by techniques used in classic consoles (tile backgrounds, sprite-sheets, etc.).<br>

- **Which programming language is used?**<br>
  The entire engine is programmed in C++, using supporting libraries like SDL2, SFML, and LodePng to expedite the creation process.<br>

- **Who is it intended for?**<br>
  It is aimed at anyone who wants to develop their own video game and has a basic understanding of object-oriented programming.<br>

- **Which systems does it run on?**<br>
  Currently, the engine runs on Windows, Linux, and Raspberry PI OS.<br>

- **Under what license is it distributed?**<br>
  The engine is distributed under the MIT license. This means you can use, modify, create, and distribute content using the engine, as long as proper attribution is given.<br>

- **How did this project originate?**<br>
  The project was born out of the need for a custom 2D tool for personal projects and, to a greater extent, as a foundation for first-year student projects at the center where I teach as a game design and programming instructor.<br>

- **In what development state is the engine?**<br>
  With over 6 years of development, it is now in a stable phase without known major bugs. The current focus is on performance improvement and the addition of new features.<br>

- **What are the main features of the engine?**<br>

  **Input Methods:**<br>
   - Keyboard<br>
   - Mouse<br>
   - X-input standard Gamepads<br>
   
  **Graphics:**<br>
   - Textures up to 8192x8192 pixels<br>
   - Large backgrounds using tiles<br>
   - Sprites with or without animation<br>
   - 2D virtual camera<br>
   - Text layers<br>
   - Canvas for primitive drawing<br>
   - Various blending modes and use of masks<br>
   - Up to 8 simultaneous and adjustable viewports<br>
   - Ability to move, rotate, and scale all graphic elements<br>
   
  **Sound:**<br>
   - Up to 4 simultaneous music/dialogue streams (ogg format)<br>
   - Up to 64 simultaneous sound effects (recommended wav format)<br>
   - 5-channel mixer<br>
   - Ability to change volume, pan (for sound effects), pitch, and loop<br>
   
  **Collisions:**<br>
   - Collision detection between 2 sprites by boxes, with the ability to add multiple colliders to each sprite<br>
   - Pixel-level collision detection between 2 sprites<br>
   - Raycasting from 1 point of a coordinate to a sprite<br>
   - Collision maps with up to 255 colors<br>
   
  **File System:**<br>
   - Loading functions for resources from the file system<br>
   - Ability to create resource packaging files with encryption options<br>
   - Loading and saving functions for binary or text files<br>
   - Ability to manage up to 127 resource repositories, specifying the list of files to load from a plain text file<br>

- **What does the engine distribution package contain?**<br>
  The file includes the engine's source code, as well as compiled libraries for Windows, Linux Mint, and Raspberry PI OS (Raspbian), installation and usage documentation, commented examples, and a package with already compiled demos showcasing the main features of the engine.<br>

- **How can I contact the author?**<br>
  You can send an email to the contact address: contact@nightfoxandco.com.<br>

Here's a demo reel with most of the engine's features:<br>
[Demo Reel](https://www.youtube.com/watch?v=kIcG9lp1oI0)
<br>
