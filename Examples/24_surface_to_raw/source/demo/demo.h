/******************************************************************************

    N'gine Lib for C++
    Ejemplo de la captura del surface (Declaraciones)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2022 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.8.0-stable o superior

    Requiere GCC 8.1.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.12) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.0.15) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.1) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20200306)
    (c) 2005 - 2020 by Lode Vandevenne
    http://lodev.org/lodepng/


    N'gine se distribuye bajo la licencia CREATIVE COMMONS
    "Attribution-NonCommercial 4.0 International"
    https://creativecommons.org/licenses/by-nc/4.0/

    You are free to:

        - Share
        copy and redistribute the material in any medium or format.
        - Adapt
        remix, transform, and build upon the material.

        The licensor cannot revoke these freedoms as long as you follow
        the license terms.

    Under the following terms:

        - Attribution
        You must give appropriate credit, provide a link to the license,
        and indicate if changes were made. You may do so in any reasonable
        manner, but not in any way that suggests the licensor endorses you
        or your use.

        - NonCommercial
        You may not use the material for commercial purposes.

        - No additional restrictions
        You may not apply legal terms or technological measures that
        legally restrict others from doing anything the license permits.

******************************************************************************/



#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "Surface capture";          // Titulo de la ventana
static const uint32_t SCR_WIDTH = 1280;                             // Resolucion
static const uint32_t SCR_HEIGHT = 720;
static const int8_t SCR_MODE_WINDOWS = NGN_SCR_WINDOW;              // Modo de pantalla en Windows
static const int8_t SCR_MODE_LINUX = NGN_SCR_WINDOW;                // Modo de pantalla en Linux
static const bool SHOW_MOUSE = false;                               // Estado del puntero del raton
static const bool BILINEAR_FILTER = false;                          // Filtrado bi-linear
static const bool VSYNC = true;                                     // Sincronismo vertical
static const bool FPS_COUNTER = true;                               // Contador de frames por segundo (solo en modo debug)



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
        uint8_t Run();


    private:

        // FLAGS de estado
        const uint8_t FLAG_ZERO = 0;
        const uint8_t FLAG_EXIT = 1;

        // Carga de archivos necesarios
        bool Load();

        // Crea los elementos
        void Create();

        // Logica del programa
        void Update();

        // Render de los elementos graficos
        void Render();

        // Textura del fondo
        NGN_TextureData* bg_data;                   // Datos de la textura
        NGN_Texture* bg;                            // Textura

        // Textura de destino del renderer
        NGN_Texture* target_texture;                // Textura de destino del renderer

        // Bola de cristal con el parallax en su interior
        void CrystalBall();

        // Capas del parallax
        static const uint8_t layers = 3;
        NGN_TiledBgData* tbg_data[layers];
        NGN_TiledBg* tbg[layers];

        // Mascara
        const uint32_t CRYSTAL_BALL_SIZE = 384;
        NGN_RendererSurface* renderer_surface;
        NGN_RawImage* renderer_target;
        NGN_RawImage* mask_texture;

        // Efecto del interior de la bola de cristal
        NGN_TextureData* glass_data;
        NGN_Texture* glass;

        // Bola de cristal
        NGN_TextureData* crystal_ball_data;
        NGN_Texture* crystal_ball;


        // Objetos mobiles
        static const uint32_t NUMBER_OF_BIRDS = 32;
        struct bird_type {
            NGN_Sprite* sprite;         // Sprite
            float speed;                // Velocidad
            uint32_t left;              // Limite izquierdo
            uint32_t right;             // Limite derecho
        };
        bird_type bird[NUMBER_OF_BIRDS];    // Sprites
        NGN_SpriteData* bird_data;          // Graficos del sprite

        // Crea los sprites
        void CreateBirds();

        // Mueve los sprites
        void MoveBirds();

};


#endif // DEMO_H_INCLUDED
