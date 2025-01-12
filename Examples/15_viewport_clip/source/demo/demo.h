/******************************************************************************

    Ejemplo de uso del recorte de area del viewport
    Archivo de Demo (Declaraciones)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.18.0-stable o superior
	(c) 2016 - 2025 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 13.2.0 MinGW64 (SEH) - 64-bits
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
// Includes C++
#include <vector>
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "N'gine viewport clipping";     // Titulo de la ventana
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

        // Awake
        bool Awake();

        // Start
        bool Start();

        // Run
        int8_t Run();


    private:

        // Parametros del programa
        const uint32_t OBJECT_NUMBER = 12;        // Numero de objetos

        // Fondo
        NGN_TextureData* bg_data;       // Imagen
        NGN_Texture* bg;                // Textura

        // Sprites
        NGN_TextureData* ball_data;     // Imagen
        struct ball_type{               // Estructura de datos de la bola
            Vector2 speed;
            int32_t top;
            int32_t right;
            int32_t bottom;
            int32_t left;
            NGN_Texture* gfx;
        };
        std::vector<ball_type> ball;    // Vector para almacenar todas las bolas

        // Carga los archivos necesarios
        bool Load();

        // Crea los objetos
        void CreateObjects();

        // Update
        void Update();

        // Mueve a los objetos
        void Move();

        // Calcula el area de recorte con el mouse
        void Clip();

        // Funcion para renderizar la escena
        void Render();

};


#endif // DEMO_H_INCLUDED
