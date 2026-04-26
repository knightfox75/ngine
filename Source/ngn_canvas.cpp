/******************************************************************************

    N'gine Lib for C++
    *** Version 1.22.0-wip_0x04 ***
    Canvas - Capa de dibujo

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
#include <cmath>
#include <algorithm>

// n'gine
#include "ngn.h"

// Class
#include "ngn_canvas.h"



/******************************************************************************

    Clase NGN_Canvas

******************************************************************************/

/*** Constructor de la clase NGN_Canvas ***/
NGN_Canvas::NGN_Canvas(
    int32_t position_x,         // Posicion X (0 por defecto)
    int32_t position_y,         // Posicion Y (0 por defecto)
    uint32_t _width,            // Ancho de la capa (Toda la pantalla por defecto)
    uint32_t _height,           // Alto de la capa (Toda la pantalla por defecto)
    bool _filtering             // Filtrado bilinear del contenido?
) {

    // Guarda el tamaño
    if ((_width != NGN_DEFAULT_VALUE) && (_height != NGN_DEFAULT_VALUE)) {
        width = _width;
        height = _height;
    } else {
        width = ngn->graphics->native_w;
        height = ngn->graphics->native_h;
    }

    // Y la posicion
    position.x = position_x;
    position.y = position_y;

    // Guarda el tamaño original en pixeles
    surface_width = (uint32_t)width;
    surface_height = (uint32_t)height;

    // Crea la superficie en base al buffer de pixeles
    surface = nullptr;
    surface = SDL_CreateRGBSurface(
        0,                      // Flag
        surface_width,          // Ancho
        surface_height,         // Alto
        32,                     // Profuncidad de color 32bpp [RGBA8888]
        0xFF000000,             // Mascara R
        0x00FF0000,             // Mascara G
        0x0000FF00,             // Mascara B
        0x000000FF              // Mascara A
    );


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
        SDL_TEXTUREACCESS_STREAMING,    // Textura de acceso streaming (update sin realloc)
        surface_width,                  // Ancho de la textura
        surface_height                  // Alto de la textura
    );
    #if !defined (DISABLE_BACKBUFFER)
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    #endif

    // Propiedades adicionales
    visible = true;             // Visibilidad
    alpha = 0xFF;               // Alpha
    blend_mode = NGN_BLENDMODE_ALPHA;   // Modo de mezcla
    filtering = _filtering;     // Filtrado?
    rotation = 0.0f;            // Rotacion
    center.x = 0.0f;            // Centro de rotacion
    center.y = 0.0f;
    flip_h = false;             // Flip
    flip_v = false;
    scale.width = 1.0f;         // Escala
    scale.height = 1.0f;

    // Color de tinta
    tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    last_tint_color = {0xFF, 0xFF, 0xFF, 0xFF};

    // Borra el contenido de la textura del canvas (basura RAM)
    BackbufferCleanUp();

    // Control del buffer de pixeles
    blit = false;

}



/*** Destructor de la clase NGN_Canvas ***/
NGN_Canvas::~NGN_Canvas() {

    // Destruye el backbuffer
    SDL_DestroyTexture(backbuffer);
    backbuffer = nullptr;

    // Destruye el surface
    SDL_FreeSurface(surface);
    surface = nullptr;

}



/*** Posiciona el canvas ***/
void NGN_Canvas::Position(float position_x, float position_y) {

    // Asigna la posicion al canvas
    position.x = position_x;
    position.y = position_y;

}
void NGN_Canvas::Position(Vector2 pos) {

    // Asigna la posicion al canvas
    Position(pos.x, pos.y);

}



/*** Mueve el canvas ***/
void NGN_Canvas::Translate(float speed_x, float speed_y) {

    // Calcula la nueva posicion
    position.x += speed_x;
    position.y += speed_y;

}
void NGN_Canvas::Translate(Vector2 spd) {

    // Calcula la nueva posicion
    Translate(spd.x, spd.y);

}



/*** Cambia el tamaño del canvas ***/
void NGN_Canvas::Size(float w, float h) {

    // Aplica el nuevo tamaño
    width = w;
    height = h;
    blit = true;

}



/*** Escala el canvas [Sobrecarga 1 - Ejes por separado] ***/
void NGN_Canvas::Scale(float w, float h) {

    // Guarda la escala
    scale.width = w;
    scale.height = h;
    blit = true;

}

/*** Escala el canvas [Sobrecarga 2 - Ambos ejes a la vez] ***/
void NGN_Canvas::Scale(float sc) {

    // Guarda la escala
    Scale(sc, sc);

}



/*** Devuelve la escala actual del fondo ***/
Size2 NGN_Canvas::GetCurrentScale() {

    return scale;

}



/*** Rota el canvas los grados solicitados ***/
void NGN_Canvas::Rotate(double degrees) {

    rotation = std::fmod((rotation + degrees), 360.0);
    if (rotation < 0.0) rotation += 360.0;

}



/*** Fija el centro del canvas (Posicion relativa desde el centro REAL) ***/
void NGN_Canvas::SetCenter(float x, float y) {

    center.x = x;
    center.y = y;

}



/*
    Funciones de dibujado en el canvas
*/



/*** Borra el contenido del canvas ***/
void NGN_Canvas::Cls(uint32_t color) {

    SDL_FillRect(surface, nullptr, color);

    // Marca el blit
    blit = true;

}



/*** Devuelve el tamaño original del lienzo ***/
Size2I32 NGN_Canvas::GetSize() {

    Size2I32 s;
    s.width = surface_width;
    s.height = surface_height;
    return s;

}



/*** Dibuja un punto ***/
void NGN_Canvas::Point(int32_t x, int32_t y, uint32_t color) {

    // Proteccion de coordenadas
    if ((x < 0) || (x >= surface_width) || (y < 0) || (y >= surface_height)) return;

    // Datos de acceso al buffer
    uint32_t* p = (uint32_t*)surface->pixels;
    uint32_t addr = ((y * surface_width) + x);

    uint8_t alpha = (color & 0xFF);
    if (alpha == 0xFF) {
        p[addr] = color;
    } else if (alpha > 0x00) {
        p[addr] = BlendPixel(p[addr], color);
    }

    // Indica el blit
    blit = true;

}



/*** Dibuja una linea entre dos puntos (Implementacion del algoritmo de Bresenham) ***/
void NGN_Canvas::Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color) {

    // Parametros iniciales
    int32_t right = (surface_width - 1);        // Limites del buffer
    int32_t bottom = (surface_height - 1);
    int32_t x = x1;                             // Coordenadas de dibujado
    int32_t y = y1;
    int32_t dx = (x2 - x1);                     // Distancias
    int32_t dy = (y2 - y1);
    int32_t ix = (dx >= 0) ? 1:-1;              // Sentido del dibujado
    int32_t iy = (dy >= 0) ? 1:-1;
    int32_t pk = 0;                             // Precision de la pendiente
    uint8_t alpha = (color & 0xFF); 
    bool blend = ((alpha > 0x00) && (alpha < 0xFF)); 

    // Valor absoluto de las distancias
    dx = std::abs(dx);
    dy = std::abs(dy);

    // Acceso al array de pixeles
    uint32_t* p = (uint32_t*)surface->pixels;

    // Dibuja el primer pixel
    if (!((x < 0) || (y < 0) || (x > right) || (y > bottom))) {
        uint32_t addr = ((y * surface_width) + x);
        if (blend) {
            p[addr] = BlendPixel(p[addr], color);
        } else {
            p[addr] = color;
        }
    }

    // Segun la pendiente (mandan la X o mandan las Y)
    if (dx >= dy) {

        // Calculo de la precision
        pk = ((dy << 1) - dx);
        // Bucle de dibujado
        for (int32_t i = 0; i < dx; i ++) {
            // Incremento de la X
            x += ix;
            // Segun el estado de la precision, recalculala
            if (pk < 0) {
                pk += (dy << 1);
            } else {
                y += iy;
                pk += ((dy << 1) - (dx << 1));
            }
            // Dibuja el pixel
            if (!((x < 0) || (y < 0) || (x > right) || (y > bottom))) {
                uint32_t addr = ((y * surface_width) + x);
                if (blend) {
                    p[addr] = BlendPixel(p[addr], color);
                } else {
                    p[addr] = color;
                }
            }
        }

    } else {

        // Calculo de la precision
        pk = ((dx << 1) - dy);
        // Bucle de dibujado
        for (int32_t i = 0; i < dy; i ++) {
            // Incremento de la Y
            y += iy;
            // Segun el estado de la precision, recalculala
            if (pk < 0) {
                pk += (dx << 1);
            } else {
                x += ix;
                pk += ((dx << 1) - (dy << 1));
            }
            // Dibuja el pixel
            if (!((x < 0) || (y < 0) || (x > right) || (y > bottom))) {
                uint32_t addr = ((y * surface_width) + x);
                if (blend) {
                    p[addr] = BlendPixel(p[addr], color);
                } else {
                    p[addr] = color;
                }
            }
        }

    }

    // Indica el blit
    blit = true;

}



/*** Dibuja un cuadrado ***/
void NGN_Canvas::Box(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, bool paint) {

    int32_t xa = 0, ya = 0, xb = 0, yb = 0;
    if (x1 <= x2) {
        xa = x1; xb = x2;
    } else {
        xa = x2; xb = x1;
    }
    if (y1 <= y2) {
        ya = y1; yb = y2;
    } else {
        ya = y2; yb = y1;
    }

    if (paint) {
        uint32_t* p = (uint32_t*)surface->pixels;
        uint8_t alpha = (color & 0xFF); 
        bool blend = ((alpha > 0x00) && (alpha < 0xFF));
        for (int32_t y = ya; y <= yb; y++) {
            FillScanline(p, y, xa, xb, color, blend);
        }
    } else {
        Line(xa, ya, xb, ya, color);
        Line(xa, yb, xb, yb, color);
        Line(xa, (ya + 1), xa, (yb - 1), color);
        Line(xb, (ya + 1), xb, (yb - 1), color);
    }

    blit = true;

}



/*** Dibuja un circulo ***/
void NGN_Canvas::Circle(int32_t cx, int32_t cy, int32_t r, uint32_t color, int32_t ry, bool paint) {

    if (paint) {
        // Con relleno
        BresenhamFilledCircle(cx, cy, r, color, ry);
    } else {
        // Circulo sin relleno
        BresenhamCircle(cx, cy, r, color, ry);
    }

}



/*** Dibuja un arco ***/
void NGN_Canvas::Arc(int32_t cx, int32_t cy, int32_t r, double start_angle, double end_angle, uint32_t color, int32_t ry, uint8_t close) {

    // Asignacion de los radios
    int32_t _rx = r;
    int32_t _ry = (ry == NGN_DEFAULT_VALUE) ? _rx : ry;

    // Ordena los angulos de inicio y fin
    if (start_angle == end_angle) return;
    if ((_rx == 0) && (_ry == 0)) return;

    double st_angle = 0.0, ed_angle = 0.0;
    if (start_angle > end_angle) {
        st_angle = start_angle;
        ed_angle = end_angle + (PI * 2.0);
    } else {
        st_angle = start_angle;
        ed_angle = end_angle;
    }

    // Precision angular: un paso por pixel aproximado segun el radio mayor
    double p = ((PI * 4.0) / (double)((_rx >= _ry) ? _rx : _ry));

    // --- Optimizacion Arc: recurrence relation (DDA angular) ---
    // En lugar de llamar cos/sin en cada iteracion del bucle,
    // se precalculan los incrementos angulares una sola vez
    // y se actualiza la posicion con rotacion incremental:
    //   x' = x*cos(p) - y*sin(p)
    //   y' = x*sin(p) + y*cos(p)
    double cos_p = std::cos(p);
    double sin_p = std::sin(p);

    // Punto de inicio sobre la elipse
    double ex = (std::cos(st_angle) * (double)_rx);
    double ey = (std::sin(st_angle) * (double)_ry);

    int32_t _fx = ((int32_t)std::round(ex) + cx);
    int32_t _fy = ((int32_t)std::round(ey) + cy);
    int32_t _x = _fx, _y = _fy;
    int32_t x = 0, y = 0;

    // El angulo cubierto se controla por el numero de pasos,
    // evitando acumulacion de error en la comparacion de doubles.
    int32_t steps = (int32_t)std::ceil(((ed_angle - st_angle) / p));

    for (int32_t i = 0; i < steps; i ++) {
        // Rotacion incremental sobre el espacio de la elipse
        double nx = (ex * cos_p) - (((ey / (double)_ry) * (double)_rx) * sin_p);
        double ny = (((ex / (double)_rx) * (double)_ry) * sin_p) + (ey * cos_p);
        ex = nx;
        ey = ny;
        x = ((int32_t)std::round(ex) + cx);
        y = ((int32_t)std::round(ey) + cy);
        if ((_x != x) || (_y != y)) Line(x, y, _x, _y, color);
        _x = x;
        _y = y;
    }

    // Ultimo punto exacto del arco (evita gap por redondeo del step count)
    x = ((int32_t)std::round(std::cos(ed_angle) * (double)_rx) + cx);
    y = ((int32_t)std::round(std::sin(ed_angle) * (double)_ry) + cy);
    if ((_x != x) || (_y != y)) Line(x, y, _x, _y, color);

    // Has de cerrar el arco? (0 = no, 1 = entre los puntos, 2 = con el centro)
    switch (close) {
        case 1:
            Line(x, y, _fx, _fy, color);
            break;
        case 2:
            Line(x, y, cx, cy, color);
            Line(_fx, _fy, cx, cy, color);
            break;
    }

}



/*** Obten el color del pixel de las coordenadas proporcionadas (Formato 0xRRGGBBAA) ***/
uint32_t NGN_Canvas::GetPixelColor(int32_t x, int32_t y) {

    // Desborde de buffer
    if ((x < 0) || (y < 0) || (x >= surface_width) || (y >= surface_height)) return 0x00000000;

    // Acceso al puntero del surface
    uint32_t* p = (uint32_t*)surface->pixels;
    uint32_t i = (y * surface_width) + x;

    // Devuelve el pixel
    return p[i];

}



/*** Obten el color del pixel de las coordenadas proporcionadas (Formato RGBA) ***/
Rgba NGN_Canvas::GetPixelRgba(int32_t x, int32_t y) {

    // Pixel a devolver
    Rgba pixel = {0, 0, 0, 0};

    // Desborde de buffer
    if ((x < 0) || (y < 0) || (x >= surface_width) || (y >= surface_height)) return pixel;

    // Acceso al puntero del surface
    uint32_t* p = (uint32_t*)surface->pixels;
    uint32_t i = (y * surface_width) + x;

    // Devuelve el pixel
    pixel = GetRgbaColor(p[i]);
    return pixel;

}



/*** Actualiza la textura con los datos del buffer ***/
void NGN_Canvas::Blit() {

    // Si no es necesario, ignora el comando
    if (!blit) return;

    // Actualiza el contenido de la textura
    SDL_UpdateTexture(backbuffer, nullptr, surface->pixels, surface->pitch);

    // Marca el comando como completado
    blit = false;

}



/*** Selecciona un color de tinte (sin parametros, resetea el color) ***/
void NGN_Canvas::SetTintColor(uint8_t r, uint8_t g, uint8_t b) {

    // Registra el color
    tint_color.r = r;
    tint_color.g = g;
    tint_color.b = b;

}



/*** Devuelve si se ha cambiado el color de tinta en este frame ***/
bool NGN_Canvas::NewTint() {

    bool color_mod = ((tint_color.r != last_tint_color.r) || (tint_color.g != last_tint_color.g) || (tint_color.b != last_tint_color.b));
    last_tint_color = tint_color;
    return color_mod;

}



/*** Limpieza de la superficie del backbuffer***/
void NGN_Canvas::BackbufferCleanUp() {

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);
    // Borra el contenido del renderer
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(ngn->graphics->renderer);
    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

}



/*** Dibuja un circulo (Implementacion del algoritmo de Bresenham) ***/
void NGN_Canvas::BresenhamCircle(int32_t cx, int32_t cy, int32_t r, uint32_t color, int32_t ry) {

    // Asigna el radio. Proteccion contra 0
    int32_t _rx = r;
    int32_t _ry = (ry == (int32_t)NGN_DEFAULT_VALUE) ? _rx : ry;

    // Si alguno de los radios es 0...
    if ((_rx == 0) && (_ry == 0)) {
        return;
    } else if (_ry == 0) {
        Line((cx - _rx), cy, (cx + _rx), cy, color);
        return;
    } else if (_rx == 0) {
        Line(cx, (cy - _ry), cx, (cy + _ry), color);
        return;
    }

    // Calculos previos
    int32_t right = (surface_width - 1);                // Limites del buffer
    int32_t bottom = (surface_height - 1);
    int32_t x = _rx;                                    // Coordenadas
    int32_t y = 0;

    // Usamos int64_t para evitar desbordes en los calculos con radios grandes (>512)
    int64_t ix = ((int64_t)_ry * _ry) * (1 - ((int64_t)_rx << 1));  // Cambios de coordenadas
    int64_t iy = ((int64_t)_rx * _rx);
    int64_t square_x = (((int64_t)_rx * _rx) << 1);                  // Cuadrados
    int64_t square_y = (((int64_t)_ry * _ry) << 1);
    int64_t stop_x = (square_y * _rx);                               // Puntos de detencion
    int64_t stop_y = 0;
    int64_t ellip_error = 0;                                         // Error en el dibujado de elipses
    uint8_t alpha = (color & 0xFF);                                  // Hay que usar alpha?

    // Variables adicionales
    int32_t _px = 0, _py = 0;

    // Acceso al array de pixeles
    uint32_t* p = (uint32_t*)surface->pixels;

    // Prime area de dibujado (lados izquierdo y derecho)
    while (stop_x >= stop_y) {
        
        // --- Dibujado por simetria de 4 vias, con proteccion de overlap ---
        
        // Cuadrante 1 (+, +) - Se dibuja siempre
        _px = cx + x; _py = cy + y;
        if (!((_px < 0) || (_py < 0) || (_px > right) || (_py > bottom))) {
            uint32_t addr = ((_py * surface_width) + _px);
            p[addr] = (alpha == 0xFF) ? color : BlendPixel(p[addr], color);
        }
        
        // Cuadrante 2 (-, +) - Solo si x > 0 para evitar el overlap en este axis
        if (x > 0) {
            _px = cx - x; _py = cy + y;
            if (!((_px < 0) || (_py < 0) || (_px > right) || (_py > bottom))) {
                uint32_t addr = ((_py * surface_width) + _px);
                p[addr] = (alpha == 0xFF) ? color : BlendPixel(p[addr], color);
            }
        }
        
        // Reflejo vertical solo si y > 0
        if (y > 0) {
            // Cuadrante 3 (-, -) - Solo si x > 0
            if (x > 0) {
                _px = cx - x; _py = cy - y;
                if (!((_px < 0) || (_py < 0) || (_px > right) || (_py > bottom))) {
                    uint32_t addr = ((_py * surface_width) + _px);
                    p[addr] = (alpha == 0xFF) ? color : BlendPixel(p[addr], color);
                }
            }
            // Cuadrante 4 (+, -)
            _px = cx + x; _py = cy - y;
            if (!((_px < 0) || (_py < 0) || (_px > right) || (_py > bottom))) {
                uint32_t addr = ((_py * surface_width) + _px);
                p[addr] = (alpha == 0xFF) ? color : BlendPixel(p[addr], color);
            }
        }

        // Incrementa la Y
        y++;
        stop_y += square_x;
        ellip_error += iy;
        iy += square_x;

        // Correccion de valores
        if (((ellip_error << 1) + ix) > 0) {
            x--;
            stop_x -= square_y;
            ellip_error += ix;
            ix += square_y;
        }
    }

    // Prepara la siguiente area de dibujado (arriba y abajo)
    x = 0;
    y = _ry;
    ix = ((int64_t)_ry * _ry);
    iy = ((int64_t)_rx * _rx) * (1 - ((int64_t)_ry << 1));
    ellip_error = 0;
    stop_x = 0;
    stop_y = (square_x * _ry);

    // Segunda area de dibujado (arcos superior e inferior)
    while (stop_x < stop_y) {
        
        // --- Dibujado por simetria de 4 vias, con proteccion de overlap ---
        
        // Cuadrante 1 (+, +)
        _px = cx + x; _py = cy + y;
        if (!((_px < 0) || (_py < 0) || (_px > right) || (_py > bottom))) {
            uint32_t addr = ((_py * surface_width) + _px);
            p[addr] = (alpha == 0xFF) ? color : BlendPixel(p[addr], color);
        }
        
        // Cuadrante 2 (-, +) - Solo si x > 0
        if (x > 0) {
            _px = cx - x; _py = cy + y;
            if (!((_px < 0) || (_py < 0) || (_px > right) || (_py > bottom))) {
                uint32_t addr = ((_py * surface_width) + _px);
                p[addr] = (alpha == 0xFF) ? color : BlendPixel(p[addr], color);
            }
        }
        
        // Reflejo vertical solo si y > 0
        if (y > 0) {
            // Cuadrante 3 (-, -) - Solo si x > 0
            if (x > 0) {
                _px = cx - x; _py = cy - y;
                if (!((_px < 0) || (_py < 0) || (_px > right) || (_py > bottom))) {
                    uint32_t addr = ((_py * surface_width) + _px);
                    p[addr] = (alpha == 0xFF) ? color : BlendPixel(p[addr], color);
                }
            }
            // Cuadrante 4 (+, -)
            _px = cx + x; _py = cy - y;
            if (!((_px < 0) || (_py < 0) || (_px > right) || (_py > bottom))) {
                uint32_t addr = ((_py * surface_width) + _px);
                p[addr] = (alpha == 0xFF) ? color : BlendPixel(p[addr], color);
            }
        }

        // Incrementa la X
        x++;
        stop_x += square_y;
        ellip_error += ix;
        ix += square_y;

        // Correccion de valores
        if (((ellip_error << 1) + iy) > 0) {
            y--;
            stop_y -= square_x;
            ellip_error += iy;
            iy += square_x;
        }
    }

    // Indica el blit
    blit = true;

}



/*** Dibuja un circulo relleno (Implementacion del algoritmo de Bresenham) ***/
void NGN_Canvas::BresenhamFilledCircle(int32_t cx, int32_t cy, int32_t r, uint32_t color, int32_t ry) {

    // Asigna los radios
    int32_t _rx = r;
    int32_t _ry = (ry == (int32_t)NGN_DEFAULT_VALUE) ? _rx : ry;

    // Proteccion contra desbordes
    if ((_rx == 0) && (_ry == 0)) {
        return;
    } else if (_ry == 0) {
        Line((cx - _rx), cy, (cx + _rx), cy, color);
        return;
    } else if (_rx == 0) {
        Line(cx, (cy - _ry), cx, (cy + _ry), color);
        return;
    }

    // Usamos int64_t para evitar desbordes en los calculos con radios grandes (>512)
    int64_t x = _rx;
    int64_t y = 0;
    int64_t ix = ((int64_t)_ry * _ry) * (1 - ((int64_t)_rx << 1));
    int64_t iy = ((int64_t)_rx * _rx);
    int64_t square_x = (((int64_t)_rx * _rx) << 1);
    int64_t square_y = (((int64_t)_ry * _ry) << 1);
    int64_t stop_x = (square_y * _rx);
    int64_t stop_y = 0;
    int64_t ellip_error = 0;

    // Uso de alpha blending?
    uint8_t alpha = (color & 0xFF); 
    bool blend = ((alpha > 0x00) && (alpha < 0xFF));

    // Puntero a la matriz de pixeles
    uint32_t* p = (uint32_t*)surface->pixels;

    // AREA 1
    while (stop_x >= stop_y) {
        // Dibuja la linea actual en Y. Nos aseguramos que no se repita en esta iteracion.
        FillScanline(p, (cy + (int32_t)y), (cx - (int32_t)x), (cx + (int32_t)x), color, blend);
        // Evita el doble dibujado en el centro
        if (y > 0) {
            FillScanline(p, (cy - (int32_t)y), (cx - (int32_t)x), (cx + (int32_t)x), color, blend);
        }
        y ++;
        stop_y += square_x;
        ellip_error += iy;
        iy += square_x;
        if (((ellip_error << 1) + ix) > 0) {
            x --;
            stop_x -= square_y;
            ellip_error += ix;
            ix += square_y;
        }
    }

    // Guarda el limite para evitar que el segundo bucle sobreescriba la linea central
    int32_t last_y = (int32_t)y - 1;

    // AREA 2
    x = 0;
    y = _ry;
    ix = ((int64_t)_ry * _ry);
    iy = ((int64_t)_rx * _rx) * (1 - ((int64_t)_ry << 1));
    ellip_error = 0;
    stop_x = 0;
    stop_y = (square_x * _ry);
    int64_t current_y = y;
    int64_t max_x = 0;
    while (stop_x <= stop_y) {
        // Calcula el ancho maximo
        max_x = x; 
        x ++;
        stop_x += square_y;
        ellip_error += ix;
        ix += square_y;
        // Si la Y va a salirse de rango en este paso, dibuja la linea en la pantalla
        if (((ellip_error << 1) + iy) > 0) {
            if (current_y > last_y) {
                FillScanline(p, (cy + (int32_t)current_y), (cx - (int32_t)max_x), (cx + (int32_t)max_x), color, blend);
                FillScanline(p, (cy - (int32_t)current_y), (cx - (int32_t)max_x), (cx + (int32_t)max_x), color, blend);
            }
            y --;
            current_y = y;
            stop_y -= square_x;
            ellip_error += iy;
            iy += square_x;
        }
    }

    // Si llegas al final y hay la ultima linea pendiente de dibujado, dibujala ahora.
    if (current_y > last_y) {
        FillScanline(p, (cy + (int32_t)current_y), (cx - (int32_t)max_x), (cx + (int32_t)max_x), color, blend);
        FillScanline(p, (cy - (int32_t)current_y), (cx - (int32_t)max_x), (cx + (int32_t)max_x), color, blend);
    }

    blit = true;

}



/*** Funcion de relleno ***/
void NGN_Canvas::FillScanline(uint32_t* p, int32_t scan_y, int32_t x_left, int32_t x_right, uint32_t color, bool blend) {

    // Calculos de seguridad previos
    if ((scan_y < 0) || (scan_y > (surface_height - 1))) return;
    int32_t xs = std::max(x_left,  0);
    int32_t xe = std::min(x_right, (surface_width - 1));
    if (xs > xe) return;

    // Calcula el puntero a la fila a dibujar
    uint32_t start_addr = ((scan_y * surface_width) + xs);
    uint32_t data_length = (xe - xs + 1);
    
    // Segun si hay mezcla o no
    if (blend) {
        uint32_t addr = start_addr;
        uint32_t last_addr = (start_addr + data_length);
        while (addr < last_addr) {
            p[addr] = BlendPixel(p[addr], color);
            addr ++;
        }  
    } else {
        uint32_t* row = (p + start_addr);
        std::fill(row, (row + data_length), color);
    }

}



/*** Devuelve un color en formato RGBA ***/
Rgba NGN_Canvas::GetRgbaColor(uint32_t color) {

    Rgba c;
    c.r = (color >> 24) & 0x000000FF;
    c.g = (color >> 16) & 0x000000FF;
    c.b = (color >> 8) & 0x000000FF;
    c.a = color & 0x000000FF;

    return c;

}



/*** Calcula la mezcla (alpha blending) entre dos pixeles y devuelve el resultado ***/
uint32_t NGN_Canvas::BlendPixel(uint32_t dst, uint32_t src) {

    uint8_t src_a = (src & 0x000000FF);
    // Si el pixel de origen es opaco o transparente, no hay nada que mezclar
    if (src_a == 0xFF) return src;
    if (src_a == 0x00) return dst;

    // Descompon los canales del destino
    uint8_t dst_r = (dst >> 24) & 0xFF;
    uint8_t dst_g = (dst >> 16) & 0xFF;
    uint8_t dst_b = (dst >> 8) & 0xFF;
    uint8_t dst_a = (dst & 0xFF);

    // Descompon los canales del origen
    uint8_t src_r = (src >> 24) & 0xFF;
    uint8_t src_g = (src >> 16) & 0xFF;
    uint8_t src_b = (src >> 8) & 0xFF;

    // Porter-Duff "over" por canal
    uint8_t inv_a = (255 - src_a);
    uint8_t out_r = (uint8_t)(((src_a * src_r) + (inv_a * dst_r)) / 255);
    uint8_t out_g = (uint8_t)(((src_a * src_g) + (inv_a * dst_g)) / 255);
    uint8_t out_b = (uint8_t)(((src_a * src_b) + (inv_a * dst_b)) / 255);
    uint8_t out_a = (uint8_t)(src_a + ((inv_a * dst_a) / 255));

    return ((out_r << 24) | (out_g << 16) | (out_b << 8) | out_a);

}
