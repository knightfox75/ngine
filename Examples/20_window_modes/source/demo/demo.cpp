/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso de los modos de pantalla (Programa)

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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
#include <string>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Punteros de datos de los fondos
    for (uint8_t i = 0; i < bg_number; i ++) {
        bg_data[i] = NULL;
        bg[i] = NULL;
    }

    // GUI
    font = NULL;
    text = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Punteros de datos de los fondos
    for (uint8_t i = 0; i < bg_number; i ++) {
        delete bg[i];
        bg[i] = NULL;
        delete bg_data[i];
        bg_data[i] = NULL;
    }

    // GUI
    delete text; text = NULL;
    delete font; font = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Habilita el archivo de registro
    ngn->log->OpenLogFile("debug_log.txt");

    // Crea la ventana con la resolucion nativa
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    ngn->graphics->Update();

    // Visibilidad del cursor del raton
    ngn->graphics->ShowMouse(SHOW_MOUSE);

    // Contador de FPS activo?
    ngn->system->fps_counter = FPS_COUNTER;

    // Selecciona el modo grafico (ventana/full screen)
    #if defined (OS_WINDOWS)
        ngn->graphics->SetMode(SCR_MODE_WINDOWS);
    #elif defined (OS_LINUX)
        ngn->graphics->SetMode(SCR_MODE_LINUX);
    #endif
    ngn->graphics->Update();

    // Usa el archivo empaquetado de datos si es la version release
    #if defined (MODE_RELEASE)
        if(!ngn->load->SetPackage("data.pkg", "0123456789ABCDEF")) return false;
    #endif

	// Muestra la version de la libreria en la consola
	#if defined (MODE_DEBUG)
		ngn->log->Message("Versions check:\n" + ngn->system->GetVersion());
		ngn->log->Message("N'gine started successfully!");
	#endif

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Carga de archivos
    if (!Load()) return false;

    // Crea los elementos del programa
    Create();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
int8_t Demo::Run() {

    // Control del loop
    int8_t loop = -1;

    while (loop < 0) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos


        /***
        Bucle principal del programa
        ***/
        Logic();        // Logica del programa
        Render();       // Render de los elementos graficos


        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

        // Control del bucle principal
        if (ngn->system->quit) {    // Si se pulsa la [X] de la ventana
            loop = 0;
        } else if (ngn->input->key_ESC->down || ngn->input->controller[0].button[10].down) {    // Si se pulsa la tecla [ESC] O se pulsa el boton XBOX
            loop = 1;
        }

    }

    // Devuelve el resultado
    return loop;

}



/*** Carga de los archivos necesarios ***/
bool Demo::Load() {

    // Carga los fondos
    const std::string basename = "data/bglow";
    std::string filename = "";
    for (uint8_t i = 0; i < bg_number; i ++) {
        filename = basename + ngn->toolbox->Int2String(i, 1, "0") + ".tbg";     // Genera el nombre de archivo
        bg_data[i] = ngn->load->TiledBg(filename);                              // Intenta cargar el archivo
        if (!bg_data[i]) return false;                                          // Verifica que la carga se haya realizado
    }

    // Fuente
    font = ngn->load->TrueTypeFont("data/monofonto.ttf", 16, true, 0xFFFFFF, 1, 0x000000);
    if (!font) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Crea los fondos de tiles
    for (uint8_t i = 0; i < bg_number; i ++) {
        bg[i] = new NGN_TiledBg(bg_data[i], 0, 0);
    }

    // GUI
    text = new NGN_TextLayer(font);
    text->Padding(8);
    text->Cls();
    update_gui = 2;

    // Modo por defecto
    mode = 1;

}



/*** Logica del programa ***/
void Demo::Logic() {

    // Parallax de los fondos
    float speed = 0.0f;
    for (uint8_t i = 0; i < bg_number; i ++) {
        speed = bg_speed[i];
        bg[i]->Translate(speed, 0.0f);
        if (bg[i]->position.x > bg_loop[i]) bg[i]->position.x -= bg_loop[i];
        if (bg[i]->position.x < 0) bg[i]->position.x += bg_loop[i];
    }

    // Cambio de modo grafico --
    if (ngn->input->key_ARROW_DOWN->down) {
        mode --;
        if (mode < -1) mode = -1;   // Modo pantalla completa (real)
        ngn->graphics->SetMode(mode);
        update_gui = true;
    }

    // Cambio de modo grafico ++
    if (ngn->input->key_ARROW_UP->down) {
        mode ++;
        if (mode > NGN_SCR_WINDOW_X4) mode = NGN_SCR_WINDOW_X4;   // Modo WINDOW X4
        ngn->graphics->SetMode(mode);
        update_gui = true;
    }

    // Filtado desactivado
    if (ngn->input->key_ARROW_LEFT->down) {
        ngn->graphics->SetFiltering(false);
        update_gui = true;
    }
    // Filtado activado
    if (ngn->input->key_ARROW_RIGHT->down) {
        ngn->graphics->SetFiltering(true);
        update_gui = true;
    }

    // GUI
    if (update_gui || ngn->graphics->force_redaw) {
        std::string txt = "";
        text->Cls();
        txt += "NATIVE RESOLUTION: " + ngn->toolbox->Int2String(ngn->graphics->native_w, 1, "0") + "x" + ngn->toolbox->Int2String(ngn->graphics->native_h, 1, "0") + "\n";
        Size2I32 desktop = ngn->graphics->GetDesktopResolution();
        txt += "DESKTOP RESOLUTION: " + ngn->toolbox->Int2String(desktop.width, 1, "0") + "x" + ngn->toolbox->Int2String(desktop.height, 1, "0") + "\n";
        txt += "MODE: ";
        switch (mode) {
            case NGN_SCR_WINDOW:
                txt += "WINDOW X1";
                break;
            case NGN_SCR_WINDOW_X2:
                txt += "WINDOW X2";
                break;
            case NGN_SCR_WINDOW_X3:
                txt += "WINDOW X3";
                break;
            case NGN_SCR_WINDOW_X4:
                txt += "WINDOW X4";
                break;
            case NGN_SCR_WINDOW_FULL:
                txt += "WINDOW FULL";
                break;
            case NGN_SCR_FULLSCREEN:
                txt += "FULL SCREEN";
                break;
        }
        txt += "\n";
        txt += "FILTERING: ";
        if (ngn->graphics->filtering) {
            txt += "ON";
        } else {
            txt += "OFF";
        }
        text->Print(txt);
        update_gui = false;
    }

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Fondos de tiles
    for (uint8_t i = 0; i < bg_number; i ++) {
        ngn->render->TiledBg(bg[i]);
    }

    // Capa de texto
    ngn->render->TextLayer(text);

}
