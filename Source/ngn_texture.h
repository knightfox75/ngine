/******************************************************************************

    N'gine Lib for C++
    *** Version 1.17.0-wip_0x02 ***
    Fondos con texturas

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



#ifndef NGN_TEXTURE_H_INCLUDED
#define NGN_TEXTURE_H_INCLUDED



/*** Includes ***/

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>
#include <string>

// Includes de la libreria
#include "ngn_defines.h"



/*** Definicion de la clase ***/
class NGN_Texture {

   // Public
    public:

        // Contructor (1ra sobrecarga)
        NGN_Texture(
            NGN_TextureData* texture,                               // Objeto de la clase Texture Data
            int32_t position_x = NGN_DEFAULT_VALUE,                 // Posicion X inicial (oculto por defecto)
            int32_t position_y = NGN_DEFAULT_VALUE,                 // Posicion Y inicial (oculto por defecto)
            uint32_t texture_width = (uint32_t)NGN_DEFAULT_VALUE,   // Ancho de la textura (por defecto, el de la textura)
            uint32_t texture_height = (uint32_t)NGN_DEFAULT_VALUE   // Altura de la textura (por defecto, la de la textura)
        );
        // Contructor (2da sobrecarga)
        NGN_Texture(
            uint32_t texture_width,     // Ancho de la textura (por defecto, el de la textura)
            uint32_t texture_height,    // Altura de la textura (por defecto, la de la textura)
            int32_t position_x = NGN_DEFAULT_VALUE,                // Posicion X inicial (oculto por defecto)
            int32_t position_y = NGN_DEFAULT_VALUE                 // Posicion Y inicial (oculto por defecto)
        );
        // Contructor (3ra sobrecarga)
        NGN_Texture(
            std::string repo_name,                                  // Nombre del repositorio
            std::string resource_name,                              // Nombre del recurso
            int32_t position_x = NGN_DEFAULT_VALUE,                 // Posicion X inicial (oculto por defecto)
            int32_t position_y = NGN_DEFAULT_VALUE,                 // Posicion Y inicial (oculto por defecto)
            uint32_t texture_width = (uint32_t)NGN_DEFAULT_VALUE,   // Ancho de la textura (por defecto, el de la textura)
            uint32_t texture_height = (uint32_t)NGN_DEFAULT_VALUE   // Altura de la textura (por defecto, la de la textura)
        );

        // Destructor
        ~NGN_Texture();

        // Propiedades de la textura (publicas)
        NGN_TextureData* data;      // Datos graficos de la textura
        Vector2 position;           // Posicion de la textura (Global/Mundo)
        Vector2 screen;             // Posicion de la textura (Pantalla)
        float width;                // Tamaño
        float height;
        Vector2 center;             // Centro de la textura para su rotacion

        bool visible;               // Visibilidad
        int32_t alpha;              // Nivel de alpha
        SDL_BlendMode blend_mode;   // Modo de mezcla de color

        double rotation;            // Rotacion de la textura

        bool flip_h;                // Flip de la textura
        bool flip_v;

        // Posiciona la textura
        void Position(float position_x, float position_y);
        void Position(Vector2 pos);

        // Mueve la textura
        void Translate(float speed_x, float speed_y);
        void Translate(Vector2 spd);

        // Cambia el tamaño de la textura
        void Size(float w, float h);

        //  Escala una textura
        void Scale(float w, float h);   // [Sobrecarga 1]       Escala los dos ejes por separado
        void Scale(float scale);        // [Sobrecarga 2]       Escala ambos ejes a la vez

        // Rota el Texture
        void Rotate(double degrees);

        // Fija el centro de la textura (Posicion relativa desde el centro REAL)
        void SetCenter(float x, float y);

        // Si se da el caso, capa en la camara virtual 2D donde esta ubicada.
        int32_t camera_layer;

        // Datos de la textura virtual para la camara 2D
        struct {
            Size2I64 texture_size;              // Tamaño virtual de la textura
            Vector2I64 loop;                    // Punto de loop del scroll de la textura (segun tamaño virtual)
            Vector2 scroll;                     // Velocidad del autoscroll
            Vector2 offset;                     // Guarda el acumulado del autoscroll
            bool enabled;                       // El fondo lo gestiona la camara en modo virtual
        } virtual_texture;


        // Si la textura no esta enlazada, borra el contenido
        void ClearContent(uint32_t rgba = 0x00000000);



    // Private
    private:

        // Propiedades internas de la textura
        Size2 original;         			// Tamaño original
        bool linked;                        // Textura creada o enlazada
        void DefaultValues();               // Carga los parametros por defecto

        // Crea el objeto que contiene la textura
        void CreateTexture(
            NGN_TextureData* texture,       // Objeto de la clase Texture Data
            int32_t position_x,             // Posicion X inicial (oculto por defecto)
            int32_t position_y,             // Posicion Y inicial (oculto por defecto)
            uint32_t texture_width,         // Ancho de la textura (por defecto, el de la textura)
            uint32_t texture_height         // Altura de la textura (por defecto, la de la textura)
        );

};



#endif // NGN_TEXTUREBG_H_INCLUDED
