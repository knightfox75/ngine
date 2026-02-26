/******************************************************************************

    Pixel Art Project Template: Titulo

    Proyecto iniciado el xx de xx del xx
    (c) xxxx by Cesar Rincon "NightFox"
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



#ifndef TITLE_H_INCLUDED
#define TITLE_H_INCLUDED



/*** Includes ***/
// Includes de la libreria NGN
#include <ngn.h>
// Includes del proyecto
#include "../system/settings.h"
#include "../system/shared_data.h"
#include "../system/soundtrack.h"
#include "../system/input.h"



/*** Declaracion de la clase ***/
class Title {

    public:

        // Constructor
        Title();

        // Destructor
        ~Title();

        // Start
        bool Start();

        // Ejecucion
        uint32_t Run();


    private:

        // Punteros a los singletons
        SharedData* shared_data;
        Soundtrack* soundtrack;
        Input* input;

        // Cache de recursos
        struct {
            NGN_AudioClipData* menu_move;
            NGN_AudioClipData* menu_accept;
        } cache;

        // Objetos
        struct {
            NGN_Texture* title;             // Titulo
            NGN_Sprite* cursor;             // Cursor
        } asset;

        // Posiciones
        const struct {
            Vector2 title = {0.0f, 0.0f};           // Titulo
            Vector2 cursor = {280.0f, 224.0f};      // Cursor
        } position;

        // Maquina de estados
        struct {
            enum {
                fade_in,            // Efecto FADE-IN
                fade_out,           // Efecto FADE-OUT
                run,                // Ejecuta el menu
                exit,               // Sal del menu
                none
            };
        } STATE;
        int32_t state, next_state;

        // Menu de opciones
        struct {
            int32_t option;                         // Opcion del menu seleccionada
            const uint8_t total_options = 4;        // Numero de opciones
            const int32_t first_option = 0;         // Definicion del numero de opciones
            const int32_t last_option = 3;
            const float cursor_offset = 32.0f;      // Distancia entre opciones
        } menu;

        // Metodos de la clase
        bool Load();                                // Carga de archivos
        void Create();                              // Crea la escena
        uint32_t Update();                          // Logica del programa
        void Render();                              // Render de los elementos
        bool Menu();                                // Control del menu
        uint32_t ApplicationQuit();                 // Salida del usuario
        bool Skip();                                // Solicita saltarte la escena

        // Procesos comunes
        void CommonProcesses();

};



#endif // TITLE_H_INCLUDED
