/******************************************************************************

    N'gine Lib for C++
    *** Version 1.5.0-wip3 ***
    Gestion del Renderer de SDL

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
NGN_Render::NGN_Render() {

    // Parametros iniciales
    render2texture = false;
    rend2text = NULL;

}



/*** Destructor ***/
NGN_Render::~NGN_Render() {

    rend2text = NULL;

}



void NGN_Render::Texture(NGN_Texture* texture, float position_x, float position_y) {

    // Si no debe dibujarse...
    if (!texture->visible) return;

    // Calculos
    int32_t _x, _y;
    if ((position_x == NGN_DEFAULT_VALUE) && (position_y == NGN_DEFAULT_VALUE)) {
        texture->screen = texture->position;
    } else {
        texture->screen.x = position_x;
        texture->screen.y = position_y;
    }
    _x = (int32_t)(texture->screen.x);
    _y = (int32_t)(texture->screen.y);

    int32_t _width = (int32_t)texture->width;
    int32_t _height = (int32_t)texture->height;
    double _rotation = 0.0f;
    SDL_RendererFlip _flip;

    // Centro de la rotacion
    SDL_Point* _center = new SDL_Point();
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
        int32_t _alpha = texture->alpha;
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
    SDL_Rect source = {
        0,                  			    // Posicion X
        0,              				    // Posicion Y
        (int32_t)texture->data->width,      // Ancho
        (int32_t)texture->data->height      // Alto
    };

    // Define el area de destino
    SDL_Rect destination = {
        _x,         // Posicion X
        _y,         // Posicion Y
        _width,     // Ancho
        _height     // Alto
    };

    // Renderiza la textura
    SDL_RenderCopyEx(ngn->graphics->renderer, texture->data->gfx, &source, &destination, (texture->rotation + _rotation), _center, _flip);
    //Texture(texture->gfx, _x, _y, _width, _height, (texture->rotation + _rotation), _center, _flip);

    // Paso de limpieza
    delete _center;

}



/*** Dibuja un Sprite en el Renderer ***/
void NGN_Render::Sprite(NGN_Sprite* sprite, float position_x, float position_y) {

    // Si no debe dibujarse...
    if (!sprite->visible) return;

    // Calculos
    int32_t _x, _y;
    if ((position_x == NGN_DEFAULT_VALUE) && (position_y == NGN_DEFAULT_VALUE)) {
        sprite->screen.x = (uint32_t)sprite->position.x;
        sprite->screen.y = (uint32_t)sprite->position.y;
    } else {
        sprite->screen.x = position_x;
        sprite->screen.y = position_y;
    }
    _x = (int32_t)(sprite->screen.x - (sprite->width / 2.0f));
    _y = (int32_t)(sprite->screen.y - (sprite->height / 2.0f));

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
        SDL_SetTextureBlendMode(sprite->data->gfx[sprite->frame], sprite->blend_mode);
        SDL_SetTextureAlphaMod(sprite->data->gfx[sprite->frame], (uint8_t)_alpha);
    }


    /* Dibujado del sprite */

    // Define el area de origen
    SDL_Rect source = {
        0,              // Posicion X
        0,              // Posicion Y
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
    //Texture(sprite->gfx, _x, _y, _width, _height, (sprite->rotation + _rotation), _center, _flip);

    // Paso de limpieza
    delete _center;

}



/*** Dibuja un fondo de Tiles en el Renderer ***/
void NGN_Render::TiledBg(NGN_TiledBg* bg) {

    // Si no debe dibujarse
    if (!bg->visible) return;

    // Rotacion y FLIP
    double _rotation = 0.0f;
    SDL_RendererFlip _flip = SDL_FLIP_NONE;

    // Centro de la rotacion
    SDL_Point* _center = new SDL_Point();

    // Define las areas de origen y destino
    SDL_Rect source = {0, 0, 0, 0};
    SDL_Rect destination = {0, 0, 0, 0};

    // Area del render
    Size2I32 render_area;
    if (ngn->graphics->current_viewport < 0) {
        render_area.width = bg->bb_size.width;
        render_area.height = bg->bb_size.height;
    } else {
        render_area.width = ngn->graphics->render_resolution.width;
        render_area.height = ngn->graphics->render_resolution.height;
    }

    // Analiza si es necesario redibujar la textura
    int32_t gap_x = (int32_t)(std::floor(bg->position.x) - std::floor(bg->last_position.x));
    int32_t gap_y = (int32_t)(std::floor(bg->position.y) - std::floor(bg->last_position.y));

    // Si hay desplazamiento, redibuja la textura en el backbuffer
    if ((gap_x != 0) || (gap_y != 0) || ngn->graphics->force_redaw) {

        // Guarda el tama�o del TILE
        uint32_t tile_size = bg->bgdata->header.tile_size;

        // Calcula el centro del tile
        _center->x = _center->y = ((float)tile_size / 2.0f);

        // Parametros del mapa
        int32_t map_x = 0;
        int32_t map_y = 0;
        int32_t map_w = (int32_t)(bg->bgdata->header.map_width / tile_size);
        int32_t map_h = (int32_t)(bg->bgdata->header.map_height / tile_size);

        // Tileset
        uint32_t id, tile, flip;      // Informacion

        // Coordenadas de dibujado
        int32_t px, py;

        // Parametros de lectura del tile
        source.w = source.h = bg->bgdata->header.tile_size;
        // Parametros de escritura del tile
        destination.w = destination.h = bg->bgdata->header.tile_size;

        // Informa al renderer que la textura "backbuffer" es su destino
        SDL_SetRenderTarget(ngn->graphics->renderer, bg->backbuffer);

        // Borra el contenido de la textura actual
        SDL_SetRenderDrawColor(ngn->graphics->renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_SetTextureBlendMode(bg->backbuffer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(bg->backbuffer, 0x00);
        SDL_RenderFillRect(ngn->graphics->renderer, NULL);

        // Tama�o del area de dibujado
        int32_t tile_last_x = (((int32_t)(std::floor(render_area.width / tile_size))) + 2);      // +2 soluciona el encage en resoluciones anamorficas
        int32_t tile_last_y = (((int32_t)(std::floor(render_area.height / tile_size))) + 2);

        // Calcula el offset del dibujado de los tiles
        int32_t tile_offset_x = std::floor(bg->position.x / tile_size);
        int32_t tile_offset_y = std::floor(bg->position.y / tile_size);
        int32_t get_tile_x = 0, get_tile_y = 0;
        int32_t draw_offset_x = ((int32_t)bg->position.x % tile_size);
        int32_t draw_offset_y = ((int32_t)bg->position.y % tile_size);

        // Recorre la pantalla para dibujar el fondo
        for (map_y = 0; map_y < tile_last_y; map_y ++) {
            // Tile Y
            get_tile_y = (map_y + tile_offset_y);
            // Si estas dentro del rango...
            if ((get_tile_y >= 0) && (get_tile_y < map_h)) {
                // Punto Y de dibujado
                py = ((map_y * tile_size) - draw_offset_y);
                for (map_x = 0; map_x < tile_last_x; map_x ++) {
                    // Tile X
                    get_tile_x = (map_x + tile_offset_x);
                    // Si estas dentro del rango...
                    if ((get_tile_x >= 0) && (get_tile_x < map_w)) {
                        // Punto X de dibujado
                        px = ((map_x * tile_size) - draw_offset_x);
                        // ID de tile
                        id = (((get_tile_y * map_w) + get_tile_x) << 2);      // * 4 bytes
                        // Numero de Tile
                        tile = bg->bgdata->tmap[id];
                        tile |= (bg->bgdata->tmap[(id + 1)] << 8);
                        tile |= (bg->bgdata->tmap[(id + 2)] << 16);
                        // Si el tile NO ES TRANSPARENTE (no es el primero del mapa), dibujalo
                        if (tile > 0) {
                            // Orientacion del tile
                            flip = bg->bgdata->tmap[(id + 3)];
                            // Define el punto de destino
                            destination.x = px;
                            destination.y = py;
                            // Parametros del FLIP del tile
                            switch (flip) {
                                case 1:     // Normal       (BIT 0) [1 << 0]
                                    _flip = SDL_FLIP_NONE;
                                    _rotation = 0.0f;
                                    break;
                                case 2:     // Flip H       (BIT 1) [1 << 1]
                                    _flip = SDL_FLIP_HORIZONTAL;
                                    _rotation = 0.0f;
                                    break;
                                case 4:     // Flip V       (BIT 2) [1 << 2]
                                    _flip = SDL_FLIP_VERTICAL;
                                    _rotation = 0.0f;
                                    break;
                                case 8:     // 180�         (BIT 3) [1 << 3]
                                    _flip = SDL_FLIP_NONE;
                                    _rotation = 180.0f;
                                    // Compensa el desplazamiento
                                    destination.x --;
                                    destination.y --;
                                    break;
                                case 16:     // 90� CW      (BIT 4) [1 << 4]
                                    _flip = SDL_FLIP_NONE;
                                    _rotation = 270.0f;
                                    // Compensa el desplazamiento
                                    destination.y --;
                                    break;
                                case 32:     // 90� ACW     (BIT 5) [1 << 5]
                                    _flip = SDL_FLIP_NONE;
                                    _rotation = 90.0f;
                                    // Compensa el desplazamiento
                                    destination.x --;
                                    break;
                            }
                            // Envia el tile al renderer
                            SDL_RenderCopyEx(ngn->graphics->renderer, bg->bgdata->tiles[tile], &source, &destination, _rotation, _center, _flip);
                        }
                    }
                }
            }
        }

        // Restaura el render al seleccionado
        ngn->graphics->RenderToSelected();

    }

    // Calculo del alpha del fondo
    int32_t _alpha = bg->alpha;
    if (_alpha < 0) {
        _alpha = 0;
    } else if (_alpha > 0xFF) {
        _alpha = 0xFF;
    }
    SDL_SetTextureBlendMode(bg->backbuffer, bg->blend_mode);
    SDL_SetTextureAlphaMod(bg->backbuffer, (uint8_t)_alpha);

    // Define los puntos de entrada y salida de la textura
    source.x = 0; source.y = 0; source.w = render_area.width; source.h = render_area.height;
    destination.w = (render_area.width * bg->scale.x);
    destination.h = (render_area.height * bg->scale.y);
    destination.x = ((ngn->graphics->render_resolution.width / 2.0f) - (destination.w / 2.0f));
    destination.y = ((ngn->graphics->render_resolution.height / 2.0f) - (destination.h / 2.0f));
    _rotation = bg->rotation;
    _center->x = (destination.w / 2.0f) + bg->center.x;
    _center->y = (destination.h / 2.0f) + bg->center.y;
    _flip = SDL_FLIP_NONE;

    // Envia el backbuffer al renderer
    SDL_RenderCopyEx(ngn->graphics->renderer, bg->backbuffer, &source, &destination, _rotation, _center, _flip);

    // Guarda la posicion del fondo para el siguiente frame
    bg->last_position.x = bg->position.x;
    bg->last_position.y = bg->position.y;

    // Paso de limpieza
    delete _center;

}



/*** Dibuja una capa de texto en el renderer ***/
void NGN_Render::TextLayer(NGN_TextLayer* layer, float position_x, float position_y) {

    // Si no debe dibujarse...
    if (!layer->visible) return;

    // Calculos
    int32_t _x, _y;
    if ((position_x != NGN_DEFAULT_VALUE) && (position_y != NGN_DEFAULT_VALUE)) {
        layer->position.x = position_x;
        layer->position.y = position_y;
    }
    _x = (int32_t)(layer->position.x);
    _y = (int32_t)(layer->position.y);

    int32_t _width = ((float)layer->width * layer->scale.width);
    int32_t _height = ((float)layer->height * layer->scale.height);
    double _rotation = 0.0f;
    SDL_RendererFlip _flip;

    // Centro de la rotacion
    SDL_Point* _center = new SDL_Point();
    _center->x = ((_width / 2.0f) + layer->center.x);
    _center->y = ((_height / 2.0f) + layer->center.y);

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
        int32_t _alpha = layer->alpha;
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
    SDL_Rect source = {
        0,                  		// Posicion X
        0,              			// Posicion Y
        layer->GetSize().width,     // Ancho
        layer->GetSize().height     // Alto
    };

    // Define el area de destino
    SDL_Rect destination = {
        _x,         // Posicion X
        _y,         // Posicion Y
        _width,     // Ancho
        _height     // Alto
    };

    // Renderiza la textura
    SDL_RenderCopyEx(ngn->graphics->renderer, layer->backbuffer, &source, &destination, (layer->rotation + _rotation), _center, _flip);
    //Texture(layer->gfx, _x, _y, _width, _height, (layer->rotation + _rotation), _center, _flip);

    // Paso de limpieza
    delete _center;

}



/*** Dibuja un canvas en el renderer ***/
void NGN_Render::Canvas(NGN_Canvas* canvas, float position_x, float position_y) {

    // Si no debe dibujarse...
    if (!canvas->visible) return;

    // Si es necesario realiza el BLIT (convierte el buffer de pixeles en textura)
    canvas->Blit();

    // Calculos
    int32_t _x, _y;
    if ((position_x != NGN_DEFAULT_VALUE) && (position_y != NGN_DEFAULT_VALUE)) {
        canvas->position.x = position_x;
        canvas->position.y = position_y;
    }
    _x = (int32_t)(canvas->position.x);
    _y = (int32_t)(canvas->position.y);

    int32_t _width = ((float)canvas->width * canvas->scale.width);
    int32_t _height = ((float)canvas->height * canvas->scale.height);
    double _rotation = 0.0f;
    SDL_RendererFlip _flip;

    // Centro de la rotacion
    SDL_Point* _center = new SDL_Point();
    _center->x = ((_width / 2.0f) + canvas->center.x);
    _center->y = ((_height / 2.0f) + canvas->center.y);

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
        int32_t _alpha = canvas->alpha;
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
    SDL_Rect source = {
        0,                  			// Posicion X
        0,              				// Posicion Y
        canvas->GetSize().width,        // Ancho
        canvas->GetSize().height        // Alto
    };

    // Define el area de destino
    SDL_Rect destination = {
        _x,         // Posicion X
        _y,         // Posicion Y
        _width,     // Ancho
        _height     // Alto
    };

    // Renderiza la textura
    SDL_RenderCopyEx(ngn->graphics->renderer, canvas->backbuffer, &source, &destination, (canvas->rotation + _rotation), _center, _flip);
    //Texture(canvas->gfx, _x, _y, _width, _height, (canvas->rotation + _rotation), _center, _flip);

    // Paso de limpieza
    delete _center;

}



/*** Render de todos los viewports disponibles ***/
void NGN_Render::Viewports() {

    // Restaura el viewport por defecto
    ngn->graphics->DefaultViewport();

    SDL_Rect source, destination;   // Areas de entrada y salida

    for (uint8_t i = 0; i < ngn->graphics->viewport_list.capacity(); i ++) {
        if (ngn->graphics->viewport_list[i].available) {
            // Defines las areas
            source = {
                0,
                0,
                ngn->graphics->viewport_list[i].render_w,
                ngn->graphics->viewport_list[i].render_h
            };
            destination = {
                ngn->graphics->viewport_list[i].x,
                ngn->graphics->viewport_list[i].y,
                ngn->graphics->viewport_list[i].w,
                ngn->graphics->viewport_list[i].h
            };
            // Render de la textura al render principal
            SDL_RenderCopy(ngn->graphics->renderer, ngn->graphics->viewport_list[i].surface, &source, &destination);
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
