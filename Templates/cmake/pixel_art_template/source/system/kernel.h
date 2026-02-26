/******************************************************************************

    Pixel Art Project Template: Nucleo de ejecucion del juego

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



#ifndef KERNEL_H_INCLUDED
#define KERNEL_H_INCLUDED



/*** Includes ***/
// Includes del proyecto
#include "shared_data.h"
#include "save_data.h"
#include "soundtrack.h"
#include "input.h"



/*** Declaracion de la clase ***/
class Kernel {

    public:

        // Constructor
        Kernel();

        // Destructor
        ~Kernel();

        // Awake
        bool Awake();

        // Start
        bool Start();

        // Ejecucion del juego
        void Run();


    private:

        // Acceso a los singletons principales
        SharedData* shared_data;            // Datos compartidos
        SaveData* save_data;                // Datos de guardado
        Soundtrack* soundtrack;             // Gestion de la banda sonora
        Input* input;                       // Metodos de entrada


        // Habilita el archivo de registro
        void EnableLogging();

        // Paso de limpieza
        void CleanUp();


        // Control de estados
        struct {
            enum {
                welcome,        // Pantalla de bienvenida
                title,          // Menu del juego
                game,           // Nucleo del juego
                options,        // Opciones del juego
                exit,           // Sal del juego
                none
            };
        } STATE;
        int32_t state, next_state;

        // Funcion del estado Welcome
        void StWelcome();
        // Funcion del estado Title
        void StTitle();
        // Funcion del nucleo del juego
        void StGame();
        // Funcion del menu de opciones
        void StOptions();

};



#endif // KERNEL_H_INCLUDED
