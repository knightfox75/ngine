/******************************************************************************

    N'gine Lib for C++
    *** Version 1.22.0+stable ***
    Definiciones de prototipos

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

    gfx = nullptr;
    width = 0;
    height = 0;

}

// Destructor
NGN_TextureData::~NGN_TextureData() {

    if (gfx != nullptr) SDL_DestroyTexture(gfx);
    gfx = nullptr;

}



/*** Prototipo de datos del FONDO TILEADO [NGN_TiledBgData] ***/

// Constructor
NGN_TiledBgData::NGN_TiledBgData() {

    tiles_atlas = nullptr;
    tmap.clear();

}

// Destructor
NGN_TiledBgData::~NGN_TiledBgData() {

    if (tiles_atlas != nullptr) {
        SDL_DestroyTexture(tiles_atlas);
        tiles_atlas = nullptr;
    }
    tmap.clear();

}



/*** Prototipo de datos de los SPRITES ANIMADOS [NGN_SpriteData] ***/

// Constructor
NGN_SpriteData::NGN_SpriteData() {

    gfx.clear();

}

// Destructor
NGN_SpriteData::~NGN_SpriteData() {

    for (uint32_t i = 0; i < gfx.size(); i ++) {
        if (gfx[i] != nullptr) SDL_DestroyTexture(gfx[i]);
        gfx[i] = nullptr;
    }
    gfx.clear();

}



/*** Prototipo de datos de los MAPAS DE COLISIONES [NGN_CollisionMapData] ***/

// Constructor
NGN_CollisionMapData::NGN_CollisionMapData() {

    palette.clear();
    tiles.clear();
    tmap.clear();

    tile_bytes = 0;                     // Tamaño de cada tile en bytes
    tiles_row_width = 0;                // Tamaño en pixeles de una fila de tiles

    is_power_of_two = false;            // El tamaño de tile es potencia de 2
    bit_shift = 0;                      // De ser potencia de 2, que bitshift hay que utilizar en las operaciones de division
    bit_mask = 0;                       // De ser potencia de 2, que bitmask hay que aplicar para el calculo de los modulos


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

    characters_atlas = nullptr;

    cell_size = {0, 0};

    char_size.clear();
    char_size.resize(256, {0, 0});

    atlas_size = {0, 0};

    height = 0;
    line_spacing = 0;

}

// Destructor
NGN_TextFont::~NGN_TextFont() {

    if (characters_atlas != nullptr) {
        SDL_DestroyTexture(characters_atlas);
        characters_atlas = nullptr;
    }

    char_size.clear();

}



/*** Prototipo de datos de las imagenes en RAW [NGN_RawImage] ***/

// Constructor
NGN_RawImage::NGN_RawImage() {

    pixels.clear();
    width = 0;
    height = 0;

}

// Destructor
NGN_RawImage::~NGN_RawImage() {

    pixels.clear();

}



/*** Prototipo de datos de una superficie [NGN_RendererSurface] ***/

// Constructor
NGN_RendererSurface::NGN_RendererSurface() {

    // Surface
    surface = nullptr;

    // Guarda el tamaño
    width = -1;
    height = -1;

}

// Destructor
NGN_RendererSurface::~NGN_RendererSurface() {

    // Elimina el surface
    SDL_FreeSurface(surface);
    surface = nullptr;

}



/*** Prototipo de datos de video ***/

// Constructor
NGN_VideoData::NGN_VideoData() {

    // Metadatos del stream
    width = 0;
    height = 0;
    framerate = 0.0;

    // Origen del archivo
    filepath = "";
    from_package = false;
    package_offset = 0;
    package_size = 0;

}

// Destructor de la clase
NGN_VideoData::~NGN_VideoData() {
}