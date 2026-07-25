/******************************************************************************

    N'gine Lib for C++
    Ejemplo de acceso al GPIO de Raspberry PI. Nota: Requiere hardware real
    (Declaraciones)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
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



#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "N'gine RPi-GPIO Interface";    // Titulo de la ventana
static const uint32_t SCR_WIDTH = 1280;                                 // Resolucion
static const uint32_t SCR_HEIGHT = 720;
static const int8_t SCR_MODE_WINDOWS = NGN_SCR_WINDOW;                  // Modo de pantalla en Windows
static const int8_t SCR_MODE_LINUX = NGN_SCR_WINDOW;                    // Modo de pantalla en Linux
static const bool SHOW_MOUSE = false;                                   // Estado del puntero del raton
static const bool BILINEAR_FILTER = false;                              // Filtrado bi-linear
static const bool VSYNC = true;                                         // Sincronismo vertical
static const bool FPS_COUNTER = false;                                  // Contador de frames por segundo (solo en modo debug)



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
        uint32_t Run();


    private:

        // FLAGS de estado
        const uint32_t FLAG_ZERO = 0;
        const uint32_t FLAG_EXIT = 1;

        // Carga de archivos necesarios
        bool Load();

        // Crea los elementos
        void Create();

        // Logica del programa
        void Update();

        // Render de los elementos graficos
        void Render();

        // Constantes
        static const uint8_t GPIO_LEDS = 28;                // Numero de puertos GPIO disponibles
        static const uint8_t GPIO_PINS = 40;                // Numero de pins del conector GPIO
        const uint8_t GPIO_LED_PIN[GPIO_LEDS] = {           // Lista de asignacion puerto -> pin real
            27, 28, 3, 5, 7, 29, 31,        // 0 ~ 6
            26, 24, 21, 19, 23, 32, 33,     // 7 ~ 13
            8, 10, 36, 11, 12, 35, 38,      // 14 ~ 20
            40, 15, 16, 18, 22, 37, 13      // 21 ~ 27
        };
        const uint8_t GPIO_ORDER[GPIO_LEDS] = {             // Orden en el que deben activarse los leds (puertos)
            0, 1,
            26, 19, 13, 6, 5, 11, 9, 10, 22, 27, 17, 4, 3, 2,
            14, 15, 18, 23, 24, 25, 8, 7, 12, 16, 20, 21
        };

        // Graficos
        NGN_TextureData* bg_data;
        NGN_Texture* bg;
        NGN_SpriteData* led_spr_data;
        NGN_Sprite* led_spr[GPIO_LEDS];

        // Inicia el GPIO
        void GpioInit();

        // Datos de acceso a los puertos
        const uint8_t GPIO_MIN = 2;         // Primer puerto valido
        const uint8_t GPIO_MAX = 27;        // Ultimo puerto valido
        const int32_t DELAY = 10;           // Espera entre cambios

        // Control de cambio de leds
        int32_t timer;                      // Temporizador general
        uint8_t cnt;                        // Numero de puerto activo



};


#endif // DEMO_H_INCLUDED
