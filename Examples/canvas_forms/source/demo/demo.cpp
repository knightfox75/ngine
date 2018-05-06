/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Canvas - Dibujo de formas

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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"

/*** Defines ***/
#define WINDOW_TITLE "N'GINE Canvas forms drawing."     // Titulo de la ventana
#define SCR_WIDTH   1280                                // Resolucion
#define SCR_HEIGHT  720



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    bg_data = NULL;
    cursor_data = NULL;

    // Inicializa los punteros de objetos
    bg = NULL;
    canvas = NULL;
    cursor = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg; bg = NULL;
    delete canvas; canvas = NULL;
    delete cursor; cursor = NULL;

    // Borra los datos
    delete bg_data; bg_data = NULL;
    delete cursor_data; cursor_data = NULL;

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

    // Carga los archivos
    if (!Load()) return false;

    // Crea la escena
    Create();

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

    // Carga el cursor
    cursor_data = ngn->load->Sprite("data/mouse_pointer.spr");
    if (cursor_data == NULL) return false;

    // Carga correcta
    return true;

}



/*** Crea las formas ***/
void Demo::Create() {

    // Crea la textura con los datos cargados
    bg = new NGN_Texture(bg_data, 0, 0);

    // Crea el canvas
    canvas = new NGN_Canvas();

    // Crea el cursor del raton
    cursor = new NGN_Sprite(cursor_data);

    // Calcula la posicion de los vertices de la forma
    double step = (PI * 2.0f) / (double)VERTEX;
    double angle = 0.0f;
    double radius = (double)SCR_HEIGHT * 0.33f;
    double cx = (double)SCR_WIDTH / 2.0f;
    double cy = (double)SCR_HEIGHT / 2.0f;
    for (uint32_t n = 0; n < VERTEX; n ++) {
        x[n] = std::round((std::cos(angle) * radius) + cx);
        y[n] = std::round((std::sin(angle) * radius) + cy);
        angle += step;
    }

    // Id de vertice activo
    point = -1;

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Dibuja en el canvas
    Draw();
    // Render de la escena
    Render();

}



/*** Dibuja en el canvas ***/
void Demo::Draw() {

    // Borra el canvas
    canvas->Cls();

    // Si sueltas el boton izquierdo, cancela la seleccion
    if (ngn->input->mouse.LB.up) point = -1;

    // Si no hay ningun selector de vertices activo...
    int32_t d = 0.0f;
    int32_t _x = 0, _y = 0;
    if (point < 0) {
        // Analiza todos los vertices
        for (uint32_t n = 0; n < VERTEX; n ++) {
            // Calcula la distancia del mouse al vertice
            _x = ngn->input->mouse.x - x[n];
            _y = ngn->input->mouse.y - y[n];
            d = (int32_t)std::sqrt((_x * _x) + (_y * _y));
            // Si esta en al zona indicalo
            if (d <= SNAP) {
                // Si pulsas el boton izquierdo del mouse
                if (ngn->input->mouse.LB.down) {
                    point = n;
                } else {
                    // Dibuja el marcador de vertices
                    canvas->FilledCircle(x[n], y[n], SNAP, 0x00FFFFFF);
                }
                // No analices el resto de vertices
                break;
            }
        }
    }

    // Si hay un punto seleccionado
    if (point >= 0) {
        // Cambia el vertice seleccionado a la posicion del raton
        x[point] = ngn->input->mouse.x;
        y[point] = ngn->input->mouse.y;
        // Dibuja el marcador de vertices
        canvas->FilledCircle(x[point], y[point], SNAP, 0xFF0000FF);
    }

    // Dibuja el poligono
    for (uint32_t n = 0; n < VERTEX; n ++) canvas->Line(x[n], y[n], x[((n + 1) % VERTEX)], y[((n + 1) % VERTEX)], 0xFFFFFFFF);

}



/*** Renderiza la escena ***/
void Demo::Render() {

    // Renderiza el fondo
    ngn->render->Texture(bg);

    // Renderiza el canvas
    ngn->render->Canvas(canvas);

    // Renderiza el cursor del raton en la posicion del raton
    ngn->render->Sprite(cursor, ngn->input->mouse.x, ngn->input->mouse.y);

}
