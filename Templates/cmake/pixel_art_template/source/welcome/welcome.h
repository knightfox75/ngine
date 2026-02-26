/******************************************************************************

    Pixel Art Project Template: Pantallas de bienvenida

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



#ifndef WELCOME_H_INCLUDED
#define WELCOME_H_INCLUDED



/*** Includes ***/
// Includes de la libreria NGN
#include <ngn.h>
// Includes del proyecto
#include "../system/input.h"



/*** Declaracion de la clase ***/
class Welcome {

    public:

        // Constructor
        Welcome();

        // Destructor
        ~Welcome();

        // Start
        bool Start();

        // Ejecucion
        uint32_t Run();


    private:

        // Singletons
        Input* input;                   // Metodos de entrada

        // Punteros a los objetos
        NGN_Texture* bg;
        NGN_AudioClip* voice;

        // Variables de control
        int32_t timer;

        // Maquina de estados
        struct {
            enum {
                start_delay,            // Inicio
                ngine_create,           // Crea el logo N'GINE
                ngine_in,               // Fade-in del logo N'GINE
                ngine_wait,             // Muestra el logo N'GINE
                ngine_out,              // Fade-out del logo N'GINE
                exit,                   // Fin del programa
                none
            };
        } STATE;
        uint32_t state, next_state;

        // Carga los archivos
        bool Load();
        // Logica del modulo
        void Update();
        // Render
        void Render();

        // Parametros de la ejecucion
        const int32_t START_DELAY = 120;        // Espera al iniciar el proceso
        const int32_t WAIT_NGINE = 30;          // Tiempo de espera mostrando el logo

};



#endif // WELCOME_H_INCLUDED
