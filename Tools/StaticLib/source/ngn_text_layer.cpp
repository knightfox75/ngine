/******************************************************************************

    N'gine Lib for C++
    *** Version 0.4.0-alpha ***
    Text Layer - Capa de texto con soporte TTF

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
#include <string>
#include <iostream>

// n'gine
#include "ngn.h"

// Class
#include "ngn_text_layer.h"



/******************************************************************************

    Clase NGN_TextLayer

******************************************************************************/

/*** Constructor de la clase NGN_TextLayer ***/
NGN_TextLayer::NGN_TextLayer(
    NGN_TextFont* default_font,         // Fuente por defecto
    NGN_TextureData* bg,                // Textura de fondo de la caja de texto
    float position_x,                   // Posicion X (0 por defecto)
    float position_y,                   // Posicion Y (0 por defecto)
    int32_t _width,                     // Ancho de la capa (Toda la pantalla por defecto)
    int32_t _height                     // Alto de la capa (Toda la pantalla por defecto)
) {

    // Fuente por defecto
    font = default_font;

    // Si existe, textura de fondo
    background = bg;

    // Guarda el tamaño
    if ((_width != DEFAULT_VALUE) && (_height != DEFAULT_VALUE)) {
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

    // Crea el backbuffer del tamaño adecuado
    backbuffer = NULL;
    backbuffer = SDL_CreateTexture(
                         ngn->graphics->renderer,       // Renderer
                         SDL_PIXELFORMAT_BGRA8888,      // Formato del pixel
                         SDL_TEXTUREACCESS_TARGET,      // Textura como destino del renderer
                         width,                         // Ancho de la textura
                         height                         // Alto de la textura
                         );

    // Propiedades adicionales
    visible = true;             // Visibilidad
    alpha = 0xFF;               // Alpha
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

    ink.b = 0xFF;               // Color por defecto (Tinta)
    ink.g = 0xFF;
    ink.r = 0xFF;

    canvas.a = 0x00;            // Color por defecto (Lienzo)
    canvas.b = 0x00;
    canvas.g = 0x00;
    canvas.r = 0x00;

    // Prepara la capa
    SurfaceCleanUp();

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
        destination.w = width;
        destination.h = height;
        // Calcula el centro de la textura
        _center->x = (destination.w / 2);
        _center->y = (destination.h / 2);
        // Renderiza el fondo en el backbuffer de la capa
        SDL_RenderCopyEx(ngn->graphics->renderer, background->gfx, &source, &destination, 0.0f, _center, SDL_FLIP_NONE);
    } else if (canvas.a > 0) {
        // Borra el contenido de la textura actual
        SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(backbuffer, 0x00);
        SDL_RenderFillRect(ngn->graphics->renderer, NULL);
        // Rellenalo del color de fondo
        SDL_SetRenderDrawColor(ngn->graphics->renderer, canvas.r, canvas.g, canvas.b, canvas.a);
        SDL_RenderFillRect(ngn->graphics->renderer, NULL);
    }

    // Cambia el destino del renderer a la pantalla
    SDL_SetRenderTarget(ngn->graphics->renderer, NULL);
    // Restaura el color y alpha del renderer
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0xFF);

    // Resetea el cabezal de escritura
    locate.x = locate.y = padding;

    // Paso de limpieza
    delete _center;

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
    if ((text == "") || (font == NULL)) return;

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
    unsigned char c = 0;
    for (char& t : text) {
        // Conversion de char a unsigned char
        c = ((t < 0) ? (0xFF - ~t) : t) & 0xFF;
        // Salto de linea
        if (c == '\n') {
            locate.x = padding;
            locate.y += font->line_spacing;
            if ((locate.y > (height - destination.h - padding)) && auto_home) locate.y = padding;
        } else {
            // Si el caracter es invalido, selecciona el caracter de espacio
            if (font->character[c]->gfx == NULL) c = 0x20;
            // Prepara las dimensiones de la copia de la textura
            source.w = font->character[c]->width;
            source.h = font->character[c]->height;
            destination.w = font->character[c]->width;
            destination.h = font->character[c]->height;
            // Coordenadas de las texturas
            source.x = 0; source.y = 0;
            if ((locate.x > (width - destination.w - padding)) && word_wrap) {
                locate.x = padding;
                locate.y += font->line_spacing;
                if ((locate.y > (height - destination.h - padding)) && auto_home) locate.y = padding;
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
        }
    }

    // Cambia el destino del renderer a la pantalla
    SDL_SetRenderTarget(ngn->graphics->renderer, NULL);
    // Restaura el color y alpha del renderer
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0xFF);

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
    position.x = pos.x;
    position.y = pos.y;

}



/*** Mueve una capa ***/
void NGN_TextLayer::Translate(float speed_x, float speed_y) {

    // Calcula la nueva posicion
    position.x += speed_x;
    position.y += speed_y;

}
void NGN_TextLayer::Translate(Vector2 spd) {

    // Calcula la nueva posicion
    position.x += spd.x;
    position.y += spd.y;

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
    scale.width = sc;
    scale.height = sc;

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
    // Cambia el destino del renderer a la pantalla
    SDL_SetRenderTarget(ngn->graphics->renderer, NULL);
    // Restaura el color y alpha del renderer
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0xFF);

}
