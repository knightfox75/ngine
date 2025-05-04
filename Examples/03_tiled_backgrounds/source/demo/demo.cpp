/******************************************************************************

    Ejemplo de carga y scroll de fondos con tiles
    Archivo de Demo (Programa)

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
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    bg_main_data = nullptr;
    bg_water_data = nullptr;
    bg_sky_data = nullptr;

    // Inicializa los punteros de objetos
    bg_main = nullptr;
    bg_water = nullptr;
    bg_sky = nullptr;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg_main; bg_main = nullptr;
    delete bg_water; bg_water = nullptr;
    delete bg_sky; bg_sky = nullptr;

    // Borra los datos de los punteros
    delete bg_main_data; bg_main_data = nullptr;
    delete bg_water_data; bg_water_data = nullptr;
    delete bg_sky_data; bg_sky_data = nullptr;

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

    // Crea el decorado
    CreateStage();

    // Parametros iniciales
    position.x = 0.0f;
    position.y = 0.0f;

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

        // Actualizacion del programa
        Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();

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



/*** Carga de archivos ***/
bool Demo::Load() {

    // Carga los datos de los fondos
    bg_main_data = ngn->load->TiledBg("data/bg_main.tbg");
    if (bg_main_data == nullptr) return false;
    bg_water_data = ngn->load->TiledBg("data/bg_water.tbg");
    if (bg_water_data == nullptr) return false;
    bg_sky_data = ngn->load->TiledBg("data/bg_sky.tbg");
    if (bg_sky_data == nullptr) return false;

    // Carga correcta
    return true;

}



/*** Crea los fondos del decorado ***/
void Demo::CreateStage() {

    // Fondo del cielo
    bg_sky = new NGN_TiledBg(bg_sky_data);
    // Fondo del agua
    bg_water = new NGN_TiledBg(bg_water_data);
    // Fondo principal
    bg_main = new NGN_TiledBg(bg_main_data);

    // Calcula los limites del movimiento
    left = 0;
    right = (int32_t)bg_main->width - ngn->graphics->native_w;

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Mueve los fondos
    Move();
    // Renderiza la escena
    Render();

}



/*** Mueve el fondo con los cursores ***/
void Demo::Move() {

    // Si se pulsa el cursor izquierdo
    if (ngn->input->key_ARROW_LEFT->held) {
        position.x -= SCROLL_SPEED;
        if (position.x < left) position.x = left;
    }

    // Si se pulsa el cursor derecho
    if (ngn->input->key_ARROW_RIGHT->held) {
        position.x += SCROLL_SPEED;
        if (position.x > right) position.x = right;
    }

    // Calculos del parallax
    Vector2I32 p;
    p.y = position.y;
    float main_width, bg_width;

    // Posiciona el fondo principal
    bg_main->Position(position.x, position.y);
    main_width = (bg_main->width - ngn->graphics->native_w);
    // Posiciona la capa media
    bg_width = (bg_water->width - ngn->graphics->native_w);
    p.x = (int32_t)((bg_width * position.x) / main_width);
    bg_water->Position(p.x, p.y);
    // Posiciona la capa del fondo
    bg_width = (bg_sky->width - ngn->graphics->native_w);
    p.x = (int32_t)((bg_width * position.x) / main_width);
    bg_sky->Position(p.x, p.y);

}



/*** Renderiza el escenario ***/
void Demo::Render() {

    // Renderiza los fondos en orden, del fondo al frente
    ngn->render->TiledBg(bg_sky);
    ngn->render->TiledBg(bg_water);
    ngn->render->TiledBg(bg_main);

}
