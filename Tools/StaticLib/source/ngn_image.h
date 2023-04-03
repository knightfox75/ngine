/******************************************************************************

    N'gine Lib for C++
    *** Version 1.13.0-stable ***
    Funciones para la manipulacion de imagenes en RAW

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2023 by Cesar Rincon "NightFox"

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



#ifndef NGN_IMAGE_H_INCLUDED
#define NGN_IMAGE_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <string>
#include <vector>
// Includes de la libreria
#include "ngn_defines.h"





/*** Declaracion de la clase ***/
class NGN_Image {

    public:

        // Devuelve la instancia
        static NGN_Image* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


        // Convierte una imagen RAW a datos de textura
        NGN_TextureData* ConvertRawToTextureData(NGN_RawImage* raw);

        // Aplica una mascara simple de recorte a una imagen RAW
        bool CutOutMask(NGN_RawImage* source, NGN_RawImage* mask, NGN_RawImage* destination);

        // Aplica una mascara simple de vaciado a una imagen RAW
        bool HollowMask(NGN_RawImage* source, NGN_RawImage* mask, NGN_RawImage* destination);

        // Aplica una mascara avanzada a una imagen RAW
        bool AdvancedMask(
            NGN_RawImage* source,                   // Imagen de origen
            NGN_RawImage* mask,                     // Imagen de la mascara
            NGN_RawImage* destination,              // Imagen de destino
            Vector2I32 offset = {0, 0},             // Offset de la mascara
            uint8_t mode = NGN_MASKMODE_CUTOUT      // Modo de la mascara
        );

        // Guarda el contenido actual del renderer a un surface
        bool RendererToSurface(NGN_RendererSurface* destination);

        // Convierte un surface a una imagen RAW
        bool SurfaceToRaw(NGN_RendererSurface* source, NGN_RawImage* destination, NGN_RawImage* mask = NULL);


    private:

        // Constructor
        NGN_Image();

        // Destructor
        ~NGN_Image();

        // Puntero de memoria a la instancia
        static NGN_Image* instance;


};



#endif // NGN_IMAGE_H_INCLUDED
