/******************************************************************************

    N'gine Lib for C++
    *** Version 1.18.0-wip_0x01 ***
    Gestion del Renderer de SDL

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2024 by Cesar Rincon "NightFox"

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



/*** Puntero de la instancia a NULL ***/
NGN_Render* NGN_Render::instance = NULL;



/*** Metodo para crear/obtener la instancia ***/
NGN_Render* NGN_Render::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Render();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Render::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = NULL;
    }

}



/*** Contructor ***/
NGN_Render::NGN_Render() {

    // Parametros iniciales
    render2texture = false;
    rend2text = NULL;
    _center = new SDL_Point();
    _x = _y = 0;
    _width = _height = 0;
    _flip = SDL_FLIP_NONE;
    _rotation = 0.0f;
    _source = {0, 0, 0, 0};
    _destination = {0, 0, 0, 0};
    _alpha = 0x00;

}



/*** Destructor ***/
NGN_Render::~NGN_Render() {

    rend2text = NULL;
    delete _center;

}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Render::BootUp() {
}



void NGN_Render::Texture(NGN_Texture* texture, float position_x, float position_y) {

    // Si no debe dibujarse...
    if (!texture->visible) return;

    // Calculos
    if ((position_x == (float)NGN_DEFAULT_VALUE) && (position_y == (float)NGN_DEFAULT_VALUE)) {
        texture->screen = texture->position;
    } else {
        texture->screen.x = position_x;
        texture->screen.y = position_y;
    }
    _x = (int32_t)(texture->screen.x);
    _y = (int32_t)(texture->screen.y);

    _width = (int32_t)texture->width;
    _height = (int32_t)texture->height;
    _rotation = 0.0f;

    // Centro de la rotacion
    _center->x = ((texture->width / 2.0f) + texture->center.x);
    _center->y = ((texture->height / 2.0f) + texture->center.y);

    // Flip
    if (!texture->flip_h && !texture->flip_v) {
        _flip = SDL_FLIP_NONE;
    } else if (texture->flip_h && !texture->flip_v) {
        _flip = SDL_FLIP_HORIZONTAL;
    } else if (!texture->flip_h && texture->flip_v) {
        _flip = SDL_FLIP_VERTICAL;
    } else {
        _flip = SDL_FLIP_NONE;
        _rotation = 180.0f;
    }

    // Alpha
    if (texture->alpha == -1) {
        // Deshabilita el canal Alpha
        SDL_SetTextureBlendMode(texture->data->gfx, SDL_BLENDMODE_NONE);
    } else {
        _alpha = texture->alpha;
        if (_alpha < 0) {
            _alpha = 0;
        } else if (_alpha > 0xFF) {
            _alpha = 0xFF;
        }
        SDL_SetTextureBlendMode(texture->data->gfx, texture->blend_mode);
        SDL_SetTextureAlphaMod(texture->data->gfx, (uint8_t)_alpha);
    }


    /* Dibujado de la textura */

    // Define el area de origen
    _source = {
        0,                  			    // Posicion X
        0,              				    // Posicion Y
        (int32_t)texture->data->width,      // Ancho
        (int32_t)texture->data->height      // Alto
    };

    // Define el area de destino
    _destination = {
        _x,         // Posicion X
        _y,         // Posicion Y
        _width,     // Ancho
        _height     // Alto
    };

    // Renderiza la textura
    if ((_rotation != 0.0f) || (_flip != SDL_FLIP_NONE) || (texture->rotation != 0.0f)) {
        SDL_RenderCopyEx(ngn->graphics->renderer, texture->data->gfx, &_source, &_destination, (texture->rotation + _rotation), _center, _flip);
    } else {
        SDL_RenderCopy(ngn->graphics->renderer, texture->data->gfx, &_source, &_destination);
    }

}



/*** Dibuja un Sprite en el Renderer ***/
void NGN_Render::Sprite(NGN_Sprite* sprite, float position_x, float position_y) {

    // Si no debe dibujarse...
    if (!sprite->visible) return;

    // Calculos
    if ((position_x == (float)NGN_DEFAULT_VALUE) && (position_y == (float)NGN_DEFAULT_VALUE)) {
        sprite->screen.x = (int32_t)sprite->position.x;
        sprite->screen.y = (int32_t)sprite->position.y;
    } else {
        sprite->screen.x = position_x;
        sprite->screen.y = position_y;
    }
    _x = (int32_t)(sprite->screen.x - (sprite->width / 2.0f));
    _y = (int32_t)(sprite->screen.y - (sprite->height / 2.0f));

    _width = (int32_t)sprite->width;
    _height = (int32_t)sprite->height;
    _rotation = 0.0f;

    // Centro de la rotacion
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
        _alpha = sprite->alpha;
        if (_alpha < 0) {
            _alpha = 0;
        } else if (_alpha > 0xFF) {
            _alpha = 0xFF;
        }
        SDL_SetTextureBlendMode(sprite->data->gfx[sprite->frame], sprite->blend_mode);
        SDL_SetTextureAlphaMod(sprite->data->gfx[sprite->frame], (uint8_t)_alpha);
    }


    /* Dibujado del sprite */

    // Define el area de origen
    _source = {
        0,              // Posicion X
        0,              // Posicion Y
        (int32_t)sprite->data->header.frame_width,      // Ancho
        (int32_t)sprite->data->header.frame_height      // Alto
    };

    // Define el area de destino
    _destination = {
        _x,         // Posicion X
        _y,         // Posicion Y
        _width,     // Ancho
        _height     // Alto
    };

    // Renderiza la textura
    if ((_rotation != 0.0f) || (_flip != SDL_FLIP_NONE) || (sprite->rotation != 0.0f)) {
        SDL_RenderCopyEx(ngn->graphics->renderer, sprite->data->gfx[sprite->frame], &_source, &_destination, (sprite->rotation + _rotation), _center, _flip);
    } else {
        SDL_RenderCopy(ngn->graphics->renderer, sprite->data->gfx[sprite->frame], &_source, &_destination);
    }

}



/*** Dibuja un fondo de Tiles en el Renderer ***/
void NGN_Render::TiledBg(NGN_TiledBg* bg) {

    // Si no debe dibujarse
    if (!bg->visible) return;

    // Decide si el render se realiza en modo rendimiento o precision
    if ((bg->rotation != 0.0f) || (bg->scale.x != 1.0f) || (bg->scale.y != 1.0f)) {
        TiledBgTransform(bg);
    } else {
        TiledBgTiles(bg);
    }

}



/*** Dibuja una capa de texto en el renderer ***/
void NGN_Render::TextLayer(NGN_TextLayer* layer, float position_x, float position_y) {

    // Si no debe dibujarse...
    if (!layer->visible) return;

    // Calculos
    if ((position_x != (float)NGN_DEFAULT_VALUE) && (position_y != (float)NGN_DEFAULT_VALUE)) {
        layer->position.x = position_x;
        layer->position.y = position_y;
    }
    _x = (int32_t)(layer->position.x);
    _y = (int32_t)(layer->position.y);

    _width = ((float)layer->width * layer->scale.width);
    _height = ((float)layer->height * layer->scale.height);
    _rotation = 0.0f;

    // Centro de la rotacion
    _center->x = (((float)_width / 2.0f) + layer->center.x);
    _center->y = (((float)_height / 2.0f) + layer->center.y);

    // Flip
    if (!layer->flip_h && !layer->flip_v) {
        _flip = SDL_FLIP_NONE;
    } else if (layer->flip_h && !layer->flip_v) {
        _flip = SDL_FLIP_HORIZONTAL;
    } else if (!layer->flip_h && layer->flip_v) {
        _flip = SDL_FLIP_VERTICAL;
    } else {
        _flip = SDL_FLIP_NONE;
        _rotation = 180.0f;
    }

    // Alpha
    if (layer->alpha == -1) {
        // Deshabilita el canal Alpha
        SDL_SetTextureBlendMode(layer->backbuffer, SDL_BLENDMODE_NONE);
    } else {
        _alpha = layer->alpha;
        if (_alpha < 0) {
            _alpha = 0;
        } else if (_alpha > 0xFF) {
            _alpha = 0xFF;
        }
        SDL_SetTextureBlendMode(layer->backbuffer, layer->blend_mode);
        SDL_SetTextureAlphaMod(layer->backbuffer, (uint8_t)_alpha);
    }


    /* Dibujado de la textura */

    // Define el area de origen
    _source = {
        0,                  		// Posicion X
        0,              			// Posicion Y
        layer->GetSize().width,     // Ancho
        layer->GetSize().height     // Alto
    };

    // Define el area de destino
    _destination = {
        _x,         // Posicion X
        _y,         // Posicion Y
        _width,     // Ancho
        _height     // Alto
    };

    // Renderiza la textura
    if ((_rotation != 0.0f) || (_flip != SDL_FLIP_NONE) || (layer->rotation != 0.0f)) {
        SDL_RenderCopyEx(ngn->graphics->renderer, layer->backbuffer, &_source, &_destination, (layer->rotation + _rotation), _center, _flip);
    } else {
        SDL_RenderCopy(ngn->graphics->renderer, layer->backbuffer, &_source, &_destination);
    }

}



/*** Dibuja un canvas en el renderer ***/
void NGN_Render::Canvas(NGN_Canvas* canvas, float position_x, float position_y) {

    // Si no debe dibujarse...
    if (!canvas->visible) return;

    // Si es necesario realiza el BLIT (convierte el buffer de pixeles en textura)
    canvas->Blit();

    // Calculos
    if ((position_x != (float)NGN_DEFAULT_VALUE) && (position_y != (float)NGN_DEFAULT_VALUE)) {
        canvas->position.x = position_x;
        canvas->position.y = position_y;
    }
    _x = (int32_t)(canvas->position.x);
    _y = (int32_t)(canvas->position.y);

    _width = ((float)canvas->width * canvas->scale.width);
    _height = ((float)canvas->height * canvas->scale.height);
    _rotation = 0.0f;

    // Centro de la rotacion
    _center->x = (((float)_width / 2.0f) + canvas->center.x);
    _center->y = (((float)_height / 2.0f) + canvas->center.y);

    // Flip
    if (!canvas->flip_h && !canvas->flip_v) {
        _flip = SDL_FLIP_NONE;
    } else if (canvas->flip_h && !canvas->flip_v) {
        _flip = SDL_FLIP_HORIZONTAL;
    } else if (!canvas->flip_h && canvas->flip_v) {
        _flip = SDL_FLIP_VERTICAL;
    } else {
        _flip = SDL_FLIP_NONE;
        _rotation = 180.0f;
    }

    // Alpha
    if (canvas->alpha == -1) {
        // Deshabilita el canal Alpha
        SDL_SetTextureBlendMode(canvas->backbuffer, SDL_BLENDMODE_NONE);
    } else {
        _alpha = canvas->alpha;
        if (_alpha < 0) {
            _alpha = 0;
        } else if (_alpha > 0xFF) {
            _alpha = 0xFF;
        }
        SDL_SetTextureBlendMode(canvas->backbuffer, canvas->blend_mode);
        SDL_SetTextureAlphaMod(canvas->backbuffer, (uint8_t)_alpha);
    }


    /* Dibujado de la textura */

    // Define el area de origen
    _source = {
        0,                  			// Posicion X
        0,              				// Posicion Y
        canvas->GetSize().width,        // Ancho
        canvas->GetSize().height        // Alto
    };

    // Define el area de destino
    _destination = {
        _x,         // Posicion X
        _y,         // Posicion Y
        _width,     // Ancho
        _height     // Alto
    };

    // Renderiza la textura
    if ((_rotation != 0.0f) || (_flip != SDL_FLIP_NONE) || (canvas->rotation != 0.0f)) {
        SDL_RenderCopyEx(ngn->graphics->renderer, canvas->backbuffer, &_source, &_destination, (canvas->rotation + _rotation), _center, _flip);
    } else {
        SDL_RenderCopy(ngn->graphics->renderer, canvas->backbuffer, &_source, &_destination);
    }

}



/*** Render de todos los viewports disponibles ***/
void NGN_Render::Viewports() {

    // Restaura el viewport por defecto
    ngn->graphics->DefaultViewport();

    for (uint8_t i = 0; i < ngn->graphics->viewport_list.capacity(); i ++) {
        if (ngn->graphics->viewport_list[i].available) {
            // Defines las areas
            _source = {
                0,
                0,
                ngn->graphics->viewport_list[i].render_w,
                ngn->graphics->viewport_list[i].render_h
            };
            _destination = {
                ngn->graphics->viewport_list[i].x,
                ngn->graphics->viewport_list[i].y,
                ngn->graphics->viewport_list[i].w,
                ngn->graphics->viewport_list[i].h
            };
            // Render de la textura al render principal
            SDL_RenderCopy(ngn->graphics->renderer, ngn->graphics->viewport_list[i].surface, &_source, &_destination);
        }
    }

    //std::cout << "Viewports: " << ngn->graphics->viewport_list.capacity() << std::endl;

}



/*** Selecciona una textura como destino del render ***/
void NGN_Render::RenderToTexture(NGN_Texture* texture) {

    // Si la textura es nula, sal
    if (!texture) return;

    // Asigna la textura
    rend2text = texture;
    render2texture = true;

    // Fija la textura como destino del render
    SDL_SetRenderTarget(ngn->graphics->renderer, rend2text->data->gfx);

}



/*** Selecciona la pantalla (o el viewport) como destino del renderer ***/
void NGN_Render::RenderToScreen() {

    render2texture = false;
    ngn->graphics->RenderToSelected();

}



/*** Fija la textura del render to texture ***/
void NGN_Render::SetRenderToTextureState() {

    if (!render2texture) return;
    if (!rend2text) {
        render2texture = false;
        return;
    }
    SDL_SetRenderTarget(ngn->graphics->renderer, rend2text->data->gfx);

}



/*** Opten el estado del render to texture ***/
bool NGN_Render::GetRenderToTextureState() {

    return render2texture;

}




/*** Dibuja un fondo de Tiles en el Renderer (Sin rotacion ni escala, optimizado para rendimiento) ***/
void NGN_Render::TiledBgTiles(NGN_TiledBg* bg) {

    // Rotacion y FLIP
    _rotation = 0.0f;
    _flip = SDL_FLIP_NONE;

    // Define las areas de origen y destino
    _source = {0, 0, 0, 0};
    _destination = {0, 0, 0, 0};

    // Area del render
    Size2I32 render_area;
    if (ngn->graphics->current_viewport < 0) {
        render_area.width = bg->bb_size.width;
        render_area.height = bg->bb_size.height;
    } else {
        render_area.width = (ngn->graphics->render_resolution.width + (bg->tile_size << 1));
        render_area.height = (ngn->graphics->render_resolution.height  + (bg->tile_size << 1));
    }

    // Analiza si es necesario redibujar la textura
    int32_t current_tile_x = std::floor(bg->position.x / (float)bg->tile_size);
    int32_t current_tile_y = std::floor(bg->position.y / (float)bg->tile_size);
    int32_t last_tile_x = std::floor(bg->last_position.x / (float)bg->tile_size);
    int32_t last_tile_y = std::floor(bg->last_position.y / (float)bg->tile_size);

    // Si hay desplazamiento o se ha forzado, redibuja la textura en el backbuffer
    if ((current_tile_x != last_tile_x) || (current_tile_y != last_tile_y) || (ngn->graphics->current_viewport != bg->last_viewport) || !bg->tile_mode || ngn->graphics->force_redaw) {

        // Calcula el centro del tile
        _center->x = _center->y = ((float)bg->tile_size / 2.0f);

        // Parametros del mapa
        int32_t map_x = 0;
        int32_t map_y = 0;
        int32_t map_w = std::floor(bg->map_size.width / bg->tile_size);
        int32_t map_h = std::floor(bg->map_size.height / bg->tile_size);

        // Tileset
        uint32_t id, tile, flip;      // Informacion

        // Coordenadas de dibujado
        int32_t px, py;

        // Parametros de lectura del tile
        _source.w = _source.h = bg->tile_size;
        // Parametros de escritura del tile
        _destination.w = _destination.h = bg->tile_size;

        // Informa al renderer que la textura "backbuffer" es su destino
        SDL_SetRenderTarget(ngn->graphics->renderer, bg->backbuffer);

        // Borra el contenido de la textura actual
        SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_SetTextureBlendMode(bg->backbuffer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(bg->backbuffer, 0x00);
        SDL_RenderFillRect(ngn->graphics->renderer, NULL);

        // Tamaño del area de dibujado
        int32_t tile_last_x = (std::floor(render_area.width / bg->tile_size) + 2);      // +2 soluciona el encage en resoluciones anamorficas
        int32_t tile_last_y = (std::floor(render_area.height / bg->tile_size) + 2);

        // Calcula el offset del dibujado de los tiles
        int32_t get_tile_x = 0, get_tile_y = 0;

        // Recorre la pantalla para dibujar el fondo
        for (map_y = 0; map_y < tile_last_y; map_y ++) {
            // Tile Y
            get_tile_y = (map_y + current_tile_y);
            // Si estas fuera del rango...
            if ((get_tile_y < 0) || (get_tile_y >= map_h)) continue;
            // Punto Y de dibujado
            py = (map_y * bg->tile_size);
            for (map_x = 0; map_x < tile_last_x; map_x ++) {
                // Tile X
                get_tile_x = (map_x + current_tile_x);
                // Si estas fuera del rango...
                if ((get_tile_x < 0) || (get_tile_x >= map_w)) continue;
                // Punto X de dibujado
                px = (map_x * bg->tile_size);
                // ID de tile
                id = (((get_tile_y * map_w) + get_tile_x) << 2);      // * 4 bytes
                // Numero de Tile
                tile = bg->bgdata->tmap[id];
                tile |= (bg->bgdata->tmap[(id + 1)] << 8);
                tile |= (bg->bgdata->tmap[(id + 2)] << 16);
                // Si el tile ES TRANSPARENTE (no es el primero del mapa), ignora el dibujado
                if (tile == 0) continue;
                // Orientacion del tile
                flip = bg->bgdata->tmap[(id + 3)];
                // Define el punto de destino
                _destination.x = px;
                _destination.y = py;
                // Parametros del FLIP del tile
                switch (flip) {
                    case 1:     // Normal       (BIT 0) [1 << 0]
                        SDL_RenderCopy(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination);
                        break;
                    case 2:     // Flip H       (BIT 1) [1 << 1]
                        _flip = SDL_FLIP_HORIZONTAL;
                        _rotation = 0.0f;
                        SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination, _rotation, _center, _flip);
                        break;
                    case 4:     // Flip V       (BIT 2) [1 << 2]
                        _flip = SDL_FLIP_VERTICAL;
                        _rotation = 0.0f;
                        SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination, _rotation, _center, _flip);
                        break;
                    case 8:     // 180�         (BIT 3) [1 << 3]
                        _flip = SDL_FLIP_NONE;
                        _rotation = 180.0f;
                        // Compensa el desplazamiento
                        _destination.x --;
                        _destination.y --;
                        SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination, _rotation, _center, _flip);
                        break;
                    case 16:     // 90� CW      (BIT 4) [1 << 4]
                        _flip = SDL_FLIP_NONE;
                        _rotation = 270.0f;
                        // Compensa el desplazamiento
                        _destination.y --;
                        SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination, _rotation, _center, _flip);
                        break;
                    case 32:     // 90� ACW     (BIT 5) [1 << 5]
                        _flip = SDL_FLIP_NONE;
                        _rotation = 90.0f;
                        // Compensa el desplazamiento
                        _destination.x --;
                        SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination, _rotation, _center, _flip);
                        break;
                }
            }
        }

        // Restaura el render al seleccionado
        ngn->graphics->RenderToSelected();

    }

    // Calculo del alpha del fondo
    _alpha = bg->alpha;
    if (_alpha < 0) {
        _alpha = 0;
    } else if (_alpha > 0xFF) {
        _alpha = 0xFF;
    }
    SDL_SetTextureBlendMode(bg->backbuffer, bg->blend_mode);
    SDL_SetTextureAlphaMod(bg->backbuffer, (uint8_t)_alpha);

    // Define los puntos de entrada y salida de la textura
    _source.x = 0; _source.y = 0; _source.w = render_area.width; _source.h = render_area.height;
    _destination.w = render_area.width;
    _destination.h = render_area.height;
    _destination.x = -((int32_t)bg->position.x % bg->tile_size);
    _destination.y = -((int32_t)bg->position.y % bg->tile_size);

    // Envia el backbuffer al renderer
    SDL_RenderCopy(ngn->graphics->renderer, bg->backbuffer, &_source, &_destination);

    // Guarda la ubicacion del fondo para el siguiente frame
    bg->last_position.x = bg->position.x;
    bg->last_position.y = bg->position.y;
    bg->last_viewport = ngn->graphics->current_viewport;
    bg->tile_mode = true;

}



/*** Dibuja un fondo de Tiles en el Renderer (Con rotacion o escala) ***/
void NGN_Render::TiledBgTransform(NGN_TiledBg* bg) {

    // Rotacion y FLIP
    _rotation = 0.0f;
    _flip = SDL_FLIP_NONE;

    // Define las areas de origen y destino
    _source = {0, 0, 0, 0};
    _destination = {0, 0, 0, 0};

    // Area del render
    Size2I32 render_area;
    render_area.width = ngn->graphics->render_resolution.width;
    render_area.height = ngn->graphics->render_resolution.height;

    // Analiza si es necesario redibujar la textura
    int32_t bg_offset_x = (std::floor(bg->position.x) - std::floor(bg->last_position.x));
    int32_t bg_offset_y = (std::floor(bg->position.y) - std::floor(bg->last_position.y));

    // Si hay desplazamiento o se ha forzado, redibuja la textura en el backbuffer
    if ((bg_offset_x != 0) || (bg_offset_y != 0) || (ngn->graphics->current_viewport != bg->last_viewport) || bg->tile_mode || ngn->graphics->force_redaw) {

        // Calcula el centro del tile
        _center->x = _center->y = ((float)bg->tile_size / 2.0f);

        // Parametros del mapa
        int32_t map_x = 0;
        int32_t map_y = 0;
        int32_t map_w = std::floor(bg->map_size.width / bg->tile_size);
        int32_t map_h = std::floor(bg->map_size.height / bg->tile_size);

        // Tileset
        uint32_t id, tile, flip;      // Informacion

        // Coordenadas de dibujado
        int32_t px, py;

        // Parametros de lectura del tile
        _source.w = _source.h = bg->tile_size;
        // Parametros de escritura del tile
        _destination.w = _destination.h = bg->tile_size;

        // Informa al renderer que la textura "backbuffer" es su destino
        SDL_SetRenderTarget(ngn->graphics->renderer, bg->backbuffer);

        // Borra el contenido de la textura actual
        SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_SetTextureBlendMode(bg->backbuffer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(bg->backbuffer, 0x00);
        SDL_RenderFillRect(ngn->graphics->renderer, NULL);

        // Tamaño del area de dibujado
        int32_t tile_last_x = (std::floor(render_area.width / bg->tile_size) + 2);      // +2 soluciona el encage en resoluciones anamorficas
        int32_t tile_last_y = (std::floor(render_area.height / bg->tile_size) + 2);

        // Calcula el offset del dibujado de los tiles
        int32_t tile_offset_x = (std::floor(bg->position.x / bg->tile_size));
        int32_t tile_offset_y = (std::floor(bg->position.y / bg->tile_size));
        int32_t get_tile_x = 0, get_tile_y = 0;
        int32_t draw_offset_x = ((int32_t)std::floor(bg->position.x) % bg->tile_size);
        int32_t draw_offset_y = ((int32_t)std::floor(bg->position.y) % bg->tile_size);

        // Recorre la pantalla para dibujar el fondo
        for (map_y = 0; map_y < tile_last_y; map_y ++) {
            // Tile Y
            get_tile_y = (map_y + tile_offset_y);
            // Si estas fuera del rango...
            if ((get_tile_y < 0) || (get_tile_y >= map_h)) continue;
            // Punto Y de dibujado
            py = ((map_y * bg->tile_size) - draw_offset_y);
            for (map_x = 0; map_x < tile_last_x; map_x ++) {
                // Tile X
                get_tile_x = (map_x + tile_offset_x);
                // Si estas fuera del rango...
                if ((get_tile_x < 0) || (get_tile_x >= map_w)) continue;
                // Punto X de dibujado
                px = ((map_x * bg->tile_size) - draw_offset_x);
                // ID de tile
                id = (((get_tile_y * map_w) + get_tile_x) << 2);      // * 4 bytes
                // Numero de Tile
                tile = bg->bgdata->tmap[id];
                tile |= (bg->bgdata->tmap[(id + 1)] << 8);
                tile |= (bg->bgdata->tmap[(id + 2)] << 16);
                // Si el tile ES TRANSPARENTE (no es el primero del mapa), ignora el dibujado
                if (tile == 0) continue;
                // Orientacion del tile
                flip = bg->bgdata->tmap[(id + 3)];
                // Define el punto de destino
                _destination.x = px;
                _destination.y = py;
                // Parametros del FLIP del tile
                switch (flip) {
                    case 1:     // Normal       (BIT 0) [1 << 0]
                        SDL_RenderCopy(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination);
                        break;
                    case 2:     // Flip H       (BIT 1) [1 << 1]
                        _flip = SDL_FLIP_HORIZONTAL;
                        _rotation = 0.0f;
                        SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination, _rotation, _center, _flip);
                        break;
                    case 4:     // Flip V       (BIT 2) [1 << 2]
                        _flip = SDL_FLIP_VERTICAL;
                        _rotation = 0.0f;
                        SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination, _rotation, _center, _flip);
                        break;
                    case 8:     // 180�         (BIT 3) [1 << 3]
                        _flip = SDL_FLIP_NONE;
                        _rotation = 180.0f;
                        // Compensa el desplazamiento
                        _destination.x --;
                        _destination.y --;
                        SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination, _rotation, _center, _flip);
                        break;
                    case 16:     // 90� CW      (BIT 4) [1 << 4]
                        _flip = SDL_FLIP_NONE;
                        _rotation = 270.0f;
                        // Compensa el desplazamiento
                        _destination.y --;
                        SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination, _rotation, _center, _flip);
                        break;
                    case 32:     // 90� ACW     (BIT 5) [1 << 5]
                        _flip = SDL_FLIP_NONE;
                        _rotation = 90.0f;
                        // Compensa el desplazamiento
                        _destination.x --;
                        SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &_source, &_destination, _rotation, _center, _flip);
                        break;
                }
            }
        }

        // Restaura el render al seleccionado
        ngn->graphics->RenderToSelected();

    }

    // Calculo del alpha del fondo
    _alpha = bg->alpha;
    if (_alpha < 0) {
        _alpha = 0;
    } else if (_alpha > 0xFF) {
        _alpha = 0xFF;
    }
    SDL_SetTextureBlendMode(bg->backbuffer, bg->blend_mode);
    SDL_SetTextureAlphaMod(bg->backbuffer, (uint8_t)_alpha);

    // Define los puntos de entrada y salida de la textura
    _source.x = 0; _source.y = 0; _source.w = render_area.width; _source.h = render_area.height;
    _destination.w = (int32_t)((float)render_area.width * bg->scale.x);
    _destination.h = (int32_t)((float)render_area.height * bg->scale.y);
    _destination.x = (int32_t)(((float)ngn->graphics->render_resolution.width / 2.0f) - ((float)_destination.w / 2.0f));
    _destination.y = (int32_t)(((float)ngn->graphics->render_resolution.height / 2.0f) - ((float)_destination.h / 2.0f));
    _rotation = bg->rotation;
    _center->x = (int32_t)(((float)_destination.w / 2.0f) + bg->center.x);
    _center->y = (int32_t)(((float)_destination.h / 2.0f) + bg->center.y);
    _flip = SDL_FLIP_NONE;

    // Envia el backbuffer al renderer
    SDL_RenderCopyEx(ngn->graphics->renderer, bg->backbuffer, &_source, &_destination, _rotation, _center, _flip);

    // Guarda la ubicacion del fondo para el siguiente frame
    bg->last_position.x = bg->position.x;
    bg->last_position.y = bg->position.y;
    bg->last_viewport = ngn->graphics->current_viewport;
    bg->tile_mode = false;

}
