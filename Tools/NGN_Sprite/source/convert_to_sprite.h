/******************************************************************************

    Conversor de PNG a Sprite (.spr) para N'gine
    - ConvertToSprite -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Sprite is under MIT License

	Copyright (c) 2016 - 2025 by Cesar Rincon "NightFox"

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



#ifndef CONVERT_TO_SPRITE_H_INCLUDED
#define CONVERT_TO_SPRITE_H_INCLUDED



/*** Includes ***/
// C++
#include <vector>
#include <string>
// Proyecto
#include "defines.h"



/*** Declaracion de la clase ***/
class ConvertToSprite {

    public:

        // Constructor
        ConvertToSprite();

        // Destructor
        ~ConvertToSprite();

        /*** Propiedades ***/


        /*** Metodos ***/
        // Convierte un archivo PNG en un Sprite
        bool Convert(
            std::string in_file,        // Archivo PNG a convertir
            std::string out_file,       // Nombre base de los archivos de salida
            uint32_t frame_w,           // Tama�o del tile
            uint32_t frame_h,           // Nivel de optimizacion
            bool strip                  // Generar archivos adicionales?
        );


    private:

        /*** Propiedades ***/

        // Parametros de las imagenes
        uint32_t in_width, in_height;               // Tama�o del archivo de entrada
        uint32_t out_width, out_height;             // Tama�o del archivo de salida

        // Parametros de los fotogramas
        uint32_t frame_width;                       // Tama�o del frame
        uint32_t frame_height;
        uint32_t total_frames;

        // Parametros adicionales
        bool sprite_strip;

        // Buffers
        std::vector<uint8_t> sheet;                 // Buffer para almacenar la composicion del Sprite Sheet


        /*** Metodos ***/

        // Corta y almacena los diferentes fotogramas del sprite
        void CutFrames(std::vector<uint8_t> &data);

        // Graba el archivo empaquetado
        bool WriteFile(std::string filename);

        // Lee el archivo .PNG y almacena los pixeles de la imagen en el buffer
        bool ReadPng(std::string filename, std::vector<uint8_t> &data);
        // Graba un buffer de pixeles en un archivo PNG
        bool WritePng(std::string filename, std::vector<uint8_t> &data, uint32_t width, uint32_t height);

};



#endif // CONVERT_TO_SPRITE_H_INCLUDED
