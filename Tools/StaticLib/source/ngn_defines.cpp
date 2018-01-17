/******************************************************************************

    N'gine Lib for C++
    *** Version 0.4.3-alpha ***
    Definiciones de prototipos

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
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

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/*** Prototipo de TEXTURA [NGN_TextureData] ***/

// Constructor
NGN_TextureData::NGN_TextureData() {

    gfx = NULL;
    width = 0;
    height = 0;

}

// Destructor
NGN_TextureData::~NGN_TextureData() {

    if (gfx != NULL) SDL_DestroyTexture(gfx);
    gfx = NULL;

}



/*** Prototipo de datos del FONDO TILEADO [NGN_TiledBgData] ***/

// Constructor
NGN_TiledBgData::NGN_TiledBgData() {

    tiles = NULL;

}

// Destructor
NGN_TiledBgData::~NGN_TiledBgData() {

    if (tiles != NULL) SDL_DestroyTexture(tiles);
    tiles = NULL;
    tmap.clear();

}



/*** Prototipo de datos de los SPRITES ANIMADOS [NGN_SpriteData] ***/

// Constructor
NGN_SpriteData::NGN_SpriteData() {

    gfx = NULL;

}

// Destructor
NGN_SpriteData::~NGN_SpriteData() {

    if (gfx != NULL) SDL_DestroyTexture(gfx);
    gfx = NULL;

}



/*** Prototipo de datos de los MAPAS DE COLISIONES [NGN_CollisionMapData] ***/

// Constructor
NGN_CollisionMapData::NGN_CollisionMapData() {
}

// Destructor
NGN_CollisionMapData::~NGN_CollisionMapData() {

    palette.clear();
    tiles.clear();
    tmap.clear();

}



/*** Prototipo de datos de los CLIPS DE SONIDO [NGN_AudioClipData] ***/

// Constructor
NGN_AudioClipData::NGN_AudioClipData() {
}


// Destructor
NGN_AudioClipData::~NGN_AudioClipData() {
}



/*** Prototipo de datos de las fuentes de texto ***/
// Constructor
NGN_TextFont::NGN_TextFont() {

    character.reserve(256);
    for (uint32_t i = 0; i < character.capacity(); i ++) {
        character[i] = NULL;
        character[i] = new NGN_TextureData();
    }

    height = 0;
    line_spacing = 0;

}

// Destructor
NGN_TextFont::~NGN_TextFont() {

    for (uint32_t i = 0; i < character.capacity(); i ++) {
        delete character[i]; character[i] = NULL;
    }

    character.clear();

}


