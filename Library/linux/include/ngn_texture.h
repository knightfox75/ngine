/******************************************************************************

    N'gine Lib for C++
    *** Version 0.2.1-alpha ***
    Fondos con texturas

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2017 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
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



#ifndef NGN_TEXTURE_H_INCLUDED
#define NGN_TEXTURE_H_INCLUDED



/*** Includes ***/

// C++
#include <vector>
#include <string>

// Includes de la libreria
#include "ngn_defines.h"



/*** Definicion de la clase ***/
class NGN_Texture {

   // Public
    public:

        // Contructor
        NGN_Texture(
                    NGN_TextureData* texture,               // Objeto de la clase Texture Data
                    int32_t position_x = DEFAULT_VALUE,         // Posicion X inicial (oculto por defecto)
                    int32_t position_y = DEFAULT_VALUE,         // Posicion Y inicial (oculto por defecto)
                    int32_t texture_width = DEFAULT_VALUE,      // Ancho de la textura (por defecto, el de la textura)
                    int32_t texture_height = DEFAULT_VALUE      // Altura de la textura (por defecto, la de la textura)
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
        int32_t alpha;                  // Nivel de alpha

        double rotation;            // Rotacion de la textura

        bool flip_h;                // Flip de la textura
        bool flip_v;

        // Posiciona la textura
        void Position(float position_x, float position_y);

        // Mueve la textura
        void Translate(float speed_x, float speed_y);

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



    // Private
    private:

        // Propiedades internas de la textura
        Size2 original;         			// Tamaño original

};



#endif // NGN_TEXTUREBG_H_INCLUDED
