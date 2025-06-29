/******************************************************************************

    N'gine Lib for C++
    *** Version 1.19.1-stable ***
    Fondos Tileados

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



#ifndef NGN_TILEDBG_H_INCLUDED
#define NGN_TILEDBG_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
// Includes de la libreria
#include "ngn_defines.h"


/*** Definicion de la clase ***/
class NGN_TiledBg {

    // Public
    public:

        // Constructor (1ra sobrecarga)
        NGN_TiledBg(
            NGN_TiledBgData* bg,            // Datos del fondo
            int32_t position_x = 0,         // Posicion X del fondo [0 por defecto]
            int32_t position_y = 0          // Posicion Y del fondo [0 por defecto]
        );

        // Constructor (2da sobrecarga)
        NGN_TiledBg(
            std::string repo_name,          // Nombre del repositorio
            std::string resource_name,      // Nombre del recurso
            int32_t position_x = 0,         // Posicion X del fondo [0 por defecto]
            int32_t position_y = 0          // Posicion Y del fondo [0 por defecto]
        );

        // Destructor
        ~NGN_TiledBg();

        // Propiedades del fondo (publicas)
        NGN_TiledBgData* bgdata;            // Tiles + Mapa del fondo
        Vector2 position;                   // Posicion del fondo
        float width;                        // Tamaño
        float height;

        uint32_t tile_size;                 // Tamaño del tile
        Size2I32 map_size;                  // Tamaño del mapa de tiles

        Vector2 center;                     // Centro del fondo para su rotacion

        double rotation;                    // Rotacion del fondo

        bool flip_h;                        // Flip de la capa
        bool flip_v;

        bool visible;                       // Visibilidad
        int32_t alpha;                      // Nivel de alpha
        SDL_BlendMode blend_mode;           // Modo de mezcla de color

        Vector2 scale;                      // Escala del fondo

        SDL_Texture* backbuffer;            // Backbufer del fondo para su renderizado
        SDL_Texture* transform_texture;     // Textura para aplicar la transformacion del fondo
        SDL_Texture* last_buffer;           // Ultimo buffer usado

        // Tamaño del backbuffer
        Size2I32 bb_size;

        // Ubicacion en el ultimo frame
        Vector2 last_position;
        int8_t last_viewport;

        // Modo de render en el ultimo frame
        bool tile_mode;

        // Posiciona el fondo
        void Position(float position_x, float position_y);
        void Position(Vector2 pos);

        // Mueve el fondo
        void Translate(float speed_x, float speed_y);
        void Translate(Vector2 spd);

        //  Escala el fondo
        void Scale(float w, float h);   // [Sobrecarga 1]       Escala los dos ejes por separado
        void Scale(float factor);       // [Sobrecarga 2]       Escala ambos ejes a la vez

        // Rota el fondo
        void Rotate(double degrees);

        // Fija el centro del fondo (Posicion relativa desde el centro REAL)
        void SetCenter(float x, float y);

        // Si se da el caso, capa en la camara virtual 2D donde esta ubicado
        int32_t camera_layer;

        // Datos del fondo virtual para la camara 2D
        struct {
            Size2I64 bg_size;                   // Tamaño virtual del fondo
            Vector2I64 loop;                    // Punto de loop del scroll del fondo (segun tamaño virtual)
            Vector2 scroll;                     // Velocidad del autoscroll
            Vector2 offset;                     // Guarda el acumulado del autoscroll
            bool enabled;                       // El fondo lo gestiona la camara en modo virtual
        } virtual_bg;

        // Tinte del fondo de tiles
        Rgba tint_color;        // Color a aplicar
        // Selecciona un color de tinte (sin parametros, resetea el color)
        void SetTintColor(uint8_t r = 0xFF, uint8_t g = 0xFF, uint8_t b = 0xFF);
        bool NewTint();
        bool ignore_camera_tint;  // Ignorar el tinte aplicado por la camara 2D


    // Private
    private:

        // Crea el objeto que contiene el fondo
        void CreateTiledBg(
            NGN_TiledBgData* bg,    // Datos del fondo
            int32_t position_x,     // Posicion X del fondo
            int32_t position_y      // Posicion Y del fondo
        );

        // Ultimo color de tinta usado
        Rgba last_tint_color;

};



#endif // NGN_TILEDBG_H_INCLUDED
