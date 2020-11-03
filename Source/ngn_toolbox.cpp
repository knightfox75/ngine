/******************************************************************************

    N'gine Lib for C++
    *** Version 1.1.0-beta ***
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



/******************************************************************************

    Funciones matematicas

******************************************************************************/

/*** Distancia entre 2 puntos (4 parametros, punto flotante, 1ra sobrecarga) ***/
float NGN_ToolBox::GetDistance(float x1, float y1, float x2, float y2) {

    return std::sqrt((std::pow((x1 - x2), 2) + std::pow((y1 - y2), 2)));

}

/*** Distancia entre 2 puntos (2 parametros, punto flotante, 2da sobrecarga) ***/
float NGN_ToolBox::GetDistance(Vector2 a, Vector2 b) {

    return std::sqrt((std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2)));

}

/*** Distancia entre 2 puntos (4 parametros, entero, 3ra sobrecarga) ***/
uint32_t NGN_ToolBox::GetDistance(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {

    return std::sqrt((std::pow((x1 - x2), 2) + std::pow((y1 - y2), 2)));

}

/*** Distancia entre 2 puntos (2 parametros, entero, 4a sobrecarga) ***/
uint32_t NGN_ToolBox::GetDistance(Vector2I32 a, Vector2I32 b) {

    return std::sqrt((std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2)));

}
