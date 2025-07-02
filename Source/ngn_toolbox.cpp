/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0-wip_0x03 ***
    TOOL BOX - Caja de herramientas

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
#include <iomanip>
#include <sstream>
#include <cmath>

// n'gine
#include "ngn.h"

// Class
#include "ngn_toolbox.h"
#include "ngn_sprite.h"



/******************************************************************************

    Clase NGN_ToolBox

******************************************************************************/



/*** Puntero de la instancia a nullptr ***/
NGN_ToolBox* NGN_ToolBox::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
NGN_ToolBox* NGN_ToolBox::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_ToolBox();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_ToolBox::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Contructor de la clase NGN_ToolBox ***/
NGN_ToolBox::NGN_ToolBox() {
}



/*** Destructor de la clase NGN_ToolBox ***/
NGN_ToolBox::~NGN_ToolBox() {
}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_ToolBox::BootUp() {
}



/******************************************************************************

    Cadenas de texto

******************************************************************************/

/*** Convierte un numero entero a cadena de texto con formato ***/
std::string NGN_ToolBox::Int2String(int32_t number, uint8_t min_width, const char* chr) {

    std::string t = "";

    if (number < 0) {
        number = abs(number);
        if (min_width > 0) min_width --;
        t += "-";
    }

    std::stringstream n;
    n << std::fixed << std::setfill(chr[0]) << std::setw(min_width) << std::setprecision(0) << number;

    return (t + n.str());

}



/*** Convierte un numero decimal a cadena de texto con formato ***/
std::string NGN_ToolBox::Float2String(float number, uint8_t min_width, uint8_t precision, const char* chr) {

    std::string t = "";

    if (number < 0) {
        number = abs(number);
        if (min_width > 0) min_width --;
        t += "-";
    }

    std::stringstream n;
    n << std::fixed << std::setfill(chr[0]) << std::setw(min_width) << std::setprecision(precision) << number;

    return (t + n.str());

}



// Convierte una cadena de texto de UTF-8 (2 bytes) a ANSI
std::string NGN_ToolBox::Utf8ToAnsi(std::string text) {

    std::string ansi = "";

    if (text.size() == 0) return ansi;

    // Prepara la conversion
    std::string character = "";
    uint8_t byte[2];
    uint32_t offset = 0;
    uint32_t length = text.size();
    uint32_t ascii = 0;

    // Analisis y conversion del texto
    while (offset < length) {

        // Lectura del primer byte
        character = text.at(offset);
        byte[0] = (uint8_t)character[0];

        // Conversion
        if (byte[0] < 0x80) {
            // Sin codificar
            ansi += character;
            offset ++;
        } else if (byte[0] < 0xE0) {
            // Codificacion en 2 bytes
            if ((offset + 1) < length) {
                // Lectura del segundo byte
                offset ++;
                character = text.at(offset);
                byte[1] = (uint8_t)character[0];
                // Conversion
                ascii = (((byte[0] & utf8_b0_mask) << 6) | (byte[1] & utf8_b1_mask));
                ansi += (uint8_t)(ascii & 0xFF);
                offset ++;
            } else {
                ansi += character;
                offset ++;
            }
        } else {
            // Codificacion no soportada (mas de 2 bytes)
            ansi += character;
            offset ++;
        }

    }

    // Devuelve la cadena convertida
    return ansi;

}



// Convierte una cadena de texto de ANSI a UTF-8 (2 bytes)
std::string NGN_ToolBox::AnsiToUtf8(std::string text) {

    std::string utf8 = "";

    if (text.size() == 0) return utf8;

    // Prepara la conversion
    std::string character = "";
    uint8_t byte;

    // Analisis y conversion del texto
    for (uint32_t i = 0; i < text.size(); i ++) {

        // Lectura del byte
        character = text.at(i);
        byte = (uint8_t)character[0];

        // Conversion
        if (byte < 0x80) {
            // Sin codificar
            utf8 += character;
        } else {
            // Codificacion en 2 bytes
            utf8 += (uint8_t)(utf8_b0_head | (byte >> 6));
            utf8 += (uint8_t)(utf8_b1_head | (byte & utf8_b1_mask));
        }

    }

    // Devuelve la cadena convertida
    return utf8;

}
