/******************************************************************************

    N'gine Lib for C++
    *** Version 1.1.0-beta ***
    Fondos con texturas

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
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
#include <vector>
#include <string>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/*** Contructor (1ra sobrecarga) ***/
NGN_Texture::NGN_Texture(
                    NGN_TextureData* texture,               // Objeto de la clase Texture Data
                    int32_t position_x,                     // Posicion X inicial (oculto por defecto)
                    int32_t position_y,                     // Posicion Y inicial (oculto por defecto)
                    uint32_t texture_width,                 // Ancho de la textura (por defecto, el de la textura)
                    uint32_t texture_height                 // Altura de la textura (por defecto, la de la textura)
) {

    // Guarda el grafico que usara este texture
    data = texture;
    // Tama�o
    if ((texture_width != NGN_DEFAULT_VALUE) && (texture_height != NGN_DEFAULT_VALUE)) {
        // Tama�o definido por el usuario
        width = texture_width;
        height = texture_height;
    } else {
        // Tama�o en base a la textura del grafico
        width = data->width;
        height = data->height;
    }
    linked = true;

    // Guarda el tama�o original al crear la textura
    original.width = width;
    original.height = height;

    // Posicion
    if ((position_x != NGN_DEFAULT_VALUE) && (position_y != NGN_DEFAULT_VALUE)) {
        // Posicion definida por el usuario
        position.x = position_x;
        position.y = position_y;
    } else {
        // Posicion fuera del escenario
        position.x = -width;
        position.y = -height;
    }

    DefaultValues();

}


/*** Contructor (2da sobrecarga) ***/
NGN_Texture::NGN_Texture(
                    uint32_t texture_width,     // Ancho de la textura (por defecto, el de la textura)
                    uint32_t texture_height,    // Altura de la textura (por defecto, la de la textura)
                    int32_t position_x,         // Posicion X inicial (oculto por defecto)
                    int32_t position_y          // Posicion Y inicial (oculto por defecto)
) {

    // Tama�o definido por el usuario
    width = texture_width;
    height = texture_height;

    // Crea la textura
    data = new NGN_TextureData();
    data->gfx = SDL_CreateTexture(
        ngn->graphics->renderer,        // Renderer
        SDL_PIXELFORMAT_BGRA8888,       // Formato del pixel
        SDL_TEXTUREACCESS_TARGET,       // Textura como destino del renderer
        width,                          // Ancho de la textura
        height                          // Alto de la textura
    );
    data->width = width;
    data->height = height;
    linked = false;

    // Guarda el tama�o original al crear la textura
    original.width = width;
    original.height = height;

    // Posicion
    if ((position_x != NGN_DEFAULT_VALUE) && (position_y != NGN_DEFAULT_VALUE)) {
        // Posicion definida por el usuario
        position.x = position_x;
        position.y = position_y;
    } else {
        // Posicion fuera del escenario
        position.x = -width;
        position.y = -height;
    }

    DefaultValues();

}



/*** Destructor ***/
NGN_Texture::~NGN_Texture() {

    // Elimina la textura si no es enlazada
    if (!linked) delete data;
    // Elimina la referencia al grafico
    data = NULL;

}



/*** Parametros por defecto del contructor ***/
void NGN_Texture::DefaultValues() {

    // Valores por defecto
    visible = true;             // Visibilidad
    alpha = 0xFF;               // Alpha
    blend_mode = NGN_BLENDMODE_ALPHA;   // Modo de mezcla
    rotation = 0.0f;            // Rotacion
    center.x = 0.0f;            // Centro de rotacion
    center.y = 0.0f;
    flip_h = false;             // Flip
    flip_v = false;
    screen = position;

    // Parametros para el sistema de camaras
    camera_layer = -1;                              // Por defecto, no estas en la camara
    virtual_texture.texture_size.width = 0;         // Sin tama�o virtual, loop, etc
    virtual_texture.texture_size.height = 0;
    virtual_texture.loop.x = 0;
    virtual_texture.loop.y = 0;
    virtual_texture.scroll.x = 0.0f;
    virtual_texture.scroll.y = 0.0f;
    virtual_texture.offset.x = 0.0f;
    virtual_texture.offset.y = 0.0f;
    virtual_texture.enabled = false;

}



/*** Posiciona la textura ***/
void NGN_Texture::Position(float position_x, float position_y) {

    // Asigna la posicion a la Textura
    position.x = position_x;
    position.y = position_y;

}
void NGN_Texture::Position(Vector2 pos) {

    // Asigna la posicion a la Textura
    position.x = pos.x;
    position.y = pos.y;

}



/*** Mueve una textura ***/
void NGN_Texture::Translate(float speed_x, float speed_y) {

    // Calcula la nueva posicion
    position.x += speed_x;
    position.y += speed_y;

}
void NGN_Texture::Translate(Vector2 spd) {

    // Calcula la nueva posicion
    position.x += spd.x;
    position.y += spd.y;

}



/*** Cambia el tama�o de la textura ***/
void NGN_Texture::Size(float w, float h) {

    // Aplica el nuevo tama�o
    width = w;
    original.width = width;
    height = h;
    original.height = height;

}



/*** Escala una textura [Sobrecarga 1 - Ejes por separado] ***/
void NGN_Texture::Scale(float w, float h) {

    // Aplica la escala
    width = original.width * w;
    height = original.height * h;

}

/*** Escala una textura [Sobrecarga 2 - Ambos ejes a la vez] ***/
void NGN_Texture::Scale(float scale) {

    // Aplica la escala
    width = original.width * scale;
    height = original.height * scale;

}



/*** Rota una textura los grados solicitados ***/
void NGN_Texture::Rotate(double degrees) {

    rotation += degrees;
    while (rotation >= 360.0f) rotation -= 360.0f;
    while (rotation < 0.0f) rotation += 360.0f;

}



/*** Fija el centro del Texture (Posicion relativa desde el centro REAL) ***/
void NGN_Texture::SetCenter(float x, float y) {

    center.x = x;
    center.y = y;

}



/*** Borra el contenido de la textura, si no esta enlazada ***/
void NGN_Texture::ClearContent() {

    if (linked) return;

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, data->gfx);
    // Borra el contenido de la textura actual
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderFillRect(ngn->graphics->renderer, NULL);

    // Devuelve el render al seleccionado
    ngn->graphics->RenderToSelected();

}
