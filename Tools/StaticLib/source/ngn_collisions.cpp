/******************************************************************************

    N'gine Lib for C++
    *** Version 0.1.0-alpha ***
    Sistema de colisiones

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



/*** Includes ***/

// C++
#include <cstdio>
#include <iostream>
#include <cmath>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"




/*** Contructor ***/
NGN_Collisions::NGN_Collisions() {
}



/*** Destructor ***/
NGN_Collisions::~NGN_Collisions() {
}



/*** Consulta el color de un pixel del mapa de colisiones ***/
uint32_t NGN_Collisions::GetPixel(NGN_CollisionMapData* cmap, int32_t position_x, int32_t position_y) {

    // Proteccion de errores (pixel fuera del mapa)
    if (
        (cmap == NULL)
        ||
        (position_x < 0)
        ||
        (position_y < 0)
        ||
        (position_x >= (int32_t)cmap->header.width)
        ||
        (position_y >= (int32_t)cmap->header.height)
        ) return 0x00000000;

    // Calcula el offset en el buffer del mapa
    uint32_t offset = (((position_y / cmap->header.tile_size) * cmap->tiles_row_width) + (position_x / cmap->header.tile_size));
    // Obten el tile
    uint32_t t = cmap->tmap[offset];
    // Calcula el offset en el buffer de tiles
    offset = ((t * cmap->tile_bytes) + (((position_y % cmap->header.tile_size) * cmap->header.tile_size) + (position_x % cmap->header.tile_size)));
    // Obten el pixel del tile
    uint8_t p = cmap->tiles[offset];
    // Devuelve el color del pixel segun la paleta
    return cmap->palette[p];

}


/*** Devuelve el tamaño de un mapa de colisiones ***/
Size2 NGN_Collisions::GetMapSize(NGN_CollisionMapData* cmap) {

    Size2 temp;
    temp.width = -1;
    temp.height = -1;

    if (cmap != NULL) {
        temp.width = cmap->header.width;
        temp.height = cmap->header.height;
    }
    return temp;

}



/*** Devuelve si hay colision entre las cajas de 2 sprites ***/
bool NGN_Collisions::HitBox(NGN_Sprite* spr1, NGN_Sprite* spr2) {

    // Proteccion de errores
    if ((spr1 == NULL) || (spr2 == NULL)) return false;

    // Calculos previos (distancia entre sprites)
    Vector2 distance;
    distance.x = std::abs(spr1->position.x - spr2->position.x);
    distance.y = std::abs(spr1->position.y - spr2->position.y);
    // Calculos previos (tamaño de la colision)
    Size2 collision_size;
    collision_size.width = ((spr1->box.width / 2.0f) + (spr2->box.width / 2.0f));
    collision_size.height = ((spr1->box.height / 2.0f) + (spr2->box.height / 2.0f));

    // Verifica si existe la colision
    if ((distance.x <= collision_size.width) && (distance.y <= collision_size.height)) {
        return true;
    } else {
        return false;
    }

}
