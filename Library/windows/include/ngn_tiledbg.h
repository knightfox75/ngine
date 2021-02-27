/******************************************************************************

    N'gine Lib for C++
    *** Version 1.4.0-beta ***
    Fondos Tileados

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
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

        // Constructor
        NGN_TiledBg(
                    NGN_TiledBgData* bg,           // Datos del fondo
                    int32_t position_x = 0,        // Posicion X del fondo [0 por defecto]
                    int32_t position_y = 0         // Posicion Y del fondo [0 por defecto]
                    );

        // Destructor
        ~NGN_TiledBg();

        // Propiedades del fondo (publicas)
        NGN_TiledBgData* bgdata;            // Tiles + Mapa del fondo
        Vector2 position;                   // Posicion del fondo
        float width;                        // Tamaño
        float height;

        Vector2 center;                     // Centro del fondo para su rotacion

        double rotation;                    // Rotacion del fondo

        bool visible;                       // Visibilidad
        int32_t alpha;                      // Nivel de alpha
        SDL_BlendMode blend_mode;           // Modo de mezcla de color

        Vector2 scale;                      // Escala del fondo

        SDL_Texture* backbuffer;            // Backbufer del fondo para su renderizado

        // Tamaño del backbuffer
        Size2I32 bb_size;

        // Posicion en el ultimo frame
        Vector2 last_position;

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

    // Private
    private:

};



#endif // NGN_TILEDBG_H_INCLUDED
