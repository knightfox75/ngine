/******************************************************************************

    Conversor de PNG a Mapa de Colisiones (.map) para N'gine
	- Sistema de mensages -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Mapa de Colisiones is under MIT License

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



/*** Includes ***/

// C++
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

// Includes del proyecto
#include "message.h"
#include "defines.h"



/*** Constructor ***/
Message::Message(std::string pname) {

    // Guarda el nombre del ejecutable del programa
    program_name = pname;

}



/*** Destructor ***/
Message::~Message() {

}



/*** Texto de encabezado de la aplicacion ***/
void Message::AppHeader() {

    // Genera las cadenas de texto adicionales
    std::string ngn_version = std::to_string(NGN_VERSION_MAJOR) + "." + std::to_string(NGN_VERSION_MINOR) + "." + std::to_string(NGN_VERSION_PATCH) + "-" + NGN_VERSION_METADATA;
    std::string program_version = std::to_string(PROGRAM_VERSION_MAJOR) + "." + std::to_string(PROGRAM_VERSION_MINOR) + "." + std::to_string(PROGRAM_VERSION_PATCH) + "-" + PROGRAM_VERSION_METADATA;

    // Imprime el texto
    std::cout <<
    std::endl <<
    std::endl << TEXT_DASHED_LINE <<
    std::endl <<
    std::endl << "  " << PROGRAM_NAME << ". (v" << program_version << ")" <<
    std::endl << "  for N'gine Version " << ngn_version << " or higher." <<
    std::endl << "  (c) " << CPR_YEARS << " by Cesar Rincon." <<
    std::endl << "  https://nightfoxandco.com" <<
    std::endl << "  contact@nightfoxandco.com" <<
    std::endl <<
    std::endl << "  " << PROGRAM_NAME << " is under MIT licence." <<
    std::endl <<
    std::endl << TEXT_DASHED_LINE <<
    std::endl << std::endl;

}



/*** Texto "Como obtener ayuda" ***/
void Message::QuestionMarkForHelp() {

    std::cout << "Type " << program_name << " " << ARG_HELP << " to display the on-screen help manual." << std::endl << std::endl;

}



/*** Texto "Error de uso" ***/
void Message::UseError() {

    std::cout << "Command line error." << std::endl;

}



/*** Texto "Manual de uso" ***/
void Message::UserManual() {

    std::cout << std::endl;
    std::cout << program_name << " INPUT_FILE.PNG [" << ARG_OUT_FILE << " filename] [" << ARG_TILE_SIZE << " xxxx]" << std::endl;
    std::cout << std::endl;
    std::cout << ARG_HELP << " Displays the on-screen user manual (this screen)." << std::endl;
    std::cout << ARG_OUT_FILE << " Name for the output file, without extension (" << MAP_EXTENSION << " will be added automaticaly)." << std::endl;
    std::cout << ARG_TILE_SIZE << " Size of the squared tile in pixels (From [" << MIN_TILE_SIZE << "] to [" << MAX_TILE_SIZE << "], " << DEFAULT_TILE_SIZE << " pixels by default)." << std::endl;
    std::cout << std::endl;

}
