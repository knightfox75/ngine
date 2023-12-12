/******************************************************************************

    Ejemplo de uso de varios viewports simultaneos
    Archivo de Demo (Declaraciones)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.15.0-stable o superior
	(c) 2016 - 2023 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 11.3.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.26.3) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.20.2) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (3.0.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <ctime>
#include <vector>
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "N'gine multiple viewports";    // Titulo de la ventana
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
        const float MOVE_SPEED = 5.0f;
        const float BIRD_MIN_SPEED = 1.0f;
        const float BIRD_MAX_SPEED = 3.0f;

        // Datos de los fondos
        NGN_TiledBgData *bg0_data, *bg1_data, *bg2_data;
        // Fondos de tiles
        NGN_TiledBg *bg0, *bg1, *bg2;

        // Datos de los sprites
        NGN_SpriteData *bird_data, *aim_data;

        // Sprites (Pajaros)
        struct bird_type {
            NGN_Sprite* sprite;
            int32_t left;
            int32_t right;
            float speed;
        };
        std::vector<bird_type> bird;

        // Sprite del cursor
        NGN_Sprite* aim;

        // Carga los archivos necesarios
        bool Load();

        // Crea la escena
        void CreateStage();

        // Crea los pajaros de una capa en concreto
        void CreateBirds(uint32_t ly, uint32_t num, float scale);

        // Update
        void Update();

        // Render
        void Render();

        // Mueve a los pajaros
        void MoveBirds();

        // Mueve el punto de mira
        void MoveAim();

        // Gestiona los diferentes viewports
        void ViewportManager();

        // Modos de pantalla y filtros
        void ScreenConfig();

        // GUI
        NGN_TextFont* mono;             // Tipografia
        NGN_TextLayer* text;            // Capa de texto
        void Gui();                     // Imprime la informacion

        // Captura de pantalla
        void TakeScreenshot();
        NGN_AudioClipData* shutter;     // Efecto del obturador
        NGN_TextureData* overlay;       // Overlay del logo

        // Acceso a la fecha y hora
        time_t current_time;
        struct  tm* datetime;


};


#endif // DEMO_H_INCLUDED
