/******************************************************************************

    Ejemplo de render de una escena a una textura
    Archivo de Demo (Declaraciones)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.5.0-beta o superior

    Requiere GCC 8.1.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.12) - 64-bits
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
static const std::string WINDOW_TITLE = "N'gine render to texture";     // Titulo de la ventana
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

        // Sprite animado
        NGN_SpriteData* man_gfx;

        // Struct
        struct {
            NGN_Sprite* sprite;                         // Sprite
            Vector2 position;                           // Posicion en pantalla
            const float speed[2] = {3.0f, -1.5f};       // Velocidad de desplazamiento
        } man[2];

        // Fondo de tiles
        NGN_TiledBgData* bg_gfx;
        NGN_TiledBg* bg;

        // Textura destino
        NGN_Texture* render_texture;
        float scale;
        double rotation;

        // Carga recursos
        bool Load();

        // Crea la escena
        void Create();

        // Logica de la demo
        void Update();

        // Render de los graficos
        void Render();

};


#endif // DEMO_H_INCLUDED
