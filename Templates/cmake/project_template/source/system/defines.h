/******************************************************************************

    Project Template: Definiciones del programa

    Proyecto iniciado el 13 de Noviembre del 2022
    (c) 2022-2023 by Cesar Rincon "NightFox"
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

#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED


const int8_t FADE_SPEED = 4;                        // Velocidad del fade
const uint32_t FLAG_ZERO = 0;                       // Todos los bits a 0



/*** Flags de control de ejecucion (Generales, del BIT 24 al 31) ***/
const uint32_t BITMASK_RUN = 0xFFFF0000;            // Mascara
const uint32_t FLAG_RUN_QUIT = (1 << 31);           // Finaliza la ejecucion del programa
const uint32_t FLAG_RUN_ERROR = (1 << 30);          // Se ha producido un error
const uint32_t FLAG_RUN_EXIT = (1 << 29);           // Finaliza la ejecucion del modulo actual
const uint32_t FLAG_RUN_GAME = (1 << 27);           // Inicia el juego
const uint32_t FLAG_RUN_TITLE = (1 << 26);          // Pantalla del titulo
const uint32_t FLAG_RUN_OPENING = (1 << 25);        // Escena de apertura del juego
const uint32_t FLAG_RUN_WELCOME = (1 << 24);        // Pantalla de bienvenida



/*** Flags de control de ejecucion (Especificos de este juego, del BIT 16 al 23) ***/



/*** Flags de control de ejecucion (Datos, del BIT 0 al 7) ***/
const uint32_t BITMASK_DATA_LOW = 0x000000FF;           // Mascara
/*** Flags de control de ejecucion (Datos, del BIT 8 al 15) ***/
const uint32_t BITMASK_DATA_HI = 0x0000FF00;            // Mascara
const uint8_t BITMASK_DATA_HI_SHIFT = 8;                // Numero de desplazamientos



/*** Definiciones de los metodos de entrada ***/
// Estados de cada tecla de entrada
struct KeyState {
    bool up;
    bool down;
    bool held;
    bool last;
};



/*** Constates: Valores devueltos por el modulo "Welcome" ***/
struct WelcomeFlags {
    enum {
        title,
        exit,
        none
    };
};
const WelcomeFlags WELCOME_FLAG;



/*** Constates: Valores devueltos por el modulo "Title" ***/
struct TitleFlags {
    enum {
        game_start,             // OP 0
        options,                // OP 1
        exit,                   // OP 2
        none
    };
};
const TitleFlags TITLE_FLAG;


/*** Constates: Valores devueltos por el modulo "Game" ***/
struct GameFlags {
    enum {
        title,
        exit,
        none
    };
};
const GameFlags GAME_FLAG;


/*** Constates: Valores devueltos por el modulo "Options" ***/
struct OptionsFlags {
    enum {
        title,
        exit,
        none
    };
};
const OptionsFlags OPTIONS_FLAG;



#endif // DEFINES_H_INCLUDED
