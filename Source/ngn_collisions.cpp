/******************************************************************************

    N'gine Lib for C++
    *** Version 0.4.4-alpha ***
    Sistema de colisiones

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
    distance.x = (int32_t)(std::abs(spr1->position.x - spr2->position.x));
    distance.y = (int32_t)(std::abs(spr1->position.y - spr2->position.y));
    // Calculos previos (tamaño de la colision)
    Size2 collision_size;
    collision_size.width = (int32_t)((spr1->box.width / 2.0f) + (spr2->box.width / 2.0f));
    collision_size.height = (int32_t)((spr1->box.height / 2.0f) + (spr2->box.height / 2.0f));

    // Verifica si existe la colision
    if ((distance.x <= collision_size.width) && (distance.y <= collision_size.height)) {
        return true;
    } else {
        return false;
    }

}



/*** Devuelve si hay colision entre 2 sprites, con precision de pixel ***/
bool NGN_Collisions::PixelPerfect(NGN_Sprite* spr1, NGN_Sprite* spr2) {

    // Proteccion de errores
    if ((spr1 == NULL) || (spr2 == NULL)) return false;

    // Verifica que ambos sprites sean visibles
    if (!spr1->visible || !spr2->visible) return false;

    // Variables
    int32_t r1 = 0, r2 = 0, dist = 0;   // Colision por circulos
    Vector2 distance;                   // Colision por cajas
    Size2 collision_size;

    // Decide el metodo para la primera comprobacion
    if ((spr1->rotation != 0.0f) || (spr2->rotation != 0.0f)) {
        // Si alguno esta rotado, colision por circulos
        r1 = (int32_t)(std::sqrt(std::pow(spr1->width, 2.0f) + std::pow(spr1->height, 2.0f)) / 2.0f);
        r2 = (int32_t)(std::sqrt(std::pow(spr2->width, 2.0f) + std::pow(spr2->height, 2.0f)) / 2.0f);
        dist = (int32_t)std::sqrt(std::pow((spr1->position.x - spr2->position.x), 2.0f) + std::pow((spr1->position.y - spr2->position.y), 2.0f));
        if (dist > (r1 + r2)) return false;
    } else {
        // Si no lo estan, colision por cajas
        distance.x = (int32_t)(std::abs(spr1->position.x - spr2->position.x));
        distance.y = (int32_t)(std::abs(spr1->position.y - spr2->position.y));
        collision_size.width = (int32_t)((spr1->width / 2.0f) + (spr2->width / 2.0f));
        collision_size.height = (int32_t)((spr1->height / 2.0f) + (spr2->height / 2.0f));
        if ((distance.x > collision_size.width) || (distance.y > collision_size.height)) return false;
    }

    // Calculo de la zona de interseccion
    int32_t xa = 0, ya = 0, xb = 0, yb = 0;
    int32_t vx[4], vy[4];
    // Calculo del tamaño segun la rotacion
    float w1 = 0.0f, w2 = 0.0f, h1 = 0.0f, h2 = 0.0f;
    if (spr1->rotation == 0.0f) {
        w1 = (spr1->width / 2.0f);
        h1 = (spr1->height / 2.0f);
    } else {
        w1 = (std::sqrt(std::pow(spr1->width, 2.0f) + std::pow(spr1->height, 2.0f)) / 2.0f);
        h1 = w1;
    }
    if (spr2->rotation == 0.0f) {
        w2 = (spr2->width / 2.0f);
        h2 = (spr2->height / 2.0f);
    } else {
        w2 = (std::sqrt(std::pow(spr2->width, 2.0f) + std::pow(spr2->height, 2.0f)) / 2.0f);
        h2 = w2;
    }

    // Lineas verticales
    vx[0] = (int32_t)(spr1->position.x - w1);
    vx[1] = (int32_t)(spr2->position.x - w2);
    vx[2] = (int32_t)(spr1->position.x + w1);
    vx[3] = (int32_t)(spr2->position.x + w2);
    // Ordena las lineas verticales (indice mas bajo, valor mas bajo)
    for (uint32_t n = 0; n < 3; n ++) {
        for (uint32_t i = (n + 1); i < 4; i ++) {
            if (vx[i] < vx[n]) {
                std::swap(vx[i], vx[n]);
            }
        }
    }
    // Lineas horizontales
    vy[0] = (int32_t)(spr1->position.y - h1);
    vy[1] = (int32_t)(spr2->position.y - h2);
    vy[2] = (int32_t)(spr1->position.y + h1);
    vy[3] = (int32_t)(spr2->position.y + h2);
    // Ordena las lineas horizontales (indice mas bajo, valor mas bajo)
    for (uint32_t n = 0; n < 3; n ++) {
        for (uint32_t i = (n + 1); i < 4; i ++) {
            if (vy[i] < vy[n]) {
                std::swap(vy[i], vy[n]);
            }
        }
    }
    // Asigna los puntos de la interseccion
    xa = vx[1]; xb = vx[2];
    ya = vy[1]; yb = vy[2];

    // Renderiza las dos subtexturas para la comparacion
    int32_t area_w = std::abs(xa - xb) + 1;     // Calcula el tamaño de los buffers
    int32_t area_h = std::abs(ya - yb) + 1;
    int32_t x = 0, y = 0;                   // Posicion del sprite en la textura
    // Sprite 1
    x = ((int32_t)spr1->position.x - xa);
    y = ((int32_t)spr1->position.y - ya);
    SDL_Surface* srf1 = NULL;
    srf1 = RenderSpriteInSurface(spr1, x, y, area_w, area_h);
    // Sprite 2
    x = (spr2->position.x - xa);
    y = (spr2->position.y - ya);
    SDL_Surface* srf2 = NULL;
    srf2 = RenderSpriteInSurface(spr2, x, y, area_w, area_h);

    // Deteccion de la colision
    bool collision = false;
    uint32_t* pixels1 = (uint32_t*)srf1->pixels;
    uint32_t* pixels2 = (uint32_t*)srf2->pixels;
    uint32_t _size = (area_w * area_h);
    for (uint32_t i = 0; i < _size; i ++) {
        if ((pixels1[i] & 0x000000FF) & (pixels2[i] & 0x000000FF)) {
            collision = true;
            break;
        }
    }

    // Paso de limpieza
    SDL_FreeSurface(srf1);
    srf1 = NULL;
    SDL_FreeSurface(srf2);
    srf2 = NULL;

    // Resultado de la colision
    return collision;

}



/*** Renderiza un sprite en una superficie ***/
SDL_Surface* NGN_Collisions::RenderSpriteInSurface(NGN_Sprite* sprite, int32_t x, int32_t y, int32_t w, int32_t h) {

    // Crea una nueva textura como buffer del tamaño necesario
    SDL_Texture* backbuffer = NULL;
    backbuffer = SDL_CreateTexture(
        ngn->graphics->renderer,        // Renderer
        SDL_PIXELFORMAT_BGRA8888,       // Formato del pixel
        SDL_TEXTUREACCESS_TARGET,       // Textura como destino del renderer (modo edicion)
        w,                              // Ancho de la textura
        h                               // Alto de la textura
        );

    // Informa al renderer que la textura "backbuffer" es su destino
    SDL_SetRenderTarget(ngn->graphics->renderer, backbuffer);
    // Borra el contenido de la textura actual (con el fondo listo para el blending)
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(backbuffer, 0xFF);
    SDL_RenderFillRect(ngn->graphics->renderer, NULL);

    // Calculos
    int32_t _x, _y;
    _x = (int32_t)(x - (sprite->width / 2.0f));
    _y = (int32_t)(y - (sprite->height / 2.0f));

    int32_t _width = (int32_t)sprite->width;
    int32_t _height = (int32_t)sprite->height;
    double _rotation = 0.0f;
    SDL_RendererFlip _flip;

    // Centro de la rotacion
    SDL_Point* _center = new SDL_Point();
    _center->x = ((sprite->width / 2.0f) + sprite->center.x);
    _center->y = ((sprite->height / 2.0f) + sprite->center.y);

    // Flip
    if (!sprite->flip_h && !sprite->flip_v) {
        _flip = SDL_FLIP_NONE;
    } else if (sprite->flip_h && !sprite->flip_v) {
        _flip = SDL_FLIP_HORIZONTAL;
    } else if (!sprite->flip_h && sprite->flip_v) {
        _flip = SDL_FLIP_VERTICAL;
    } else {
        _flip = SDL_FLIP_NONE;
        _rotation = 180.0f;
    }

    // Alpha
    if (sprite->alpha == -1) {
        // Deshabilita el canal Alpha
        SDL_SetTextureBlendMode(sprite->data->gfx, SDL_BLENDMODE_NONE);
    } else {
        int32_t _alpha = sprite->alpha;
        if (_alpha < 0) {
            _alpha = 0;
        } else if (_alpha > 0xFF) {
            _alpha = 0xFF;
        }
        SDL_SetTextureBlendMode(sprite->data->gfx, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(sprite->data->gfx, (Uint8)_alpha);
    }

    /* Dibujado del sprite */
    // Define el area de origen
    int32_t frame = (sprite->frame * sprite->data->header.frame_height);
    SDL_Rect source = {
        0,                  // Posicion X
        frame,              // Posicion Y
        (int32_t)sprite->data->header.frame_width,      // Ancho
        (int32_t)sprite->data->header.frame_height      // Alto
    };
    // Define el area de destino
    SDL_Rect destination = {
        _x,         // Posicion X
        _y,         // Posicion Y
        _width,     // Ancho
        _height     // Alto
    };

    // Renderiza la textura
    SDL_RenderCopyEx(ngn->graphics->renderer, sprite->data->gfx, &source, &destination, (sprite->rotation + _rotation), _center, _flip);

    // Area a extraer
    SDL_Rect area = {0, 0, w, h};
    // Superficie para guardar los pixeles extraidos
    SDL_Surface *surface = SDL_CreateRGBSurface(
        0,              // Flag (siempre 0)
        w,              // Ancho
        h,              // Alto
        32,             // Bits por pixels
        0x000000FF,     // Mascara R
        0x0000FF00,     // Mascara G
        0x00FF0000,     // Mascara B
        0xFF000000      // Mascara A
         );
    // Extrae los pixeles
    SDL_RenderReadPixels(ngn->graphics->renderer, &area, SDL_PIXELFORMAT_BGRA8888, surface->pixels, surface->pitch);

    // Cambia el destino del renderer a la pantalla
    SDL_SetRenderTarget(ngn->graphics->renderer, NULL);
    // Restaura el color y alpha del renderer
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0xFF);

    // Paso de limpieza
    delete _center;
    SDL_DestroyTexture(backbuffer);
    backbuffer = NULL;

    // Devuelve la superficie creada
    return surface;

}
