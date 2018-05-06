/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Cajas de colision de sprite ajustables

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
#define WINDOW_TITLE "Sprite Hitbox"
#define SCR_WIDTH   1280
#define SCR_HEIGHT  720




/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicia los punteros

    bg_data = NULL;
    bg = NULL;

    for (uint8_t i = 0; i < number_of_sprites; i ++) {
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

    for (uint8_t i = 0; i < number_of_sprites; i ++) {
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

    // Selecciona el modo grafico
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW)) return false;

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

    // Carga de archivos
    if (!Load()) return false;

    // Crea la escena
    Create();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
void Demo::Run() {

    bool loop = true;

    // Loop principal
    while (loop) {

        // Gestor de eventos de SDL y N'gine (Incluye la lectura de las entradas)
        ngn->system->EventUpdate();

        // Actualiza el programa
        Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();

        // Control del bucle principal
        loop &= !ngn->system->quit;             // Si se pulsa la [X] de la ventana
        loop &= !ngn->input->key_ESC->down;     // O se pulsa la tecla [ESC] sal del bucle de ejecucion

    }

}



/*** Actualizacion ***/
void Demo::Update() {

    // Mueve el cursor con el raton
    aim->Position(ngn->input->mouse.x, ngn->input->mouse.y);

    // Deteccion de colision
    for (uint8_t i = 0; i < number_of_sprites; i ++) {
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
    for (uint8_t i = 0; i < number_of_sprites; i ++) ngn->render->Sprite(spr[i]);
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
    spr_data[spr_normal] = ngn->load->Sprite("data/bluemark_normal.spr");
    spr_data[spr_top] = ngn->load->Sprite("data/bluemark_top.spr");
    spr_data[spr_bottom] = ngn->load->Sprite("data/bluemark_bottom.spr");
    spr_data[spr_left] = ngn->load->Sprite("data/bluemark_left.spr");
    spr_data[spr_right] = ngn->load->Sprite("data/bluemark_right.spr");
    spr_data[spr_topleft] = ngn->load->Sprite("data/bluemark_topleft.spr");
    spr_data[spr_bottomright] = ngn->load->Sprite("data/bluemark_bottomright.spr");
    for (uint8_t i = 0; i < number_of_sprites; i ++) if (spr_data[i] == NULL) return false;

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
    for (uint8_t i = 0; i < number_of_sprites; i ++) {
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
    spr[spr_normal]->box.width = spr[spr_normal]->width;
    spr[spr_normal]->box.height = spr[spr_normal]->height;
    spr[spr_normal]->box.offset.x = 0.0f;
    spr[spr_normal]->box.offset.y = 0.0f;
    // Solo parte superior
    spr[spr_top]->box.width = spr[spr_top]->width;
    spr[spr_top]->box.height = (spr[spr_top]->height / 2.0f);
    spr[spr_top]->box.offset.x = 0.0f;
    spr[spr_top]->box.offset.y = -(spr[spr_top]->box.height / 2.0f);
    // Solo parte inferior
    spr[spr_bottom]->box.width = spr[spr_bottom]->width;
    spr[spr_bottom]->box.height = (spr[spr_bottom]->height / 2.0f);
    spr[spr_bottom]->box.offset.x = 0.0f;
    spr[spr_bottom]->box.offset.y = (spr[spr_bottom]->box.height / 2.0f);
    // Solo parte izquierda
    spr[spr_left]->box.width = (spr[spr_left]->width / 2.0f);
    spr[spr_left]->box.height = spr[spr_left]->height;
    spr[spr_left]->box.offset.x = -(spr[spr_left]->box.width / 2.0f);
    spr[spr_left]->box.offset.y = 0.0f;
    // Solo parte derecha
    spr[spr_right]->box.width = (spr[spr_right]->width / 2.0f);
    spr[spr_right]->box.height = spr[spr_right]->height;
    spr[spr_right]->box.offset.x = (spr[spr_right]->box.width / 2.0f);
    spr[spr_right]->box.offset.y = 0.0f;
    // Seccion superior izquierda
    spr[spr_topleft]->box.width = (spr[spr_topleft]->width / 2.0f);
    spr[spr_topleft]->box.height = (spr[spr_topleft]->height / 2.0f);
    spr[spr_topleft]->box.offset.x = -(spr[spr_topleft]->box.width / 2.0f);
    spr[spr_topleft]->box.offset.y = -(spr[spr_topleft]->box.height / 2.0f);
    // Seccion inferior derecha
    spr[spr_bottomright]->box.width = (spr[spr_bottomright]->width / 2.0f);
    spr[spr_bottomright]->box.height = (spr[spr_bottomright]->height / 2.0f);
    spr[spr_bottomright]->box.offset.x = (spr[spr_bottomright]->box.width / 2.0f);
    spr[spr_bottomright]->box.offset.y = (spr[spr_bottomright]->box.height / 2.0f);


}
