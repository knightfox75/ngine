/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso de los modos de pantalla (Declaraciones)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.19.0-stable o superior
	(c) 2016 - 2025 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
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
static const std::string WINDOW_TITLE = "Window modes";             // Titulo de la ventana
static const uint32_t SCR_WIDTH = 320;                              // Resolucion
static const uint32_t SCR_HEIGHT = 240;
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

        // Procesos iniciales
        bool Awake();

        // Al iniciar el programa
        bool Start();

        // Nucleo de ejecucion
        int8_t Run();


    private:

        // Carga de archivos necesarios
        bool Load();

        // Crea los elementos
        void Create();

        // Logica del programa
        void Logic();

        // Render de los elementos graficos
        void Render();

        // Fondos de tiles
        static const uint8_t bg_number = 5;         // Numero de fondos
        NGN_TiledBgData* bg_data[bg_number];        // Datos de los fondos
        NGN_TiledBg* bg[bg_number];                 // Fondos de tiles
        const float bg_speed[bg_number] = {0.0f, 0.25f, 0.50f, 1.0f, 2.0f};         // Velocidades de los fondos
        const float bg_loop[bg_number] = {0.0f, 408.0f, 408.0f, 408.0f, 408.0f};    // Puntos de LOOP del parallax

        // Control del modo grafico
        int8_t mode;

        // GUI
        bool update_gui;            // Actualizacion de la GUI?
        NGN_TextFont* font;         // Fuente
        NGN_TextLayer* text;        // Capa de texto


};


#endif // DEMO_H_INCLUDED
