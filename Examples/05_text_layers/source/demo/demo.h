/******************************************************************************

    Ejemplo del uso de las capas de texto
    Archivo de Demo (Declaraciones)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.11.0-stable o superior

    Requiere GCC 11.3.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.0.22) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.1) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
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
static const std::string WINDOW_TITLE = "N'gine text layers";       // Titulo de la ventana
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

        // Textura de fondo
        NGN_TextureData* bg_data;
        NGN_Texture* bg;

        // Textura para la caja de texto
        NGN_TextureData* textbox_data;

        // Tipografias
        NGN_TextFont* fnt_banner;       // Fuente para el banner de texto
        NGN_TextFont* fnt_box_aa;       // Fuente para la caja de texto con anti-alias
        NGN_TextFont* fnt_box_px;       // Fuente para la caja de texto sin anti-alias
        NGN_TextFont* fnt_cursor;       // Fuente para el cursor en movimiento

        // Capas de texto
        NGN_TextLayer* textbox;
        NGN_TextLayer* banner;
        NGN_TextLayer* cursor;

        // Carga los archivos necesarios
        bool Load();

        // Crea la escena
        void CreateStage();

        // Crea la caja de texto
        void CreateTextBox();

        // Crea el banner superior
        void CreateBanner();

        // Crea el cursor
        void CreateCursor();

        // Update
        void Update();

        // Actualiza el cursor
        void UpdateCursor();

        // Renderiza la escena
        void Render();

};


#endif // DEMO_H_INCLUDED
