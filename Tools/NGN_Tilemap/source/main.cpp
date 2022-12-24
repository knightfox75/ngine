/******************************************************************************

    Conversor de PNG a Fondo de Tiles (.tbg) para N'gine
    - Archivo principal -

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



/*** Includes ***/

// C++
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Programa
#include "tile_map.h"
#include "defines.h"



/*** Main ***/
int32_t main(int32_t argc, char* args[]) {

    // Crea el objeto principal del programa
    TileMap* tm = new TileMap(argc, args);

    // Si no se crea con exito, sal del programa
    if (!tm) return ERR_CODE_BOOTFAILURE;

    // Transfiere el control del objeto principal
    int32_t r = tm->Run();

    // Elimina el objeto principal
    delete tm;

    // Devuelve el resultado de ejecucion del programa
    return r;

}
