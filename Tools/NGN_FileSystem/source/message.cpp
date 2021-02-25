/******************************************************************************

    N'gine Lib for C++
    Sistema de archivos
	- Sistema de mensages -

    Proyecto iniciado el 14 de Febrero del 2021
    (cc) 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

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
    std::endl << "    " << PROGRAM_NAME << ". (v" << program_version << ")." <<
    std::endl << "    For N'gine Version " << ngn_version << " or higher." <<
    std::endl << "    (cc) " << CC_YEARS << " by Cesar Rincon." <<
    std::endl << "    https://nightfoxandco.com" <<
    std::endl << "    contact@nightfoxandco.com" <<
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
    std::cout << program_name << " -idir DIRECTORY -ofile FILE [-key ENCRYPTION_KEY]" << std::endl;
    std::cout << program_name << " -ifile FILE -odir DIRECTORY [-key ENCRYPTION_KEY]" << std::endl;
    std::cout << std::endl;
    std::cout << ARG_HELP << " Displays the on-screen user manual (this screen)." << std::endl;
    std::cout << ARG_IN_DIR << " Source folder with the files to add (and encrypt)." << std::endl;
    std::cout << ARG_OUT_DIR << " Destination folder to put the extracted (and decrypted) files." << std::endl;
    std::cout << ARG_IN_FILE << " Sets the source package file." << std::endl;
    std::cout << ARG_OUT_FILE << " Sets the destination package file." << std::endl;
    std::cout << ARG_KEY << " Sets the encryption key." << std::endl;
    std::cout << std::endl;

}
