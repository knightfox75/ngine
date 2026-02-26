/******************************************************************************

    Pixel Art Project Template: Metodos de entrada

    Proyecto iniciado el xx de xx del xx
    (c) xxxx by Cesar Rincon "NightFox"
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



#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>
// Includes del proyecto
#include "defines.h"



/*** Declaracion de la clase ***/
class Input {

    public:

        // Devuelve la instancia
        static Input* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();

        // Teclas generales
        KeyState up, down, left, right, accept, cancel;

        // Especificas del gameplay
        // KeyState attack, jump, run;

        // Actualiza la entrada
        void Update();

        // Reinicia TODAS las teclas
        void Reset();


    private:

        // Constructor de la clase
        Input();

        // Destructor de la clase
        ~Input();

        // Puntero de memoria a la instancia
        static Input* instance;

        // Reset de la tecla
        void ResetKey(KeyState &key);

        // Actualiza el estado de una tecla
        void UpdateKey(KeyState &key);


};



#endif // INPUT_H_INCLUDED
