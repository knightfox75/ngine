/******************************************************************************

    N'gine Lib for C++
    *** Version 1.17.0-wip_0x02 ***
    Canvas - Capa de dibujo

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2023 by Cesar Rincon "NightFox"

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
    surface = NULL;
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
    backbuffer = NULL;
    backbuffer = SDL_CreateTexture(
        ngn->graphics->renderer,       // Renderer
        SDL_PIXELFORMAT_BGRA8888,      // Formato del pixel
        SDL_TEXTUREACCESS_TARGET,      // Textura como destino del renderer
        surface_width,                 // Ancho de la textura
        surface_height                 // Alto de la textura
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

    // Borra el contenido de la textura del canvas (basura RAM)
    BackbufferCleanUp();

    // Control del buffer de pixeles
    blit = false;

}



/*** Destructor de la clase NGN_Canvas ***/
NGN_Canvas::~NGN_Canvas() {

    // Destruye el backbuffer
    SDL_DestroyTexture(backbuffer);
    backbuffer = NULL;

    // Destruye el surface
    SDL_FreeSurface(surface);
    surface = NULL;

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



/*** Rota el canvas los grados solicitados ***/
void NGN_Canvas::Rotate(double degrees) {

    rotation += degrees;
    while (rotation >= 360.0f) rotation -= 360.0f;
    while (rotation < 0.0f) rotation += 360.0f;

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

    SDL_FillRect(surface, NULL, color);

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

    // Bloquea el surface
    //SDL_LockSurface(surface);

    // Dibuja el punto del color dado
    uint32_t* p = (uint32_t*)surface->pixels;
    p[((y * surface_width) + x)] = color;

    // Desbloquea el surface
    //SDL_UnlockSurface(surface);

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

    // Valor absoluto de las distancias
    dx = std::abs(dx);
    dy = std::abs(dy);

    // Bloquea el surface
    //SDL_LockSurface(surface);

    // Acceso al array de pixeles
    uint32_t* p = (uint32_t*)surface->pixels;

    // Dibuja el primer pixel
    if (!((x < 0) || (y < 0) || (x > right) || (y > bottom))) {
        p[((y * surface_width) + x)] = color;
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
                p[((y * surface_width) + x)] = color;
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
                p[((y * surface_width) + x)] = color;
            }
        }

    }

    // Desbloquea el surface
    //SDL_UnlockSurface(surface);

    // Indica el blit
    blit = true;

}



/*** Dibuja un cuadrado ***/
void NGN_Canvas::Box(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, bool paint) {

    // Calcula y ordena los puntos del rectangulo
    int32_t xa = 0, ya = 0, xb = 0, yb = 0;
    if (x1 <= x2) {
        xa = x1;
        xb = x2;
    } else {
        xa = x2;
        xb = x1;
    }
    if (y1 <= y2) {
        ya = y1;
        yb = y2;
    } else {
        ya = y2;
        yb = y1;
    }

    if (paint) {
        int32_t x = 0, y = 0;
        // Bloquea el surface
        //SDL_LockSurface(surface);
        // Acceso al array de pixeles
        uint32_t* p = (uint32_t*)surface->pixels;
        uint32_t idx = 0;
        // Rectangulo con relleno (rellena el buffer de pixeles)
        for (y = ya; y <= yb; y ++) {
            // Proteccion de coordenadas
            if ((y < 0) || (y >= surface_height)) continue;
            // Calcula la posicion del puntero
            idx = (y * surface_width);
            for (x = xa; x <= xb; x ++) {
                // Proteccion de coordenadas
                if ((x < 0) || (x >= surface_width)) continue;
                // Escribe el dato
                p[(idx + x)] = color;
            }
        }
        // Desbloquea el surface
        //SDL_UnlockSurface(surface);
    } else {
        // Rectangulo sin relleno (usa lineas)
        Line(xa, ya, xb, ya, color);
        Line(xb, ya, xb, yb, color);
        Line(xb, yb, xa, yb, color);
        Line(xa, yb, xa, ya, color);
    }

    // Indica el blit
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
    int32_t _ry = (ry == NGN_DEFAULT_VALUE) ? _rx:ry;

    // Ordena los angulos de inicio y fin
    double st_angle = 0.0f, ed_angle = 0.0f;
    if (start_angle == end_angle) {
        // Si son identicos, sal
        return;
    } else if (start_angle > end_angle) {
        // Si el angulo inicial es mayor que el final
        st_angle = start_angle;
        ed_angle = end_angle += (PI * 2.0f);
    } else {
        st_angle = start_angle;
        ed_angle = end_angle;
    }

    // Algun ambos radios son 0...
    if ((_rx == 0) && (_ry == 0)) {
        return;
    }

    // Calculos
    int32_t x = 0, y = 0;                               // Coordenadas
    int32_t _x = 0, _y = 0;
    int32_t _fx = 0, _fy = 0;

    // Precision
    double p = 0.0f;
    if (_rx >= _ry) {
        p = (PI * 4.0f) / (double)_rx;
    } else {
        p = (PI * 4.0f) / (double)_ry;
    }

    // Primer punto del angulo
    _fx = _x = (std::round((std::cos(st_angle) * (float)_rx)) + cx);
    _fy = _y = (std::round((std::sin(st_angle) * (float)_ry)) + cy);
    // Dibuja los segmentos del arco
    for (double angle = st_angle; angle <= ed_angle; angle += p) {
        // Calculos de las coordenadas
        x = (std::round((std::cos(angle) * (float)_rx)) + cx);
        y = (std::round((std::sin(angle) * (float)_ry)) + cy);
        if ((_x != x) || (_y != y)) Line(x, y, _x, _y, color);
        _x = x;
        _y = y;
    }
    // Ultimo punto del arco
    x = (std::round((std::cos(ed_angle) * (float)_rx)) + cx);
    y = (std::round((std::sin(ed_angle) * (float)_ry)) + cy);
    if ((_x != x) || (_y != y)) Line(x, y, _x, _y, color);

    // Has de cerrar el arco (0 = no, 1 = entre los puntos, 2 = con el centro)
    switch (close) {
        // Entre ellos
        case 1:
            Line(x, y, _fx, _fy, color);
            break;
        // Con el centro
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



/*** Convierte el buffer de pixeles en una textura ***/
void NGN_Canvas::Blit() {

    // Si no es necesario, ignora el comando
    if (!blit) return;

    // Convierte la superficie generada en textura
    SDL_DestroyTexture(backbuffer);
    backbuffer = NULL;
    #if !defined (DISABLE_BACKBUFFER)
        if (filtering) {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        }
    #endif
    backbuffer = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface);
    #if !defined (DISABLE_BACKBUFFER)
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    #endif

    // Marca la conversion como realizada
    blit = false;

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

    // Asignacion de los radios y proteccion de 0
    int32_t _rx = r;
    int32_t _ry = (ry == NGN_DEFAULT_VALUE) ? _rx:ry;

    // Algun radio es 0...
    if ((_rx == 0) && (_ry == 0)) {
        return;
    } else if (_ry == 0) {
        Line((cx - _rx), cy, (cx + _rx), cy, color);
        return;
    } else if (_rx == 0) {
        Line(cx, (cy - _ry), cx, (cy + _ry), color);
        return;
    }

    // Calculos
    int32_t right = (surface_width - 1);                // Limites del buffer
    int32_t bottom = (surface_height - 1);
    int32_t x = _rx;                                    // Coordenadas
    int32_t y = 0;
    int32_t ix = ((_ry * _ry) * (1 - (_rx << 1)));      // Cambios de coordenada
    int32_t iy = (_rx * _rx);
    int32_t square_x = ((_rx * _rx) << 1);              // Cuadrados
    int32_t square_y = ((_ry * _ry) << 1);
    int32_t stop_x = (square_y * _rx);                  // Puntos de detencion
    int32_t stop_y = 0;
    int32_t ellip_error = 0;                            // Error en el dibujado de la elipse

    // Variables adicionales
    int32_t _x = 0, _y = 0;

    // Bloquea el surface
    //SDL_LockSurface(surface);
    // Acceso al array de pixeles
    uint32_t* p = (uint32_t*)surface->pixels;

    // Primera area de dibujado (izquierda y derecha)
    while (stop_x >= stop_y) {
        // Dibuja el pixel del cuadrante 1
        _x = cx + x;
        _y = cy + y;
        if (!((_x < 0) || (_y < 0) || (_x > right) || (_y > bottom))) {
            p[((_y * surface_width) + _x)] = color;
        }
        // Dibuja el pixel del cuadrante 2
        _x = cx - x;
        _y = cy + y;
        if (!((_x < 0) || (_y < 0) || (_x > right) || (_y > bottom))) {
            p[((_y * surface_width) + _x)] = color;
        }
        // Dibuja el pixel del cuadrante 3
        _x = cx - x;
        _y = cy - y;
        if (!((_x < 0) || (_y < 0) || (_x > right) || (_y > bottom))) {
            p[((_y * surface_width) + _x)] = color;
        }
        // Dibuja el pixel del cuadrante 4
        _x = cx + x;
        _y = cy - y;
        if (!((_x < 0) || (_y < 0) || (_x > right) || (_y > bottom))) {
            p[((_y * surface_width) + _x)] = color;
        }
        // Incrementa la Y
        y ++;
        // Incrementa stop_y
        stop_y += square_x;
        // Incrementa el error de elipse
        ellip_error += iy;
        // Incrementa el incremento de Y
        iy += square_x;
        // Correccion de valores
        if (((ellip_error << 1) + ix) > 0) {
            // Reduce X
            x --;
            // Reduce stop_x
            stop_x -= square_y;
            // Incrementa el error de elipse
            ellip_error += ix;
            // Incremente el incremento de X
            ix += square_y;
        }
    }

    // Prepara la segunda area (arriba y abajo)
    x = 0;
    y = _ry;
    ix = (_ry * _ry);
    iy = ((_rx * _rx) * (1 - (_ry << 1)));
    ellip_error = 0;
    stop_x = 0;
    stop_y = (square_x * _ry);

    // Segunda area de dibujado (arriba y abajo)
    while (stop_x <= stop_y) {
        // Dibuja el pixel del cuadrante 1
        _x = cx + x;
        _y = cy + y;
        if (!((_x < 0) || (_y < 0) || (_x > right) || (_y > bottom))) {
            p[((_y * surface_width) + _x)] = color;
        }
        // Dibuja el pixel del cuadrante 2
        _x = cx - x;
        _y = cy + y;
        if (!((_x < 0) || (_y < 0) || (_x > right) || (_y > bottom))) {
            p[((_y * surface_width) + _x)] = color;
        }
        // Dibuja el pixel del cuadrante 3
        _x = cx - x;
        _y = cy - y;
        if (!((_x < 0) || (_y < 0) || (_x > right) || (_y > bottom))) {
            p[((_y * surface_width) + _x)] = color;
        }
        // Dibuja el pixel del cuadrante 4
        _x = cx + x;
        _y = cy - y;
        if (!((_x < 0) || (_y < 0) || (_x > right) || (_y > bottom))) {
            p[((_y * surface_width) + _x)] = color;
        }
        // Incrementa la X
        x ++;
        // Incrementa stop_x
        stop_x += square_y;
        // Incrementa el error de elipse
        ellip_error += ix;
        // Incrementa el incremento de X
        ix += square_y;
        // Correccion de valores
        if (((ellip_error << 1) + iy) > 0) {
            // Reduce Y
            y --;
            // Reduce stop_y
            stop_y -= square_x;
            // Incrementa el error de elipse
            ellip_error += iy;
            // Incremente el incremento de Y
            iy += square_x;
        }
    }

    // Desbloquea el surface
    //SDL_UnlockSurface(surface);

    // Indica el blit
    blit = true;

}



/*** Dibuja un circulo relleno (Implementacion del algoritmo de Bresenham) ***/
void NGN_Canvas::BresenhamFilledCircle(int32_t cx, int32_t cy, int32_t r, uint32_t color, int32_t ry) {

    // Asignacion de los radios y proteccion de 0
    int32_t _rx = r;
    int32_t _ry = (ry == NGN_DEFAULT_VALUE) ? _rx:ry;

    // Algun radio es 0...
    if ((_rx == 0) && (_ry == 0)) {
        return;
    } else if (_ry == 0) {
        Line((cx - _rx), cy, (cx + _rx), cy, color);
        return;
    } else if (_rx == 0) {
        Line(cx, (cy - _ry), cx, (cy + _ry), color);
        return;
    }

    // Calculos
    int32_t right = (surface_width - 1);                // Limites del buffer
    int32_t bottom = (surface_height - 1);
    int32_t x = _rx;                                    // Coordenadas
    int32_t y = 0;
    int32_t ix = ((_ry * _ry) * (1 - (_rx << 1)));      // Cambios de coordenada
    int32_t iy = (_rx * _rx);
    int32_t square_x = ((_rx * _rx) << 1);              // Cuadrados
    int32_t square_y = ((_ry * _ry) << 1);
    int32_t stop_x = (square_y * _rx);                  // Puntos de detencion
    int32_t stop_y = 0;
    int32_t ellip_error = 0;                            // Error en el dibujado de la elipse

    // Variables adicionales
    int32_t _x = 0, _y = 0;
    int32_t _start = 0, _end = 0;
    int32_t _py = 0;

    // Bloquea el surface
    //SDL_LockSurface(surface);
    // Acceso al array de pixeles
    uint32_t* p = (uint32_t*)surface->pixels;

    // Primera area de dibujado (izquierda y derecha)
    while (stop_x >= stop_y) {
        // Dibuja la linea del cuadrante 1
        _start = cx - x;
        _end = cx + x;
        _y = cy - y;
        if (!((_y < 0) || (_y > bottom))) {
            _py = (_y * surface_width);
            for (_x = _start; _x <= _end; _x ++) {
                if (!((_x < 0) || (_x > right))) {
                    p[(_py + _x)] = color;
                }
            }
        }
        // Dibuja la linea del cuadrante 2
        _start = cx - x;
        _end = cx + x;
        _y = cy + y;
        if (!((_y < 0) || (_y > bottom))) {
            _py = (_y * surface_width);
            for (_x = _start; _x <= _end; _x ++) {
                if (!((_x < 0) || (_x > right))) {
                    p[(_py + _x)] = color;
                }
            }
        }
        // Incrementa la Y
        y ++;
        // Incrementa stop_y
        stop_y += square_x;
        // Incrementa el error de elipse
        ellip_error += iy;
        // Incrementa el incremento de Y
        iy += square_x;
        // Correccion de valores
        if (((ellip_error << 1) + ix) > 0) {
            // Reduce X
            x --;
            // Reduce stop_x
            stop_x -= square_y;
            // Incrementa el error de elipse
            ellip_error += ix;
            // Incremente el incremento de X
            ix += square_y;
        }
    }

    // Prepara la segunda area (arriba y abajo)
    x = 0;
    y = _ry;
    ix = (_ry * _ry);
    iy = ((_rx * _rx) * (1 - (_ry << 1)));
    ellip_error = 0;
    stop_x = 0;
    stop_y = (square_x * _ry);

    // Segunda area de dibujado (arriba y abajo)
    while (stop_x <= stop_y) {
        // Dibuja la linea del cuadrante 1
        _start = cx - x;
        _end = cx + x;
        _y = cy - y;
        if (!((_y < 0) || (_y > bottom))) {
            _py = (_y * surface_width);
            for (_x = _start; _x <= _end; _x ++) {
                if (!((_x < 0) || (_x > right))) {
                    p[(_py + _x)] = color;
                }
            }
        }
        // Dibuja la linea del cuadrante 2
        _start = cx - x;
        _end = cx + x;
        _y = cy + y;
        if (!((_y < 0) || (_y > bottom))) {
            _py = (_y * surface_width);
            for (_x = _start; _x <= _end; _x ++) {
                if (!((_x < 0) || (_x > right))) {
                    p[(_py + _x)] = color;
                }
            }
        }
        // Incrementa la X
        x ++;
        // Incrementa stop_x
        stop_x += square_y;
        // Incrementa el error de elipse
        ellip_error += ix;
        // Incrementa el incremento de X
        ix += square_y;
        // Correccion de valores
        if (((ellip_error << 1) + iy) > 0) {
            // Reduce Y
            y --;
            // Reduce stop_y
            stop_y -= square_x;
            // Incrementa el error de elipse
            ellip_error += iy;
            // Incremente el incremento de Y
            iy += square_x;
        }
    }

    // Desbloquea el surface
    //SDL_UnlockSurface(surface);

    // Indica el blit
    blit = true;

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
