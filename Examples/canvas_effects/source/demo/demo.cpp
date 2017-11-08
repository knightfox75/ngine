/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Canvas - Efectos graficos

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2017 by Cesar Rincon "NightFox"
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
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"

/*** Defines ***/
#define WINDOW_TITLE "N'GINE Canvas effects"
#define SCR_WIDTH   1280
#define SCR_HEIGHT  720



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros a objetos
    canvas_bg = NULL;
    canvas_snow = NULL;

    // Inicializa los vectores
    buffer.clear();

    // Variables
    animate = true;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los punteros de datos
    delete canvas_bg; canvas_bg = NULL;
    delete canvas_snow; canvas_snow = NULL;

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

    // Pantalla completa?
    ngn->graphics->full_screen = false;

    // Vsync
    ngn->graphics->vsync = true;

    // Debug?
    SDL_ShowCursor(SDL_DISABLE);
    ngn->system->fps_counter = false;

    // Fuerza la actualizacion de la pantalla
    ngn->graphics->Update();

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    CreateBg();     // Crea el fondo
    CreateSnow();   // Crea el efecto de nieve

    // Procesos correctos
    return true;

}



/*** Actualizacion a cada frame ***/
bool Demo::Update() {

    // Control del loop
    bool loop = true;

    // Gestor de eventos de SDL y N'gine
    ngn->system->EventUpdate();            // Actualiza los eventos

    // Control del bucle principal
    loop &= !ngn->system->quit;                                 // Si se pulsa la [X] de la ventana
    loop &= !ngn->input->key_ESC->down;                         // O se pulsa la tecla [ESC] sal del bucle de ejecucion
    loop &= !ngn->input->controller[0].button[10].down;         // O se pulsa el boton XBOX

    /***
    Actualizacion del programa
    ***/
    // Animacion
    Animation();
    // Render de objetos
    Render();

    // Actualiza el contenido de la pantalla
    ngn->graphics->Update();
    // Actualiza el sonido
    ngn->sound->Update();

    // Devuelve el resultado
    return loop;

}



/*** Crea el fondo ***/
void Demo::CreateBg() {

    // Crea el canvas
    canvas_bg = new NGN_Canvas();

    // Crea el buffer
    buffer.clear();
    buffer.resize((SCR_WIDTH * SCR_HEIGHT));

    // Estructura de color
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } color;

    // Crea un fondo
    CanvasPoint point;
    uint32_t id = 0;
    color.a = 0xFF;
    for (uint32_t y = 0; y < SCR_HEIGHT; y ++) {
        point.y = y;
        for (uint32_t x = 0; x < SCR_WIDTH; x ++) {
            point.x = x;
            color.r = (uint32_t)std::sqrt(x * (SCR_HEIGHT - y)) % 0xFF;
            color.g = (uint32_t)std::sqrt((SCR_WIDTH - x) * y) % 0xFF;
            color.b = (uint32_t)std::sqrt(x * y) % 0xFF;
            point.color = (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
            buffer[id] = point;
            id ++;
        }
    }

    // Mandalo al canvas
    canvas_bg->Points(buffer);

    // Limpia el buffer
    buffer.clear();

}



/*** Crea el canvas con nieve ***/
void Demo::CreateSnow() {

    // Crea el canvas
    const uint32_t width = (SCR_WIDTH / SCREEN_DIVIDER);
    const uint32_t height = (SCR_HEIGHT / SCREEN_DIVIDER);
    canvas_snow = new NGN_Canvas(0.0f, 0.0f, width, height);

    // Crea el buffer
    buffer.clear();
    buffer.resize((width * height));

    // Crea un fondo
    CanvasPoint point;
    uint32_t id = 0;
    uint8_t color = 0x00;
    for (uint32_t y = 0; y < height; y ++) {
        point.y = y;
        for (uint32_t x = 0; x < width; x ++) {
            point.x = x;
            color = (rand() % 0xFF);
            point.color = (color << 24) | (color << 16) | (color << 8) | 0xFF;
            buffer[id] = point;
            id ++;
        }
    }

    // Mandalo al canvas
    canvas_snow->Points(buffer);

    // Ajusta el alpha
    canvas_snow->alpha = 200;

}



/*** Animacion del canvas de nieve ***/
void Demo::Animation() {

    // Datos
    int32_t width = canvas_snow->width;
    int32_t height = canvas_snow->height;

    // Crea un fondo

    if (animate) {
        CanvasPoint point;
        uint32_t id = 0;
        uint8_t color = 0x00;
        for (int32_t y = 0; y < height; y ++) {
            point.y = y;
            for (int32_t x = 0; x < width; x ++) {
                point.x = x;
                color = (rand() % 0xFF);
                point.color = (color << 24) | (color << 16) | (color << 8) | 0xFF;
                buffer[id] = point;
                id ++;
            }
        }
    } else {
        // Mandalo al canvas
        canvas_snow->Points(buffer);
    }

    // Posicionalo
    int32_t x = ngn->input->mouse.x - (width / 2);
    int32_t y = ngn->input->mouse.y - (height / 2);
    canvas_snow->Position(x, y);

    animate = !animate;

}



/*** Renderiza los objetos ***/
void Demo::Render() {

    // Renderiza los canvas
    ngn->render->Canvas(canvas_bg);
    ngn->render->Canvas(canvas_snow);

}
