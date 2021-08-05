/******************************************************************************

    N'gine Lib for C++
    *** Version 1.5.0-stable ***
    TOOL BOX - Caja de herramientas

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
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

/*** Contructor de la clase NGN_ToolBox ***/
NGN_ToolBox::NGN_ToolBox() {
}



/*** Destructor de la clase NGN_ToolBox ***/
NGN_ToolBox::~NGN_ToolBox() {
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
