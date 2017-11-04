/******************************************************************************

    N'gine Lib for C++
    *** Version 0.2.1-alpha ***
    Sprites

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



#ifndef NGN_SPRITE_H_INCLUDED
#define NGN_SPRITE_H_INCLUDED



/*** Includes ***/

// C++
#include <vector>
#include <string>

// Includes de la libreria
#include "ngn_defines.h"



/*** Definicion de la clase ***/
class NGN_Sprite {

   // Public
    public:

        // Contructor
        NGN_Sprite(
                    NGN_SpriteData* sprite,                // Objeto de la clase Sprite Data
                    int32_t position_x = DEFAULT_VALUE,         // Posicion X inicial (oculto por defecto)
                    int32_t position_y = DEFAULT_VALUE,         // Posicion Y inicial (oculto por defecto)
                    int32_t sprite_width = DEFAULT_VALUE,       // Ancho del sprite (por defecto, el de la textura)
                    int32_t sprite_height = DEFAULT_VALUE,      // Altura del sprite (por defecto, la de la textura)
                    int32_t box_width = DEFAULT_VALUE,          // Ancho de la caja de colisiones
                    int32_t box_height = DEFAULT_VALUE          // Alto de la caja de colisiones
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
        Size2 box;                  // Caja de colisiones

        bool visible;               // Visibilidad
        int32_t alpha;                  // Nivel de alpha
        bool on_screen;             // Usar este flag para indicar si esta o no en pantalla

        double rotation;            // Rotacion del Sprite

        bool flip_h;                // Flip del sprite
        bool flip_v;

        // Posiciona el sprite
        void Position(float position_x, float position_y);

        // Mueve el sprite
        void Translate(float speed_x, float speed_y);

        // Cambia el tamaño del Sprite
        void Size(float w, float h);

        //  Escala un Sprite
        void Scale(float w, float h);   // [Sobrecarga 1]       Escala los dos ejes por separado
        void Scale(float scale);        // [Sobrecarga 2]       Escala ambos ejes a la vez

        // Rota el Sprite
        void Rotate(double degrees);

        // Fija el centro del Sprite (Posicion relativa desde el centro REAL)
        void SetCenter(float x, float y);

        // Gestor de animaciones
        uint32_t frame;                         // Fotograma actual
        struct animation_data{
            std::string name;                       // Nombre de la animacion
            int32_t id;                                 // ID de la animacion
            uint32_t first_frame;               // Primer frame de la animacion
            uint32_t last_frame;                // Ultimo frame (inclusive) de la animacion
            uint32_t loop;                      // Frame que es el punto de loop
            uint32_t frame_duration;            // Duracion de cada frame de la animacion en ciclos del juego
        };
        std::vector<animation_data> animation;      // Vector con las animaciones
        int32_t AddAnimation(
            std::string name,                       // Nombre de la animacion
            uint32_t first_frame,               // Primer fotograma de la animacion
            uint32_t last_frame,                // Ultimo fotograma de la animacion
            uint32_t loop,                      // Fotograma "punto de loop"
            uint32_t frame_duration             // Duracion en ciclos de cada fotograma
            );
        int32_t SetAnimation(std::string name = "");    // Selecciona una animacion a reproducir
        void PlayAnimation();                       // Reproduce la animacion seleccionada
        animation_data current_animation;           // Parametros de la animacion actual
        bool animation_pause;                       // Pausa la animacion actual

        // Si se da el caso, capa en la camara virtual 2D donde esta ubicado.
        int32_t camera_layer;



    // Private
    private:

        // Propiedades internas del Sprite
        Size2 original;         			// Tamaño original
        uint32_t animation_timer;       // Control de velocidad de la animacion

};



#endif // NGN_SPRITE_H_INCLUDED
