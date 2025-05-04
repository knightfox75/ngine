/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso del Canvas: Tunel 3D (Declaraciones)

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
// Includes de C++
#include <vector>
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "Canvas Fake 3D Tunnel";    // Titulo de la ventana
static const uint32_t SCR_WIDTH = 1280;                             // Resolucion
static const uint32_t SCR_HEIGHT = 720;
static const int8_t SCR_MODE_WINDOWS = NGN_SCR_WINDOW;              // Modo de pantalla en Windows
static const int8_t SCR_MODE_LINUX = NGN_SCR_WINDOW;                // Modo de pantalla en Linux
static const bool SHOW_MOUSE = true;                                // Estado del puntero del raton
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

        // Crea los canvas necesarios
        NGN_Canvas* bg;

        // Datos para un cuadrado
        struct SquareData {
            int32_t top;
            int32_t bottom;
            int32_t left;
            int32_t right;
        };
        SquareData tunnel_bg, origin;

        const int32_t deep_size = 40;           // Profundidad del tunel
        const int32_t segments_number = 6;      // Segmentos horizontales
        const float deep_step = 1.2f;           // Incremento de los segmentos de profundidad
        const float tunnel_speed = 1.005f;      // Multiplicador de velocidad del tunel
        const float z_size = 100.0f;            // Valor de profundidad relativa
        int32_t w, h;                           // Tamaño del fondo del tunel

        std::vector<float> deep;                // Calculo de profundidad

        Vector2 delta_mouse;                    // Control suavizado del mouse
        const float mouse_speed = 8.0f;

        // Metodos de la demo
        void DrawTunnel();
        void DrawLines(
            int32_t ox1, int32_t oy1,
            int32_t ox2, int32_t oy2,
            int32_t lx1, int32_t ly1,
            int32_t lx2, int32_t ly2,
            uint32_t color
        );

        float zst_point;        // Punto inicial de dibujado

};


#endif // DEMO_H_INCLUDED
