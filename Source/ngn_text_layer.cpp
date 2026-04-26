/******************************************************************************

    N'gine Lib for C++
    *** Version 1.22.0-wip_0x04 ***
    Text Layer - Capa de texto con soporte TTF

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
    NGN_TextureData* bg = nullptr;
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
    backbuffer = nullptr;

    // Desvincula la fuente
    font = nullptr;

    // Desvincula la textura del fondo
    background = nullptr;

}



/*** Borra el contenido de la capa de texto ***/
void NGN_TextLayer::Cls() {

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);

    // Borra el contenido previo
    SDL_SetRenderDrawBlendMode(ngn->graphics->renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(ngn->graphics->renderer);

    // Segun si hay o no fondo...
    if (background != nullptr) {
        // Configura el modo de dibujado
        SDL_SetTextureBlendMode(background->gfx, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(background->gfx, 0xFF);
        // Define el origen y destino de la textura de fondo
        SDL_Rect source_rect = { 0, 0, background->width, background->height };
        SDL_Rect dest_rect = { 0, 0, layer_width, layer_height };
        // Renderiza el fondo en el backbuffer de la capa
        SDL_RenderCopy(ngn->graphics->renderer, background->gfx, &source_rect, &dest_rect);
    } else {
        // Rellenalo del color de fondo si es necesario
        if (canvas.a > 0) {
            SDL_SetRenderDrawBlendMode(ngn->graphics->renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(ngn->graphics->renderer, canvas.r, canvas.g, canvas.b, canvas.a);
            SDL_RenderFillRect(ngn->graphics->renderer, nullptr);
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

    // Resetea el blitting y el vector de vertices
    blit_text = false;
    vertex.clear();

}



/*** Devuelve el tamaño original de la capa ***/
Size2I32 NGN_TextLayer::GetSize() {

    Size2I32 s;
    s.width = layer_width;
    s.height = layer_height;
    return s;

}



/*** Selecciona un color de tinte (sin parametros, resetea el color) ***/
void NGN_TextLayer::SetTintColor(uint8_t r, uint8_t g, uint8_t b) {

    // Registra el color
    tint_color.r = r;
    tint_color.g = g;
    tint_color.b = b;

}



/*** Devuelve si se ha cambiado el color de tinta en este frame ***/
bool NGN_TextLayer::NewTint() {

    bool color_mod = ((tint_color.r != last_tint_color.r) || (tint_color.g != last_tint_color.g) || (tint_color.b != last_tint_color.b));
    last_tint_color = tint_color;
    return color_mod;

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

    // Si es la misma sal
    if (fnt == font) return;

    // De no serlo, vuelva el contenido actual del buffer de vertices al backbuffer
    BlitText();

    // Y asigna la nueva fuente
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

    // Si no hay texto, fuente o atlas definido, sal
    if ((text.size() == 0) || !font || !font->characters_atlas) return;

    // Color de tinta actual embebido en los vertices
    SDL_Color ink_color = {ink.r, ink.g, ink.b, 0xFF};

	// Codigo de caracter a imprimir
	uint8_t c = 0;
	
	// Lee la cadena de texto y construye los quads
    for (uint32_t i = 0; i < text.size(); i ++) {

        // Lee el codigo ASCII del caracter actual
        c = (uint8_t)text.at(i);

        // Salto de linea
        if (c == CHAR_CODE.new_line) {            // Salto de linea
            locate.x = padding;
            locate.y += font->line_spacing;
            if ((locate.y > (layer_height - font->cell_size.height - padding)) && auto_home) locate.y = padding;
            continue;
        } else if (c == CHAR_CODE.color) {        // Cambia el color de tinta
            if ((i + 8) < text.size()) {
                ink_color = HexStringToColor(text.substr((i + 1), 8));
                i += 8;
            }
            continue;
        }

        // Si el caracter esta fuera de rango, ignoralo
        if (c < CHAR_CODE.space) continue;

        // Si el caracter no tiene un ancho valido, imprime un espacio (0x20)
        if (font->char_size[c].width == 0) c = CHAR_CODE.space;

        // Dimensiones del caracter actual
        float glyph_w = (float)font->char_size[c].width;
        float glyph_h = (float)font->char_size[c].height;
        // Distancia a avanzar el cursor
        float glyph_advance = (float)font->char_size[c].width;

        // Word wrap horizontal
        if ((locate.x > (layer_width - (int32_t)glyph_w - padding)) && word_wrap) {
            locate.x = padding;
            locate.y += font->line_spacing;
            if ((locate.y > (layer_height - (int32_t)glyph_h - padding)) && auto_home) locate.y = padding;
        }

        // Coordenadas de destino en el backbuffer
        float dx = (float)locate.x;
        float dy = (float)locate.y;

        // Avanza el cabezal de escritura
        locate.x += (int32_t)glyph_advance;

        // Calculo del tamaño del texto escrito (usa las coordenadas y tamaño del caracter escrito)
        GetTextBoundaries((int32_t)dx, (int32_t)dy, (int32_t)glyph_w, (int32_t)glyph_h);

        // Si es un espacio, no es necesario añadirlo al buffer de vectores
        if (c == CHAR_CODE.space) continue; 

        // Calculo de la posicion del caracter en el atlas via bitwise (grid 16x16)
        // col = 4 bits bajos del codigo ASCII
        // row = 4 bits altos del codigo ASCII
        float src_x = (float)((c & 0x0F) * font->cell_size.width);
        float src_y = (float)(((c >> 4) & 0x0F) * font->cell_size.height);

        // UVs normalizadas (0.0 a 1.0) usando division directa para evitar pérdida de precision IEEE 754
        float u1 = (src_x / (float)font->atlas_size.width);
        float v1 = (src_y / (float)font->atlas_size.height);
        float u2 = ((src_x + glyph_w) / (float)font->atlas_size.width);
        float v2 = ((src_y + glyph_h) / (float)font->atlas_size.height);

        // Genera los 6 vertices del quad (2 triangulos)
        // Triangulo 1
        vertex.push_back({{dx, dy}, ink_color, {u1, v1}});
        vertex.push_back({{(dx + glyph_w), dy}, ink_color, {u2, v1}});
        vertex.push_back({{dx, (dy + glyph_h)}, ink_color, {u1, v2}});
        // Triangulo 2
        vertex.push_back({{(dx + glyph_w), dy}, ink_color, {u2, v1}});
        vertex.push_back({{(dx + glyph_w), (dy + glyph_h)}, ink_color, {u2, v2}});
        vertex.push_back({{dx, (dy + glyph_h)}, ink_color, {u1, v2}});

    }

    // Indica que hay contenido a actualizar
    blit_text = true;

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



/*** Devuelve la escala actual del fondo ***/
Size2 NGN_TextLayer::GetCurrentScale() {

    return scale;

}



/*** Rota una capa los grados solicitados ***/
void NGN_TextLayer::Rotate(double degrees) {

    rotation = std::fmod((rotation + degrees), 360.0);
    if (rotation < 0.0) rotation += 360.0;

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
void NGN_TextLayer::GetTextBoundaries(int32_t x, int32_t y, int32_t w, int32_t h) {

    // Calcula los limites
    if ((x < text_boundaries.left) || (text_boundaries.left < 0)) text_boundaries.left = x;
    if (((x + w) > text_boundaries.right) || (text_boundaries.right < 0)) text_boundaries.right = (x + w);
    if ((y < text_boundaries.top)  || (text_boundaries.top < 0)) text_boundaries.top = y;
    if (((y + h) > text_boundaries.bottom) || (text_boundaries.bottom < 0)) text_boundaries.bottom = (y + h);

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
    } else if (bg != nullptr) {
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
    backbuffer = nullptr;
    backbuffer = SDL_CreateTexture(
        ngn->graphics->renderer,        // Renderer
        NGN_PIXEL_FORMAT,               // Formato del pixel
        SDL_TEXTUREACCESS_TARGET,       // Textura como destino del renderer
        layer_width,                    // Ancho de la textura
        layer_height                    // Alto de la textura
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

    // Color de tinta
    tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    last_tint_color = {0xFF, 0xFF, 0xFF, 0xFF};

    // Blitting
    vertex.clear();
    vertex.reserve(MIN_VERTEX_BUFFER);
    blit_text = false;

    // Prepara la capa
    SurfaceCleanUp();

    // Borra el contenido para actualizar su fondo, padding etc
    Cls();

}



/*** Blittin del contenido al backbuffer ***/
void NGN_TextLayer::BlitText() {

    // Si no hay contenido a volcar, sal
    if (!blit_text) return;

    // Registra que la funcion se ha llamado
    blit_text = false;

    // Si no hay vertices que dibujar, sal
    if (vertex.size() == 0) return;

    // Informa al renderer que el backbuffer es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);
    SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(font->characters_atlas, SDL_BLENDMODE_BLEND);

    // Render de todos los caracteres del string en una unica llamada
    SDL_RenderGeometry(
        ngn->graphics->renderer,
        font->characters_atlas,
        vertex.data(),
        (int32_t)vertex.size(),
        nullptr,
        0
    );

    // Restaura el renderer al destino seleccionado
    ngn->graphics->RenderToSelected();

    // Vacia el array de vertices
    vertex.clear();

}



/*** Convierte una cadena Hexadecimal con el formato 0x000000 a un uint32_t y aplicalo como color de tinta ***/
SDL_Color NGN_TextLayer::HexStringToColor(const std::string& hex_str) {

    SDL_Color ink_color = {ink.r, ink.g, ink.b, 0xFF};

    if (hex_str.size() == 0) return ink_color;

    // Puntero a los caracteres
    char* end_ptr = nullptr;

    // El "16" le indica que es base hexadecimal y acepta el prefijo "0x" o "0X"
    uint64_t color = std::strtoul(hex_str.c_str(), &end_ptr, 16);

    // Si el puntero final es igual al inicial, conversion fallida
    if (end_ptr == hex_str.c_str()) return ink_color;

    // Si el color es valido, aplicalo
    InkColor((uint32_t)color);

    // Calcula los valores de la tinta y devuelvelos
    ink_color = {ink.r, ink.g, ink.b, 0xFF};
    return ink_color;

}
