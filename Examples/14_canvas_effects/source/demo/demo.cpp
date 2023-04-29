/******************************************************************************

    Ejemplo del uso de la capa de dibujado (canvas) para efectos
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.14.0-stable o superior
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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
#include <cmath>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros a objetos
    canvas_bg = NULL;
    canvas_snow = NULL;

    // Variables
    animate = true;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los punteros de datos
    delete canvas_bg; canvas_bg = NULL;
    delete canvas_snow; canvas_snow = NULL;

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

    CreateBg();     // Crea el fondo
    CreateSnow();   // Crea el efecto de nieve

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



/*** Crea el fondo ***/
void Demo::CreateBg() {

    // Crea el canvas
    canvas_bg = new NGN_Canvas();

    // Estructura de color
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0xFF;
    uint32_t color = 0;

    // Crea un fondo
    uint32_t id = 0;
    for (uint32_t y = 0; y < SCR_HEIGHT; y ++) {
        for (uint32_t x = 0; x < SCR_WIDTH; x ++) {
            r = (uint32_t)std::sqrt(x * (SCR_HEIGHT - y)) % 0xFF;
            g = (uint32_t)std::sqrt((SCR_WIDTH - x) * y) % 0xFF;
            b = (uint32_t)std::sqrt(x * y) % 0xFF;
            color = (r << 24) | (g << 16) | (b << 8) | a;
            canvas_bg->Point(x, y, color);
            id ++;
        }
    }

}



/*** Crea el canvas con nieve ***/
void Demo::CreateSnow() {

    // Crea el canvas
    const uint32_t width = (SCR_WIDTH / SCREEN_DIVIDER);
    const uint32_t height = (SCR_HEIGHT / SCREEN_DIVIDER);
    canvas_snow = new NGN_Canvas(0.0f, 0.0f, width, height);

    // Crea un fondo
    uint32_t id = 0;
    uint8_t gray = 0;
    uint32_t color = 0;
    for (uint32_t y = 0; y < height; y ++) {
        for (uint32_t x = 0; x < width; x ++) {
            gray = (rand() % 0xFF);
            color = (gray << 24) | (gray << 16) | (gray << 8) | 0xFF;
            canvas_snow->Point(x, y, color);
            id ++;
        }
    }

    // Ajusta el alpha
    canvas_snow->alpha = 200;

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Animacion
    Animation();
    // Render de objetos
    Render();

}



/*** Animacion del canvas de nieve ***/
void Demo::Animation() {

    // Datos
    int32_t width = canvas_snow->width;
    int32_t height = canvas_snow->height;

    // Crea un fondo

    if (animate) {
        uint32_t id = 0;
        uint8_t gray = 0;
        uint32_t color = 0;
        for (int32_t y = 0; y < height; y ++) {
            for (int32_t x = 0; x < width; x ++) {
                gray = (rand() % 0xFF);
                color = (gray << 24) | (gray << 16) | (gray << 8) | 0xFF;
                canvas_snow->Point(x, y, color);
                id ++;
            }
        }
    }

    // Posicionalo
    int32_t x = ngn->input->mouse.x - (width / 2);
    int32_t y = ngn->input->mouse.y - (height / 2);
    canvas_snow->Position(x, y);

    animate = !animate;

}



/*** Renderiza los objetos ***/
void Demo::Render() {

    // Renderiza los canvas
    ngn->render->Canvas(canvas_bg);
    ngn->render->Canvas(canvas_snow);

}
