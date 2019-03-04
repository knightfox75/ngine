/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Camara virtual 2D

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2019 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 0.6.2-a o superior

    Requiere GCC 7.3.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.8) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG
    (c) 2005 - 2016 by Lode Vandevenne
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
    int r = 0;

    // Crea el objeto de la libreria
    ngn = NULL;
    ngn = new NGN();

    // Crea el objecto de la demo
    Demo* demo = NULL;
    demo = new Demo();

    // Nucleo de ejecucion
    if (demo->Awake()) {
        // Libreria inicializada con exito
        std::cout << "N'GINE test succefull!" << std::endl;
        // Ejecucion
        if (demo->Start()) demo->Run();
    } else {
        // Error al inicializar la libreria
        std::cout << "N'GINE test failed!" << std::endl;
    }

    // Elimina el objecto demo
    delete demo; demo = NULL;

    // Elimina el objeto de la libreria
    delete ngn; ngn = NULL;

    // Devuelve el resultado de la ejecucion
    return r;

}
