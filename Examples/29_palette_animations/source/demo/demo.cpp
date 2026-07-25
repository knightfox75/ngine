/******************************************************************************

    Ejemplo de animaciones de fondo rotando los colores de la paleta
    (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.20.0+10th-anniversary o superior
	(c) 2016 - 2026 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC MinGW64 (SEH) - 64-bits
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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    monofonto_32 = nullptr;
    bg_data = nullptr;
    
    // Inicializa los punteros de objetos
    profiling_text = nullptr;
    bg = nullptr;

    // Variables
    next_ticks = 0;
    work_load_acc = 0.0;
    frame_time_acc = 0.0;
    profiling_calls = 0;
    work_load_avg = 0.0;
    frame_time_avg = 0.0;


}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete profiling_text; profiling_text = nullptr;
    delete bg; bg = nullptr;

    // Borra los datos de los punteros
    delete monofonto_32; monofonto_32 = nullptr;
    delete bg_data; bg_data = nullptr;

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
uint32_t Demo::Run() {

    // Control del loop
    uint32_t loop = FLAG_ZERO;

    // Mientras loop valga cero, repite
    while (!loop) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();

        // Logica del programa
        Update();

        // Render de los elementos graficos
        Render();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

        // Salida del programa
        if (
            ngn->system->quit                                           // Si se pulsa la [X] de la ventana o ALT+F4
            ||
            ngn->input->key_ESC->down                                   // la tecla ESC
            ||
            ngn->input->controller[0].button[XBOX_BUTTON_XBOX].down     // o el boton [X] del gamepad de XBOX
        ) loop |= FLAG_EXIT;

    }

    // Devuelve el resultado
    return loop;

}



/*** Carga de los archivos necesarios ***/
bool Demo::Load() {

    // Carga la tipografia para las metricas
    monofonto_32 = ngn->load->TrueTypeFont("data/monofonto.otf", 32, false, 0XFFFFFF, 2, 0x202020);
    if (!monofonto_32) return false;

    // Carga el fondo indexado
    bg_data = ngn->load->IndexedTiledBg("data/indexed_bg.itb", "data/indexed_bg.pal", "data/indexed_bg.cyc");
    if (!bg_data) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Crea la capa de texto para las metricas
    profiling_text = new NGN_TextLayer(monofonto_32, nullptr, 180, 16, 1280, 48);

    // Crea el fondo indexado
    bg = new NGN_TiledBg(bg_data);

    // Prepara los datos para las metricas
    next_ticks = 0;
    work_load_acc = 0.0;
    frame_time_acc = 0.0;
    profiling_calls = 0;
    work_load_avg = 0.0;
    frame_time_avg = 0.0;

}



/*** Logica del programa ***/
void Demo::Update() {

    // Metricas del juego (pertenecen al frame anterior)
    GameProfiling();

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Render del fondo
    ngn->render->TiledBg(bg);
    
    // Render del texto con las metricas del juego
    ngn->render->TextLayer(profiling_text);

}



/*** Metricas del programa ***/
void Demo::GameProfiling() {

    // Acumulador de las metricas
    uint32_t current_ticks = SDL_GetTicks();
    work_load_acc += ngn->system->GetWorkLoad();
    frame_time_acc += ngn->system->GetFrameTime();
    profiling_calls ++;

    // Media por segundo de las metricas
    if (current_ticks >= next_ticks) {
        if (profiling_calls > 0) {
            work_load_avg = work_load_acc / (double)profiling_calls;
            frame_time_avg = frame_time_acc / (double)profiling_calls;
        }
        next_ticks += PROFILING_UPDATE_TIME;
        work_load_acc = 0.0;
        frame_time_acc = 0.0;
        profiling_calls = 0;
    }

    // Imprime las metricas
    uint32_t frames = ngn->graphics->GetFps();
    std::string text = "  " + ngn->toolbox->Int2String(frames, 3, "0") + " FPS  ";
    text += "FRAME TIME: " + ngn->toolbox->Float2String((frame_time_avg * 1000.0), 8, 4, "0") + "ms  ";
    text += "WORKLOAD: " + ngn->toolbox->Float2String(work_load_avg, 6, 2, "0") + "%";
    profiling_text->Cls();
    profiling_text->Print(text);
    
}
