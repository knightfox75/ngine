/******************************************************************************

    Project Template: Datos compartidos entre modulos

    Proyecto iniciado el 13 de Noviembre del 2022
    (c) 2022-2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.20.0+10th-anniversary o superior
	(c) 2016 - 2026 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.11) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.24.0) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.6.2) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "shared_data.h"
#include "settings.h"



/*** Puntero de la instancia a nullptr ***/
SharedData* SharedData::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
SharedData* SharedData::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new SharedData();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void SharedData::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Constructor de la clase ***/
SharedData::SharedData() {

    // Assets
    asset.black_texture = nullptr;

}



/*** Destructor de la clase ***/
SharedData::~SharedData() {

    // Elimina los assets creados
    delete asset.black_texture; asset.black_texture = nullptr;

    // Elimina los recursos persistentes
    ngn->resources->Clear(RS_PERSISTENT);

}



/*** Procesos iniciales despues de crear la instancia ***/
void SharedData::BootUp() {

}



/*** Carga los archivos de recursos comunes de todo el programa ***/
bool SharedData::Load() {

    if (!ngn->resources->Load(RS_PERSISTENT, "data/shared/shared.txt")) return false;

    return true;

}



/*** Crea los assets comunes ***/
void SharedData::Create() {

    asset.black_texture = new NGN_Texture(RS_PERSISTENT, "black_mask", 0, 0);

}



/*** Devuelve un string con la version actual del programa ***/
std::string SharedData::GetVersion() {

    std::string version = "";
    #if defined (OS_WINDOWS)
        version += "OS Windows ";
    #elif defined (OS_LINUX)
        version += "OS Linux ";
    #endif
    #if defined (MODE_DEBUG)
        version += "Debug ";
    #elif defined (MODE_RELEASE)
        #if defined (MODE_DEMO)
            version += "Demo ";
        #else
            version += "Rel ";
        #endif
    #endif
    version += "Ver.";
    version += ngn->toolbox->Int2String(VERSION_MAJOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(VERSION_MINOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(VERSION_PATCH, 1, "0");
    version += "-";
    version += VERSION_METADATA;

    return version;

}
