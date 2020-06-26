/******************************************************************************

    Ejemplo del uso de deteccion de colisiones entre sprites por caja
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.0.0-stable o superior

    Requiere GCC 7.3.0 MinGW (SEH) - 64-bits
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
// c++
#include <cstdio>
#include <iostream>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicia los punteros

    bg_data = NULL;
    bg = NULL;

    for (uint8_t i = 0; i < NUMBER_OF_SPRITES; i ++) {
        spr_data[i] = NULL;
        spr[i] = NULL;
    }
    aim_data = NULL;
    aim = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Elimina los datos

    delete bg; bg = NULL;
    delete bg_data; bg_data = NULL;

    for (uint8_t i = 0; i < NUMBER_OF_SPRITES; i ++) {
        delete spr[i]; spr[i] = NULL;
        delete spr_data[i]; spr_data[i] = NULL;
    }

    delete aim; aim = NULL;
    delete aim_data; aim_data = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Crea la ventana con la resolucion nativa
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    ngn->graphics->Update();

    // visibilidad del cursor del raton
    ngn->graphics->ShowMouse(SHOW_MOUSE);

    // Contador de FPS activo?
    ngn->system->fps_counter = FPS_COUNTER;

    // Selecciona el modo grafico (ventana/full screen)
    #if defined (OS_WINDOWS)
        ngn->graphics->SetMode(SCR_MODE_WINDOWS);
    #elif defined (OS_LINUX)
        ngn->graphics->SetMode(SCR_MODE_LINUX);
    #endif
    ngn->graphics->Update();

    // Muestra la version de la libreria en la consola
    #if defined (MODE_DEBUG)
        std::cout << ngn->system->GetVersion() << std::endl;
    #endif

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Carga de archivos
    if (!Load()) return false;

    // Crea la escena
    Create();

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

        // Actualiza el programa
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



/*** Actualizacion ***/
void Demo::Update() {

    // Mueve el cursor con el raton
    aim->Position(ngn->input->mouse.x, ngn->input->mouse.y);

    // Deteccion de colision
    for (uint8_t i = 0; i < NUMBER_OF_SPRITES; i ++) {
        if (ngn->collisions->HitBox(aim, spr[i])) {
            spr[i]->alpha = 0x80;
        } else {
            spr[i]->alpha = 0xFF;
        }
    }

    // Renderiza la escena
    Render();

}



/*** Renderizado ***/
void Demo::Render() {

    // Fondo
    ngn->render->Texture(bg);

    // Sprites
    for (uint8_t i = 0; i < NUMBER_OF_SPRITES; i ++) ngn->render->Sprite(spr[i]);
    ngn->render->Sprite(aim);

}



/*** Carga los archivos ***/
bool Demo::Load() {

    // Carga los fondos
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (bg_data == NULL) return false;

    // Carga los sprites
    aim_data = ngn->load->Sprite("data/aim.spr");
    if (aim_data == NULL) return false;
    spr_data[SPR_NORMAL] = ngn->load->Sprite("data/bluemark_normal.spr");
    spr_data[SPR_TOP] = ngn->load->Sprite("data/bluemark_top.spr");
    spr_data[SPR_BOTTOM] = ngn->load->Sprite("data/bluemark_bottom.spr");
    spr_data[SPR_LEFT] = ngn->load->Sprite("data/bluemark_left.spr");
    spr_data[SPR_RIGHT] = ngn->load->Sprite("data/bluemark_right.spr");
    spr_data[SPR_TOP_LEFT] = ngn->load->Sprite("data/bluemark_topleft.spr");
    spr_data[SPR_BOTTOM_RIGHT] = ngn->load->Sprite("data/bluemark_bottomright.spr");
    for (uint8_t i = 0; i < NUMBER_OF_SPRITES; i ++) if (spr_data[i] == NULL) return false;

    // Carga correcta
    return true;

}



/*** Crea la escena ***/
void Demo::Create() {

    // Fondo
    bg = new NGN_Texture(bg_data, 0, 0);

    // Crea los sprites de referencia
    Vector2I32 pos;
    pos.x = 350;
    pos.y = 250;
    for (uint8_t i = 0; i < NUMBER_OF_SPRITES; i ++) {
        spr[i] = new NGN_Sprite(spr_data[i], pos.x, pos.y);
        pos.x += 200;
        if (pos.x > 950) {
            pos.x = 350;
            pos.y += 250;
        }
    }

    // Crea la mirilla
    aim = new NGN_Sprite(aim_data, 0, 0);

    // Ajusta la caja de colision de cada sprite
    // Normal
    spr[SPR_NORMAL]->box.width = spr[SPR_NORMAL]->width;
    spr[SPR_NORMAL]->box.height = spr[SPR_NORMAL]->height;
    spr[SPR_NORMAL]->box.offset.x = 0.0f;
    spr[SPR_NORMAL]->box.offset.y = 0.0f;
    // Solo parte superior
    spr[SPR_TOP]->box.width = spr[SPR_TOP]->width;
    spr[SPR_TOP]->box.height = (spr[SPR_TOP]->height / 2.0f);
    spr[SPR_TOP]->box.offset.x = 0.0f;
    spr[SPR_TOP]->box.offset.y = -(spr[SPR_TOP]->box.height / 2.0f);
    // Solo parte inferior
    spr[SPR_BOTTOM]->box.width = spr[SPR_BOTTOM]->width;
    spr[SPR_BOTTOM]->box.height = (spr[SPR_BOTTOM]->height / 2.0f);
    spr[SPR_BOTTOM]->box.offset.x = 0.0f;
    spr[SPR_BOTTOM]->box.offset.y = (spr[SPR_BOTTOM]->box.height / 2.0f);
    // Solo parte izquierda
    spr[SPR_LEFT]->box.width = (spr[SPR_LEFT]->width / 2.0f);
    spr[SPR_LEFT]->box.height = spr[SPR_LEFT]->height;
    spr[SPR_LEFT]->box.offset.x = -(spr[SPR_LEFT]->box.width / 2.0f);
    spr[SPR_LEFT]->box.offset.y = 0.0f;
    // Solo parte derecha
    spr[SPR_RIGHT]->box.width = (spr[SPR_RIGHT]->width / 2.0f);
    spr[SPR_RIGHT]->box.height = spr[SPR_RIGHT]->height;
    spr[SPR_RIGHT]->box.offset.x = (spr[SPR_RIGHT]->box.width / 2.0f);
    spr[SPR_RIGHT]->box.offset.y = 0.0f;
    // Seccion superior izquierda
    spr[SPR_TOP_LEFT]->box.width = (spr[SPR_TOP_LEFT]->width / 2.0f);
    spr[SPR_TOP_LEFT]->box.height = (spr[SPR_TOP_LEFT]->height / 2.0f);
    spr[SPR_TOP_LEFT]->box.offset.x = -(spr[SPR_TOP_LEFT]->box.width / 2.0f);
    spr[SPR_TOP_LEFT]->box.offset.y = -(spr[SPR_TOP_LEFT]->box.height / 2.0f);
    // Seccion inferior derecha
    spr[SPR_BOTTOM_RIGHT]->box.width = (spr[SPR_BOTTOM_RIGHT]->width / 2.0f);
    spr[SPR_BOTTOM_RIGHT]->box.height = (spr[SPR_BOTTOM_RIGHT]->height / 2.0f);
    spr[SPR_BOTTOM_RIGHT]->box.offset.x = (spr[SPR_BOTTOM_RIGHT]->box.width / 2.0f);
    spr[SPR_BOTTOM_RIGHT]->box.offset.y = (spr[SPR_BOTTOM_RIGHT]->box.height / 2.0f);

}
