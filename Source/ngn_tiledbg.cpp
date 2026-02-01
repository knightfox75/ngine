/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0+10th-anniversary ***
    Fondos Tileados

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016 - 2026 by Cesar Rincon "NightFox"

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
#include <cmath>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/*** Contructor (1ra sobrecarga) ***/
NGN_TiledBg::NGN_TiledBg(
    NGN_TiledBgData* bg,            // Datos del fondo
    int32_t position_x,             // Posicion X del fondo [0 por defecto]
    int32_t position_y              // Posicion Y del fondo [0 por defecto]
) {

    // Crea el fondo
    CreateTiledBg(bg, position_x, position_y);

}



/*** Contructor (2da sobrecarga) ***/
NGN_TiledBg::NGN_TiledBg(
    std::string repo_name,          // Nombre del repositorio
    std::string resource_name,      // Nombre del recurso
    int32_t position_x,             // Posicion X del fondo [0 por defecto]
    int32_t position_y              // Posicion Y del fondo [0 por defecto]
) {

    // Crea el fondo
    CreateTiledBg(ngn->resources->GetTiledbg(repo_name, resource_name), position_x, position_y);

}



/*** Destructor ***/
NGN_TiledBg::~NGN_TiledBg() {

    // Destruye el backbuffer
    SDL_DestroyTexture(backbuffer);
    backbuffer = nullptr;

    // De existir, elimina la textura de transformacion
    if (transform_texture != nullptr) SDL_DestroyTexture(transform_texture);
    transform_texture = nullptr;

    // Elimina la referencia al fondo
    bgdata = nullptr;

}



/*** Posiciona el fondo en una coordenada especifica ***/
void NGN_TiledBg::Position(float position_x, float position_y) {

    // Asigna la posicion
    position.x = position_x;
    position.y = position_y;

}
void NGN_TiledBg::Position(Vector2 pos) {

    // Asigna la posicion
    Position(pos.x, pos.y);

}



/*** Mueve un fondo en la direccion indicada ***/
void NGN_TiledBg::Translate(float speed_x, float speed_y) {

    // Calcula la nueva posicion
    position.x += speed_x;
    position.y += speed_y;

}
void NGN_TiledBg::Translate(Vector2 spd) {

    // Calcula la nueva posicion
    Translate(spd.x, spd.y);

}



/*** Escala el fondo [Sobrecarga 1 - Ejes por separado] ***/
void NGN_TiledBg::Scale(float w, float h) {

    // Limita la escala del fondo a valores >= 1.0f

    // Aplica la escala
    scale.x = (w < 1.0f) ? 1.0f:w;
    scale.y = (h < 1.0f) ? 1.0f:h;

}
/*** Escala el fondo [Sobrecarga 2 - Ambos ejes a la vez] ***/
void NGN_TiledBg::Scale(float factor) {

    // Aplica la escala
    Scale(factor, factor);

}



/*** Devuelve la escala actual del fondo ***/
Size2 NGN_TiledBg::GetCurrentScale() {

    Size2 s;
    s.width = scale.x;
    s.height = scale.y;
    return s;

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



/*** Selecciona un color de tinte (sin parametros, resetea el color) ***/
void NGN_TiledBg::SetTintColor(uint8_t r, uint8_t g, uint8_t b) {

    // Registra el color
    tint_color.r = r;
    tint_color.g = g;
    tint_color.b = b;

}



/*** Devuelve si se ha cambiado el color de tinta en este frame ***/
bool NGN_TiledBg::NewTint() {

    bool color_mod = ((tint_color.r != last_tint_color.r) || (tint_color.g != last_tint_color.g) || (tint_color.b != last_tint_color.b));
    last_tint_color = tint_color;
    return color_mod;

}



/*** Crea el objeto que contiene el fondo ***/
void NGN_TiledBg::CreateTiledBg(
    NGN_TiledBgData* bg,    // Datos del fondo
    int32_t position_x,     // Posicion X del fondo
    int32_t position_y      // Posicion Y del fondo
) {

    // Copia del fondo
    bgdata = bg;

    // Guarda el tamaño del tile
    tile_size = bg->header.tile_size;
    // Guarda el tamaño del mapa
    map_size.width = bg->header.map_width;
    map_size.height = bg->header.map_height;

    // Tamaño del backbuffer
    bb_size.width = ngn->graphics->native_w + (tile_size << 1);
    bb_size.height = ngn->graphics->native_h + (tile_size << 1);
    // Tamaño del fondo completo
    width = bgdata->header.bg_width;
    height = bgdata->header.bg_height;

    // Almacena los datos del fondo, segun los datos facilitados
    position.x = position_x;
    position.y = position_y;

    last_position.x = NGN_DEFAULT_VALUE;
    last_position.y = NGN_DEFAULT_VALUE;
    last_viewport = -1;

    center.x = 0;
    center.y = 0;

    rotation = 0;

    flip_h = false;
    flip_v = false;

    visible = true;
    alpha = 0xFF;
    blend_mode = NGN_BLENDMODE_ALPHA;   // Modo de mezcla

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

    // Textura para aplicar la transformacion del fondo (si se requiere)
    transform_texture = nullptr;
    last_buffer = nullptr;

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

    // Color de tinta
    tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    last_tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    ignore_camera_tint = false;

}
