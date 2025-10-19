/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0-wip_0x04 ***
    Sprites

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
#include <vector>
#include <string>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/*** Contructor (1ra sobrecarga) ***/
NGN_Sprite::NGN_Sprite(
    NGN_SpriteData* sprite,         // Objeto de la clase Sprite Data
    int32_t position_x,             // Posicion X inicial (oculto por defecto)
    int32_t position_y,             // Posicion Y inicial (oculto por defecto)
    uint32_t sprite_width,          // Ancho del sprite (por defecto, el de la textura)
    uint32_t sprite_height,         // Altura del sprite (por defecto, la de la textura)
    uint32_t box_width,             // Ancho de la caja de colisiones
    uint32_t box_height,            // Alto de la caja de colisiones
    int32_t box_offset_x,           // Offset horizontal de la caja de colisiones
    int32_t box_offset_y            // Offset vertical de la de colisiones
) {

    // Crea el sprite
    CreateSprite(
        sprite,             // Objeto de la clase Sprite Data
        position_x,         // Posicion X inicial (oculto por defecto)
        position_y,         // Posicion Y inicial (oculto por defecto)
        sprite_width,       // Ancho del sprite (por defecto, el de la textura)
        sprite_height,      // Altura del sprite (por defecto, la de la textura)
        box_width,          // Ancho de la caja de colisiones
        box_height,         // Alto de la caja de colisiones
        box_offset_x,       // Offset horizontal de la caja de colisiones
        box_offset_y        // Offset vertical de la de colisiones
    );

}



/*** Contructor (2da sobrecarga) ***/
NGN_Sprite::NGN_Sprite(
    std::string repo_name,          // Nombre del repositorio
    std::string resource_name,      // Nombre del recurso
    int32_t position_x,             // Posicion X inicial (oculto por defecto)
    int32_t position_y,             // Posicion Y inicial (oculto por defecto)
    uint32_t sprite_width,          // Ancho del sprite (por defecto, el de la textura)
    uint32_t sprite_height,         // Altura del sprite (por defecto, la de la textura)
    uint32_t box_width,             // Ancho de la caja de colisiones
    uint32_t box_height,            // Alto de la caja de colisiones
    int32_t box_offset_x,           // Offset horizontal de la caja de colisiones
    int32_t box_offset_y            // Offset vertical de la de colisiones
) {

    // Crea el sprite
    CreateSprite(
        ngn->resources->GetSprite(repo_name, resource_name),    // Objeto de la clase Sprite Data
        position_x,         // Posicion X inicial (oculto por defecto)
        position_y,         // Posicion Y inicial (oculto por defecto)
        sprite_width,       // Ancho del sprite (por defecto, el de la textura)
        sprite_height,      // Altura del sprite (por defecto, la de la textura)
        box_width,          // Ancho de la caja de colisiones
        box_height,         // Alto de la caja de colisiones
        box_offset_x,       // Offset horizontal de la caja de colisiones
        box_offset_y        // Offset vertical de la de colisiones
    );

}



/*** Destructor ***/
NGN_Sprite::~NGN_Sprite() {

    // Elimina la referencia al grafico
    data = nullptr;

}



/*** Posiciona el sprite ***/
void NGN_Sprite::Position(float position_x, float position_y) {

    // Asigna la posicion al Sprite
    position.x = position_x;
    position.y = position_y;

}
void NGN_Sprite::Position(Vector2 pos) {

    // Asigna la posicion al Sprite
    Position(pos.x, pos.y);

}



/*** Mueve un sprite ***/
void NGN_Sprite::Translate(float speed_x, float speed_y) {

    // Calcula la nueva posicion
    position.x += speed_x;
    position.y += speed_y;

}
void NGN_Sprite::Translate(Vector2 spd) {

    // Calcula la nueva posicion
    Translate(spd.x, spd.y);

}



/*** Cambia el tamaño del sprite ***/
void NGN_Sprite::Size(float w, float h) {

    // Aplica el nuevo tamaño
    width = w;
    original_size.width = width;
    height = h;
    original_size.height = height;
    current_scale.width = 1.0f;
    current_scale.height = 1.0f;

}



/*** Escala un sprite [Sobrecarga 1 - Ejes por separado] ***/
void NGN_Sprite::Scale(float w, float h) {

    // Aplica la escala
    width = original_size.width * w;
    height = original_size.height * h;

    // Guarda la escala
    current_scale.width = w;
    current_scale.height = h;

}

/*** Escala un sprite [Sobrecarga 2 - Ambos ejes a la vez] ***/
void NGN_Sprite::Scale(float scale) {

    // Aplica la escala
    Scale(scale, scale);

}



/*** Devuelve la escala actual del sprite ***/
Size2 NGN_Sprite::GetCurrentScale() {

    return current_scale;

}



/*** Rota un sprite los grados solicitados ***/
void NGN_Sprite::Rotate(double degrees) {

    rotation += degrees;
    while (rotation >= 360.0f) rotation -= 360.0f;
    while (rotation < 0.0f) rotation += 360.0f;

}



/*** Fija el centro del Sprite (Posicion relativa desde el centro REAL) ***/
void NGN_Sprite::SetCenter(float x, float y) {

    center.x = x;
    center.y = y;

}


/*** Funcion interna de la libreria: Calcula el radio del sprite para los calculos avanzados de colision ***/
float NGN_Sprite::GetSpriteRadius() {

    // Decide si hay que recalcular el radio (ha cambiado de tamaño?)
    if ((radius_info.width != width) || (radius_info.height != height)) {
        radius_info.radius = (std::sqrt((width * width) + (height * height)) / 2.0f);
        radius_info.width = width;
        radius_info.height = height;
    }

    // Devuelve el valor del radio del sprite
    return radius_info.radius;

}


/*** Añade un nuevo collider al Sprite ***/
int32_t NGN_Sprite::AddCollider(
    std::string name,           // Nombre del colisionador
    float offset_x,             // Offset X
    float offset_y,             // Offset Y
    float width,                // Ancho del colisionador
    float height                // Altura del colisionador
) {

    // Verifica el nombre (longitud)
    if ((name.length() < 1) || (name.length() > 255)) return 1;
    // Verifica el nombre (nombre repetido)
    if (GetColliderId(name) >= 0) return 2;

    // Crea un collider temporal con los datos necesarios
    ColliderData c;
    c.name = name;
    c.offset.x = offset_x;
    c.offset.y = offset_y;
    c.width = width;
    c.height = height;
    c.enabled = true;

    // Registra el collider en el vector
    colliders.push_back(c);

    // Collider creado con exito
    return 0;

}



/*** Opten la ID de un collider ***/
int32_t NGN_Sprite::GetColliderId(std::string name) {

    // Resultado
    int32_t r = -1;

    // Busca el collider
    for (uint32_t i = 0; i < colliders.size(); i ++) {
        if (name == colliders[i].name) {
            r = i;
            break;
        }
    }

    // Revuelve el resultado
    return r;

}



/*** Cambia el estado de un collider ***/
int32_t NGN_Sprite::ColliderEnabled(std::string name, bool status) {

    // Busca y verifica la ID del collider
    int32_t id = GetColliderId(name);
    if (id < 0) return 1;

    // Cambia su estado
    colliders[id].enabled = status;

    // Cambio correcto
    return 0;

}



/*** Elimina un collider ***/
int32_t NGN_Sprite::RemoveCollider(std::string name) {

    // Busca y verifica la ID del collider
    int32_t id = GetColliderId(name);
    if (id < 0) return 1;

    // Elimina el collider del vector
    colliders.erase((colliders.begin() + id));

    // Eliminacion correcta
    return 0;

}



/*** Añade una animacion al Sprite ***/
int32_t NGN_Sprite::AddAnimation(
                              std::string name,
                              int32_t first_frame,
                              int32_t last_frame,
                              int32_t loop,
                              int32_t frame_duration
                            ) {

    // Verifica el nombre
    if ((name.length() < 1) || (name.length() > 255)) return 1;
    for (uint32_t i = 0; i < animation.size(); i ++) if (animation[i].name == name) return 2;

    // Proteccion de frames
    if (
        (first_frame < 0) || (first_frame >= total_frames)
        ||
        (last_frame < 0) || (last_frame >= total_frames)
        ||
        (loop < 0) || (loop >= total_frames)
    ) return 1;


    // Variable temporal con la animacion
    animation_data a;
    a.name = name;
    a.id = animation.size();
    a.first_frame = first_frame;
    a.last_frame = last_frame;
    a.loop = loop;
    a.frame_duration = frame_duration;

    // Añade la animacion
    animation.push_back(a);

    // Todo correcto
    return 0;

}



/*** Selecciona la animacion del Sprite ***/
int32_t NGN_Sprite::SetAnimation(std::string name) {

    // Se solicita la animacion seleccionada actualmente?
    if (name == current_animation.name) return 2;

    // Si no, busca la animacion
    int32_t a = -1;
    for (uint32_t i = 0; i < animation.size(); i ++) {
        if (animation[i].name == name) {
            a = i;
            break;
        }
    }
    // Animacion no encontrada
    if (a < 0) return 1;

    // Si la animacion existe, registrala como la actual y reinicia los contadores
    current_animation.name = animation[a].name;
    current_animation.id = animation[a].id;
    current_animation.first_frame = animation[a].first_frame;
    current_animation.last_frame = animation[a].last_frame;
    current_animation.loop = animation[a].loop;
    current_animation.frame_duration = animation[a].frame_duration;
    animation_timer = 0;
    frame = current_animation.first_frame;

    // Animacion seleccionada con exito
    return 0;

}



/*** Reproduce la animacion del Sprite ***/
void NGN_Sprite::PlayAnimation() {

    // Si hay una animacion valida cargada, no se repite el frame y no esta en pausa, reproduce la animacion
    if (current_animation.id < 0) return;
    if (runtime_frame == ngn->graphics->runtime_frame) return;
    if (animation_pause) return;

    animation_timer ++;
    if (animation_timer >= current_animation.frame_duration) {
        animation_timer = 0;
        if (current_animation.first_frame <= current_animation.last_frame) {
            frame ++;
            if (frame > current_animation.last_frame) frame = current_animation.loop;
        } else {
            frame --;
            if (frame < current_animation.last_frame) frame = current_animation.loop;
        }
    }

    runtime_frame = ngn->graphics->runtime_frame;

}



/*** Selecciona un color de tinte (sin parametros, resetea el color) ***/
void NGN_Sprite::SetTintColor(uint8_t r, uint8_t g, uint8_t b) {

    // Registra el color
    tint_color.r = r;
    tint_color.g = g;
    tint_color.b = b;

}



/*** Devuelve la capa actual en la camara que tienes asignado (-1 si no estas en ninguna capa) ***/
int32_t NGN_Sprite::GetCameraLayer() {

    return camera_layer;

}



/*** Devuelve si se ha cambiado el color de tinta en este frame ***/
bool NGN_Sprite::NewTint() {

    if (runtime_frame == ngn->graphics->runtime_frame) return true;

    bool color_mod = ((tint_color.r != last_tint_color.r) || (tint_color.g != last_tint_color.g) || (tint_color.b != last_tint_color.b));
    color_mod |= (frame != tint_last_frame);

    if (!color_mod) return false;

    last_tint_color = tint_color;
    tint_last_frame = frame;
    return true;

}



/*** Crea el objeto que contiene el sprite ***/
void NGN_Sprite::CreateSprite(
    NGN_SpriteData* sprite,     // Objeto de la clase Sprite Data
    int32_t position_x,         // Posicion X inicial (oculto por defecto)
    int32_t position_y,         // Posicion Y inicial (oculto por defecto)
    uint32_t sprite_width,      // Ancho del sprite (por defecto, el de la textura)
    uint32_t sprite_height,     // Altura del sprite (por defecto, la de la textura)
    uint32_t box_width,         // Ancho de la caja de colisiones
    uint32_t box_height,        // Alto de la caja de colisiones
    int32_t box_offset_x,       // Offset horizontal de la caja de colisiones
    int32_t box_offset_y        // Offset vertical de la de colisiones
) {

    // Guarda el grafico que usara este sprite
    data = sprite;

    // Tamaño
    if ((sprite_width != NGN_DEFAULT_VALUE) && (sprite_height != NGN_DEFAULT_VALUE)) {
        // Tamaño definido por el usuario
        width = sprite_width;
        height = sprite_height;
    } else {
        // Tamaño en base a la textura del grafico
        width = data->header.frame_width;
        height = data->header.frame_height;
    }

    // Guarda el tamaño original al crear el sprite
    original_size.width = width;
    original_size.height = height;
    current_scale.width = 1.0f;
    current_scale.height = 1.0f;

    // Tamaño de la caja de colisiones
    if ((box_width != NGN_DEFAULT_VALUE) && (box_height != NGN_DEFAULT_VALUE)) {
        box.width = box_width;
        box.height = box_height;
    } else {
        box.width = width;
        box.height = height;
    }
    // Caja de colisiones habilitada por defecto
    box_enabled = true;

    // Prepara la lista de colisionadores adicionales
    colliders.clear();

    // Offset de la caja de colisiones
    if ((box_offset_x != NGN_DEFAULT_VALUE) && (box_offset_y != NGN_DEFAULT_VALUE)) {
        box.offset.x = box_offset_x;
        box.offset.y = box_offset_y;
    } else {
        box.offset.x = 0.0f;
        box.offset.y = 0.0f;
    }

    // Posicion
    if ((position_x != NGN_DEFAULT_VALUE) && (position_y != NGN_DEFAULT_VALUE)) {
        // Posicion definida por el usuario
        position.x = position_x;
        position.y = position_y;
    } else {
        // Posicion fuera del escenario
        position.x = -width;
        position.y = -height;
    }

    // Valores por defecto
    frame = 0;                                  // Primer fotograma
    total_frames = data->header.total_frames;   // Nº total de fotogramas
    visible = true;                             // Visibilidad
    alpha = 0xFF;                               // Alpha
    blend_mode = NGN_BLENDMODE_ALPHA;           // Modo de mezcla
    on_screen = false;                          // Reset del flag
    rotation = 0.0f;                            // Rotacion
    center.x = 0.0f;                            // Centro de rotacion
    center.y = 0.0f;
    flip_h = false;                             // Flip
    flip_v = false;
    screen.x = (uint32_t)position.x;
    screen.y = (uint32_t)position.y;

    // Animaciones
    animation.clear();
    current_animation.name = "";
    current_animation.id = -1;
    animation_pause = false;
    animation_timer = 0;

    // Otros parametros
    camera_layer = -1;      // Por defecto, no estas en la camara

    // Color de tinta
    tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    last_tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    tint_last_frame = 0;
    ignore_camera_tint = false;

    // Gestion de la geometria del sprite
    radius_info.radius = -1.0f;
    radius_info.width = -1.0f;
    radius_info.height = -1.0f;

}
