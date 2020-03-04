/******************************************************************************

    N'gine Lib for C++
    *** Version 0.10.1-wip_03 ***
    Canvas - Capa de dibujo

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
    uint32_t _height            // Alto de la capa (Toda la pantalla por defecto)
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

    // Borra el contenido del canvas (basura RAM)
    SurfaceCleanUp();

}



/*** Destructor de la clase NGN_Canvas ***/
NGN_Canvas::~NGN_Canvas() {

    // Destruye el backbuffer
    SDL_DestroyTexture(backbuffer);
    backbuffer = NULL;

}



/*** Posiciona el canvas ***/
void NGN_Canvas::Position(float position_x, float position_y) {

    // Asigna la posicion al canvas
    position.x = position_x;
    position.y = position_y;

}
void NGN_Canvas::Position(Vector2 pos) {

    // Asigna la posicion al canvas
    position.x = pos.x;
    position.y = pos.y;

}



/*** Mueve el canvas ***/
void NGN_Canvas::Translate(float speed_x, float speed_y) {

    // Calcula la nueva posicion
    position.x += speed_x;
    position.y += speed_y;

}
void NGN_Canvas::Translate(Vector2 spd) {

    // Calcula la nueva posicion
    position.x += spd.x;
    position.y += spd.y;

}



/*** Cambia el tamaño del canvas ***/
void NGN_Canvas::Size(float w, float h) {

    // Aplica el nuevo tamaño
    width = w;
    height = h;

}



/*** Escala el canvas [Sobrecarga 1 - Ejes por separado] ***/
void NGN_Canvas::Scale(float w, float h) {

    // Guarda la escala
    scale.width = w;
    scale.height = h;

}

/*** Escala el canvas [Sobrecarga 2 - Ambos ejes a la vez] ***/
void NGN_Canvas::Scale(float sc) {

    // Guarda la escala
    scale.width = sc;
    scale.height = sc;

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



/*** Borra el contenido del canvas ***/
void NGN_Canvas::Cls(uint32_t color) {

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);


    // Borra el contenido de la textura actual
    SDL_SetRenderDrawColor( ngn->graphics->renderer,
                            ((color & 0xFF000000) >> 24),
                            ((color & 0x00FF0000) >> 16),
                            ((color & 0x0000FF00) >> 8),
                            (color & 0x000000FF)
                            );
    SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(backbuffer, 0x00);
    SDL_RenderFillRect(ngn->graphics->renderer, NULL);

    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

}



/*** Dibuja un punto ***/
void NGN_Canvas::Point(uint32_t x, uint32_t y, uint32_t color) {

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);

    // Prepara el dibujado del punto
    SDL_SetRenderDrawColor( ngn->graphics->renderer,
                            ((color & 0xFF000000) >> 24),
                            ((color & 0x00FF0000) >> 16),
                            ((color & 0x0000FF00) >> 8),
                            (color & 0x000000FF)
                            );
    SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(backbuffer, 0x00);

    // Dibuja el punto
    SDL_RenderDrawPoint(ngn->graphics->renderer, x, y);

    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

}



/*** Dibuja una matriz de puntos ***/
void NGN_Canvas::Points(const std::vector<CanvasPoint> &points) {

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);

    // Prepara el dibujado del punto
    SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(backbuffer, 0x00);

    // Dibuja la matriz de puntos
    for (uint32_t n = 0; n < points.size(); n ++) {
        SDL_SetRenderDrawColor( ngn->graphics->renderer,
                               ((points[n].color & 0xFF000000) >> 24),
                               ((points[n].color & 0x00FF0000) >> 16),
                               ((points[n].color & 0x0000FF00) >> 8),
                               (points[n].color & 0x000000FF)
                               );
        SDL_RenderDrawPoint(ngn->graphics->renderer, points[n].x, points[n].y);
    }

    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

}



/*** Dibuja una linea entre dos puntos ***/
void NGN_Canvas::Line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color) {

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);

    // Prepara el dibujado de la linea
    SDL_SetRenderDrawColor( ngn->graphics->renderer,
                            ((color & 0xFF000000) >> 24),
                            ((color & 0x00FF0000) >> 16),
                            ((color & 0x0000FF00) >> 8),
                            (color & 0x000000FF)
                            );
    SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(backbuffer, 0x00);

    // Dibuja la linea
    SDL_RenderDrawLine(ngn->graphics->renderer, x1, y1, x2, y2);

    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

}



/*** Dibuja una matriz de lineas ***/
void NGN_Canvas::Lines(const std::vector<CanvasLine> &lines) {

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);

    // Prepara el dibujado de la linea
    SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(backbuffer, 0x00);

    // Dibuja la matriz de lineas
    for (uint32_t n = 0; n < lines.size(); n ++) {
        SDL_SetRenderDrawColor( ngn->graphics->renderer,
                                ((lines[n].color & 0xFF000000) >> 24),
                                ((lines[n].color & 0x00FF0000) >> 16),
                                ((lines[n].color & 0x0000FF00) >> 8),
                                (lines[n].color & 0x000000FF)
                                );
        SDL_RenderDrawLine(ngn->graphics->renderer, lines[n].x1, lines[n].y1, lines[n].x2, lines[n].y2);
    }

    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

}



/*** Dibuja un cuadrado ***/
void NGN_Canvas::Box(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color, bool paint) {

    // Ordena los vertices
    int32_t _x1 = 0, _x2 = 0, _y1 = 0, _y2 = 0;
    if (x2 > x1) {
        _x1 = x1;
        _x2 = x2;
    } else {
        _x1 = x2;
        _x2 = x1;
    }
    if (y2 > y1) {
        _y1 = y1;
        _y2 = y2;
    } else {
        _y1 = y2;
        _y2 = y1;
    }

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);

    // Prepara el dibujado de la linea
    SDL_SetRenderDrawColor( ngn->graphics->renderer,
                            ((color & 0xFF000000) >> 24),
                            ((color & 0x00FF0000) >> 16),
                            ((color & 0x0000FF00) >> 8),
                            (color & 0x000000FF)
                            );
    SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(backbuffer, 0x00);

    // Dibuja el cuadrado
    if (paint) {
        for (int32_t i = _y1; i <= _y2; i ++) SDL_RenderDrawLine(ngn->graphics->renderer, _x1, i, _x2, i);
    } else {
        SDL_RenderDrawLine(ngn->graphics->renderer, _x1, _y1, _x1, _y2);
        SDL_RenderDrawLine(ngn->graphics->renderer, _x1, _y2, _x2, _y2);
        SDL_RenderDrawLine(ngn->graphics->renderer, _x2, _y2, _x2, _y1);
        SDL_RenderDrawLine(ngn->graphics->renderer, _x2, _y1, _x1, _y1);
    }

    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

}



/*** Dibuja un circulo ***/
void NGN_Canvas::Circle(uint32_t x, uint32_t y, uint32_t r, uint32_t color, uint32_t r2, double in_angle, double out_angle) {

    // Asignacion de los radios y proteccion de 0
    uint32_t _r = (r < 1) ? 1 : r;
    uint32_t _r2 = ((r2 == NGN_DEFAULT_VALUE) || (r2 < 1)) ? _r : r2;

    // Calculo del arco a dibujar
    bool arc = false;
    double st = 0.0f, ed = 0.0f;
    if ((in_angle == 0.0f) && (out_angle == 0.0f)) {
        ed = (PI / 2.0f);
    } else {
        // Si el angulo de entrada y salida son los mismos
        if (in_angle == out_angle) return;
        // Ordena los angulos de entrada y salida
        if (out_angle > in_angle) {
            st = in_angle;
            ed = out_angle;
        } else {
            st = out_angle;
            ed = in_angle;
        }
        // Si superan 2PI
        if ((ed - st) > (PI * 2.0f)) {
            st = 0.0f;
            ed = (PI / 2.0f);
        } else {
            arc = true;
        }
    }

    // Calculos iniciales para el dibujo del circulo (1/4 del mismo)
    double step = (_r2 > _r) ? ((((PI / 2.0f) / (double)_r2)) * CIRCLE_PRECISION) : ((((PI / 2.0f) / (double)_r)) * CIRCLE_PRECISION);    // Calculo del numero de segmentos segun el radio
    int32_t px = 0, py = 0;                                         // Variables de calculo de vertices
    int32_t _px = 0.0f, _py = 0.0f;
    int32_t sg = (std::ceil((std::abs((ed - st)) / step)) + 1);      // Numero de segmentos del arco

    // Inicializa el vector contenedor de segmentos (arco * 4)
    segment.clear();
    segment.resize((sg << 2));
    //std::cout << n << " : " << step << std::endl;

    // Calculo del punto inicial
    _px = std::round((std::cos(st) * (double)_r));
    _py = std::round((std::sin(st) * (double)_r2));
    st += step;

    // Calculo del resto de segmentos
    double a = 0.0f;
    uint32_t id = 0;
    CanvasLine line;
    line.color = color;
    double angle_end = (ed + step);
    for (double angle = st; angle < angle_end; angle += step) {
        a = (angle > ed) ? ed : angle;
        px = std::round((std::cos(a) * (double)_r));
        py = std::round((std::sin(a) * (double)_r2));
        // Segmento A (0 - 90) o arco definido
        line.x1 = (x + _px); line.y1 = (y + _py); line.x2 = (x + px); line.y2 = (y + py);
        segment[id] = line; id ++;
        // Si no es un arco simple...
        if (!arc) {
            // Segmento B (90 - 180)
            line.x1 = (x - _px); line.y1 = (y + _py); line.x2 = (x - px); line.y2 = (y + py);
            segment[id] = line; id ++;
            // Segmento C (180 - 270)
            line.x1 = (x - _px); line.y1 = (y - _py); line.x2 = (x - px); line.y2 = (y - py);
            segment[id] = line; id ++;
            // Segmento D (270 - 360)
            line.x1 = (x + _px); line.y1 = (y - _py); line.x2 = (x + px); line.y2 = (y - py);
            segment[id] = line; id ++;
        }
        _px = px;
        _py = py;
    }

    // Dibuja el circulo en base los segmentos
    segment.resize(id);
    Lines(segment);

    // Limpia el vector de segmentos
    segment.clear();

}



/*** Dibuja un circulo relleno ***/
void NGN_Canvas::FilledCircle(uint32_t x, uint32_t y, uint32_t r, uint32_t color, uint32_t r2) {

    // Asignacion de los radios y proteccion de 0
    uint32_t _r = (r < 1) ? 1 : r;
    uint32_t _r2 = ((r2 == NGN_DEFAULT_VALUE) || (r2 < 1)) ? _r : r2;

    // Angulos de inicio y final
    double st = -(PI / 2.0f), ed = (PI / 2.0f);

    // Calculos iniciales para el dibujo del circulo
    double step = (_r2 != _r) ? (PI / ((double)_r2 * 2.0f)) : (PI / ((double)_r * 2.0f));    // Calculo del numero de segmentos segun el radio
    int32_t px = 0, py = 0;                                             // Variables de calculo de vertices
    int32_t sg = (std::ceil((std::abs((ed - st)) / step)) + 2);         // Numero de segmentos del circulo

    // Inicializa el vector contenedor de segmentos
    segment.clear();
    segment.resize(sg);

    // Calculo de los vertices
    double a = 0.0f;
    uint32_t id = 0;
    CanvasLine line;
    double angle_end = (ed + step);
    // Primer segmento
    py = std::round((std::sin(st) * (double)_r2));
    px = std::round((std::cos(st) * (double)_r));
    line.y1 = line.y2 = (y + py);
    line.x1 = (x + px);
    line.x2 = (x - px);
    line.color = color;
    segment[id] = line;
    id ++;
    st += step;
    int32_t _y = py;
    // Resto de segmentos
    for (double angle = st; angle < angle_end; angle += step) {
        a = (angle > ed) ? ed : angle;
        py = std::round((std::sin(a) * (double)_r2));
        if (_y != py) {
            px = std::round((std::cos(a) * (double)_r));
            // Define el segmento
            line.y1 = line.y2 = (y + py);
            line.x1 = (x + px);
            line.x2 = (x - px);
            segment[id] = line;
            id ++;
            // Si ha quedado un espacio entre lines
            if (std::abs(_y - py) > 1) {
                line.y1 = line.y2 = (y + py - 1);
                segment[id] = line;
                id ++;
            }
        }
        _y = py;
    }

    // Ajuste del tamaño del buffer
    segment.resize(id);
    //std::cout << "Total segments: " << id << std::endl;

    // Genera el circulo pintado
    Lines(segment);

    // Borra los buffers
    segment.clear();

}



/*** Limpieza de la superficie ***/
void NGN_Canvas::SurfaceCleanUp() {

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);
    // Borra el contenido del renderer
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(ngn->graphics->renderer);
    // Restaura el render al seleccionado
    ngn->graphics->RenderToSelected();

}
