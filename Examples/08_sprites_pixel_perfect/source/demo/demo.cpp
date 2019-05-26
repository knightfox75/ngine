/******************************************************************************

    Ejemplo de colisiones "pixel perfect" entre sprites
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 0.9.0-a o superior

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
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Punteros de datos
    bg_data = NULL;
    star_data = NULL;
    cake_data = NULL;

    // Punteros de objetos
    bg = NULL;
    star = NULL;
    cake = NULL;

    // Variables
    zoom.x = zoom.y = 1.0f;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg; bg = NULL;
    delete star; star = NULL;
    delete cake; cake = NULL;

    // Borra los datos
    delete bg_data; bg_data = NULL;
    delete star_data; star_data = NULL;
    delete cake_data; cake_data = NULL;

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

    // Carga los archivos
    if (!Load()) return false;

    // Crea la escena
    CreateStage();

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



/*** Carga los archivos necesarios ***/
bool Demo::Load() {

    // Fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (bg_data == NULL) return false;

    // Sprites
    star_data = ngn->load->Sprite("data/red_star.spr");
    if (star_data == NULL) return false;
    cake_data = ngn->load->Sprite("data/cake.spr");
    if (cake_data == NULL) return false;

    // Carga correcta
    return true;

}



/*** Crea la escena ***/
void Demo::CreateStage() {

    // Crea el fondo
    bg = new NGN_Texture(bg_data, 0, 0);
    // Crea el sprite de la tarta
    cake = new NGN_Sprite(cake_data, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));
    cake->frame = 0;
    // Crea el sprite de la estrella
    star = new NGN_Sprite(star_data, 0, 0);

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    Logic();        // Logica del programa
    Render();       // Renderiza la escena

}



/*** Logica del programa ***/
void Demo::Logic() {

    // Mueve la estrella con el raton
    star->Position(ngn->input->mouse.x, ngn->input->mouse.y);

    // Rotacion
    if (ngn->input->key_ARROW_RIGHT->held) cake->Rotate(2.0f);
    if (ngn->input->key_ARROW_LEFT->held) cake->Rotate(-2.0f);
    if (ngn->input->key_Q->down) cake->rotation = 0.0f;

    // Zoom Global
    if (ngn->input->key_ARROW_UP->held) {
        zoom.x += 0.02f;
        zoom.y += 0.02f;
        if (zoom.x > 4.0f) zoom.x = 4.0f;
        if (zoom.y > 4.0f) zoom.y = 4.0f;
    }
    if (ngn->input->key_ARROW_DOWN->held) {
        zoom.x -= 0.02f;
        zoom.y -= 0.02f;
        if (zoom.x < 0.25f) zoom.x = 0.25f;
        if (zoom.y < 0.25f) zoom.y = 0.25f;
    }
    // Zoom X
    if (ngn->input->key_D->held) {
        zoom.x += 0.02f;
        if (zoom.x > 4.0f) zoom.x = 4.0f;
    }
    if (ngn->input->key_A->held) {
        zoom.x -= 0.02f;
        if (zoom.x < 0.25f) zoom.x = 0.25f;
    }
    // Zoom Y
    if (ngn->input->key_W->held) {
        zoom.y += 0.02f;
        if (zoom.y > 4.0f) zoom.y = 4.0f;
    }
    if (ngn->input->key_S->held) {
        zoom.y -= 0.02f;
        if (zoom.y < 0.25f) zoom.y = 0.25f;
    }
    if (ngn->input->key_E->down) zoom.x = zoom.y = 1.0f;
    cake->Scale(zoom.x, zoom.y);

    // Dependiendo de si hay o no colision
    if (ngn->collisions->PixelPerfect(star, cake)) {
        cake->frame = 1;
    } else {
        cake->frame = 0;
    }

}



/*** Renderiza la escena ***/
void Demo::Render() {

    ngn->render->Texture(bg);       // Fondo
    ngn->render->Sprite(cake);      // Sprite de la tarta
    ngn->render->Sprite(star);      // Sprite de la estrella

}
