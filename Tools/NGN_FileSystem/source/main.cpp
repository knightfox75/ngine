/******************************************************************************

    N'gine Lib for C++
    Conversor de PNG a Mapa de Colisiones (.map)

    Proyecto iniciado el 8 de Abril del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG
    (c) 2005 - 2020 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



/*** Includes ***/

// C++
#include <cstdio>
#include <cstdlib>
#include <iostream>

// CollisionMap class
#include "file_system.h"
#include "defines.h"



/*** Main ***/
int32_t main(int32_t argc, char* args[]) {

    // Crea el objeto principal del programa
    FileSystem* fs = new FileSystem(argc, args);

    // Si no se crea con exito, sal del programa
    if (!fs) return ERR_CODE_BOOTFAILURE;

    // Transfiere el control del objeto principal
    int32_t r = fs->Run();

    // Elimina el objeto principal
    delete fs;

    // Devuelve el resultado de ejecucion del programa
    return r;

}
