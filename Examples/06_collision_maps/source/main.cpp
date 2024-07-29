/******************************************************************************

    Ejemplo de uso de los mapas de colisiones y hitbox de sprites

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.17.0-stable o superior
	(c) 2016 - 2024 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 13.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.5) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.22.0) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (3.0.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20230410)
    (c) 2005 - 2023 by Lode Vandevenne
    http://lodev.org/lodepng/

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
