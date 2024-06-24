/******************************************************************************

    N'gine Lib for C++
    *** Version 1.17.0-wip_0x02 ***
    Archivo principal de la libreria

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere GCC 11.3.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.26.3) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (3.0.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/


	N'gine Lib is under MIT License

	Copyright (c) 2016-2023 by Cesar Rincon "NightFox"

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

    // Prepara los singletons de los objetos de la libreria
    log = NULL;             // Mensages de depuracion
    system = NULL;          // Funciones del sistema
    math = NULL;            // Funciones matematicas
    toolbox = NULL;         // Caja de herramientas
    input = NULL;           // Metodos de entrada
    graphics = NULL;        // Gestion del Renderer de SDL
    render = NULL;          // Dibuja los diferentes elementos graficos
    load = NULL;            // Carga de archivos
    collisions = NULL;      // Sistema de colisiones
    sound = NULL;           // Efectos de sonido
    image = NULL;           // Manipulacion de imagenes en RAW
    disk = NULL;            // Gestion de archivos en el disco
    resources = NULL;       // Gestion integrada de recursos

    // Prepara los objetos de la libreria
    camera = NULL;          // Crea la camara virtual 2D

}



/*** Destructor ***/
NGN::~NGN() {

    // Elimina los objetos
    delete camera; camera = NULL;

    // Elimina todas las instancias a los singletons
    NGN_Resources::RemoveInstance(); resources = NULL;
    NGN_Disk::RemoveInstance(); disk = NULL;
    NGN_Image::RemoveInstance(); image = NULL;
    NGN_Sound::RemoveInstance(); sound = NULL;
    NGN_Collisions::RemoveInstance(); collisions = NULL;
    NGN_Load::RemoveInstance(); load = NULL;
    NGN_Render::RemoveInstance(); render = NULL;
    NGN_Graphics::RemoveInstance(); graphics = NULL;
    NGN_Input::RemoveInstance(); input = NULL;
    NGN_ToolBox::RemoveInstance(); toolbox = NULL;
    NGN_Math::RemoveInstance(); math = NULL;
    NGN_System::RemoveInstance(); system = NULL;
    NGN_Log::RemoveInstance(); log = NULL;

    // Cierra los subsistemas de SDL
    SDL_Quit();     // Cierra la libreria SDL correctamente

}



/*** Inicializa la libreria ***/
bool NGN::Init() {

    // Crea las instancias a los singletons de los modulos de la libreria
    log = NGN_Log::GetInstance();                   // Mensages de depuracion
    system = NGN_System::GetInstance();             // Funciones del sistema
    math = NGN_Math::GetInstance();                 // Funciones matematicas
    toolbox = NGN_ToolBox::GetInstance();           // Caja de herramientas
    input = NGN_Input::GetInstance();               // Metodos de entrada
    graphics = NGN_Graphics::GetInstance();         // Gestion del Renderer de SDL
    render = NGN_Render::GetInstance();             // Dibuja los diferentes elementos graficos
    load = NGN_Load::GetInstance();                 // Carga de archivos
    collisions = NGN_Collisions::GetInstance();     // Sistema de colisiones
    sound = NGN_Sound::GetInstance();               // Efectos de sonido
    image = NGN_Image::GetInstance();               // Manipulacion de imagenes en RAW
    disk = NGN_Disk::GetInstance();                 // Gestion de archivos en el disco
    resources = NGN_Resources::GetInstance();       // Gestion integrada de recursos

    // Inicia los singletons de la libreria
    log->BootUp();                      // Mensages de depuracion
    system->BootUp();                   // Funciones del sistema
    math->BootUp();                     // Funciones matematicas
    toolbox->BootUp();                  // Caja de herramientas
    input->BootUp();                    // Metodos de entrada
    graphics->BootUp();                 // Gestion del Renderer de SDL
    render->BootUp();                   // Dibuja los diferentes elementos graficos
    load->BootUp();                     // Carga de archivos
    collisions->BootUp();               // Sistema de colisiones
    sound->BootUp();                    // Efectos de sonido
    image->BootUp();                    // Manipulacion de imagenes en RAW
    disk->BootUp();                     // Gestion de archivos en el disco
    resources->BootUp();                // Gestion integrada de recursos

    // Crea los objetos adicionales de a libreria
    camera = new NGN_Camera();          // Crea la camara virtual 2D

    // Inicializa la libreria SDL
    if (!system->Init()) return false;

    // Inicializacion correcta
    return true;

}

