# ngine

- ¿Qué es N’gine?<br>
N’gine es un motor de código abierto para el desarrollo de juegos en 2D, en especial para juegos pixel art, inspirado en las técnicas usadas por las consolas clásicas (fondos de tiles, sprite-sheets, etc).<br>

- ¿Qué lenguaje de programación se ha usado?<br>
Todo el motor está programado enteramente en C++, usando librerías de apoyo para acelerar el proceso de creación del mismo, como SDL2, SFML y LodePng entre otras.<br>

- ¿A quién va destinado?<br>
A cualquier persona que quiera desarrollar su videojuego y tenga nociones de programación orientada a objetos.<br>

- ¿Bajo qué sistemas funciona?<br>
Actualmente el motor funciona en sistemas Windows y Linux.<br>

- ¿Bajo qué licencia se distribuye?<br>
El motor se distribuye bajo la licencia MIT. Eso significa que puedes usar, modificar, crear y distribuir contenido usando el motor, siempre y cuando lo acredites correctamente.<br>

- ¿Cómo nació este proyecto?<br>
El proyecto nació por la necesidad de tener una herramienta 2D a medida para el desarrollo de proyectos personales y en mayor medida, como base para la realización de proyectos de los estudiantes de primer curso en el centro donde imparto clases como profesor de diseño y programación de videojuegos.<br>

- ¿En qué estado de desarrollo se encuentra el motor?<br>
Con más de 6 años de desarrollo, ya se encuentra en una fase estable de desarrollo y sin bugs graves conocidos, por lo que actualmente se trabaja en la mejora de rendimiento y la incorporación de nuevas funcionalidades.<br>

- ¿Cuáles son las principales características del motor?<br>
	Métodos de entrada:<br>
		· Teclado<br>
		· Ratón<br>
		· Gamepads norma x-input<br>
	Gráficos:<br>
		· Texturas de hasta 8192x8192 píxeles<br>
		· Fondos de gran tamaño usando tiles<br>
		· Sprites con o sin animación<br>
		· Cámara virtual 2D<br>
		· Capas de texto<br>
		· Lienzo para el dibujado de primitivas<br>
		· Diferentes modos de mezcla y uso de mascaras<br>
		· Hasta 8 viewports simultáneos y ajustables<br>
		· Capacidad de mover, rotar y escalar todos los elementos gráficos<br>
	Sonido:<br>
		· Hasta 4 streams de música/diálogos simultáneos (formato ogg)<br>
		· Hasta 64 efectos de sonido simultáneos (formato wav recomendado)<br>
		· Mezclador de 5 canales<br>
		· Posibilidad de cambiar el volumen, pan (en los efectos de sonido), pitch y loop<br>
	Colisiones:<br>
		· Detección de colisión entre 2 sprites por cajas, con la posibilidad de añadir múltiples colisionadores en cada sprite<br>
		· Detección de colisión entre 2 sprites a nivel de píxel<br>
		· Raycast de 1 punto de una coordenada a un sprite<br>
		· Mapas de colisión de hasta 255 colores<br>
	Sistema de archivos<br>
		· Funciones de carga de los recursos desde el sistema de archivos<br>
		· Posibilidad de crear archivos de empaquetado de recursos con la posibilidad de encriptación<br>
		· Funciones de carga y grabación de archivos en formato binario o texto<br>
		· Posibilidad de gestionar hasta 127 repositorios de recursos, indicado la lista de archivos a cargar desde un archivo de texto plano.<br>
		
- ¿Que contiene el paquete de distribución del motor?<br>
El archivo contiene el código fuente del motor, así como las librerías del mismo compiladas para Windows y Linux Mint, documentación de instalación y uso, ejemplos comentados y un pack con demos ya compiladas mostrando las principales funcionalidades del motor<br>

- ¿Dónde puedo conseguir el motor?<br>
Actualmente el motor está hospedado en Github, puedes descargarte la última versión desde el enlace al final de esta página<br>

- ¿Cómo puedo contactar con el autor?<br>
Tan pronto como sea posible, habilitaré un foro para las posibles consultas y comentarios.<br>

Aquí tenéis una demo reel con la mayoría de características del motor:<br>
https://www.youtube.com/watch?v=kIcG9lp1oI0
<br>
