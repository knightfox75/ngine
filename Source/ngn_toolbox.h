/******************************************************************************

    N'gine Lib for C++
    *** Version 0.10.1-wip_03 ***
    TOOL BOX - Caja de herramientas

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
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



        /*** Funciones matematicas ***/

        // Distancia entre 2 puntos (4 parametros, punto flotante, 1a sobrecarga)
        float GetDistance(float x1, float y1, float x2, float y2);

        // Distancia entre 2 puntos (2 parametros, punto flotante, 2a sobrecarga)
        float GetDistance(Vector2 a, Vector2 b);

        // Distancia entre 2 puntos (4 parametros, entero, 3a sobrecarga)
        uint32_t GetDistance(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

        // Distancia entre 2 puntos (2 parametros, entero, 4a sobrecarga)
        uint32_t GetDistance(Vector2I32 a, Vector2I32 b);


    // Segmento privado
    private:

};



#endif // NGN_TOOLBOX_H_INCLUDED
