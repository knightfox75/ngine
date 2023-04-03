/******************************************************************************

    Sistema de empaquetado de archivos para N'gine
	- Sistema de mensages -

    Proyecto iniciado el 14 de Febrero del 2021
    (c) 2021 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	Sistema de empaquetado de archivos para N'gine is under MIT License

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
    std::cout << program_name << " " << ARG_IN_DIR << " DIRECTORY " << ARG_OUT_FILE << " FILE [" << ARG_KEY << " ENCRYPTION_KEY]" << std::endl;
    std::cout << program_name << " " << ARG_IN_FILE << " FILE " << ARG_OUT_DIR << " DIRECTORY [" << ARG_KEY << " ENCRYPTION_KEY]" << std::endl;
    std::cout << std::endl;
    std::cout << ARG_HELP << " Displays the on-screen user manual (this screen)." << std::endl;
    std::cout << ARG_IN_DIR << " Source folder with the files to add (and encrypt)." << std::endl;
    std::cout << ARG_OUT_DIR << " Destination folder to put the extracted (and decrypted) files." << std::endl;
    std::cout << ARG_IN_FILE << " Sets the source package file." << std::endl;
    std::cout << ARG_OUT_FILE << " Sets the destination package file." << std::endl;
    std::cout << ARG_KEY << " Sets the encryption key." << std::endl;
    std::cout << std::endl;

}
