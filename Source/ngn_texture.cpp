/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0-wip_0x03 ***
    Fondos con texturas

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2025 by Cesar Rincon "NightFox"

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

    // Crea la textura
    CreateTexture(texture, position_x, position_y, texture_width, texture_height);

}


/*** Contructor (2da sobrecarga) ***/
NGN_Texture::NGN_Texture(
    uint32_t texture_width,     // Ancho de la textura (por defecto, el de la textura)
    uint32_t texture_height,    // Altura de la textura (por defecto, la de la textura)
    int32_t position_x,         // Posicion X inicial (oculto por defecto)
    int32_t position_y          // Posicion Y inicial (oculto por defecto)
) {

    // Tamaño definido por el usuario
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

    // Guarda el tamaño original al crear la textura
    original_size.width = width;
    original_size.height = height;
    current_scale.width = 1.0f;
    current_scale.height = 1.0f;

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



/*** Contructor (3ra sobrecarga) ***/
NGN_Texture::NGN_Texture(
    std::string repo_name,                  // Nombre del repositorio
    std::string resource_name,              // Nombre del recurso
    int32_t position_x,                     // Posicion X inicial (oculto por defecto)
    int32_t position_y,                     // Posicion Y inicial (oculto por defecto)
    uint32_t texture_width,                 // Ancho de la textura (por defecto, el de la textura)
    uint32_t texture_height                 // Altura de la textura (por defecto, la de la textura)
) {

    // Crea la textura
    CreateTexture(ngn->resources->GetTexture(repo_name, resource_name), position_x, position_y, texture_width, texture_height);

}



/*** Destructor ***/
NGN_Texture::~NGN_Texture() {

    // Elimina la textura si no es enlazada
    if (!linked) delete data;
    // Elimina la referencia al grafico
    data = nullptr;

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
    virtual_texture.texture_size.width = 0;         // Sin tamaño virtual, loop, etc
    virtual_texture.texture_size.height = 0;
    virtual_texture.loop.x = 0;
    virtual_texture.loop.y = 0;
    virtual_texture.scroll.x = 0.0f;
    virtual_texture.scroll.y = 0.0f;
    virtual_texture.offset.x = 0.0f;
    virtual_texture.offset.y = 0.0f;
    virtual_texture.enabled = false;

    // Color de tinta
    tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    last_tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    ignore_camera_tint = false;

}



/*** Posiciona la textura ***/
void NGN_Texture::Position(float position_x, float position_y) {

    // Asigna la posicion a la Textura
    position.x = position_x;
    position.y = position_y;

}
void NGN_Texture::Position(Vector2 pos) {

    // Asigna la posicion a la Textura
    Position(pos.x, pos.y);

}



/*** Mueve una textura ***/
void NGN_Texture::Translate(float speed_x, float speed_y) {

    // Calcula la nueva posicion
    position.x += speed_x;
    position.y += speed_y;

}
void NGN_Texture::Translate(Vector2 spd) {

    // Calcula la nueva posicion
    Translate(spd.x, spd.y);

}



/*** Cambia el tamaño de la textura ***/
void NGN_Texture::Size(float w, float h) {

    // Aplica el nuevo tamaño
    width = w;
    original_size.width = width;
    height = h;
    original_size.height = height;
    current_scale.width = 1.0f;
    current_scale.height = 1.0f;

}



/*** Escala una textura [Sobrecarga 1 - Ejes por separado] ***/
void NGN_Texture::Scale(float w, float h) {

    // Aplica la escala
    width = original_size.width * w;
    height = original_size.height * h;

    // Guarda la escala
    current_scale.width = w;
    current_scale.height = h;

}

/*** Escala una textura [Sobrecarga 2 - Ambos ejes a la vez] ***/
void NGN_Texture::Scale(float scale) {

    // Aplica la escala
    Scale(scale, scale);

}



/*** Devuelve la escala actual ***/
Size2 NGN_Texture::GetCurrentScale() {

    return current_scale;

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
void NGN_Texture::ClearContent(uint32_t rgba) {

    if (linked) return;

    // Calcula el color de borrado
    uint8_t r = ((rgba >> 24) & 0xFF);
    uint8_t g = ((rgba >> 16) & 0xFF);
    uint8_t b = ((rgba >> 8) & 0xFF);
    uint8_t a = (rgba & 0xFF);

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, data->gfx);
    // Borra el contenido de la textura actual
    SDL_SetRenderDrawColor(ngn->graphics->renderer, r, g, b, a);
    SDL_RenderFillRect(ngn->graphics->renderer, nullptr);

    // Devuelve el render al seleccionado
    ngn->graphics->RenderToSelected();

}



/*** Selecciona un color de tinte (sin parametros, resetea el color) ***/
void NGN_Texture::SetTintColor(uint8_t r, uint8_t g, uint8_t b) {

    // Registra el color
    tint_color.r = r;
    tint_color.g = g;
    tint_color.b = b;

}



/*** Capa en al camara 2d (de estar asignada, si no -1) ***/
int32_t NGN_Texture::GetCameraLayer() {

    return camera_layer;

}



/*** Devuelve si se ha cambiado el color de tinta en este frame ***/
bool NGN_Texture::NewTint() {

    bool color_mod = ((tint_color.r != last_tint_color.r) || (tint_color.g != last_tint_color.g) || (tint_color.b != last_tint_color.b));
    last_tint_color = tint_color;
    return color_mod;

}



/*** Crea el objeto que contiene la textura ***/
void NGN_Texture::CreateTexture(
    NGN_TextureData* texture,       // Objeto de la clase Texture Data
    int32_t position_x,             // Posicion X inicial (oculto por defecto)
    int32_t position_y,             // Posicion Y inicial (oculto por defecto)
    uint32_t texture_width,         // Ancho de la textura (por defecto, el de la textura)
    uint32_t texture_height         // Altura de la textura (por defecto, la de la textura)
) {

    // Guarda el grafico que usara este texture
    data = texture;

    // Tamaño
    if ((texture_width != NGN_DEFAULT_VALUE) && (texture_height != NGN_DEFAULT_VALUE)) {
        // Tamaño definido por el usuario
        width = texture_width;
        height = texture_height;
    } else {
        // Tamaño en base a la textura del grafico
        width = data->width;
        height = data->height;
    }
    linked = true;

    // Guarda el tamaño original al crear la textura
    original_size.width = width;
    original_size.height = height;
    current_scale.width = 1.0f;
    current_scale.height = 1.0f;

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
