/******************************************************************************

    N'gine Lib for C++
    *** Version 1.17.0-stable ***
    Sistema de colisiones

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



#ifndef NGN_COLLISIONS_H_INCLUDED
#define NGN_COLLISIONS_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11



/*** Definicion de la clase ***/
class NGN_Collisions {

    // Public
    public:

        // Devuelve la instancia
        static NGN_Collisions* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


        // Consulta el color de un pixel del mapa de colisiones
        uint32_t GetPixel(NGN_CollisionMapData* cmap, int32_t position_x, int32_t position_y);

        // Devuelve el tamaño de un mapa de colisiones
        Size2 GetMapSize(NGN_CollisionMapData* cmap);

        // Consulta si hay colision por cajas entre 2 sprites
        bool HitBox(NGN_Sprite* spr1, NGN_Sprite* spr2);

        // Consulta si hay colision por "pixel perfect" entre 2 sprites
        bool PixelPerfect(NGN_Sprite* spr1, NGN_Sprite* spr2);

        // Consulta si has tocado un punto de un sprite (Raycast point)
        bool RaycastPoint(NGN_Sprite* spr, float position_x, float position_y);
        bool RaycastPoint(NGN_Sprite* spr, Vector2 position);


    // Private
    private:

        // Constructor
        NGN_Collisions();

        // Destructor
        ~NGN_Collisions();

        // Puntero de memoria a la instancia
        static NGN_Collisions* instance;


        // Algoritmo de colision por cajas
        bool CheckBoxColliders(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

        // Deteccion de colisiones entre sprites por "pixel perfect"
        SDL_Surface* RenderSpriteInSurface(
            NGN_Sprite* sprite,     // Sprite a renderizar
            int32_t x,              // Posicion
            int32_t y,
            uint32_t w,             // Tamaño del buffer
            uint32_t h
            );


};


#endif // NGN_COLLISIONS_H_INCLUDED
