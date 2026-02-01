/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso del Canvas: GetPixel (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.20.0+10th-anniversary o superior
	(c) 2016 - 2026 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
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
#include <ctime>
#include <string>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Canvas
    bg = nullptr;
    frame = nullptr;

    // Texto
    mono = nullptr;
    text = nullptr;

    // Semilla del random
    srand(time(nullptr));

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Canvas
    delete bg; bg = nullptr;
    delete frame; frame = nullptr;

    // Texto
    delete text; text = nullptr;
    delete mono; mono = nullptr;

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

    // Carga la tipografia
    mono = ngn->load->TrueTypeFont("data/monofonto.ttf", 56, true);
    if (!mono) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Crea el canvas del fondo
    bg = new NGN_Canvas();
    // Crea el canvas del marco
    frame = new NGN_Canvas(16, 16, frame_size, frame_size);

    // Genera el fondo con bloques de color aleatorios
    Rgba color = {0x00, 0x00, 0x00, 0xFF};                          // Color por componentes RGBA
    uint32_t _color = 0x00000000;                                   // Color en formato 0xRRGGBBAA
    for (uint32_t y = 0; y < SCR_HEIGHT; y += tile_size) {          // Filas
        for (uint32_t x = 0; x < SCR_WIDTH; x += tile_size) {       // Columnas
            // Genera un color aleatorio
            color.r = ((rand() % 8192) % 256);
            color.g = ((rand() % 8192) % 256);
            color.b = ((rand() % 8192) % 256);
            _color = ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
            // Genera la tile con el color aleatorio
            bg->Box(x, y, (x + tile_size -1), (y + tile_size -1), _color, true);
        }
    }

    // Crea la capa de texto
    text = new NGN_TextLayer(mono, nullptr, 672, 32, 512, 64);
    text->CanvasColor(0x202020B0);
    text->InkColor(0xFFFFFFFF);
    text->Cls();

}



/*** Logica del programa ***/
void Demo::Logic() {

    // Marco de la muestra de color
    uint32_t color32 = bg->GetPixelColor(ngn->input->mouse.x, ngn->input->mouse.y);     // Lee el color del pixel en formato Color32
    frame->Cls();       // Borra el contenido actual
    frame->Box(shadow_size, shadow_size, frame_size, frame_size, 0x202020B0, true);             // Sombra
    frame->Box(0, 0, (frame_size - shadow_size), (frame_size - shadow_size), color32, true);    // Relleno
    frame->Box(0, 0, (frame_size - shadow_size), (frame_size - shadow_size), 0x000000FF);       // Contorno

    // Texto con los valores RGB
    Rgba color_rgba = bg->GetPixelRgba(ngn->input->mouse.x, ngn->input->mouse.y);     // Lee el color del pixel en formato Color32
    std::string t = "R:";
    t += ngn->toolbox->Int2String(color_rgba.r, 3, "0");
    t += " G:";
    t += ngn->toolbox->Int2String(color_rgba.g, 3, "0");
    t += " B:";
    t += ngn->toolbox->Int2String(color_rgba.b, 3, "0");
    text->Cls();
    text->Locate(16, 0);
    text->Print(t);

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Render del fondo
    ngn->render->Canvas(bg);
    // Render del marco de la muestra de color
    ngn->render->Canvas(frame);
    // Render de la capa de texto
    ngn->render->TextLayer(text);


}
