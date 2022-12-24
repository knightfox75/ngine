/******************************************************************************

    Sistema de empaquetado de archivos para N'gine

    Proyecto iniciado el 14 de Febrero del 2021
    (cc) 2021 - 2023 by Cesar Rincon "NightFox"
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
