/******************************************************************************

    N'gine Lib for C++
    *** Version 1.8.0-stable ***
    Sistema de colisiones

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2022 by Cesar Rincon "NightFox"
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



#ifndef NGN_COLLISIONS_H_INCLUDED
#define NGN_COLLISIONS_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11



/*** Definicion de la clase ***/
class NGN_Collisions {

    // Public
    public:

        // Constructor
        NGN_Collisions();

        // Destructor
        ~NGN_Collisions();

        // Consulta el color de un pixel del mapa de colisiones
        uint32_t GetPixel(NGN_CollisionMapData* cmap, int32_t position_x, int32_t position_y);

        // Devuelve el tamaño de un mapa de colisiones
        Size2 GetMapSize(NGN_CollisionMapData* cmap);

        // Consulta si hay colision por cajas entre 2 sprites
        bool HitBox(NGN_Sprite* spr1, NGN_Sprite* spr2);

        // Consulta si hay colision por "pixel perfect" entre 2 sprites
        bool PixelPerfect(NGN_Sprite* spr1, NGN_Sprite* spr2);

        // Consulta si has tocado un punto de un sprite (Raycast point)
        bool RaycastPoint(NGN_Sprite* spr, float position_x, float position_y);
        bool RaycastPoint(NGN_Sprite* spr, Vector2 position);


    // Private
    private:

        // Algoritmo de colision por cajas
        bool CheckColliders(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

        // Deteccion de colisiones entre sprites por "pixel perfect"
        SDL_Surface* RenderSpriteInSurface(
            NGN_Sprite* sprite,     // Sprite a renderizar
            int32_t x,              // Posicion
            int32_t y,
            uint32_t w,             // Tamaño del buffer
            uint32_t h
            );


};


#endif // NGN_COLLISIONS_H_INCLUDED
