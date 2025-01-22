/******************************************************************************

    N'gine Lib for C++
    *** Version 1.18.1-stable ***
    Text Layer - Capa de texto con soporte TTF

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2024 by Cesar Rincon "NightFox"

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
#include <string>
#include <iostream>

// n'gine
#include "ngn.h"

// Class
#include "ngn_text_layer.h"



/******************************************************************************

    Clase NGN_TextLayer

******************************************************************************/

/*** Constructor de la clase NGN_TextLayer (1ra sobrecarga) ***/
NGN_TextLayer::NGN_TextLayer(
    NGN_TextFont* default_font,         // Fuente por defecto
    NGN_TextureData* bg,                // Textura de fondo de la caja de texto
    int32_t position_x,                 // Posicion X (0 por defecto)
    int32_t position_y,                 // Posicion Y (0 por defecto)
    uint32_t _width,                    // Ancho de la capa (Toda la pantalla por defecto)
    uint32_t _height,                   // Alto de la capa (Toda la pantalla por defecto)
    bool _filtering                     // Filtrado
) {

    // Crea la capa de texto
    CreateTextLayer(
        default_font,   // Fuente por defecto
        bg,             // Textura de fondo
        position_x,     // Posicion X (0 por defecto)
        position_y,     // Posicion Y (0 por defecto)
        _width,         // Ancho de la capa (Toda la pantalla por defecto)
        _height,        // Alto de la capa (Toda la pantalla por defecto)
        _filtering      // Filtrado del contenido?
    );

}



/*** Constructor de la clase NGN_TextLayer (2da sobrecarga) ***/
NGN_TextLayer::NGN_TextLayer(
    std::string repo_name,              // Nombre del repositorio
    std::string resource_name,          // Nombre del recurso
    std::string bg_name,                // Nombre de la textura de fondo
    int32_t position_x,                 // Posicion X (0 por defecto)
    int32_t position_y,                 // Posicion Y (0 por defecto)
    uint32_t _width,                    // Ancho de la capa (Toda la pantalla por defecto)
    uint32_t _height,                   // Alto de la capa (Toda la pantalla por defecto)
    bool _filtering                     // Filtrado
) {

    // Decide si hay que aplicar o no una textura de fondo
    NGN_TextureData* bg = NULL;
    if (bg_name.size() > 0) bg = ngn->resources->GetTexture(repo_name, bg_name);

    // Crea la capa de texto
    CreateTextLayer(
        ngn->resources->GetTypeface(repo_name, resource_name),  // Fuente por defecto
        bg,             // Textura de fondo
        position_x,     // Posicion X (0 por defecto)
        position_y,     // Posicion Y (0 por defecto)
        _width,         // Ancho de la capa (Toda la pantalla por defecto)
        _height,        // Alto de la capa (Toda la pantalla por defecto)
        _filtering      // Filtrado del contenido?
    );

}



/*** Destructor de la clase NGN_TextLayer ***/
NGN_TextLayer::~NGN_TextLayer() {

    // Destruye el backbuffer
    SDL_DestroyTexture(backbuffer);
    backbuffer = NULL;

    // Desvincula la fuente
    font = NULL;

    // Desvincula la textura del fondo
    background = NULL;

}



/*** Borra el contenido de la capa de texto ***/
void NGN_TextLayer::Cls() {

    // Centro de la rotacion
    SDL_Point* _center = new SDL_Point();
    // Define las areas de origen y destino
    SDL_Rect source = {0, 0, 0, 0};
    SDL_Rect destination = {0, 0, 0, 0};

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);

    // Segun si hay o no fondo...
    if (background != NULL) {
        // Bugfix SDL2 en linux (previene el fallo de transparencia)
        SDL_SetRenderDrawColor(ngn->graphics->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(backbuffer, 0x00);
        SDL_RenderFillRect(ngn->graphics->renderer, NULL);
        // Borra el contenido de la textura actual
        SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(backbuffer, 0x00);
        SDL_RenderFillRect(ngn->graphics->renderer, NULL);
        // Define el origen
        source.w = background->width;
        source.h = background->height;
        // Define el destino
        destination.w = layer_width;
        destination.h = layer_height;
        // Calcula el centro de la textura
        _center->x = (destination.w / 2);
        _center->y = (destination.h / 2);
        // Renderiza el fondo en el backbuffer de la capa
        SDL_RenderCopyEx(ngn->graphics->renderer, background->gfx, &source, &destination, 0.0f, _center, SDL_FLIP_NONE);
    } else {
        // Borra el contenido de la textura actual
        SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(backbuffer, 0x00);
        SDL_RenderFillRect(ngn->graphics->renderer, NULL);
        // Rellenalo del color de fondo
        if (canvas.a > 0) {
            SDL_SetRenderDrawColor(ngn->graphics->renderer, canvas.r, canvas.g, canvas.b, canvas.a);
            SDL_RenderFillRect(ngn->graphics->renderer, NULL);
        }
    }

    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

    // Resetea el cabezal de escritura
    locate.x = locate.y = padding;

    // Resetea los limites del texto
    text_boundaries.top = -1;
    text_boundaries.bottom = -1;
    text_boundaries.left = -1;
    text_boundaries.right = -1;
    text_boundaries.width = 0;
    text_boundaries.height = 0;

    // Paso de limpieza
    delete _center;

}



/*** Devuelve el tamaño original de la capa ***/
Size2I32 NGN_TextLayer::GetSize() {

    Size2I32 s;
    s.width = layer_width;
    s.height = layer_height;
    return s;

}



/*** Posiciona el cabezal de escritura del texto ***/
void NGN_TextLayer::Locate(int32_t x, int32_t y) {

    locate.x = x;
    locate.y = y;

}



/*** Ajusta el padding de la capa de texto ***/
void NGN_TextLayer::Padding(uint32_t pd) {

    locate.x = ((locate.x == 0) || (locate.x == padding)) ? pd : locate.x;
    locate.y = ((locate.y == 0) || (locate.y == padding)) ? pd : locate.y;

    padding = pd;

}



/*** Selecciona la fuente de escritura ***/
void NGN_TextLayer::Font(NGN_TextFont* fnt) {

    font = fnt;

}



/*** Selecciona el color de la tinta ***/
void NGN_TextLayer::InkColor(uint8_t r, uint8_t g, uint8_t b) {     // R, G, B

    ink.r = r;
    ink.g = g;
    ink.b = b;

}

void NGN_TextLayer::InkColor(uint32_t rgb) {     // 0xRRGGBB

    ink.r = ((rgb & 0xFF0000) >> 16);
    ink.g = ((rgb & 0x00FF00) >> 8);
    ink.b = (rgb & 0x0000FF);

}



/*** Selecciona el color del lienzo ***/
void NGN_TextLayer::CanvasColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {   // R, G, B, A

    canvas.r = r;
    canvas.g = g;
    canvas.b = b;
    canvas.a = a;

}

void NGN_TextLayer::CanvasColor(uint32_t rgba) {    // 0xRRGGBBAA

    canvas.r = ((rgba & 0xFF000000) >> 24);
    canvas.g = ((rgba & 0x00FF0000) >> 16);
    canvas.b = ((rgba & 0x0000FF00) >> 8);
    canvas.a = (rgba & 0x000000FF);

}



/*** Escribe un texto en la capa ***/
void NGN_TextLayer::Print(std::string text) {

    // Si no hay texto o fuente definida, sal
    if ((text.size() ==  0) || (font == NULL)) return;

    // Prepara el render al backbuffer
    // Centro de la rotacion
    SDL_Point* _center = new SDL_Point();
    // Define las areas de origen y destino
    SDL_Rect source = {0, 0, 0, 0};
    SDL_Rect destination = {0, 0, 0, 0};

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);

    // Prepara el contenido de la textura actual
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(backbuffer, 0x00);

    // Lee la cadena de texto
    std::string character;
    uint8_t c = 0;
    for (uint32_t i = 0; i < text.size(); i ++) {
        // Lee un caracter y conviertelo a su codigo ASCII
        character = text.at(i);
        c = (uint8_t)character[0];
        // Salto de linea
        if (c == 0x0A) {
            locate.x = padding;
            locate.y += font->line_spacing;
            if ((locate.y > (layer_height - destination.h - padding)) && auto_home) locate.y = padding;
        } else {
            // Calculo del tamaño del texto escrito
            GetTextBoundaries(locate.x, locate.y);
            // Si el caracter es invalido, selecciona el caracter de espacio
            if (font->character[c]->gfx == NULL) c = 0x20;
            // Prepara las dimensiones de la copia de la textura
            source.w = font->character[c]->width;
            source.h = font->character[c]->height;
            destination.w = font->character[c]->width;
            destination.h = font->character[c]->height;
            // Coordenadas de las texturas
            source.x = 0; source.y = 0;
            if ((locate.x > (layer_width - destination.w - padding)) && word_wrap) {
                locate.x = padding;
                locate.y += font->line_spacing;
                if ((locate.y > (layer_height - destination.h - padding)) && auto_home) locate.y = padding;
            }
            destination.x = locate.x; destination.y = locate.y;
            // Calcula el centro de la textura
            _center->x = (destination.w / 2);
            _center->y = (destination.h / 2);
            // Aplica el color
            SDL_SetTextureColorMod(font->character[c]->gfx, ink.r, ink.g, ink.b);
            // Renderiza el caracter en el backbuffer de la capa
            SDL_RenderCopyEx(ngn->graphics->renderer, font->character[c]->gfx, &source, &destination, 0.0f, _center, SDL_FLIP_NONE);
            // Actualiza la posicion del cabezal de escritura
            locate.x += destination.w;
            // Calculo del tamaño del texto escrito
            GetTextBoundaries(locate.x, (locate.y + destination.h));
        }
    }

    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

    // Paso de limpieza
    delete _center;

}



/*** Posiciona la capa ***/
void NGN_TextLayer::Position(float position_x, float position_y) {

    // Asigna la posicion a la capa
    position.x = position_x;
    position.y = position_y;

}
void NGN_TextLayer::Position(Vector2 pos) {

    // Asigna la posicion a la capa
    Position(pos.x, pos.y);

}



/*** Mueve una capa ***/
void NGN_TextLayer::Translate(float speed_x, float speed_y) {

    // Calcula la nueva posicion
    position.x += speed_x;
    position.y += speed_y;

}
void NGN_TextLayer::Translate(Vector2 spd) {

    // Calcula la nueva posicion
    Translate(spd.x, spd.y);

}



/*** Cambia el tamaño de la capa ***/
void NGN_TextLayer::Size(float w, float h) {

    // Aplica el nuevo tamaño
    width = w;
    height = h;

}



/*** Escala una capa [Sobrecarga 1 - Ejes por separado] ***/
void NGN_TextLayer::Scale(float w, float h) {

    // Guarda la escala
    scale.width = w;
    scale.height = h;

}
/*** Escala una capa [Sobrecarga 2 - Ambos ejes a la vez] ***/
void NGN_TextLayer::Scale(float sc) {

    // Guarda la escala
    Scale(sc, sc);

}



/*** Rota una capa los grados solicitados ***/
void NGN_TextLayer::Rotate(double degrees) {

    rotation += degrees;
    while (rotation >= 360.0f) rotation -= 360.0f;
    while (rotation < 0.0f) rotation += 360.0f;

}



/*** Fija el centro de la capa (Posicion relativa desde el centro REAL) ***/
void NGN_TextLayer::SetCenter(float x, float y) {

    center.x = x;
    center.y = y;

}



/*** Limpieza de la superficie ***/
void NGN_TextLayer::SurfaceCleanUp() {

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);
    // Borra el contenido del renderer
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(ngn->graphics->renderer);
    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

}



/*** Calcula el tamaño del texto escrito en la capa ***/
void NGN_TextLayer::GetTextBoundaries(int32_t x, int32_t y) {

    // Calcula los limites
    if ((x < text_boundaries.left) || (text_boundaries.left < 0)) text_boundaries.left = x;
    if ((x > text_boundaries.right) || (text_boundaries.right < 0)) text_boundaries.right = x;
    if ((y < text_boundaries.top)  || (text_boundaries.top < 0)) text_boundaries.top = y;
    if ((y > text_boundaries.bottom) || (text_boundaries.bottom < 0)) text_boundaries.bottom = y;

    // Calcula el tamaño de texto escrito
    text_boundaries.width = (text_boundaries.right - text_boundaries.left);
    text_boundaries.height = (text_boundaries.bottom - text_boundaries.top);

}



/*** Crea el objeto que contiene la capa de texto ***/
void NGN_TextLayer::CreateTextLayer(
    NGN_TextFont* default_font,     // Fuente por defecto
    NGN_TextureData* bg,            // Textura de fondo
    int32_t position_x,             // Posicion X (0 por defecto)
    int32_t position_y,             // Posicion Y (0 por defecto)
    uint32_t _width,                // Ancho de la capa (Toda la pantalla por defecto)
    uint32_t _height,               // Alto de la capa (Toda la pantalla por defecto)
    bool _filtering                 // Filtrado del contenido?
) {

    // Fuente por defecto
    font = default_font;

    // Si existe, textura de fondo
    background = bg;

    // Guarda el tamaño
    if ((_width != NGN_DEFAULT_VALUE) && (_height != NGN_DEFAULT_VALUE)) {
        width = _width;
        height = _height;
    } else if (bg != NULL) {
        width = bg->width;
        height = bg->height;
    } else {
        width = ngn->graphics->native_w;
        height = ngn->graphics->native_h;
    }

    // Y la posicion
    position.x = position_x;
    position.y = position_y;

    // Guarda el tamaño original en pixeles
    layer_width = (uint32_t)width;
    layer_height = (uint32_t)height;

    // Crea el backbuffer del tamaño adecuado
    #if !defined (DISABLE_BACKBUFFER)
        if (_filtering) {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        }
    #endif
    backbuffer = NULL;
    backbuffer = SDL_CreateTexture(
                         ngn->graphics->renderer,       // Renderer
                         SDL_PIXELFORMAT_BGRA8888,      // Formato del pixel
                         SDL_TEXTUREACCESS_TARGET,      // Textura como destino del renderer
                         layer_width,                   // Ancho de la textura
                         layer_height                   // Alto de la textura
                         );
    #if !defined (DISABLE_BACKBUFFER)
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    #endif

    // Propiedades adicionales
    visible = true;             // Visibilidad
    alpha = 0xFF;               // Alpha
    blend_mode = NGN_BLENDMODE_ALPHA;   // Modo de mezcla
    rotation = 0.0f;            // Rotacion
    center.x = 0.0f;            // Centro de rotacion
    center.y = 0.0f;
    flip_h = false;             // Flip
    flip_v = false;
    scale.width = 1.0f;         // Escala
    scale.height = 1.0f;

    padding = 0;                // Margen interior de la caja de texto
    word_wrap = true;           // Autoajuste horizontal
    auto_home = false;          // Autoajuste vertical

    locate.x = 0;               // Posicion del cabezal de escritura
    locate.y = 0;

    text_boundaries.top = 0;
    text_boundaries.bottom = 0;
    text_boundaries.left = 0;
    text_boundaries.right = 0;

    ink.b = 0xFF;               // Color por defecto (Tinta)
    ink.g = 0xFF;
    ink.r = 0xFF;

    canvas.a = 0x00;            // Color por defecto (Lienzo)
    canvas.b = 0x00;
    canvas.g = 0x00;
    canvas.r = 0x00;

    // Prepara la capa
    SurfaceCleanUp();

    // Borra el contenido para actualizar su fondo, padding etc
    Cls();

}
