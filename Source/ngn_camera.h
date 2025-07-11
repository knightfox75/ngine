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



#ifndef NGN_CAMERA_H_INCLUDED
#define NGN_CAMERA_H_INCLUDED


/*** Includes ***/

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>

// n'gine
#include "ngn_defines.h"
#include "ngn_sprite.h"
#include "ngn_tiledbg.h"



/******************************************************************************

    Clase NGN_Camera

******************************************************************************/

/*** Declaracion de la clase de la Camara virtual en 2D ***/
class NGN_Camera {

    // Metodos y propiedades publicas
    public:

        // Contructor de la clase
        NGN_Camera();

        // Destructor de la clase
        ~NGN_Camera();


        // Tamaño del mundo
        Size2I32 world;
        // Posicion de la camara en el mundo
        Vector2I32 position;


        // Pausa la animacion de los sprites/efectos
        bool animation_pause;

        // Define en numero de capas a crear (elimina las existentes)
        void CreateLayers(uint32_t layers);

        // Define el tamaño la capa de sprites, si en esa capa no existe ningun fondo
        void SetSizeOfSpritesLayer(uint32_t layer_number, uint32_t width, uint32_t height);

        // Inicializa la camara
        void Setup(uint32_t world_width, uint32_t world_height, NGN_Sprite* target_sprite = nullptr);

        // Añade un fondo de textura o de tiles a la capa y devuelve su indice en la lista. En caso de error, devuelve -1
        int32_t PushBackground(uint32_t layer_number, NGN_Texture* texture);
        int32_t PushBackground(uint32_t layer_number, NGN_TiledBg* background);

        // Añade un fondo de textura o de tiles en modo tamaño virtual a la capa y devuelve su indice en la lista. En caso de error, devuelve -1
        int32_t PushVirtualBg(uint32_t layer_number, NGN_Texture* texture, uint32_t bg_width, uint32_t bg_height, uint32_t loop_x, uint32_t loop_y, float auto_x = 0.0f, float auto_y = 0.0f);
        int32_t PushVirtualBg(uint32_t layer_number, NGN_TiledBg* background, uint32_t bg_width, uint32_t bg_height, uint32_t loop_x, uint32_t loop_y, float auto_x = 0.0f, float auto_y = 0.0f);

        // Añade un sprite a la capa  y devuelve su indice en la lista. En caso de error, devuelve -1
        int32_t PushSprite(uint32_t layer_number, NGN_Sprite* sprite);          // Primera sobrecarga
        int32_t PushSprite(uint32_t layer_number, NGN_Texture* texture);        // Segunda sobrecarga

        // Selecciona donde mira la camara
        void LookAt(NGN_Sprite* target_sprite);                     // Sigue a un sprite
        void LookAt(uint32_t position_x, uint32_t position_y);      // Colocala en la coordenada indicada
        void LookAt(Vector2I32 pos);
        Vector2 GetLookAt();                // Recupera el punto central de la camara en el mundo
        NGN_Sprite* GetTargetSprite();      // Recupera el sprite al que sigue la camara, si es que sigue alguno
        Vector2 GetTargetPosition();        // Recupera la posicion a la que se le ha ordenado colocarse la camara

        // Actualiza la vista de la camara
        void Update();

        // Quita un fondo de la camara
        int32_t RemoveBackground(NGN_Texture* texture);         // Primera sobrecarga
        int32_t RemoveBackground(NGN_TiledBg* background);      // Segunda sobrecarga

        // Quita un Sprite de la camara
        int32_t RemoveSprite(NGN_Sprite* sprite);       // Primera sobrecarga
        int32_t RemoveSprite(NGN_Texture* texture);     // Segunda sobrecarga

        // Cambia un elemento de capa
        int32_t ChangeLayer(NGN_Sprite* sprite, uint32_t layer_number);         // Primera sobrecarga
        int32_t ChangeLayer(NGN_Texture* texture, uint32_t layer_number);       // Segunda sobrecarga

        // Manda al frente de la capa a un elemento
        int32_t SendToFront(NGN_Sprite* sprite);        // Primera sobrecarga
        int32_t SendToFront(NGN_Texture* texture);      // Segunda sobrecarga

        // Manda al fondo de la capa a un elemento
        int32_t SendToBack(NGN_Sprite* sprite);         // Primera sobrecarga
        int32_t SendToBack(NGN_Texture* texture);       // Segunda sobrecarga

        // Busca si un sprite esta registrado en la camara
        bool CheckIfRegistered(NGN_Sprite* sprite);     // Primera sobrecarga
        bool CheckIfRegistered(NGN_Texture* texture);   // Segunda sobrecarga

        // Devuelve el tamaño actual del renderer de esta camara
        Size2I32 GetRendererSize();


        // Devuelve la posicion de una capa en la camara (punto central de la misma)
        Vector2 GetLayerPosition(uint32_t layer_number);
        // Devuelve el tamaño asignado a una capa
        Size2I32 GetLayerSize(uint32_t layer_number);

        // Ejecuta el efecto de "temblor" en la camara
        void Shake(float intensity, float frequency, bool split = true);

        // Asigna el conlor de tinte a una capa
        void SetLayerTintColor(uint32_t layer_number, uint8_t r, uint8_t g, uint8_t b);

        // Reset de la camara
        void Reset();



    // Datos privados de la clase
    private:

        // Estructura de capas de dibujado
        struct LayerData{
            NGN_Texture* texture;               // Fondo de textura en esta capa
            NGN_TiledBg* tiled_bg;              // Fondo de tiles en esta capa
            std::vector<NGN_Texture*> spr_t;    // Texturas como Sprites para esta capa
            std::vector<NGN_Sprite*> spr;       // Sprites en esta capa
            struct {
                Size2I32 texture;
                Size2I32 tiled_bg;
                Size2I32 virtual_bg;
                Size2I32 sprites;
            } layer_size;                       // Tamaño de la capa
            bool visible;                       // Visibilidad de la capa
            bool in_use;                        // Hay datos en la capa
            bool is_texture;                    // Es una capa de textura
            bool is_tiled;                      // Es una capa de fondo de tiles
            bool is_virtual;                    // Es una capa de tamaño virtual
            Vector2 central_point;              // Posicion del centro de la capa en el mundo (es decir donde ha desplazado la capa la camara)
            Rgba tint_color;                    // Color de tinte de la capa
            Rgba last_tint_color;               // Ultimo color asignado (para el control de cambios)
        };

        // Parametros internos de la camara para su funcionamiento
        NGN_Sprite* target;                     // Sprite al que seguira la camara
        Size2I32 scroll;                        // Tamaño total del scroll en el mundo
        Size2I32 render_area;                   // Tamaño del area del render
        uint32_t runtime_frame;                 // ID del fotograma actual (evita duplicidades al usar varios viewports)

        // Calculos de la camara
        Vector2I64 world_origin;                // Esquina superior-izquierda del mundo en pantalla (64 bits de rango para evitar desvordes)
        Vector2I32 screen_pos;                  // Posicion del objeto en la pantalla
        Vector2I32 sprite_campos;               // Posicion del sprite en la camara
        Vector2I64 temp;                        // Vector2 de uso general (64 bits de rango para evitar desvordes)
        Vector2 world_lookat;                   // Posicion a la que mira la camara en el mundo
        Vector2 world_position;                 // Posicion real en la que la camara esta colocada

        // Funciones de actualizacion
        void ApplyTintColor(uint32_t l);            // Aplica el tinte a las capas que lo requieran
        void RenderTextures(uint32_t l);            // Fondos de textura
        void RenderTiles(uint32_t l);               // Fondos de tiles
        void RenderTextureSprites(uint32_t l);      // Texturas
        void RenderSprites(uint32_t l);             // Sprites

        // Efecto de "temblor"
        struct {
            float intensity;
            float frequency;
            float angle;
            const float angle_limit = 6.2831854f;
            Vector2 offset;
            bool split;
            bool angle_increased;
        } shake_effect;
        void ApplyShake(uint32_t l);




    // Vector de datos gestionados por la camara
    public:

        // Vector de memoria con las capas
        std::vector<LayerData> layer;

};



#endif // NGN_CAMERA_H_INCLUDED
