/******************************************************************************

    Project Template: Archivo principal

    Proyecto iniciado el xx de xx del xxxx
    (c) xxxx by xxxx

	Requiere N'gine 1.20.0+10th-anniversary o superior
	(c) 2016 - 2026 by Cesar Rincon "NightFox"
	https://nightfoxandco.com
	contact@nightfoxandco.com

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
// C++
#include <cstdio>
#include <iostream>
// Includes de la libreria NGN
#include <ngn.h>
// Includes del proyecto
#include "system/kernel.h"



/*** Main ***/
int main(int argc, char* args[]) {

    // Resultado
    int r = 0x00;

    // Crea el objeto de la libreria
    ngn = nullptr;
    ngn = new NGN();

    // Crea el objecto del nucleo del juego
    Kernel* kernel = nullptr;
    kernel = new Kernel();

    // Intenta iniciar N'gine para ejecutar el juego
    if (kernel->Awake()) {
        // Intenta inicializar el juego
        if (kernel->Start()) {
            // Si se ha iniciado correctamente, ejecutalo.
            kernel->Run();
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

    // Elimina el objecto del nucleo del juego
    delete kernel; kernel = nullptr;

    // Elimina el objeto de la libreria
    delete ngn; ngn = nullptr;

    // Devuelve el resultado de la ejecucion
    return r;

}
