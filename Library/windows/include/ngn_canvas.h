/******************************************************************************

    N'gine Lib for C++
    *** Version 1.18.0-stable ***
    Canvas - Capa de dibujo

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



#ifndef NGN_CANVAS_H_INCLUDED
#define NGN_CANVAS_H_INCLUDED



/*** Includes ***/

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>

// Includes de la libreria
#include "ngn_defines.h"





/******************************************************************************

    Clase NGN_Canvas

******************************************************************************/

/*** Declaracion de la clase del canvas ***/
class NGN_Canvas {

    // Segmento publico
    public:

        // Constructor de la clase
        NGN_Canvas(
            int32_t position_x = 0,                     // Posicion X (0 por defecto)
            int32_t position_y = 0,                     // Posicion Y (0 por defecto)
            uint32_t _width = NGN_DEFAULT_VALUE,        // Ancho de la capa (Toda la pantalla por defecto)
            uint32_t _height = NGN_DEFAULT_VALUE,       // Alto de la capa (Toda la pantalla por defecto)
            bool _filtering = false                     // Filtrado bilinear del contenido?
        );

        // Destructor de la clase
        ~NGN_Canvas();

        // Propiedades
        Vector2 position;                   // Posicion de la capa
        float width;                        // Tamaño
        float height;
        Vector2 center;                     // Centro de la capa para su rotacion

        bool visible;                       // Visibilidad
        int32_t alpha;                      // Nivel de alpha
        SDL_BlendMode blend_mode;           // Modo de mezcla de color
        bool filtering;                     // Filtrado del contenido?

        double rotation;                    // Rotacion de la capa

        bool flip_h;                        // Flip de la capa
        bool flip_v;

        Size2 scale;                        // Escala actual

        // Posiciona la capa
        void Position(float position_x, float position_y);
        void Position(Vector2 pos);

        // Mueve la capa
        void Translate(float speed_x, float speed_y);
        void Translate(Vector2 spd);

        // Cambia el tamaño de la capa
        void Size(float w, float h);

        //  Escala una capa
        void Scale(float w, float h);   // [Sobrecarga 1]       Escala los dos ejes por separado
        void Scale(float sc);           // [Sobrecarga 2]       Escala ambos ejes a la vez

        // Rota la capa
        void Rotate(double degrees);

        // Fija el centro de la capa (Posicion relativa desde el centro REAL)
        void SetCenter(float x, float y);



        /*** Metodos y propiedades del lienzo ***/

        // Almacen de datos del lienzo
        SDL_Surface* surface;               // Surface
        SDL_Texture* backbuffer;            // Backbufer de la capa para su renderizado

        // Borra la capa
        void Cls(uint32_t color = 0x00000000);

        // Devuelve el tamaño original del lienzo
        Size2I32 GetSize();

        // Dibuja un punto
        void Point(int32_t x, int32_t y, uint32_t color);

        // Dibuja una linea (Implementacion del algoritmo de Bresenham)
        void Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);

        // Dibuja un cuadrado
        void Box(int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color, bool paint = false);

        // Dibuja un circulo
        void Circle(int32_t cx, int32_t cy, int32_t r, uint32_t color, int32_t ry = NGN_DEFAULT_VALUE, bool paint = false);

        // Dibuja un arco
        void Arc(int32_t cx, int32_t cy, int32_t r, double start_angle, double end_angle, uint32_t color, int32_t ry = NGN_DEFAULT_VALUE, uint8_t close = 0);

        // Obten el color del pixel de las coordenadas proporcionadas
        uint32_t GetPixelColor(int32_t x, int32_t y);
        Rgba GetPixelRgba(int32_t x, int32_t y);

        // Convierte el buffer a textura
        void Blit();



    // Segmento privado
    private:

        // Limpieza del surface
        void BackbufferCleanUp();

        // Dibuja un circulo sin relleno (Implementacion del algoritmo de Bresenham)
        void BresenhamCircle(int32_t cx, int32_t cy, int32_t r, uint32_t color, int32_t ry = NGN_DEFAULT_VALUE);
        // Dibuja un circulo con relleno (Implementacion del algoritmo de Bresenham)
        void BresenhamFilledCircle(int32_t cx, int32_t cy, int32_t r, uint32_t color, int32_t ry = NGN_DEFAULT_VALUE);

        // Devuelve el color en formato RGBA
        Rgba GetRgbaColor(uint32_t color);

        // Tamaño en pixeles del buffer de pixeles
        int32_t surface_width, surface_height;

        // Flag de conversion a textura
        bool blit;

};




#endif // NGN_CANVAS_H_INCLUDED
