/******************************************************************************

    Ejemplo del uso de los modos de mezcla
    Archivo de Demo (Declaraciones)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.13.0-stable o superior
	(c) 2016 - 2023 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 11.3.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.26.3) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.20.2) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.1) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "N'gine use blending modes";        // Titulo de la ventana
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

        // Parametros del programa
        const uint32_t NUMBER_OF_SPRITES = 32;

        // Fondo
        NGN_TextureData* bg_data;       // Imagen
        NGN_Texture* bg;                // Textura

        NGN_TextureData* mask_data[4];      // Mascaras para el efecto
        NGN_Texture* mask[4];

        // Sprites
        NGN_SpriteData* sprite_data;    // Graficos del sprite

        // Objetos estaticos para mostrar la animacion
        std::vector<NGN_Sprite*> sprite;

        // Objetos mobiles
        struct sprite_type {
            NGN_Sprite* sprite;         // Sprite
            float speed;                // Velocidad
            uint32_t left;              // Limite izquierdo
            uint32_t right;             // Limite derecho
        };
        std::vector<sprite_type> bird;


        // Mascara a mostrar
        NGN_Texture* current_mask;      // Guarda la textura que se renderizara
        int32_t mask_num;               // n� de mascara a mostrar
        int32_t mask_alpha;             // nivel de alpha
        int32_t mask_mode;              // modo de mezcla de color

        // Tipografia
        NGN_TextFont* font;             // Tipografia

        // Capa de texto
        NGN_TextLayer* text;            // Capa de texto


        // Carga los archivos necesarios
        bool Load();

        // Crea el escenario
        void CreateStage();

        // Crea los sprites
        void CreateSprites();

        // Update
        void Update();

        // Mueve los sprites
        void MoveSprites();

        // Gestion de las mascaras
        void MaskUpdate();

        // Funcion para renderizar la escena
        void Render();

};


#endif // DEMO_H_INCLUDED
