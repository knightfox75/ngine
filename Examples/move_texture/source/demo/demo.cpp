/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Movimiento de texturas

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 0.6.0-a o superior

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
// c++
#include <cstdio>
#include <iostream>
#include <vector>
#include <ctime>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"

/*** Defines ***/
#define WINDOW_TITLE "N'GINE Texture moving example."   // Titulo de la ventana
#define SCR_WIDTH   1280                                // Resolucion
#define SCR_HEIGHT  720

#define OBJECT_NUMBER 12        // Numero de objetos



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    bg_data = NULL;
    ball_data = NULL;

    // Inicializa los punteros de objetos
    bg = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    for (uint32_t i = 0; i < ball.capacity(); i ++) {
        delete ball[i].gfx; ball[i].gfx = NULL;
    }
    ball.clear();
    delete bg; bg = NULL;

    // Borra los datos
    delete ball_data; ball_data = NULL;
    delete bg_data; bg_data = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Selecciona el modo grafico
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT)) return false;

    // Esconde el cursor del raton
    ngn->graphics->ShowMouse(false);

    // Debug?
    ngn->system->fps_counter = false;

    // Fuerza la actualizacion de la pantalla
    ngn->graphics->Update();

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Cambia la semilla del random
    srand(time(NULL));

    // Carga los archivos
    if (!Load()) return false;

    // Crea la textura con los datos cargados
    bg = new NGN_Texture(bg_data, 0, 0);

    // Crea los objetos mobiles
    CreateObjects();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
void Demo::Run() {

    // Control del loop
    bool loop = true;

    // Loop principal
    while (loop) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos

        // Actualizacion del programa
        Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();

        // Control del bucle principal
        loop &= !ngn->system->quit;                                 // Si se pulsa la [X] de la ventana
        loop &= !ngn->input->key_ESC->down;                         // O se pulsa la tecla [ESC] sal del bucle de ejecucion

    }

}



/*** Carga todos los archivos necesarios ***/
bool Demo::Load() {

    // Carga la imagen para el fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (bg_data == NULL) return false;

    // Carga la imagen para los objetos
    ball_data = ngn->load->Texture("data/basketball.png");
    if (ball_data == NULL) return false;

    // Carga correcta
    return true;

}



/*** Crea los objetos ***/
void Demo::CreateObjects() {

    // Variables
    uint32_t min_x, max_x, min_y, max_y, _size, x, y;

    // Crea una lista con el numero de elementos necesarios
    ball.clear();
    ball.reserve(OBJECT_NUMBER);

    // Inicializa la lista
    for (uint32_t i = 0; i < ball.capacity(); i ++) {
        // Crea la textura
        ball[i].gfx = new NGN_Texture(ball_data);
        // Tamaño aleatorio
        _size = ((rand() % 96) + 32);
        ball[i].gfx->Size(_size, _size);
        // Posicion aleatoria (esquina superior izquierda)
        min_x = min_y = 0;
        max_x = (ngn->graphics->native_w - _size);
        max_y = (ngn->graphics->native_h - _size);
        x = (rand() % max_x);
        y = (rand() % max_y);
        ball[i].gfx->Position(x, y);
        // Limites del movimiento para este objeto
        ball[i].top = min_y;
        ball[i].left = min_x;
        ball[i].bottom = max_y;
        ball[i].right = max_x;
        // Velocidades aleatorias
        ball[i].speed.x = (((float)(rand() % 100) / 10.0f) + 1.0f);
        if ((rand() % 100) > 50) ball[i].speed.x = -ball[i].speed.x;
        ball[i].speed.y = (((float)(rand() % 100) / 10.0f) + 1.0f);
        if ((rand() % 100) > 50) ball[i].speed.y = -ball[i].speed.y;
    }

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Mueve los objetos
    Move();
    // Render de la escena
    Render();

}



/*** Mueve los objetos ***/
void Demo::Move() {

    for (uint32_t i = 0; i < ball.capacity(); i ++) {

        // Mueve los objetos
        ball[i].gfx->Translate(ball[i].speed.x, ball[i].speed.y);

        // Limites
        if (ball[i].gfx->position.x < ball[i].left) {
            ball[i].gfx->position.x = ball[i].left;
            ball[i].speed.x = -ball[i].speed.x;
        }
        if (ball[i].gfx->position.x > ball[i].right) {
            ball[i].gfx->position.x = ball[i].right;
            ball[i].speed.x = -ball[i].speed.x;
        }
        if (ball[i].gfx->position.y < ball[i].top) {
            ball[i].gfx->position.y = ball[i].top;
            ball[i].speed.y = -ball[i].speed.y;
        }
        if (ball[i].gfx->position.y > ball[i].bottom) {
            ball[i].gfx->position.y = ball[i].bottom;
            ball[i].speed.y = -ball[i].speed.y;
        }

        // Rotalos
        ball[i].gfx->Rotate(ball[i].speed.x);

    }

}



/*** Renderiza la escena ***/
void Demo::Render() {

    // Renderiza el fondo
    ngn->render->Texture(bg);

    // Renderiza los objetos mobiles
    for (uint32_t i = 0; i < ball.capacity(); i ++) {
        ngn->render->Texture(ball[i].gfx);
    }

}
