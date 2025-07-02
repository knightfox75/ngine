/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0-wip_0x03 ***
    Camara virtual en 2D

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

// n'gine
#include "ngn.h"

// Class
#include "ngn_camera.h"



/******************************************************************************

    Clase NGN_Camera

******************************************************************************/

/*** Contructor de la clase NGN_Camera ***/
NGN_Camera::NGN_Camera() {

    // Limpia los vectores
    layer.clear();

    // Parametros iniciales
    target = nullptr;
    position.x = position.y = 0.0f;
    scroll.width = scroll.height = 0.0f;
    animation_pause = false;

}



/*** Destructor de la clase NGN_Camera ***/
NGN_Camera::~NGN_Camera() {

    // Elimina el contenido del gestor de capas
    Reset();

}



/*** Crea las capas que usara la camara ***/
void NGN_Camera::CreateLayers(uint32_t layers) {

    // Borra el contenido actual
    Reset();

    // Datos iniciales
    LayerData ld;
    ld.texture = nullptr;
    ld.tiled_bg = nullptr;
    ld.spr_t.clear();
    ld.spr.clear();
    ld.layer_size.texture.width = ld.layer_size.texture.height = 0;
    ld.layer_size.tiled_bg.width = ld.layer_size.tiled_bg.height = 0;
    ld.layer_size.virtual_bg.width = ld.layer_size.virtual_bg.height = 0;
    ld.layer_size.sprites.width = ld.layer_size.sprites.height = NGN_DEFAULT_VALUE;
    ld.visible = true;
    ld.in_use = false;
    ld.is_texture = false;
    ld.is_tiled = false;
    ld.is_virtual = false;
    ld.central_point = Vector2::Zero();
    ld.tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    ld.last_tint_color = {0xFF, 0xFF, 0xFF, 0xFF};

    // Asigna el espacio solicitado e inicializa todas las capas
    layer.clear();
    layer.assign(layers, ld);

}



/*** Especifica el tamaño de la capa para los sprites ***/
void NGN_Camera::SetSizeOfSpritesLayer(uint32_t layer_number, uint32_t width, uint32_t height) {

    if (layer_number >= layer.size()) return;

    if (layer[layer_number].texture || layer[layer_number].tiled_bg) return;

    if ((width == 0) || (height == 0)) return;

    layer[layer_number].layer_size.sprites.width = width;
    layer[layer_number].layer_size.sprites.height = height;
    layer[layer_number].in_use = true;

}



/*** Inicializa la camara con los parametros por defecto ***/
void NGN_Camera::Setup(uint32_t world_width, uint32_t world_height, NGN_Sprite* target_sprite) {

    // Registra las medidas del mundo
    world.width = world_width;
    world.height = world_height;

    // El mundo NO puede ser menor del tamaño de la pantalla
    if (world.width < ngn->graphics->native_w) world.width = ngn->graphics->native_w;
    if (world.height < ngn->graphics->native_h) world.height = ngn->graphics->native_h;

    // Registra el target
    target = target_sprite;

}



/*** Añade un fondo de textura a la capa especificada ***/
int32_t NGN_Camera::PushBackground(uint32_t layer_number, NGN_Texture* texture) {

    // Si la capa y la textura son validas
    if ((layer_number >= layer.size()) || !texture) return -1;

    // Si ya hay un fondo en esta capa, ignora la orden
    if (layer[layer_number].texture || layer[layer_number].tiled_bg) return -1;

    // Indica al fondo en que capa se ha añadido
    texture->camera_layer = layer_number;

    // Registra la textura como fondo
    layer[layer_number].texture = texture;
    // Y registra que esta en uso la capa
    layer[layer_number].is_texture = true;
    layer[layer_number].in_use = true;

    // Guarda el tamaño de la capa
    layer[layer_number].layer_size.texture.width = texture->width;
    layer[layer_number].layer_size.texture.height = texture->height;

    // Si es necesario, registra el tamaño del fondo como el de la capa de Sprites
    if ((layer[layer_number].layer_size.sprites.width == NGN_DEFAULT_VALUE) || (layer[layer_number].layer_size.sprites.height == NGN_DEFAULT_VALUE)) {
        layer[layer_number].layer_size.sprites.width = texture->width;
        layer[layer_number].layer_size.sprites.height = texture->height;
    }

    // Devuelve 0
    return 0;

}



/*** Añade un fondo de tiles a la capa especificada ***/
int32_t NGN_Camera::PushBackground(uint32_t layer_number, NGN_TiledBg* background) {

    // Si la capa y la textura son validas
    if ((layer_number >= layer.size()) || !background) return -1;

    // Si ya hay un fondo en esta capa, ignora la orden
    if (layer[layer_number].texture || layer[layer_number].tiled_bg) return -1;

    // Indica al fondo en que capa se ha añadido
    background->camera_layer = layer_number;

    // Registra la textura como fondo
    layer[layer_number].tiled_bg = background;
    // Y registra que esta en uso la capa
    layer[layer_number].is_tiled = true;
    layer[layer_number].in_use = true;

    // Guarda el tamaño de la capa
    layer[layer_number].layer_size.tiled_bg.width = background->width;
    layer[layer_number].layer_size.tiled_bg.height = background->height;

    // Si es necesario, registra el tamaño del fondo como el de la capa de Sprites
    if ((layer[layer_number].layer_size.sprites.width == NGN_DEFAULT_VALUE) || (layer[layer_number].layer_size.sprites.height == NGN_DEFAULT_VALUE)) {
        layer[layer_number].layer_size.sprites.width = background->width;
        layer[layer_number].layer_size.sprites.height = background->height;
    }

    // Devuelve 0
    return 0;

}



/*** Añade un fondo de textura a la camara en modo virtual ***/
int32_t NGN_Camera::PushVirtualBg(uint32_t layer_number, NGN_Texture* texture, uint32_t bg_width, uint32_t bg_height, uint32_t loop_x, uint32_t loop_y, float auto_x, float auto_y) {

    // Añade el fondo a la camara
    int32_t r = PushBackground(layer_number, texture);
    if (r < 0) return r;

    // Registra los parametros virtuales
    layer[layer_number].texture->virtual_texture.enabled = true;
    layer[layer_number].texture->virtual_texture.texture_size.width = bg_width;
    layer[layer_number].texture->virtual_texture.texture_size.height = bg_height;
    layer[layer_number].texture->virtual_texture.loop.x = loop_x;
    layer[layer_number].texture->virtual_texture.loop.y = loop_y;
    layer[layer_number].texture->virtual_texture.scroll.x = -auto_x;
    layer[layer_number].texture->virtual_texture.scroll.y = -auto_y;
    layer[layer_number].layer_size.sprites.width = bg_width;
    layer[layer_number].layer_size.sprites.height = bg_height;
    layer[layer_number].layer_size.virtual_bg.width = bg_width;
    layer[layer_number].layer_size.virtual_bg.height = bg_height;
    layer[layer_number].is_virtual = true;

    // Devuelve el indice
    return r;

}



/*** Añade un fondo de tiles a la camara en modo virtual ***/
int32_t NGN_Camera::PushVirtualBg(uint32_t layer_number, NGN_TiledBg* background, uint32_t bg_width, uint32_t bg_height, uint32_t loop_x, uint32_t loop_y, float auto_x, float auto_y) {

    // Añade el fondo a la camara
    int32_t r = PushBackground(layer_number, background);
    if (r < 0) return r;

    // Registra los parametros virtuales
    layer[layer_number].tiled_bg->virtual_bg.enabled = true;
    layer[layer_number].tiled_bg->virtual_bg.bg_size.width = bg_width;
    layer[layer_number].tiled_bg->virtual_bg.bg_size.height = bg_height;
    layer[layer_number].tiled_bg->virtual_bg.loop.x = loop_x;
    layer[layer_number].tiled_bg->virtual_bg.loop.y = loop_y;
    layer[layer_number].tiled_bg->virtual_bg.scroll.x = -auto_x;
    layer[layer_number].tiled_bg->virtual_bg.scroll.y = -auto_y;
    layer[layer_number].layer_size.sprites.width = bg_width;
    layer[layer_number].layer_size.sprites.height = bg_height;
    layer[layer_number].layer_size.virtual_bg.width = bg_width;
    layer[layer_number].layer_size.virtual_bg.height = bg_height;
    layer[layer_number].is_virtual = true;

    // Devuelve el indice
    return r;

}



/*** Añade un sprite a la capa especificada (1ra sobrecarga) ***/
int32_t NGN_Camera::PushSprite(uint32_t layer_number, NGN_Sprite* sprite) {

    // Si la capa y el sprite son validos
    if ((layer_number < layer.size()) && sprite) {

        // Indica al sprite en que capa se ha añadido
        sprite->camera_layer = layer_number;
        // Añade el fondo a la lista
        layer[layer_number].spr.push_back(sprite);
        // Y registra que esta en uso la capa
        layer[layer_number].in_use = true;

    } else {

        return -1;

    }

    // Devuelve el indice
    return (layer[layer_number].spr.size() - 1);

}



/*** Añade un sprite a la capa especificada (2da sobrecarga) ***/
int32_t NGN_Camera::PushSprite(uint32_t layer_number, NGN_Texture* texture) {

    // Si la capa y la textura son validas
    if ((layer_number < layer.size()) && texture) {

        // Indica al sprite en que capa se ha añadido
        texture->camera_layer = layer_number;
        // Añade el fondo a la lista
        layer[layer_number].spr_t.push_back(texture);
        // Y registra que esta en uso la capa
        layer[layer_number].in_use = true;

    } else {

        return -1;

    }

    // Devuelve el indice
    return (layer[layer_number].spr_t.size() - 1);

}



/*** Haz que la camara siga a un sprite concreto [Sobrecarga 1] ***/
void NGN_Camera::LookAt(NGN_Sprite* target_sprite) {

    // Si el target es valido, asignalo
    if (target_sprite) target = target_sprite;

}



/*** Haz que la camara se mueva a una coordenada concreta [Sobrecarga 2] ***/
void NGN_Camera::LookAt(uint32_t position_x, uint32_t position_y) {

    // Anula el target actual si este existe
    target = nullptr;

    // Posiciona la camara en estas coordenadas
    position.x = position_x;
    position.y = position_y;

}



/*** Haz que la camara se mueva a una coordenada concreta [Sobrecarga 3] ***/
void NGN_Camera::LookAt(Vector2I32 pos) {

    // Anula el target actual si este existe
    target = nullptr;

    // Posiciona la camara en estas coordenadas
    position.x = (pos.x >= 0) ? (uint32_t)pos.x : 0;
    position.y = (pos.y >= 0) ? (uint32_t)pos.y : 0;

}



/*** Recupera el punto central de la camara en el mundo ***/
Vector2 NGN_Camera::GetLookAt() {

    return world_lookat;

}



/*** Recuepera el sprite al que sigue la camara, si es que sigue alguno ***/
NGN_Sprite* NGN_Camera::GetTargetSprite() {

    return target;

}



/*** Recupera la posicion a la que se le ha ordenado colocarse la camara ***/
Vector2 NGN_Camera::GetTargetPosition() {

    return world_position;

}



/*** Actualiza la vista de la camara ***/
void NGN_Camera::Update() {

    // Si hay no capas definidas, sal
    if (layer.size() == 0) return;

    // Opten el area de render
    render_area.width = ngn->graphics->render_resolution.width;
    render_area.height = ngn->graphics->render_resolution.height;

    // Calcula el tamaño del scroll
    scroll.width = (world.width - render_area.width);
    scroll.height = (world.height - render_area.height);

    // Reinicia los datos de control
    world_origin.x = world_origin.y = 0;
    screen_pos.x = screen_pos.y = 0;
    sprite_campos.x = sprite_campos.y = 0;
    temp.x = temp.y = 0;

    // Calcula la posicion REAL de la camara en el mundo segun su target
    Vector2 campos;
    if (target != nullptr) {
        campos.x = target->position.x;
        campos.y = target->position.y;
    } else {
        campos.x = position.x;
        campos.y = position.y;
    }

    // Guarda el valor de posion de la camara antes de aplicarle los limites
    world_position = campos;

    // Ajusta su posicion para que quede dentro del mundo
    if (campos.x < (render_area.width / 2)) campos.x = (render_area.width / 2);
    if (campos.x > (world.width - (render_area.width / 2))) campos.x = (world.width - (render_area.width / 2));
    if (campos.y < (render_area.height / 2)) campos.y = (render_area.height / 2);
    if (campos.y > (world.height - (render_area.height / 2))) campos.y = (world.height - (render_area.height / 2));

    // Registra la posicion de donde esta mirando la camara actualmente
    world_lookat = campos;

    // Calcula la coordenada de origen del dibujado
    world_origin.x = (campos.x - (render_area.width / 2));
    world_origin.y = (campos.y - (render_area.height / 2));

    //std::cout << scroll.width << " " << scroll.height << std::endl;

    // Dibuja las capas una a una (la ultima se dibuja en frente)
    for (uint32_t l = 0; l < layer.size(); l ++) {

        // Si la capa tiene contenido y es visible
        if (layer[l].in_use && layer[l].visible) {

            // Reset del punto central
            layer[l].central_point = Vector2::Zero();

            // De ser aplicable, calcula el offset de esta capa para el efecto "shake"
            ApplyShake(l);

            // Aplica el efecto de tinta si se requiere
            ApplyTintColor(l);

            // Primero, dibuja los fondos de textura (si existen)
            RenderTextures(l);
            // Luego, dibuja los fondos de tiles (si existen)
            RenderTiles(l);

            // Dibuja las texturas encima de estos fondos (si existen)
            RenderTextureSprites(l);
            // Dibuja los sprites encima de estos fondos (si existen)
            RenderSprites(l);

        }

    }

    // Registra el id de frame actual (se asegura que solo se ejecute la funcion 1 vez por frame)
    runtime_frame = ngn->graphics->runtime_frame;

}



/*** Aplica el color de tinte si es necesario ***/
void NGN_Camera::ApplyTintColor(uint32_t l) {

    // Si ya se ha actualizado en este frame, ignora la orden
    if (runtime_frame == ngn->graphics->runtime_frame) return;

    // Si esta en pausa, ignora la orden
    if (animation_pause) return;

    // Hay cambios en el color de tinte?
    if (
        (layer[l].tint_color.r == layer[l].last_tint_color.r)
        &&
        (layer[l].tint_color.g == layer[l].last_tint_color.g)
        &&
        (layer[l].tint_color.b == layer[l].last_tint_color.b)
    ) return;


    // Aplica los cambios a la capa de texturas
    if (layer[l].texture) {
        if (!layer[l].texture->ignore_camera_tint) layer[l].texture->SetTintColor(layer[l].tint_color.r, layer[l].tint_color.g, layer[l].tint_color.b);
    }

    // Aplica los cambios a la capa de tiles
    if (layer[l].tiled_bg) {
        if (!layer[l].tiled_bg->ignore_camera_tint) layer[l].tiled_bg->SetTintColor(layer[l].tint_color.r, layer[l].tint_color.g, layer[l].tint_color.b);
    }

    // Aplica los cambios a los sprites (texturas como sprites)
    for (uint32_t s = 0; s < layer[l].spr_t.size(); s ++) {
        if (!layer[l].spr_t[s]->ignore_camera_tint) layer[l].spr_t[s]->SetTintColor(layer[l].tint_color.r, layer[l].tint_color.g, layer[l].tint_color.b);
    }

    // Aplica los cambios a los sprites
    for (uint32_t s = 0; s < layer[l].spr.size(); s ++) {
        if (!layer[l].spr[s]->ignore_camera_tint) layer[l].spr[s]->SetTintColor(layer[l].tint_color.r, layer[l].tint_color.g, layer[l].tint_color.b);
    }

    // Guarda el color actual
    layer[l].last_tint_color = layer[l].tint_color;

    //std::cout << "Tint update!" << std::endl;

}



/*** Render de los fondos de textura de una capa ***/
void NGN_Camera::RenderTextures(uint32_t l) {

    // Si no hay textura, sal
    if (!layer[l].texture) return;

	// Calcula el rango de desplazamiento de este fondo
	if (layer[l].texture->virtual_texture.enabled) {
		temp.x = (layer[l].texture->virtual_texture.texture_size.width - render_area.width);
		temp.y = (layer[l].texture->virtual_texture.texture_size.height - render_area.height);
	} else {
		temp.x = (layer[l].texture->width - render_area.width);
		temp.y = (layer[l].texture->height - render_area.height);
	}

	// Calcula la posicion relativa en X (efecto parallax)
	if (scroll.width > 0) {
		screen_pos.x = ((temp.x * world_origin.x) / scroll.width);
	} else {
		screen_pos.x = 0;
	}
	if (layer[l].central_point.x == 0.0f) layer[l].central_point.x = (screen_pos.x + (render_area.width / 2));

	// Auto scroll en X
	if (layer[l].texture->virtual_texture.scroll.x != 0.0f) {
		if (!animation_pause && (runtime_frame != ngn->graphics->runtime_frame)) layer[l].texture->virtual_texture.offset.x += layer[l].texture->virtual_texture.scroll.x;
		if (layer[l].texture->virtual_texture.scroll.x > 0.0f) {
			if (layer[l].texture->virtual_texture.offset.x > layer[l].texture->virtual_texture.loop.x) layer[l].texture->virtual_texture.offset.x -= layer[l].texture->virtual_texture.loop.x;
		} else {
			if (layer[l].texture->virtual_texture.offset.x < 0) layer[l].texture->virtual_texture.offset.x += layer[l].texture->virtual_texture.loop.x;
		}
		screen_pos.x += (int32_t)layer[l].texture->virtual_texture.offset.x;
	}

	// Punto de loop
	if (layer[l].texture->virtual_texture.enabled && (layer[l].texture->virtual_texture.loop.x > 0)) screen_pos.x %= layer[l].texture->virtual_texture.loop.x;

	// Calcula la posicion relativa en Y (efecto parallax)
	if (scroll.height > 0) {
		screen_pos.y = ((temp.y * world_origin.y) / scroll.height);
	} else {
		screen_pos.y = 0;
	}
	if (layer[l].central_point.y == 0.0f) layer[l].central_point.y = (screen_pos.y + (render_area.height / 2));

	// Auto scroll en Y
	if (layer[l].texture->virtual_texture.scroll.y) {
		if (!animation_pause && (runtime_frame != ngn->graphics->runtime_frame)) layer[l].texture->virtual_texture.offset.y += layer[l].texture->virtual_texture.scroll.y;
		if (layer[l].texture->virtual_texture.scroll.y > 0.0f) {
			if (layer[l].texture->virtual_texture.offset.y > layer[l].texture->virtual_texture.loop.y) layer[l].texture->virtual_texture.offset.y -= layer[l].texture->virtual_texture.loop.y;
		} else {
			if (layer[l].texture->virtual_texture.offset.y < 0) layer[l].texture->virtual_texture.offset.y += layer[l].texture->virtual_texture.loop.y;
		}
		screen_pos.y += (int32_t)layer[l].texture->virtual_texture.offset.y;
	}

	// Punto de loop Y
	if (layer[l].texture->virtual_texture.enabled && (layer[l].texture->virtual_texture.loop.y > 0)) screen_pos.y %= layer[l].texture->virtual_texture.loop.y;

	// Y dibujalo en el renderer
	ngn->render->Texture(layer[l].texture, -screen_pos.x, -((float)screen_pos.y - shake_effect.offset.y));


}



/*** Render de los fondos de tiles de una capa ***/
void NGN_Camera::RenderTiles(uint32_t l) {

    // Si no hay fondo de tiles, sal
    if (!layer[l].tiled_bg) return;


	// Calcula el rango de desplazamiento de este fondo, sea real o virtual
	if (layer[l].tiled_bg->virtual_bg.enabled) {
		temp.x = (layer[l].tiled_bg->virtual_bg.bg_size.width - render_area.width);
		temp.y = (layer[l].tiled_bg->virtual_bg.bg_size.height - render_area.height);
	} else {
		temp.x = (layer[l].tiled_bg->width - render_area.width);
		temp.y = (layer[l].tiled_bg->height - render_area.height);
	}
	//if (b == 0) std::cout << layer[l].tiled_bg->width << " " << layer[l].tiled_bg->height << std::endl;

	// Calcula la posicion relativa en X (efecto parallax)
	if (scroll.width > 0) {
		screen_pos.x = ((temp.x * world_origin.x) / scroll.width);
	} else {
		screen_pos.x = 0;
	}
	if (layer[l].central_point.x == 0.0f) layer[l].central_point.x = (screen_pos.x + (render_area.width / 2));

	// Auto scroll en X
	if (layer[l].tiled_bg->virtual_bg.scroll.x != 0.0f) {
		if (!animation_pause && (runtime_frame != ngn->graphics->runtime_frame)) layer[l].tiled_bg->virtual_bg.offset.x += layer[l].tiled_bg->virtual_bg.scroll.x;
		if (layer[l].tiled_bg->virtual_bg.scroll.x > 0.0f) {
			if (layer[l].tiled_bg->virtual_bg.offset.x > layer[l].tiled_bg->virtual_bg.loop.x) layer[l].tiled_bg->virtual_bg.offset.x -= layer[l].tiled_bg->virtual_bg.loop.x;
		} else {
			if (layer[l].tiled_bg->virtual_bg.offset.x < 0) layer[l].tiled_bg->virtual_bg.offset.x += layer[l].tiled_bg->virtual_bg.loop.x;
		}
		screen_pos.x += (int32_t)layer[l].tiled_bg->virtual_bg.offset.x;
	}

	// Punto de loop en X
	if (layer[l].tiled_bg->virtual_bg.enabled && (layer[l].tiled_bg->virtual_bg.loop.x > 0)) screen_pos.x %= layer[l].tiled_bg->virtual_bg.loop.x;

	// Calcula la posicion relativa en Y (efecto parallax)
	if (scroll.height > 0) {
		screen_pos.y = ((temp.y * world_origin.y) / scroll.height);
	} else {
		screen_pos.y = 0;
	}
	if (layer[l].central_point.y == 0.0f) layer[l].central_point.y = (screen_pos.y + (render_area.height / 2));

	// Auto scroll en Y
	if (layer[l].tiled_bg->virtual_bg.scroll.y != 0.0f) {
		if (!animation_pause && (runtime_frame != ngn->graphics->runtime_frame)) layer[l].tiled_bg->virtual_bg.offset.y += layer[l].tiled_bg->virtual_bg.scroll.y;
		if (layer[l].tiled_bg->virtual_bg.scroll.y > 0.0f) {
			if (layer[l].tiled_bg->virtual_bg.offset.y > layer[l].tiled_bg->virtual_bg.loop.y) layer[l].tiled_bg->virtual_bg.offset.y -= layer[l].tiled_bg->virtual_bg.loop.y;
		} else {
			if (layer[l].tiled_bg->virtual_bg.offset.y < 0) layer[l].tiled_bg->virtual_bg.offset.y += layer[l].tiled_bg->virtual_bg.loop.y;
		}
		screen_pos.y += (int32_t)layer[l].tiled_bg->virtual_bg.offset.y;
	}

	// Punto de loop en Y
	if (layer[l].tiled_bg->virtual_bg.enabled && (layer[l].tiled_bg->virtual_bg.loop.y > 0)) screen_pos.y %= layer[l].tiled_bg->virtual_bg.loop.y;

	// Posiciona el fondo
	layer[l].tiled_bg->Position(screen_pos.x, ((float)screen_pos.y + shake_effect.offset.y));
	//std::cout << "L: " << l << " BG:" << b << " POS:" << layer[l].tiled_bg->position.x << "x" << layer[l].tiled_bg->position.y << std::endl;

	// Y dibujalo en el renderer
	ngn->render->TiledBg(layer[l].tiled_bg);


}



/*** Render de los sprites de textura de una capa ***/
void NGN_Camera::RenderTextureSprites(uint32_t l) {

    // Si no hay sprites, sal
    if (layer[l].spr_t.size() == 0) return;

    // Si el tamaño de la capa es invalido, sal
    if ((layer[l].layer_size.sprites.width == NGN_DEFAULT_VALUE) || (layer[l].layer_size.sprites.height == NGN_DEFAULT_VALUE)) return;

    // Calcula el rango de desplazamiento de este fondo
    temp.x = (layer[l].layer_size.sprites.width - render_area.width);
    temp.y = (layer[l].layer_size.sprites.height - render_area.height);

    // Calcula la posicion relativa segun la capa que esta
    if (scroll.width > 0) {
        sprite_campos.x = ((temp.x * world_origin.x) / scroll.width);
    } else {
        sprite_campos.x = 0.0f;
    }
    if (scroll.height > 0) {
        sprite_campos.y = ((temp.y * world_origin.y) / scroll.height);
    } else {
        sprite_campos.y = 0.0f;
    }
    if ((layer[l].central_point.x == 0.0f) && (layer[l].central_point.y == 0.0f)) {
        layer[l].central_point.x = (sprite_campos.x + (render_area.width / 2));
        layer[l].central_point.y = (sprite_campos.y + (render_area.height / 2));
    }

    for (uint32_t s = 0; s < layer[l].spr_t.size(); s ++) {

        // Calcula la posicion del sprite en pantalla segun el origen de dibujado
        screen_pos.x = layer[l].spr_t[s]->position.x - sprite_campos.x;
        screen_pos.y = layer[l].spr_t[s]->position.y - sprite_campos.y;

        // Si esta dentro de la pantalla, dibujalo en el renderer
        if (
            (screen_pos.x > -layer[l].spr_t[s]->width)
            &&
            (screen_pos.x < (render_area.width + layer[l].spr_t[s]->width))
            &&
            (screen_pos.y > -layer[l].spr_t[s]->height)
            &&
            (screen_pos.y < (render_area.height + layer[l].spr_t[s]->height))
        ) {
            // Dibujalo
            ngn->render->Texture(layer[l].spr_t[s], screen_pos.x, ((float)screen_pos.y + shake_effect.offset.y));
        }

    }

}



/*** Render de los sprites de una capa ***/
void NGN_Camera::RenderSprites(uint32_t l) {

    // Si no hay sprites sal
    if (layer[l].spr.size() == 0) return;

    // Si el tamaño de la capa es invalido, sal
    if ((layer[l].layer_size.sprites.width == NGN_DEFAULT_VALUE) || (layer[l].layer_size.sprites.height == NGN_DEFAULT_VALUE)) return;

    // Calcula el rango de desplazamiento de este fondo
    temp.x = (layer[l].layer_size.sprites.width - render_area.width);
    temp.y = (layer[l].layer_size.sprites.height - render_area.height);

    // Calcula la posicion relativa segun la capa que esta
    if (scroll.width > 0) {
        sprite_campos.x = ((temp.x * world_origin.x) / scroll.width);
    } else {
        sprite_campos.x = 0.0f;
    }
    if (scroll.height > 0) {
        sprite_campos.y = ((temp.y * world_origin.y) / scroll.height);
    } else {
        sprite_campos.y = 0.0f;
    }
    if ((layer[l].central_point.x == 0.0f) && (layer[l].central_point.y == 0.0f)) {
        layer[l].central_point.x = (sprite_campos.x + (render_area.width / 2));
        layer[l].central_point.y = (sprite_campos.y + (render_area.height / 2));
    }

    for (uint32_t s = 0; s < layer[l].spr.size(); s ++) {

        // Resetea si es necesario el flag "on_screen"
        if (layer[l].spr[s]->runtime_frame != ngn->graphics->runtime_frame) layer[l].spr[s]->on_screen = false;

        // Calcula la posicion del sprite en pantalla segun el origen de dibujado
        screen_pos.x = layer[l].spr[s]->position.x - sprite_campos.x;
        screen_pos.y = layer[l].spr[s]->position.y - sprite_campos.y;

        // Si existe una animacion y no hay pausa, aplicala
        if (!animation_pause) layer[l].spr[s]->PlayAnimation();

        // Si esta dentro de la pantalla, dibujalo en el renderer e indicalo
        if (
            (screen_pos.x > -(layer[l].spr[s]->width / 2.0f))
            &&
            (screen_pos.x < (render_area.width + (layer[l].spr[s]->width / 2.0f)))
            &&
            (screen_pos.y > -(layer[l].spr[s]->height / 2.0f))
            &&
            (screen_pos.y < (render_area.height + (layer[l].spr[s]->height / 2.0f)))
        ) {
            // Indica que esta en pantalla
            layer[l].spr[s]->on_screen |= true;
            // Y dibujalo
            ngn->render->Sprite(layer[l].spr[s], screen_pos.x, ((float)screen_pos.y + shake_effect.offset.y));
        }

    }

}



/*** Quita un fondo de textura de la camara ***/
int32_t NGN_Camera::RemoveBackground(NGN_Texture* texture) {

    int32_t r = -1;

    if (!texture) return r;

    // Capas
    for (uint32_t l = 0; l < layer.size(); l ++) {
        // Si no es la textura que buscas, continua
        if (layer[l].texture != texture) continue;
        // Si encuentras la textura, limpia la capa de fondos (conserva los sprites)
        layer[l].texture = nullptr;
        r = l;
        // Sal del bucle
        break;
    }

    // Si no queda contenido en esta capa, restaura los valores de esa capa
    if (!layer[r].texture && !layer[r].tiled_bg && (layer[r].spr_t.size() == 0) && (layer[r].spr.size() == 0) && (r >= 0)) {
        layer[r].layer_size.texture.width = layer[r].layer_size.texture.height = 0;
        layer[r].layer_size.tiled_bg.width = layer[r].layer_size.tiled_bg.height = 0;
        layer[r].layer_size.virtual_bg.width = layer[r].layer_size.virtual_bg.height = 0;
        layer[r].layer_size.sprites.width = layer[r].layer_size.sprites.height = NGN_DEFAULT_VALUE;
        layer[r].visible = true;
        layer[r].in_use = false;
        layer[r].is_texture = false;
        layer[r].is_tiled = false;
        layer[r].is_virtual = false;
    }

    return r;

}



/*** Quita un fondo de tiles de la camara ***/
int32_t NGN_Camera::RemoveBackground(NGN_TiledBg* background) {

    int32_t r = -1;

    if (!background) return r;

    // Capas
    for (uint32_t l = 0; l < layer.size(); l ++) {
        // Si no es la textura que buscas, continua
        if (layer[l].tiled_bg != background) continue;
        // Si encuentras la textura, limpia la capa de fondos (conserva los sprites)
        layer[l].tiled_bg = nullptr;
        r = l;
        // Sal del bucle
        break;
    }

    // Si no queda contenido en esta capa, restaura los valores de esa capa
    if (!layer[r].texture && !layer[r].tiled_bg && (layer[r].spr_t.size() == 0) && (layer[r].spr.size() == 0) && (r >= 0)) {
        layer[r].layer_size.texture.width = layer[r].layer_size.texture.height = 0;
        layer[r].layer_size.tiled_bg.width = layer[r].layer_size.tiled_bg.height = 0;
        layer[r].layer_size.virtual_bg.width = layer[r].layer_size.virtual_bg.height = 0;
        layer[r].layer_size.sprites.width = layer[r].layer_size.sprites.height = NGN_DEFAULT_VALUE;
        layer[r].visible = true;
        layer[r].in_use = false;
        layer[r].is_texture = false;
        layer[r].is_tiled = false;
        layer[r].is_virtual = false;
    }

    return r;

}



/*** Quita un Sprite de la camara (1ra sobrecarga) ***/
int32_t NGN_Camera::RemoveSprite(NGN_Sprite* sprite) {

    int32_t r = -1;                     // Resultado de la operacion
    if (!sprite) return r;
    int32_t l = sprite->camera_layer;   // Capa donde esta el sprite alojado

    // Si el sprite no esta asignado a la camara, sal
    if (l < 0) return r;

    // Sprites
    if (layer[l].spr.size() > 0) {
        for (uint32_t s = 0; s < layer[l].spr.size(); s ++) {
            // Si el sprite es el solicitado...
            if (layer[l].spr[s] == sprite) {
                // Indica al sprite que ya no pertenece a ninguna capa
                sprite->camera_layer = -1;
                // Borra el elemento
                layer[l].spr.erase((layer[l].spr.begin() + s));
                r = 0;
                break;
            }
        }
    }

    //for (uint32_t l = 0; l < layer.size(); l ++) std::cout << "Sprites in layer " << l << ": " << layer[l].spr.size() << std::endl;

    // Devuelve el resultado de la operacion
    return r;

}



/*** Quita un Sprite de la camara (2da sobrecarga) ***/
int32_t NGN_Camera::RemoveSprite(NGN_Texture* texture) {

    int32_t r = -1;                         // Resultado de la operacion
    if (!texture) return r;
    int32_t l = texture->camera_layer;      // Capa donde esta el sprite alojado

    // Si el sprite no esta asignado a la camara, sal
    if (l < 0) return r;

    // Sprites
    if (layer[l].spr_t.size() > 0) {
        for (uint32_t s = 0; s < layer[l].spr_t.size(); s ++) {
            // Si el sprite es el solicitado...
            if (layer[l].spr_t[s] == texture) {
                // Indica al sprite que ya no pertenece a ninguna capa
                texture->camera_layer = -1;
                // Borra el elemento
                layer[l].spr_t.erase((layer[l].spr_t.begin() + s));
                r = 0;
                break;
            }
        }
    }

    //for (uint32_t l = 0; l < layer.size(); l ++) std::cout << "Sprites in layer " << l << ": " << layer[l].spr_t.size() << std::endl;

    // Devuelve el resultado de la operacion
    return r;

}



/*** Cambia un sprite de capa (1ra sobrecarga) ***/
int32_t NGN_Camera::ChangeLayer(NGN_Sprite* sprite, uint32_t layer_number) {

    uint32_t s = 0;
    uint32_t l = 0;
    int32_t r = -1;

    // Verifica que la capa sea valida y el sprite no sea nulo
    if ((layer_number < layer.size()) && sprite) {
        // Capa actual
        l = sprite->camera_layer;
        // Si la capa de origen y destino son identicas, sal y devuelve error
        if (l == layer_number) return r;
        // Busca el sprite en su capa asignada
        for (s = 0; s < layer[l].spr.size(); s ++) {
            if (layer[l].spr[s] == sprite) {
                // Borra el elemento de la capa actual
                layer[l].spr.erase((layer[l].spr.begin() + s));
                // Y añadelo a su nueva capa de destino
                r = PushSprite(layer_number, sprite);
                // Fuerza la salida del bucle
                break;
            }
        }
    }

    // Resultado de la operacion
    return r;

}



/*** Cambia un sprite de capa (2da sobrecarga) ***/
int32_t NGN_Camera::ChangeLayer(NGN_Texture* texture, uint32_t layer_number) {

    uint32_t s = 0;
    uint32_t l = 0;
    int32_t r = -1;

    // Verifica que la capa sea valida y el sprite no sea nulo
    if ((layer_number < layer.size()) && texture) {
        // Capa actual
        l = texture->camera_layer;
        // Si la capa de origen y destino son identicas, sal y devuelve error
        if (l == layer_number) return r;
        // Busca el sprite en su capa asignada
        for (s = 0; s < layer[l].spr_t.size(); s ++) {
            if (layer[l].spr_t[s] == texture) {
                // Borra el elemento de la capa actual
                layer[l].spr_t.erase((layer[l].spr_t.begin() + s));
                // Y añadelo a su nueva capa de destino
                r = PushSprite(layer_number, texture);
                // Fuerza la salida del bucle
                break;
            }
        }
    }

    // Resultado de la operacion
    return r;

}



/*** Manda un sprite al frente (1ra sobrecarga) ***/
int32_t NGN_Camera::SendToFront(NGN_Sprite* sprite) {

    int32_t r = -1;                     // Resultado de la operacion
    if (!sprite) return r;
    int32_t l = sprite->camera_layer;   // Capa donde esta el sprite alojado

    // Si el sprite no esta asignado a la camara, sal
    if (l < 0) return r;

    // Reordena la lista de sprite, poniendo el seleccionado al final
    std::vector<NGN_Sprite*> _tmp;
    _tmp.clear();
    _tmp.resize(layer[l].spr.size());
    uint32_t cnt = 0;
    for (uint32_t s = 0; s < layer[l].spr.size(); s ++) {
        if (layer[l].spr[s] == sprite) continue;
        _tmp[cnt] = layer[l].spr[s];
        cnt ++;
    }
    _tmp[cnt] = sprite;
    // Copia la lista temporal a la original
    layer[l].spr = _tmp;
    // Y borra la lista temporal
    _tmp.clear();

    // Devuelve el resultado de la operacion
    return cnt;

}



/*** Manda un sprite al frente (2da sobrecarga) ***/
int32_t NGN_Camera::SendToFront(NGN_Texture* texture) {

    int32_t r = -1;                     // Resultado de la operacion
    if (!texture) return r;
    int32_t l = texture->camera_layer;   // Capa donde esta el sprite alojado

    // Si el sprite no esta asignado a la camara, sal
    if (l < 0) return r;

    // Reordena la lista de sprite, poniendo el seleccionado al final
    std::vector<NGN_Texture*> _tmp;
    _tmp.clear();
    _tmp.resize(layer[l].spr_t.size());
    uint32_t cnt = 0;
    for (uint32_t s = 0; s < layer[l].spr_t.size(); s ++) {
        if (layer[l].spr_t[s] == texture) continue;
        _tmp[cnt] = layer[l].spr_t[s];
        cnt ++;
    }
    _tmp[cnt] = texture;
    // Copia la lista temporal a la original
    layer[l].spr_t = _tmp;
    // Y borra la lista temporal
    _tmp.clear();

    // Devuelve el resultado de la operacion
    return cnt;

}



/*** Manda un sprite al fondo (1ra sobrecarga) ***/
int32_t NGN_Camera::SendToBack(NGN_Sprite* sprite) {

    int32_t r = -1;                     // Resultado de la operacion
    if (!sprite) return r;
    int32_t l = sprite->camera_layer;   // Capa donde esta el sprite alojado

    // Si el sprite no esta asignado a la camara, sal
    if (l < 0) return r;

    // Reordena la lista de sprite, poniendo el seleccionado al final
    std::vector<NGN_Sprite*> _tmp;
    _tmp.clear();
    _tmp.resize(layer[l].spr.size());
    uint32_t cnt = 0;
    _tmp[cnt] = sprite;
    cnt ++;
    for (uint32_t s = 0; s < layer[l].spr.size(); s ++) {
        if (layer[l].spr[s] == sprite) continue;
        _tmp[cnt] = layer[l].spr[s];
        cnt ++;
    }
    // Copia la lista temporal a la original
    layer[l].spr = _tmp;
    // Y borra la lista temporal
    _tmp.clear();

    // Devuelve el resultado de la operacion
    return 0;

}



/*** Manda un sprite al fondo (2da sobrecarga) ***/
int32_t NGN_Camera::SendToBack(NGN_Texture* texture) {

    int32_t r = -1;                     // Resultado de la operacion
    if (!texture) return r;
    int32_t l = texture->camera_layer;   // Capa donde esta el sprite alojado

    // Si el sprite no esta asignado a la camara, sal
    if (l < 0) return r;

    // Reordena la lista de sprite, poniendo el seleccionado al final
    std::vector<NGN_Texture*> _tmp;
    _tmp.clear();
    _tmp.resize(layer[l].spr_t.size());
    uint32_t cnt = 0;
    _tmp[cnt] = texture;
    cnt ++;
    for (uint32_t s = 0; s < layer[l].spr_t.size(); s ++) {
        if (layer[l].spr_t[s] == texture) continue;
        _tmp[cnt] = layer[l].spr_t[s];
        cnt ++;
    }
    // Copia la lista temporal a la original
    layer[l].spr_t = _tmp;
    // Y borra la lista temporal
    _tmp.clear();

    // Devuelve el resultado de la operacion
    return cnt;

}



/*** Verifica si un sprite esta registrado en la camara [1a sobrecarga] ***/
bool NGN_Camera::CheckIfRegistered(NGN_Sprite* sprite) {

    if (!sprite) return false;          // Si el sprite es nulo, sal

    int32_t l = sprite->camera_layer;   // Capa donde esta el sprite alojado
    if (l < 0) return false;            // Si la capa es invalida, no esta en la camara

    // Busca el sprite en la camara
    for (uint32_t s = 0; s < layer[l].spr.size(); s ++) {
        if (layer[l].spr[s] == sprite) return true;
    }

    // Si no lo encuentras, devuelve false
    return false;

}



/*** Verifica si un sprite (textura) esta registrado en la camara [2a sobrecarga] ***/
bool NGN_Camera::CheckIfRegistered(NGN_Texture* texture) {

    if (!texture) return false;             // Si la textura es nula, sal

    int32_t l = texture->camera_layer;      // Capa donde esta alojada
    if (l < 0) return false;                // Si la capa es invalida, no esta en la camara

    // Busca la textura en la camara
    for (uint32_t s = 0; s < layer[l].spr_t.size(); s ++) {
        if (layer[l].spr_t[s] == texture) return true;
    }

    // Si no la encuentras, devuelve false
    return false;

}



/*** Devuelve el tamaño actual del renderer de esta camara ***/
Size2I32 NGN_Camera::GetRendererSize() {

    return render_area;

}



/*** Devuelve la posicion de una capa en la camara (punto central de la misma) ***/
Vector2 NGN_Camera::GetLayerPosition(uint32_t layer_number) {

    if (layer_number >= layer.size()) return Vector2::Zero();
    return layer[layer_number].central_point;

}



/*** Devuelve el tamaño asignado a una capa ***/
Size2I32 NGN_Camera::GetLayerSize(uint32_t layer_number) {

    Size2I32 layer_size = {0, 0};

    if (layer_number >= layer.size()) return layer_size;

    if (layer[layer_number].is_virtual) return layer[layer_number].layer_size.virtual_bg;
    if (layer[layer_number].is_tiled) return layer[layer_number].layer_size.tiled_bg;
    if (layer[layer_number].is_texture) return layer[layer_number].layer_size.texture;

    if ((layer[layer_number].spr.size() > 0) || (layer[layer_number].spr_t.size() > 0)) return layer[layer_number].layer_size.sprites;

    return layer_size;

}



/*** Ejecuta el efecto de "temblor" en la camara ***/
void NGN_Camera::Shake(float intensity, float frequency, bool split) {

    // Si no hay capas, sal
    if (layer.size() == 0) return;

    // Si la intensidad es 0, desactiva y reinicia el efecto
    if (intensity == 0.0f) {
        shake_effect.angle = 0.0f;
        shake_effect.offset = {0.0f, 0.0f};
    }

    // Almacena los valores
    shake_effect.intensity = intensity;
    shake_effect.frequency = split ? (frequency / (float)layer.size()):frequency;
    shake_effect.split = split;

}



// Asigna el conlor de tinte a una capa
void NGN_Camera::SetLayerTintColor(uint32_t layer_number, uint8_t r, uint8_t g, uint8_t b) {

    if (layer_number >= layer.size()) return;

    layer[layer_number].tint_color.r = r;
    layer[layer_number].tint_color.g = g;
    layer[layer_number].tint_color.b = b;

}



/*** Reinicia la camara ***/
void NGN_Camera::Reset() {

    // Limpia las listas de sprites
    for (uint32_t i = 0; i < layer.size(); i ++) {
        layer[i].spr_t.clear();
        layer[i].spr.clear();
    }
    // Limpia la lista de capas
    layer.clear();

    // Parametros iniciales
    target = nullptr;
    position = Vector2I32::Zero();
    scroll.width = scroll.height = 0.0f;
    world_lookat = Vector2::Zero();
    world_position = Vector2::Zero();
    animation_pause = false;
    runtime_frame = 0;
    shake_effect.intensity = 0.0f;
    shake_effect.frequency = 0.0f;
    shake_effect.angle = 0.0f;
    shake_effect.offset = {0.0f, 0.0f};
    shake_effect.split = false;
    shake_effect.angle_increased = false;

}



/*** Calcula el efecto "shake" para esta capa ***/
void NGN_Camera::ApplyShake(uint32_t l) {

    // Si ya se ha actualizado en este frame, ignora la orden
    if (runtime_frame == ngn->graphics->runtime_frame) return;

    // Si esta en pausa, ignora la orden
    if (animation_pause) return;

    // De existir, actualiza el efecto "shake"
    if (shake_effect.intensity <= 0.0f) return;

    // Calcula el factor de intensidad segun el tamaño de la capa
    float deep = 0.0f;
    Size2I32 layer_size = GetLayerSize(l);
    if (l == 0) shake_effect.angle_increased = false;
    if (world.width > world.height) {
        deep = ((float)layer_size.width / (float) world.width);
    } else {
        deep = ((float)layer_size.height / (float) world.height);
    }

    // Calcula el angulo segun la frecuencia
    if (!shake_effect.angle_increased || shake_effect.split) {
        shake_effect.angle += shake_effect.frequency;
        while (shake_effect.angle > shake_effect.angle_limit) shake_effect.angle -= shake_effect.angle_limit;
        shake_effect.angle_increased = true;
    }

    // Calcula el offset
    shake_effect.offset.y = std::round(std::sin(shake_effect.angle) * (shake_effect.intensity * deep));

}
