/******************************************************************************

    N'gine Lib for C++
    *** Version 0.8.0-alpha-WIP2 ***
    Archivo principal de la libreria

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2019 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere GCC 7.3.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.8) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.0) - 64-bits
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



/*** Includes ***/

// C++
#include <cstdio>
#include <iostream>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/*** Puntero a la clase de la libreria ***/
NGN* ngn;                                       // Clase principal



/*** Contructor ***/
NGN::NGN() {

    // Crea los objetos de la libreria
    system = NULL;          // Funciones del sistema
    input = NULL;           // Metodos de entrada
    graphics = NULL;        // Gestion del Renderer de SDL
    render = NULL;          // Dibuja los diferentes elementos graficos
    camera = NULL;          // Crea la camara virtual 2D
    load = NULL;            // Carga de archivos
    collisions = NULL;      // Sistema de colisiones
    sound = NULL;           // Efectos de sonido

}



/*** Destructor ***/
NGN::~NGN() {

    // Elimina todos los objetos creados
    delete sound; sound = NULL;
    delete collisions; collisions = NULL;
    delete load; load = NULL;
    delete camera; camera = NULL;
    delete render; render = NULL;
    delete graphics; graphics = NULL;
    delete input; input = NULL;
    delete system; system = NULL;

    // Cierra los subsistemas de SDL
    SDL_Quit();     // Cierra la libreria SDL correctamente

}



/*** Inicializa la libreria ***/
bool NGN::Init() {

    // Crea los objetos de la libreria
    system = new NGN_System();              // Funciones del sistema
    if (system == NULL) return false;
    input = new NGN_Input();                // Metodos de entrada
    if (input == NULL) return false;
    graphics = new NGN_Graphics();          // Gestion del Renderer de SDL
    if (graphics == NULL) return false;
    render = new NGN_Render();              // Dibuja los diferentes elementos graficos
    if (render == NULL) return false;
    camera = new NGN_Camera();              // Crea la camara virtual 2D
    if (camera == NULL) return false;
    load = new NGN_Load();                  // Carga de archivos
    if (load == NULL) return false;
    collisions = new NGN_Collisions();      // Sistema de colisiones
    if (collisions == NULL) return false;
    sound = new NGN_Sound();                // Efectos de sonido
    if (sound == NULL) return false;

    // Inicializa la libreria SDL
    if (!system->Init()) return false;

    // Inicializacion correcta
    return true;

}

