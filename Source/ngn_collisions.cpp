/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0-wip_0x03 ***
    Sistema de colisiones

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2025 by Cesar Rincon "NightFox"

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
#include <cmath>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/*** Puntero de la instancia a nullptr ***/
NGN_Collisions* NGN_Collisions::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
NGN_Collisions* NGN_Collisions::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Collisions();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Collisions::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Contructor ***/
NGN_Collisions::NGN_Collisions() {
}



/*** Destructor ***/
NGN_Collisions::~NGN_Collisions() {
}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Collisions::BootUp() {
}



/*** Consulta el color de un pixel del mapa de colisiones ***/
uint32_t NGN_Collisions::GetPixel(NGN_CollisionMapData* cmap, int32_t position_x, int32_t position_y) {

    // Proteccion de errores (pixel fuera del mapa)
    if (
        (cmap == nullptr)
        ||
        (position_x < 0)
        ||
        (position_y < 0)
        ||
        (position_x >= (int32_t)cmap->header.width)
        ||
        (position_y >= (int32_t)cmap->header.height)
        ) return 0x00000000;

    // Obtencion del tile/idx de pixel
    uint32_t offset = 0;
    uint32_t t = 0;
    // Segun si el tamaño de tile se ha detectado como potencia de 2 o no
    if (cmap->is_power_of_two) {
        // Calcula el offset en el buffer del mapa
        offset = (((position_y >> cmap->bit_shift) * cmap->tiles_row_width) + (position_x >> cmap->bit_shift));
        // Obten el tile
        t = cmap->tmap[offset];
        // Calcula el offset en el buffer de tiles
        offset = ((t * cmap->tile_bytes) + (((position_y & cmap->bit_mask) << cmap->bit_shift) + (position_x & cmap->bit_mask)));
    } else {
        // Calcula el offset en el buffer del mapa
        offset = (((position_y / cmap->header.tile_size) * cmap->tiles_row_width) + (position_x / cmap->header.tile_size));
        // Obten el tile
        t = cmap->tmap[offset];
        // Calcula el offset en el buffer de tiles
        offset = ((t * cmap->tile_bytes) + (((position_y % cmap->header.tile_size) * cmap->header.tile_size) + (position_x % cmap->header.tile_size)));
    }

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

    if (cmap != nullptr) {
        temp.width = cmap->header.width;
        temp.height = cmap->header.height;
    }
    return temp;

}



/*** Devuelve si hay colision entre las cajas de 2 sprites ***/
bool NGN_Collisions::HitBox(NGN_Sprite* spr1, NGN_Sprite* spr2) {

    // Proteccion de errores
    if ((spr1 == nullptr) || (spr2 == nullptr)) return false;

    // Almacena el resultado
    bool r = false;

    // Variables para los calculos
    float x1, y1, w1, h1, x2, y2, w2, h2;

    // Fase 1 - Si estan habilitadas, colision entre las cajas principales
    if (spr1->box_enabled && spr2->box_enabled) {
        // Introduce los datos
        x1 = (spr1->position.x + spr1->box.offset.x);
        y1 = (spr1->position.y + spr1->box.offset.y);
        x2 = (spr2->position.x + spr2->box.offset.x);
        y2 = (spr2->position.y + spr2->box.offset.y);
        w1 = spr1->box.width;
        h1 = spr1->box.height;
        w2 = spr2->box.width;
        h2 = spr2->box.height;
        // Colision entre las cajas principales
        r |= CheckBoxColliders(x1, y1, w1, h1, x2, y2, w2, h2);
    }

    // Fase 2 - Si la caja principal del Sprite 1 esta habilitada, verifica la colision con los colliders del Sprite 2, si estos existen
    if (spr1->box_enabled && (spr2->colliders.size() > 0)) {
        // Introduce los datos del Sprite 1
        x1 = (spr1->position.x + spr1->box.offset.x);
        y1 = (spr1->position.y + spr1->box.offset.y);
        w1 = spr1->box.width;
        h1 = spr1->box.height;
        // Recorre el vector de colliders del Sprite 2
        for (uint32_t i = 0; i < spr2->colliders.size(); i ++) {
            // Si esta habilitado
            if (spr2->colliders[i].enabled) {
                // Introduce los datos del collider
                x2 = (spr2->position.x + spr2->colliders[i].offset.x);
                y2 = (spr2->position.y + spr2->colliders[i].offset.y);
                w2 = spr2->colliders[i].width;
                h2 = spr2->colliders[i].height;
                // Verifica la colision
                r |= CheckBoxColliders(x1, y1, w1, h1, x2, y2, w2, h2);
            }
        }
    }

    // Fase 3 - Si la caja principal del Sprite 2 esta habilitada, verifica la colision con los colliders del Sprite 1, si estos existen
    if (spr2->box_enabled && (spr1->colliders.size() > 0)) {
        // Introduce los datos del Sprite 2
        x2 = (spr2->position.x + spr2->box.offset.x);
        y2 = (spr2->position.y + spr2->box.offset.y);
        w2 = spr2->box.width;
        h2 = spr2->box.height;
        // Recorre el vector de colliders del Sprite 1
        for (uint32_t i = 0; i < spr1->colliders.size(); i ++) {
            // Si esta habilitado
            if (spr1->colliders[i].enabled) {
                // Introduce los datos del collider
                x1 = (spr1->position.x + spr1->colliders[i].offset.x);
                y1 = (spr1->position.y + spr1->colliders[i].offset.y);
                w1 = spr1->colliders[i].width;
                h1 = spr1->colliders[i].height;
                // Verifica la colision
                r |= CheckBoxColliders(x1, y1, w1, h1, x2, y2, w2, h2);
            }
        }
    }

    // Fase 4 - Si ambos Sprites tienes colliders, comparalos entre ellos
    if ((spr1->colliders.size() > 0) && (spr2->colliders.size() > 0)) {
        // Recorre el vector de colliders del Sprite 1
        for (uint32_t n = 0; n < spr1->colliders.size(); n ++) {
            // Si esta habilitado
            if (spr1->colliders[n].enabled) {
                // Introduce los datos del collider
                x1 = (spr1->position.x + spr1->colliders[n].offset.x);
                y1 = (spr1->position.y + spr1->colliders[n].offset.y);
                w1 = spr1->colliders[n].width;
                h1 = spr1->colliders[n].height;
                // Recorre el vector de colliders del Sprite 2
                for (uint32_t i = 0; i < spr2->colliders.size(); i ++) {
                    // Si esta habilitado
                    if (spr2->colliders[i].enabled) {
                        // Introduce los datos del collider
                        x2 = (spr2->position.x + spr2->colliders[i].offset.x);
                        y2 = (spr2->position.y + spr2->colliders[i].offset.y);
                        w2 = spr2->colliders[i].width;
                        h2 = spr2->colliders[i].height;
                        // Verifica la colision
                        r |= CheckBoxColliders(x1, y1, w1, h1, x2, y2, w2, h2);
                    }
                }
            }
        }
    }

    // Devuelve el resultado
    return r;

}



/*** Algoritmo de colision por cajas ***/
bool NGN_Collisions::CheckBoxColliders(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {

    // Verifica si existe la colision
    return ((std::abs(x1 - x2) < ((w1 + w2) / 2.0f)) && (std::abs(y1 - y2) < ((h1 + h2) / 2.0f)));

}



/*** Devuelve si hay colision entre 2 sprites, con precision de pixel ***/
bool NGN_Collisions::PixelPerfect(NGN_Sprite* spr1, NGN_Sprite* spr2) {

    // Proteccion de errores
    if ((spr1 == nullptr) || (spr2 == nullptr)) return false;

    // Verifica que ambos sprites sean visibles
    if (!spr1->visible || !spr2->visible) return false;

    // Decide el metodo para la primera comprobacion
    if ((spr1->rotation != 0.0f) || (spr2->rotation != 0.0f)) {
        // Si alguno esta rotado, colision por circulos
        const int64_t r1 = (int64_t)spr1->GetSpriteRadius();
        const int64_t r2 = (int64_t)spr2->GetSpriteRadius();
        const int64_t rz = ((r1 + r2) * (r1 + r2));
        const int64_t dx = (int64_t)(spr1->position.x - spr2->position.x);
        const int64_t dy = (int64_t)(spr1->position.y - spr2->position.y);
        const int64_t dz = ((dx * dx) + (dy * dy));
        if (dz > rz) return false;
    } else {
        // Si no lo estan, colision por cajas
        const uint32_t dx = (uint32_t)(std::abs(spr1->position.x - spr2->position.x));
        const uint32_t dy = (uint32_t)(std::abs(spr1->position.y - spr2->position.y));
        const uint32_t sw = (uint32_t)((spr1->width / 2.0f) + (spr2->width / 2.0f));
        const uint32_t sh = (uint32_t)((spr1->height / 2.0f) + (spr2->height / 2.0f));
        if ((dx > sw) || (dy > sh)) return false;
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
        w1 = (std::sqrt((spr1->width * spr1->width) + (spr1->height * spr1->height)) / 2.0f);
        h1 = w1;
    }
    if (spr2->rotation == 0.0f) {
        w2 = (spr2->width / 2.0f);
        h2 = (spr2->height / 2.0f);
    } else {
        w2 = (std::sqrt((spr2->width * spr2->width) + (spr2->height * spr2->height)) / 2.0f);
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
    int32_t x = 0, y = 0;                       // Posicion del sprite en la textura
    // Sprite 1
    x = ((int32_t)spr1->position.x - xa);
    y = ((int32_t)spr1->position.y - ya);
    SDL_Surface* srf1 = nullptr;
    srf1 = RenderSpriteInSurface(spr1, x, y, area_w, area_h);
    // Sprite 2
    x = (spr2->position.x - xa);
    y = (spr2->position.y - ya);
    SDL_Surface* srf2 = nullptr;
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
    srf1 = nullptr;
    SDL_FreeSurface(srf2);
    srf2 = nullptr;

    // Resultado de la colision
    return collision;

}



/*** Consulta si has tocado un punto de un sprite (Raycast point) [1ra sobrecarga] ***/
bool NGN_Collisions::RaycastPoint(NGN_Sprite* spr, float position_x, float position_y) {

    // Sprite invalido
    if (spr == nullptr) return false;
    // Sprite invisible
    if (!spr->visible) return false;

    // Decide el metodo para la primera comprobacion
    if (spr->rotation != 0.0f) {
        // Si esta rotado, colision por circulos
        const uint64_t dx = std::abs(spr->position.x - position_x);
        const uint64_t dy = std::abs(spr->position.y - position_y);
        const uint64_t r = (uint64_t)spr->GetSpriteRadius();
        const uint64_t rz = (r * r);
        const uint64_t dz = ((dx * dx) + (dy * dy));
        if (dz > rz) return false;
    } else {
        // Si no lo estan, colision por cajas
        const uint32_t dx = std::abs(spr->position.x - position_x);
        const uint32_t dy = std::abs(spr->position.y - position_y);
        const uint32_t sw = (uint32_t)(spr->width / 2.0f);
        const uint32_t sh = (uint32_t)(spr->height / 2.0f);
        if ((dx > sw) || (dy > sh)) return false;
    }

    // Renderiza una textura de 1x1 para la comprobacion
    const int32_t px = (int32_t)(spr->position.x - position_x);
    const int32_t py = (int32_t)(spr->position.y - position_y);
    SDL_Surface* srf = nullptr;
    srf = RenderSpriteInSurface(spr, px, py, 1, 1);

    // Analiza si hay datos en el pixel solicitado
    bool collision = false;
    uint32_t* pixels = (uint32_t*)srf->pixels;
    if ((pixels[0] & 0x000000FF)) collision = true;

    // Paso de limpieza
    SDL_FreeSurface(srf);
    srf = nullptr;

    // Resultado final
    return collision;

}

/*** Consulta si has tocado un punto de un sprite (Raycast point) [1ra sobrecarga] ***/
bool NGN_Collisions::RaycastPoint(NGN_Sprite* spr, Vector2 position) {

    return RaycastPoint(spr, position.x, position.y);

}



/*** Renderiza un sprite en una superficie ***/
SDL_Surface* NGN_Collisions::RenderSpriteInSurface(NGN_Sprite* sprite, int32_t x, int32_t y, uint32_t w, uint32_t h) {

    // Crea una nueva textura como buffer del tamaño necesario
    SDL_Texture* backbuffer = nullptr;
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
    SDL_RenderFillRect(ngn->graphics->renderer, nullptr);

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
        SDL_SetTextureBlendMode(sprite->data->gfx[sprite->frame], SDL_BLENDMODE_NONE);
    } else {
        int32_t _alpha = sprite->alpha;
        if (_alpha < 0) {
            _alpha = 0;
        } else if (_alpha > 0xFF) {
            _alpha = 0xFF;
        }
        SDL_SetTextureBlendMode(sprite->data->gfx[sprite->frame], SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(sprite->data->gfx[sprite->frame], (uint8_t)_alpha);
    }

    /* Dibujado del sprite */
    // Define el area de origen
    SDL_Rect source = {
        0,                  // Posicion X
        0,                  // Posicion Y
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
    SDL_RenderCopyEx(ngn->graphics->renderer, sprite->data->gfx[sprite->frame], &source, &destination, (sprite->rotation + _rotation), _center, _flip);

    // Area a extraer
    SDL_Rect area = {0, 0, (int32_t)w, (int32_t)h};
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
    SDL_SetRenderTarget(ngn->graphics->renderer, nullptr);
    // Restaura el color y alpha del renderer
    SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0xFF);

    // Paso de limpieza
    delete _center;
    SDL_DestroyTexture(backbuffer);
    backbuffer = nullptr;

    // Devuelve la superficie creada
    return surface;

}
