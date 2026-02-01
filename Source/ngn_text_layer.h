/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0+10th-anniversary ***
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



#ifndef NGN_TEXT_LAYER_H_INCLUDED
#define NGN_TEXT_LAYER_H_INCLUDED



/*** Includes ***/

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>
#include <string>

// Includes de la libreria
#include "ngn_defines.h"



/******************************************************************************

    Clase NGN_TextLayer

******************************************************************************/

/*** Declaracion de la clase del motor de texto ***/
class NGN_TextLayer {

    // Segmento publico
    public:

        // Constructor de la clase (1ra sobrecarga)
        NGN_TextLayer(
            NGN_TextFont* default_font,                         // Fuente por defecto
            NGN_TextureData* bg = nullptr,                         // Textura de fondo
            int32_t position_x = 0,                             // Posicion X (0 por defecto)
            int32_t position_y = 0,                             // Posicion Y (0 por defecto)
            uint32_t _width = (uint32_t)NGN_DEFAULT_VALUE,      // Ancho de la capa (Toda la pantalla por defecto)
            uint32_t _height = (uint32_t)NGN_DEFAULT_VALUE,     // Alto de la capa (Toda la pantalla por defecto)
            bool _filtering = false                             // Filtrado del contenido?
        );

        // Constructor de la clase (2da sobrecarga)
        NGN_TextLayer(
            std::string repo_name,                              // Nombre del repositorio
            std::string resource_name,                          // Nombre del recurso
            std::string bg_name = "",                           // Nombre de la textura de fondo
            int32_t position_x = 0,                             // Posicion X (0 por defecto)
            int32_t position_y = 0,                             // Posicion Y (0 por defecto)
            uint32_t _width = (uint32_t)NGN_DEFAULT_VALUE,      // Ancho de la capa (Toda la pantalla por defecto)
            uint32_t _height = (uint32_t)NGN_DEFAULT_VALUE,     // Alto de la capa (Toda la pantalla por defecto)
            bool _filtering = false                             // Filtrado del contenido?
        );

        // Destructor de la clase
        ~NGN_TextLayer();

        // Propiedades
        Vector2 position;                   // Posicion de la capa
        float width;                        // Tamaño
        float height;
        Vector2 center;                     // Centro de la capa para su rotacion

        bool visible;                       // Visibilidad
        int32_t alpha;                      // Nivel de alpha
        SDL_BlendMode blend_mode;           // Modo de mezcla de color

        double rotation;                    // Rotacion de la capa

        bool flip_h;                        // Flip de la capa
        bool flip_v;

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
        Size2 GetCurrentScale();        // Devuelve la escala actual

        // Rota la capa
        void Rotate(double degrees);

        // Fija el centro de la capa (Posicion relativa desde el centro REAL)
        void SetCenter(float x, float y);

        Vector2I32 locate;                  // Posicion del cabezal de escritura

        struct {                            // Color del texto
            uint8_t r;
            uint8_t g;
            uint8_t b;
        } ink;

        struct {                            // Color del canvas
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        } canvas;

        int32_t padding;        // Margen interior de la caja de texto
        bool word_wrap;         // Autoajuste horizontal del texto
        bool auto_home;         // Reinicio vertical del texto (solo en caso de word wrap)

        // Borra la capa
        void Cls();

        // Posiciona el cabezal de escritura
        void Locate(int32_t x, int32_t y);

        // Margen de la capa de texto
        void Padding(uint32_t pd);

        // Selecciona la fuente del texto
        void Font(NGN_TextFont* fnt);

        // Selecciona el color de la tinta
        void InkColor(uint8_t r, uint8_t g, uint8_t b);     // [R, G, B]
        void InkColor(uint32_t rgb);                        // [0xRRGGBB]

        // Selecciona el color del lienzo
        void CanvasColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);   // [R, G, B, A]
        void CanvasColor(uint32_t rgba);                                // [0xRRGGBBAA]

        // Escribe el texto en la capa
        void Print(std::string text);

        // Devuelve el tamaño original del lienzo
        Size2I32 GetSize();

        // Limites del texto escrito en la capa
        struct {
            int32_t top;
            int32_t bottom;
            int32_t left;
            int32_t right;
            int32_t width;
            int32_t height;
        } text_boundaries;

        // Selecciona un color de tinte (sin parametros, resetea el color)
        void SetTintColor(uint8_t r = 0xFF, uint8_t g = 0xFF, uint8_t b = 0xFF);



    // Segmento privado
    private:

        // Ajuste de los permisos
        friend class NGN_Render;

        // Bufferes internos
        SDL_Texture* backbuffer;            // Backbufer de la capa para su renderizado
        NGN_TextFont* font;                 // Fuente actual
        NGN_TextureData* background;        // Textura de fondo (opcional)

        // Tamaño en pixeles del tamaño original de la capa
        int32_t layer_width, layer_height;

        // Limpieza del surface
        void SurfaceCleanUp();

        // Calcula el tamaño del texto escrito
        void GetTextBoundaries(int32_t x, int32_t y);

        // Propiedades adicionales
        Size2 scale;                        // Escala actual

        // Tinte de la capa de texto
        Rgba tint_color;
        Rgba last_tint_color;
        bool NewTint();

        // Crea el objeto que contiene la capa de texto
        void CreateTextLayer(
            NGN_TextFont* default_font,     // Fuente por defecto
            NGN_TextureData* bg,            // Textura de fondo
            int32_t position_x,             // Posicion X (0 por defecto)
            int32_t position_y,             // Posicion Y (0 por defecto)
            uint32_t _width,                // Ancho de la capa (Toda la pantalla por defecto)
            uint32_t _height,               // Alto de la capa (Toda la pantalla por defecto)
            bool _filtering                 // Filtrado del contenido?
        );

};



#endif // NGN_TEXT_LAYER_H_INCLUDED
