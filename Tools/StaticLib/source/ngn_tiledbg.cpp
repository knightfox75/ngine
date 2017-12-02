/******************************************************************************

    N'gine Lib for C++
    *** Version 0.4.0-alpha ***
    Fondos Tileados

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2017 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com


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
#include <cmath>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/*** Contructor ***/
NGN_TiledBg::NGN_TiledBg(
                        NGN_TiledBgData* bg,            // Datos del fondo
                        int32_t position_x,             // Posicion X del fondo [0 por defecto]
                        int32_t position_y,             // Posicion Y del fondo [0 por defecto]
                        bool over_scan                  // Uso de overscan en el fondo
                        ) {

    // Copia del fondo
    bgdata = bg;

    // En caso de estar el over scan activo, haz los calculos pertinentes
    if (!over_scan) {
        // Over scan inactivo
        bb_size.width = ngn->graphics->native_w;
        bb_size.height = ngn->graphics->native_h;
        // Tamaño del fondo completo
        width = bgdata->header.bg_width;
        height = bgdata->header.bg_height;
        // Parametros del overscan inactivo
        overscan.border.width = overscan.border.height = 0;
        overscan.in_size.width = overscan.in_size.height = 0;
        overscan.out_size.width = overscan.out_size.height = 0;
        // Indica que esta desactivado
        overscan.enabled = false;
    } else {
        // Over scan activo
        bb_size.width = bb_size.height = std::sqrt((ngn->graphics->native_w * ngn->graphics->native_w) + (ngn->graphics->native_h * ngn->graphics->native_h));
        overscan.border.width = (((float)(bb_size.width - ngn->graphics->native_w)) / 2.0f);
        overscan.border.height = (((float)(bb_size.height - ngn->graphics->native_h)) / 2.0f);
        overscan.out_size.width = bgdata->header.bg_width;
        overscan.out_size.height = bgdata->header.bg_height;
        overscan.in_size.width = (bgdata->header.bg_width - (bb_size.width - ngn->graphics->native_w));
        overscan.in_size.height = (bgdata->header.bg_height - (bb_size.height - ngn->graphics->native_h));
        // Tamaño del fondo recortado
        width = overscan.in_size.width;
        height = overscan.in_size.height;
        // Indica que esta activado
        overscan.enabled = true;
    }

    // Almacena los datos del fondo, segun los datos facilitados
    position.x = position_x;
    position.y = position_y;

    last_position.x = DEFAULT_VALUE;
    last_position.y = DEFAULT_VALUE;

    center.x = 0;
    center.y = 0;

    rotation = 0;

    visible = true;
    alpha = 0xFF;

    scale.x = 1.0f;
    scale.y = 1.0f;


    // Crea el backbuffer de este fondo
    backbuffer = SDL_CreateTexture(
                             ngn->graphics->renderer,       // Renderer
                             SDL_PIXELFORMAT_BGRA8888,      // Formato del pixel
                             SDL_TEXTUREACCESS_TARGET,      // Textura como destino del renderer
                             bb_size.width,                 // Ancho de la textura
                             bb_size.height                 // Alto de la textura
                             );

    // Parametros para el sistema de camaras
    camera_layer = -1;                      // Por defecto, no estas en la camara
    virtual_bg.bg_size.width = 0;           // Sin tamaño virtual, loop, etc
    virtual_bg.bg_size.height = 0;
    virtual_bg.loop.x = 0;
    virtual_bg.loop.y = 0;
    virtual_bg.scroll.x = 0.0f;
    virtual_bg.scroll.y = 0.0f;
    virtual_bg.offset.x = 0.0f;
    virtual_bg.offset.y = 0.0f;
    virtual_bg.enabled = false;

}



/*** Destructor ***/
NGN_TiledBg::~NGN_TiledBg() {

    // Destruye el backbuffer
    SDL_DestroyTexture(backbuffer);
    backbuffer = NULL;
    // Elimina la referencia al fondo
    bgdata = NULL;

}



/*** Posiciona el fondo en una coordenada especifica ***/
void NGN_TiledBg::Position(float position_x, float position_y) {

    // Asigna la posicion
    position.x = position_x;
    position.y = position_y;

}
void NGN_TiledBg::Position(Vector2 pos) {

    // Asigna la posicion
    position.x = pos.x;
    position.y = pos.y;

}



/*** Mueve un fondo en la direccion indicada ***/
void NGN_TiledBg::Translate(float speed_x, float speed_y) {

    // Calcula la nueva posicion
    position.x += speed_x;
    position.y += speed_y;

}
void NGN_TiledBg::Translate(Vector2 spd) {

    // Calcula la nueva posicion
    position.x += spd.x;
    position.y += spd.y;

}



/*** Escala el fondo [Sobrecarga 1 - Ejes por separado] ***/
void NGN_TiledBg::Scale(float w, float h) {

    // Aplica la escala
    scale.x = w;
    scale.y = h;

}

/*** Escala el fondo [Sobrecarga 2 - Ambos ejes a la vez] ***/
void NGN_TiledBg::Scale(float factor) {

    // Aplica la escala
    scale.x = factor;
    scale.y = factor;

}



/*** Rota el fondo los grados solicitados ***/
void NGN_TiledBg::Rotate(double degrees) {

    rotation += degrees;
    while (rotation >= 360.0f) rotation -= 360.0f;
    while (rotation < 0.0f) rotation += 360.0f;

}



/*** Fija el centro del fondo (Posicion relativa desde el centro REAL) ***/
void NGN_TiledBg::SetCenter(float x, float y) {

    center.x = x;
    center.y = y;

}

