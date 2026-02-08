/******************************************************************************

    N'gine Lib for C++
    *** Version 1.21.0-wip0x01 ***
    Archivo principal de la libreria

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.11) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.24.0) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.6.2) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/


	N'gine Lib is under MIT License

	Copyright (c) 2016 - 2026 by Cesar Rincon "NightFox"

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal	in the Software without restriction,
	including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be
	included in all	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER	LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************************/



#ifndef NGN_H_INCLUDED
#define NGN_H_INCLUDED





/*** Includes ***/

// FLAGS de configuracion de la libreria
#include "config_flags.h"

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11

// SDL
#include <SDL.h>                    // Tipos de datos de SDL2

// Libreria
#include "ngn_defines.h"            // Definiciones
#include "ngn_log.h"                // Mensages de depuracion
#include "ngn_math.h"               // Funciones matematicas
#include "ngn_system.h"             // Funciones del sistema
#include "ngn_toolbox.h"            // Caja de herramientas (metodos diversos y utiles)
#include "ngn_input.h"              // Metodos de entrada
#include "ngn_graphics.h"           // Gestion del Renderer de SDL
#include "ngn_render.h"             // Renderiza los diferentes elementos graficos
#include "ngn_filesystem.h"         // Funciones del sistema de archivos
#include "ngn_load.h"               // Carga de archivos
#include "ngn_image.h"              // Manipulacion de imagenes en RAW
#include "ngn_sprite.h"             // Clase Sprite
#include "ngn_tiledbg.h"            // Clase Fondos Tileados
#include "ngn_texture.h"            // Clase para los fondos con texturas
#include "ngn_camera.h"             // Clase de la camara virtual 2D
#include "ngn_text_layer.h"         // Clase para generar una capa de texto TFF en pantalla
#include "ngn_collisions.h"         // Clase del sistema de colisiones
#include "ngn_sound.h"              // Clase para la gestion del sonido
#include "ngn_audio_clip.h"         // Clase para los clips de audio
#include "ngn_music_clip.h"         // Clase para los clips de musica
#include "ngn_canvas.h"             // Clase para el lienzo de dibujo
#include "ngn_disk.h"               // Clase para la gestion de archivos en disco
#include "ngn_resources.h"          // Clase para la gestion de recursos



/*** Definicion de la clase ***/
class NGN {

   // Public
    public:

        // Contructor
        NGN();

        // Destructor
        ~NGN();

        // Punteros a los singletons de las clases
        NGN_Log* log;                   // Mensages de depuracion
        NGN_System* system;             // Funciones del sistema
        NGN_Math* math;                 // Funciones matematicas
        NGN_ToolBox* toolbox;           // Caja de herramientas
        NGN_Input* input;               // Metodos de entrada
        NGN_Graphics* graphics;         // Gestion del Renderer de SDL
        NGN_Render* render;             // Dibuja los diferentes elementos graficos
        NGN_Load* load;                 // Funciones para la carga de archivos
        NGN_Collisions* collisions;     // Funciones del sistema de colisiones
        NGN_Sound* sound;               // Gestion de los efectos de sonido
        NGN_Image* image;               // Manipulacion de imagenes en RAW
        NGN_Disk* disk;                 // Gestion de archivos en el disco
        NGN_Resources* resources;       // Gestion integrada de recursos

        // Punteros a los objetos
        NGN_Camera* camera;             // Camara virtual 2D integrada

        // Iniciliaza la libreria
        bool Init();



    // Private
    private:

};

/*** Puntero a la clase de la libreria ***/
extern NGN* ngn;                                       // Clase principal




#endif // NGN_H_INCLUDED
