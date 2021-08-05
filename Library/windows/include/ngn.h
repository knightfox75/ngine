/******************************************************************************

    N'gine Lib for C++
    *** Version 1.5.0-stable ***
    Archivo principal de la libreria

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere GCC 8.1.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.12) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.0.15) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.1) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20200306)
    (c) 2005 - 2020 by Lode Vandevenne
    http://lodev.org/lodepng/


    N'gine se distribuye bajo la licencia CREATIVE COMMONS
    "Attribution-NonCommercial 4.0 International"
    https://creativecommons.org/licenses/by-nc/4.0/

    You are free to:

        - Share
        copy and redistribute the material in any medium or format.
        - Adapt
        remix, transform, and build upon the material.

        The licensor cannot revoke these freedoms as long as you follow
        the license terms.

    Under the following terms:

        - Attribution
        You must give appropriate credit, provide a link to the license,
        and indicate if changes were made. You may do so in any reasonable
        manner, but not in any way that suggests the licensor endorses you
        or your use.

        - NonCommercial
        You may not use the material for commercial purposes.

        - No additional restrictions
        You may not apply legal terms or technological measures that
        legally restrict others from doing anything the license permits.

******************************************************************************/



#ifndef NGN_H_INCLUDED
#define NGN_H_INCLUDED



/*** Defines del proyecto (usar SOLO si no se usan los defines del proyecto de code::blocks ***/
//#define OS_WINDOWS
//#define OS_LINUX
//#define MODE_DEBUG
//#define MODE_RELEASE



/*** Includes ***/

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



/*** Definicion de la clase ***/
class NGN {

   // Public
    public:

        // Contructor
        NGN();

        // Destructor
        ~NGN();

        // Punteros a las clases
        NGN_Log* log;                   // Mensages de depuracion
        NGN_System* system;             // Funciones del sistema
        NGN_Math* math;                 // Funciones matematicas
        NGN_ToolBox* toolbox;           // Caja de herramientas
        NGN_Input* input;               // Metodos de entrada
        NGN_Graphics* graphics;         // Gestion del Renderer de SDL
        NGN_Render* render;             // Dibuja los diferentes elementos graficos
        NGN_Camera* camera;             // Camara virtual 2D integrada
        NGN_Load* load;                 // Funciones para la carga de archivos
        NGN_Collisions* collisions;     // Funciones del sistema de colisiones
        NGN_Sound* sound;               // Gestion de los efectos de sonido
        NGN_Image* image;               // Manipulacion de imagenes en RAW
        NGN_Disk* disk;                 // Gestion de archivos en el disco

        // Iniciliaza la libreria
        bool Init();



    // Private
    private:

};

/*** Puntero a la clase de la libreria ***/
extern NGN* ngn;                                       // Clase principal




#endif // NGN_H_INCLUDED
