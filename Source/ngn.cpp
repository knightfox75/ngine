/******************************************************************************

    N'gine Lib for C++
    *** Version 1.19.0-stable ***
    Archivo principal de la libreria

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.11) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.6.2) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/


	N'gine Lib is under MIT License

	Copyright (c) 2016-2025 by Cesar Rincon "NightFox"

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
    log = nullptr;             // Mensages de depuracion
    system = nullptr;          // Funciones del sistema
    math = nullptr;            // Funciones matematicas
    toolbox = nullptr;         // Caja de herramientas
    input = nullptr;           // Metodos de entrada
    graphics = nullptr;        // Gestion del Renderer de SDL
    render = nullptr;          // Dibuja los diferentes elementos graficos
    load = nullptr;            // Carga de archivos
    collisions = nullptr;      // Sistema de colisiones
    sound = nullptr;           // Efectos de sonido
    image = nullptr;           // Manipulacion de imagenes en RAW
    disk = nullptr;            // Gestion de archivos en el disco
    resources = nullptr;       // Gestion integrada de recursos

    // Prepara los objetos de la libreria
    camera = nullptr;          // Crea la camara virtual 2D

}



/*** Destructor ***/
NGN::~NGN() {

    // Elimina los objetos
    delete camera; camera = nullptr;

    // Elimina todas las instancias a los singletons
    NGN_Resources::RemoveInstance(); resources = nullptr;
    NGN_Disk::RemoveInstance(); disk = nullptr;
    NGN_Image::RemoveInstance(); image = nullptr;
    NGN_Sound::RemoveInstance(); sound = nullptr;
    NGN_Collisions::RemoveInstance(); collisions = nullptr;
    NGN_Load::RemoveInstance(); load = nullptr;
    NGN_Render::RemoveInstance(); render = nullptr;
    NGN_Graphics::RemoveInstance(); graphics = nullptr;
    NGN_Input::RemoveInstance(); input = nullptr;
    NGN_ToolBox::RemoveInstance(); toolbox = nullptr;
    NGN_Math::RemoveInstance(); math = nullptr;
    NGN_System::RemoveInstance(); system = nullptr;
    NGN_Log::RemoveInstance(); log = nullptr;

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

