/******************************************************************************

    Ejemplo del uso de deteccion de colisiones entre sprites por caja
    Archivo de Demo (Declaraciones)

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



#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "N'gine sprites hitbox";    // Titulo de la ventana
static const uint32_t SCR_WIDTH = 1280;                             // Resolucion
static const uint32_t SCR_HEIGHT = 720;
static const int8_t SCR_MODE_WINDOWS = NGN_SCR_WINDOW;              // Modo de pantalla en Windows
static const int8_t SCR_MODE_LINUX = NGN_SCR_WINDOW;                // Modo de pantalla en Linux
static const bool SHOW_MOUSE = false;                               // Estado del puntero del raton
static const bool BILINEAR_FILTER = false;                          // Filtrado bi-linear
static const bool VSYNC = true;                                     // Sincronismo vertical
static const bool FPS_COUNTER = false;                              // Contador de frames por segundo (solo en modo debug)



/*** Declaracion de la clase ***/
class Demo {

    public:

        // Constructor
        Demo();

        // Destructor
        ~Demo();

        // Awake
        bool Awake();

        // Start
        bool Start();

        // Run
        int8_t Run();


    private:

        // Punteros de los fondos
        NGN_TextureData* bg_data;
        NGN_Texture* bg;

        // Punteros para sprites
        static const uint8_t NUMBER_OF_SPRITES = 7;
        NGN_SpriteData* spr_data[NUMBER_OF_SPRITES];
        NGN_Sprite* spr[NUMBER_OF_SPRITES];
        const uint8_t SPR_NORMAL = 0;
        const uint8_t SPR_TOP = 1;
        const uint8_t SPR_BOTTOM = 2;
        const uint8_t SPR_LEFT = 3;
        const uint8_t SPR_RIGHT = 4;
        const uint8_t SPR_TOP_LEFT = 5;
        const uint8_t SPR_BOTTOM_RIGHT = 6;

        NGN_SpriteData* aim_data;
        NGN_Sprite* aim;

        // Metodos
        bool Load();
        void Create();
        void Update();
        void Render();

};


#endif // DEMO_H_INCLUDED
