/******************************************************************************

    N'gine Lib for C++
    *** Version 0.5.1-alpha ***
    Archivo principal de la libreria

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere SDL2 (2.0.5 or higher)
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.4.1 or higher)
    http://www.sfml-dev.org/

    Requiere LodePNG
    (c) 2005 - 2016 by Lode Vandevenne
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



/*** Includes ***/

// SDL
#include <SDL.h>

// Libreria
#include "ngn_defines.h"            // Definiciones
#include "ngn_system.h"             // Funciones del sistema
#include "ngn_input.h"              // Metodos de entrada
#include "ngn_graphics.h"           // Gestion del Renderer de SDL
#include "ngn_render.h"             // Renderiza los diferentes elementos graficos
#include "ngn_load.h"               // Carga de archivos
#include "ngn_sprite.h"             // Clase Sprite
#include "ngn_tiledbg.h"            // Clase Fondos Tileados
#include "ngn_texture.h"            // Clase para los fondos con texturas
#include "ngn_camera.h"             // Clase de la camara virtual 2D
#include "ngn_text_layer.h"         // Clase para generar una capa de texto TFF en pantalla
#include "ngn_gui.h"                // Clase de la interfaz grafica de usuario
#include "ngn_collisions.h"         // Clase del sistema de colisiones
#include "ngn_sound.h"              // Clase para la gestion del sonido
#include "ngn_audio_clip.h"         // Clase para los clips de audio
#include "ngn_music_clip.h"         // Clase para los clips de musica
#include "ngn_canvas.h"             // Clase para el lienzo de dibujo



/*** Definicion de la clase ***/
class NGN {

   // Public
    public:

        // Contructor
        NGN();

        // Destructor
        ~NGN();

        // Punteros a las clases
        NGN_System* system;             // Funciones del sistema
        NGN_Input* input;               // Metodos de entrada
        NGN_Graphics* graphics;         // Gestion del Renderer de SDL
        NGN_Render* render;             // Dibuja los diferentes elementos graficos
        NGN_Camera* camera;             // Camara virtual 2D integrada
        NGN_Load* load;                 // Funciones para la carga de archivos
        NGN_Collisions* collisions;     // Funciones del sistema de colisiones
        NGN_Sound* sound;               // Gestion de los efectos de sonido

        // Iniciliaza la libreria
        bool Init();



    // Private
    private:

};

/*** Puntero a la clase de la libreria ***/
extern NGN* ngn;                                       // Clase principal




#endif // NGN_H_INCLUDED
