/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso los efectos visuales de la camara

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.19.0-stable o superior
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
// Includes de C++
#include <vector>
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "2D Camera VFX example";    // Titulo de la ventana
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

        // Configuracion del mundo
        static const uint8_t BG_LAYERS = 4;
        const uint32_t WORLD_WIDTH = 6400;
        const uint32_t WORLD_HEIGHT = 720;
        const uint32_t LAYER_LOOP = 1280;
        const float PARALLAX_FACTOR[BG_LAYERS] = {0.2f, 0.4f, 0.7f, 1.0f};
        // Imagenes del fondo
        std::vector<NGN_TextureData*> bg_data;
        // Texturas del fondo
        std::vector<NGN_Texture*> bg_layer;

        // Control del movimiento de camara
        const float WORLD_LEFT = (SCR_WIDTH / 2);
        const float WORLD_RIGHT = (WORLD_WIDTH - (SCR_WIDTH / 2));
        const float CAM_SPEED = 4.0f;
        Vector2 camera_position;

        // Sprites
        NGN_SpriteData* blue_bird_data;
        std::vector<std::vector<NGN_Sprite*>> bird_sprite;
        const uint32_t BIRDS_PER_LAYER[BG_LAYERS] = {20, 15, 10, 5};
        const float BIRD_SPEED[BG_LAYERS] = {0.6f, 1.2f, 2.1f, 3.0f};

        // Efecto "screen shake"
        const float SHAKE_MAX_INTENSITY = 5.0f;
        const float SHAKE_MIN_INTENSITY = 0.1f;
        const float SHAKE_FREQ = 1.57f;     // Best PI/2
        const float SHAKE_LERP = 1.15f;     // Best 1.15f
        float shake_intensity;

        // Efecto de "tinte"
        const Rgba DAY_COLOR = {0xFF, 0xFF, 0xFF, 0xFF};
        const Rgba NIGHT_COLOR = {0x20, 0x40, 0xFF, 0xFF};
        const float FADE_SPEED = 0.5f;
        float effect_amount;    // 100 - night, 0 - day
        Rgba COLOR_DELTA;

};


#endif // DEMO_H_INCLUDED
