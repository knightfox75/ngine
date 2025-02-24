/******************************************************************************

    N'gine Lib for C++
    *** Version 1.19.0-wip_0x01 ***
    TOOL BOX - Caja de herramientas

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



#ifndef NGN_TOOLBOX_H_INCLUDED
#define NGN_TOOLBOX_H_INCLUDED



/*** Includes ***/

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>
#include <sstream>

// n'gine
#include "ngn_defines.h"
#include "ngn_sprite.h"



/******************************************************************************

    Clase NGN_ToolBox

******************************************************************************/

/*** Declaracion de la clase de la caja de herramientas ***/
class NGN_ToolBox {

    // Segmento publico
    public:

        // Devuelve la instancia
        static NGN_ToolBox* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


        /*** Cadenas de texto ***/

        // Convierte un numero entero a cadena de texto con formato
        std::string Int2String(int32_t number, uint8_t min_width, const char* chr);

        // Convierte un numero decimal a cadena de texto con formato
        std::string Float2String(float number, uint8_t min_width, uint8_t precision, const char* chr);

        // Convierte una cadena de texto de UTF-8 (2 bytes) a ANSI
        std::string Utf8ToAnsi(std::string text);

        // Convierte una cadena de texto de ANSI a UTF-8 (2 bytes)
        std::string AnsiToUtf8(std::string text);


    // Segmento privado
    private:

        // Constructor de la clase
        NGN_ToolBox();

        // Destructor de la clase
        ~NGN_ToolBox();

        // Puntero de memoria a la instancia
        static NGN_ToolBox* instance;

        // Constantes de conversion a UTF-8 (2 bytes)
        // El primer byte en la codificacion en 2 bytes empieza con 110 y contiene 5 bits de datos
        const uint8_t utf8_b0_head = 0xC0;      // 1100 0000
        const uint8_t utf8_b0_mask = 0x1F;      // 0001 1111
        // El siguiente byte en la codificacion en 2 bytes empieza con 10 y contiene 6 bits de datos
        const uint8_t utf8_b1_head = 0x80;      // 1000 0000
        const uint8_t utf8_b1_mask = 0x3F;      // 0011 1111

};



#endif // NGN_TOOLBOX_H_INCLUDED
