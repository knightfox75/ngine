/******************************************************************************

    Ejemplo de carga y movimiento de texturas

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2022 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.8.0-stable o superior

    Requiere GCC 8.1.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.12) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.1) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20200306)
    (c) 2005 - 2020 by Lode Vandevenne
    http://lodev.org/lodepng/


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
// Includes de la libreria
#include <ngn.h>
// Includes de la demo
#include "demo/demo.h"



/*** Main ***/
int main(int argc, char* args[]) {

    // Resultado
    int r = 0x00;

    // Crea el objeto de la libreria
    ngn = NULL;
    ngn = new NGN();

    // Crea el objecto de la demo
    Demo* demo = NULL;
    demo = new Demo();

    // Intenta iniciar N'gine para ejecutar el programa
    if (demo->Awake()) {
        // Intenta inicializar el programa
        if (demo->Start()) {
            // Si se ha iniciado correctamente, ejecutalo.
            demo->Run();
        } else {
            // Error al inicializar el juego
            ngn->log->Message("Game start-up failed!");
            r = 0x02;
        }
        #if defined (MODE_DEBUG)
            ngn->log->Message("Execution terminated.");
        #endif
    } else {
        // Error al inicializar la libreria
        std::cout << "N'GINE boot-up failed!" << std::endl;
        r = 0x01;
    }

    // Elimina el objecto demo
    delete demo; demo = NULL;

    // Elimina el objeto de la libreria
    delete ngn; ngn = NULL;

    // Devuelve el resultado de la ejecucion
    return r;

}
