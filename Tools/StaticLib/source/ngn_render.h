/******************************************************************************

    N'gine Lib for C++
    *** Version 0.11.0-a ***
    Gestion del Renderer de SDL

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

        // Constructor
        NGN_Render();

        // Destructor
        ~NGN_Render();

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

        bool render2texture;        // Flag de render to texture
        NGN_Texture* rend2text;     // Textura de destino del render

};



#endif // NGN_RENDER_H_INCLUDED
