/******************************************************************************

    Pixel Art Project Template: Nucleo del juego

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



#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>
// Includes del proyecto
#include "../system/settings.h"
#include "../system/shared_data.h"
#include "../system/soundtrack.h"
#include "../system/input.h"



/*** Declaracion de la clase ***/
class Game {

    public:

        // Constructor
        Game();

        // Destructor
        ~Game();

        // Al iniciar el modulo
        bool Start();

        // Ejecuta el modulo
        uint32_t Run();


    private:

        // Punteros a los singletons (compartidos)
        SharedData* shared_data;
        Soundtrack* soundtrack;
        Input* input;

        // Punteros a los singletons (propios)


        // Carga de recursos
        bool Load();

        // Creacion de los diferentes elementos
        void Create();

        // Actualizacion de los procesos
        uint32_t Update();

        // Render de los elementos graficos
        void Render();

        // Actualizacion de los procesos post-render
        void LateUpdate();

        // Salida del usuario
        uint32_t ApplicationQuit();

        // Maquina de estados
        struct {
            enum {
                fade_in,            // Efecto FADE-IN
                fade_out,           // Efecto FADE-OUT
                run,                // Ejecuta el menu
                exit,               // Sal del menu
                none
            };
        } STATE;
        int32_t state, next_state;

        /// DEBUG - Dummy texture ///
        NGN_Texture* dummy;

};


#endif // GAME_H_INCLUDED
