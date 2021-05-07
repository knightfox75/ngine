/******************************************************************************

    N'gine Lib for C++
    *** Version 1.5.0-wip4 ***
    Archivo principal de la libreria

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere GCC 8.1.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.12) - 64-bits
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
    log = NULL;             // Mensages de depuracion
    system = NULL;          // Funciones del sistema
    math = NULL;            // Funciones matematicas
    toolbox = NULL;         // Caja de herramientas
    input = NULL;           // Metodos de entrada
    graphics = NULL;        // Gestion del Renderer de SDL
    render = NULL;          // Dibuja los diferentes elementos graficos
    camera = NULL;          // Crea la camara virtual 2D
    load = NULL;            // Carga de archivos
    collisions = NULL;      // Sistema de colisiones
    sound = NULL;           // Efectos de sonido
    image = NULL;           // Manipulacion de imagenes en RAW
    disk = NULL;            // Gestion de archivos en el disco

}



/*** Destructor ***/
NGN::~NGN() {

    // Elimina todos los objetos creados
    delete disk; disk = NULL;
    delete image; image = NULL;
    delete sound; sound = NULL;
    delete collisions; collisions = NULL;
    delete load; load = NULL;
    delete camera; camera = NULL;
    delete render; render = NULL;
    delete graphics; graphics = NULL;
    delete input; input = NULL;
    delete toolbox; toolbox = NULL;
    delete math; math = NULL;
    delete system; system = NULL;
    delete log; log = NULL;

    // Cierra los subsistemas de SDL
    SDL_Quit();     // Cierra la libreria SDL correctamente

}



/*** Inicializa la libreria ***/
bool NGN::Init() {

    // Crea los objetos de la libreria
    log = new NGN_Log();                    // Mensages de depuracion
    if (!log) return false;
    system = new NGN_System();              // Funciones del sistema
    if (!system) return false;
    toolbox = new NGN_ToolBox();            // Caja de herramientas
    if (!toolbox) return false;
    input = new NGN_Input();                // Metodos de entrada
    if (!input) return false;
    graphics = new NGN_Graphics();          // Gestion del Renderer de SDL
    if (!graphics) return false;
    render = new NGN_Render();              // Dibuja los diferentes elementos graficos
    if (!render) return false;
    camera = new NGN_Camera();              // Crea la camara virtual 2D
    if (!camera) return false;
    load = new NGN_Load();                  // Carga de archivos
    if (!load) return false;
    collisions = new NGN_Collisions();      // Sistema de colisiones
    if (!collisions) return false;
    sound = new NGN_Sound();                // Efectos de sonido
    if (!sound) return false;
    image = new NGN_Image();                // Manipulacion de imagenes en RAW
    if (!image) return false;
    disk = new NGN_Disk();                  // Gestion de archivos en el disco
    if (!disk) return false;

    // Inicializa la libreria SDL
    if (!system->Init()) return false;

    // Inicializacion correcta
    return true;

}

