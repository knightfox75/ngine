/******************************************************************************

    N'gine Lib for C++
    *** Version 1.18.0-stable ***
    Sprites

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
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



#ifndef NGN_SPRITE_H_INCLUDED
#define NGN_SPRITE_H_INCLUDED



/*** Includes ***/

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>
#include <string>

// Includes de la libreria
#include "ngn_defines.h"



/*** Definicion de la clase ***/
class NGN_Sprite {

   // Public
    public:

        // Contructor (1ra sobrecarga)
        NGN_Sprite(
            NGN_SpriteData* sprite,                                 // Objeto de la clase Sprite Data
            int32_t position_x = NGN_DEFAULT_VALUE,                 // Posicion X inicial (oculto por defecto)
            int32_t position_y = NGN_DEFAULT_VALUE,                 // Posicion Y inicial (oculto por defecto)
            uint32_t sprite_width = (uint32_t)NGN_DEFAULT_VALUE,    // Ancho del sprite (por defecto, el de la textura)
            uint32_t sprite_height = (uint32_t)NGN_DEFAULT_VALUE,   // Altura del sprite (por defecto, la de la textura)
            uint32_t box_width = (uint32_t)NGN_DEFAULT_VALUE,       // Ancho de la caja de colisiones
            uint32_t box_height = (uint32_t)NGN_DEFAULT_VALUE,      // Alto de la caja de colisiones
            int32_t box_offset_x = NGN_DEFAULT_VALUE,               // Offset horizontal de la caja de colisiones
            int32_t box_offset_y = NGN_DEFAULT_VALUE                // Offset vertical de la de colisiones
        );

        // Contructor (2da sobrecarga)
        NGN_Sprite(
            std::string repo_name,                                  // Nombre del repositorio
            std::string resource_name,                              // Nombre del recurso
            int32_t position_x = NGN_DEFAULT_VALUE,                 // Posicion X inicial (oculto por defecto)
            int32_t position_y = NGN_DEFAULT_VALUE,                 // Posicion Y inicial (oculto por defecto)
            uint32_t sprite_width = (uint32_t)NGN_DEFAULT_VALUE,    // Ancho del sprite (por defecto, el de la textura)
            uint32_t sprite_height = (uint32_t)NGN_DEFAULT_VALUE,   // Altura del sprite (por defecto, la de la textura)
            uint32_t box_width = (uint32_t)NGN_DEFAULT_VALUE,       // Ancho de la caja de colisiones
            uint32_t box_height = (uint32_t)NGN_DEFAULT_VALUE,      // Alto de la caja de colisiones
            int32_t box_offset_x = NGN_DEFAULT_VALUE,               // Offset horizontal de la caja de colisiones
            int32_t box_offset_y = NGN_DEFAULT_VALUE                // Offset vertical de la de colisiones
        );

        // Destructor
        ~NGN_Sprite();

        // Propiedades del sprite (publicas)
        NGN_SpriteData* data;       // Datos graficos del Sprite
        Vector2 position;           // Posicion del Sprite (Global/Mundo)
        Vector2I32 screen;          // Posicion del Sprite (Pantalla)
        float width;                // Tamaño
        float height;
        Vector2 center;             // Centro del Sprite para su rotacion

        struct {
            float width;            // Ancho de la caja
            float height;           // Alto de la caja
            struct {
                float x;            // Offset horizontal de la caja
                float y;            // Offset vertical de la caja
            } offset;
        } box;                      // Caja principal de colision

        bool box_enabled;           // Caja principal de colisiones habilitada

        bool visible;               // Visibilidad
        int32_t alpha;              // Nivel de alpha
        SDL_BlendMode blend_mode;   // Modo de mezcla de color
        bool on_screen;             // Usar este flag para indicar si esta o no en pantalla

        double rotation;            // Rotacion del Sprite

        bool flip_h;                // Flip del sprite
        bool flip_v;

        // Posiciona el sprite
        void Position(float position_x, float position_y);
        void Position(Vector2 pos);

        // Mueve el sprite
        void Translate(float speed_x, float speed_y);
        void Translate(Vector2 spd);

        // Cambia el tamaño del Sprite
        void Size(float w, float h);

        //  Escala un Sprite
        void Scale(float w, float h);   // [Sobrecarga 1]       Escala los dos ejes por separado
        void Scale(float scale);        // [Sobrecarga 2]       Escala ambos ejes a la vez

        // Rota el Sprite
        void Rotate(double degrees);

        // Fija el centro del Sprite (Posicion relativa desde el centro REAL)
        void SetCenter(float x, float y);


        // Gestion de colisionadores
        struct ColliderData{
            std::string name;       // ID (nombre) del collider
            struct {
                float x;            // Posicion X respecto al centro del sprite
                float y;            // Posicion Y respecto al centro del sprite
            } offset;
            float width;            // Ancho del collider
            float height;           // Altura del Collider
            bool enabled;           // Colisionador activo
        };
        std::vector<ColliderData> colliders;    // Colisionadores adicionales

        // Añade un nuevo collider
        int32_t AddCollider(
            std::string name,           // Nombre del colisionador
            float offset_x,             // Offset X
            float offset_y,             // Offset Y
            float width,                // Ancho del colisionador
            float height                // Altura del colisionador
        );

        // Busca la ID de un collider, devuelve -1 si no se encuentra
        int32_t GetColliderId(std::string name);

        // Habilita o deshabilita un collider
        int32_t ColliderEnabled(std::string name, bool status);

        // Elimina un collider
        int32_t RemoveCollider(std::string name);



        // Gestor de animaciones
        int32_t frame;                          // Fotograma actual
        int32_t total_frames;                   // Numero total de fotogramas
        struct animation_data{
            std::string name;                   // Nombre de la animacion
            int32_t id;                         // ID de la animacion
            int32_t first_frame;                // Primer frame de la animacion
            int32_t last_frame;                 // Ultimo frame (inclusive) de la animacion
            int32_t loop;                       // Frame que es el punto de loop
            int32_t frame_duration;             // Duracion de cada frame de la animacion en ciclos del juego
        };
        std::vector<animation_data> animation;      // Vector con las animaciones
        int32_t AddAnimation(
            std::string name,                       // Nombre de la animacion
            int32_t first_frame,                    // Primer fotograma de la animacion
            int32_t last_frame,                     // Ultimo fotograma de la animacion
            int32_t loop,                           // Fotograma "punto de loop"
            int32_t frame_duration                  // Duracion en ciclos de cada fotograma
            );
        int32_t SetAnimation(std::string name = "");    // Selecciona una animacion a reproducir
        void PlayAnimation();                           // Reproduce la animacion seleccionada
        animation_data current_animation;               // Parametros de la animacion actual
        bool animation_pause;                           // Pausa la animacion actual

        // Si se da el caso, capa en la camara virtual 2D donde esta ubicado.
        int32_t camera_layer;
        uint32_t runtime_frame;         // ID del frame en tiempo de ejecuci�n



    // Private
    private:

        // Propiedades internas del Sprite
        Size2 original;         		// Tamaño original
        int32_t animation_timer;        // Control de velocidad de la animacion

        // Crea el objeto que contiene el sprite
        void CreateSprite(
            NGN_SpriteData* sprite,     // Objeto de la clase Sprite Data
            int32_t position_x,         // Posicion X inicial (oculto por defecto)
            int32_t position_y,         // Posicion Y inicial (oculto por defecto)
            uint32_t sprite_width,      // Ancho del sprite (por defecto, el de la textura)
            uint32_t sprite_height,     // Altura del sprite (por defecto, la de la textura)
            uint32_t box_width,         // Ancho de la caja de colisiones
            uint32_t box_height,        // Alto de la caja de colisiones
            int32_t box_offset_x,       // Offset horizontal de la caja de colisiones
            int32_t box_offset_y        // Offset vertical de la de colisiones
        );

};



#endif // NGN_SPRITE_H_INCLUDED
