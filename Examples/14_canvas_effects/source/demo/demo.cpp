/******************************************************************************

    Ejemplo del uso de la capa de dibujado (canvas) para efectos
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 0.10.0-a o superior

    Requiere GCC 7.3.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.10) - 64-bits
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
#include <cmath>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



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
    #if defined (OS_WINDOWS)
        if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, SCR_MODE_WINDOWS, BILINEAR_FILTER, VSYNC)) return false;
    #elif defined (OS_LINUX)
        if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    #endif

    // Esconde el cursor del raton
    ngn->graphics->ShowMouse(SHOW_MOUSE);

    // Contador de FPS
    ngn->system->fps_counter = FPS_COUNTER;

    // Fuerza la actualizacion de la pantalla
    ngn->graphics->Update();
    #if defined (OS_LINUX)
        ngn->graphics->full_screen = SCR_MODE_LINUX;
    #endif

    // Muestra la version de la libreria en la consola
    #if defined (MODE_DEBUG)
        std::cout << ngn->system->GetVersion() << std::endl;
    #endif

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



/*** Ejecucion del programa ***/
int8_t Demo::Run() {

    // Control del loop
    int8_t loop = -1;

    while (loop < 0) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos

        // Actualizacion del programa
        Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();

        // Control del bucle principal
        if (ngn->system->quit) {    // Si se pulsa la [X] de la ventana
            loop = 0;
        } else if (ngn->input->key_ESC->down || ngn->input->controller[0].button[10].down) {    // Si se pulsa la tecla [ESC] O se pulsa el boton XBOX
            loop = 1;
        }

    }

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



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Animacion
    Animation();
    // Render de objetos
    Render();

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
