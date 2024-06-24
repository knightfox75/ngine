/******************************************************************************

    N'gine Lib for C++
    *** Version 1.17.0-wip_0x02 ***
    Gestion del Renderer de SDL

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



#ifndef NGN_RENDER_H_INCLUDED
#define NGN_RENDER_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
// Includes de la libreria
#include "ngn_sprite.h"
#include "ngn_tiledbg.h"
#include "ngn_texture.h"
#include "ngn_text_layer.h"
#include "ngn_canvas.h"



/*** Definicion de la clase ***/
class NGN_Render {

    // Public
    public:

        // Devuelve la instancia
        static NGN_Render* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


        // Dibuja una textura en la surface del renderer
        void Texture(NGN_Texture* texture, float position_x = (float)NGN_DEFAULT_VALUE, float position_y = (float)NGN_DEFAULT_VALUE);

        // Dibuja un Sprite
        void Sprite(NGN_Sprite* sprite, float position_x = (float)NGN_DEFAULT_VALUE, float position_y = (float)NGN_DEFAULT_VALUE);

        // Dibuja un fondo de Tiles
        void TiledBg(NGN_TiledBg* bg);

        // Dibuja una capa de texto
        void TextLayer(NGN_TextLayer* layer, float position_x = (float)NGN_DEFAULT_VALUE, float position_y = (float)NGN_DEFAULT_VALUE);

        // Dibuja un canvas
        void Canvas(NGN_Canvas* canvas, float position_x = (float)NGN_DEFAULT_VALUE, float position_y = (float)NGN_DEFAULT_VALUE);

        // Render de los viewports
        void Viewports();

        // Establece una textura como destino del render
        void RenderToTexture(NGN_Texture* texture);

        // Establece la pantalla como destino del render
        void RenderToScreen();

        // Fija la textura al renderer en caso de estar activo
        void SetRenderToTextureState();
        // Opten el estado del render to texture;
        bool GetRenderToTextureState();


    // Private
    private:

        // Constructor
        NGN_Render();

        // Destructor
        ~NGN_Render();

        // Puntero de memoria a la instancia
        static NGN_Render* instance;


        bool render2texture;        // Flag de render to texture
        NGN_Texture* rend2text;     // Textura de destino del render

        /*** Metodos para el dibujado de fondos de tiles ***/
        void TiledBgTiles(NGN_TiledBg* bg);
        void TiledBgTransform(NGN_TiledBg* bg);

        /*** Propiedades comunes ***/
        SDL_RendererFlip _flip;             // Estado del flip
        SDL_Point* _center;                 // Centro de la rotacion
        int32_t _x, _y;                     // Coordenadas
        int32_t _width, _height;            // Medidas
        double _rotation;                   // Rotacion
        SDL_Rect _source, _destination;     // Areas de origen y destino del render
        int32_t _alpha;                     // Nivel del alpha

};



#endif // NGN_RENDER_H_INCLUDED
