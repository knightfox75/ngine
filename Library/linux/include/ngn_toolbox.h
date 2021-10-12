/******************************************************************************

    N'gine Lib for C++
    *** Version 1.6.0-beta ***
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

        // Constructor de la clase
        NGN_ToolBox();

        // Destructor de la clase
        ~NGN_ToolBox();


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

        // Constantes de conversion a UTF-8 (2 bytes)
        // El primer byte en la codificacion en 2 bytes empieza con 110 y contiene 5 bits de datos
        const uint8_t utf8_b0_head = 0xC0;      // 1100 0000
        const uint8_t utf8_b0_mask = 0x1F;      // 0001 1111
        // El siguiente byte en la codificacion en 2 bytes empieza con 10 y contiene 6 bits de datos
        const uint8_t utf8_b1_head = 0x80;      // 1000 0000
        const uint8_t utf8_b1_mask = 0x3F;      // 0011 1111

};



#endif // NGN_TOOLBOX_H_INCLUDED
