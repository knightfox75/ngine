/******************************************************************************

    Ejemplo del uso de la capa de dibujado (canvas) para formas
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

    // Inicializa los punteros de datos
    bg_data = NULL;
    cursor_data = NULL;

    // Inicializa los punteros de objetos
    bg = NULL;
    canvas = NULL;
    cursor = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg; bg = NULL;
    delete canvas; canvas = NULL;
    delete cursor; cursor = NULL;

    // Borra los datos
    delete bg_data; bg_data = NULL;
    delete cursor_data; cursor_data = NULL;

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

    // Carga los archivos
    if (!Load()) return false;

    // Crea la escena
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



/*** Carga todos los archivos necesarios ***/
bool Demo::Load() {

    // Carga la imagen para el fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (bg_data == NULL) return false;

    // Carga el cursor
    cursor_data = ngn->load->Sprite("data/mouse_pointer.spr");
    if (cursor_data == NULL) return false;

    // Carga correcta
    return true;

}



/*** Crea las formas ***/
void Demo::Create() {

    // Crea la textura con los datos cargados
    bg = new NGN_Texture(bg_data, 0, 0);

    // Crea el canvas
    canvas = new NGN_Canvas();

    // Crea el cursor del raton
    cursor = new NGN_Sprite(cursor_data);

    // Calcula la posicion de los vertices de la forma
    double step = (PI * 2.0f) / (double)VERTEX;
    double angle = 0.0f;
    double radius = (double)SCR_HEIGHT * 0.33f;
    double cx = (double)SCR_WIDTH / 2.0f;
    double cy = (double)SCR_HEIGHT / 2.0f;
    for (uint32_t n = 0; n < VERTEX; n ++) {
        x[n] = std::round((std::cos(angle) * radius) + cx);
        y[n] = std::round((std::sin(angle) * radius) + cy);
        angle += step;
    }

    // Id de vertice activo
    point = -1;

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Dibuja en el canvas
    Draw();
    // Render de la escena
    Render();

}



/*** Dibuja en el canvas ***/
void Demo::Draw() {

    // Borra el canvas
    canvas->Cls();

    // Si sueltas el boton izquierdo, cancela la seleccion
    if (ngn->input->mouse.LB.up) point = -1;

    // Si no hay ningun selector de vertices activo...
    int32_t d = 0.0f;
    int32_t _x = 0, _y = 0;
    if (point < 0) {
        // Analiza todos los vertices
        for (uint32_t n = 0; n < VERTEX; n ++) {
            // Calcula la distancia del mouse al vertice
            _x = ngn->input->mouse.x - x[n];
            _y = ngn->input->mouse.y - y[n];
            d = (int32_t)std::sqrt((_x * _x) + (_y * _y));
            // Si esta en al zona indicalo
            if (d <= SNAP) {
                // Si pulsas el boton izquierdo del mouse
                if (ngn->input->mouse.LB.down) {
                    point = n;
                } else {
                    // Dibuja el marcador de vertices
                    canvas->Circle(x[n], y[n], SNAP, 0x00FFFFFF, SNAP, true);
                }
                // No analices el resto de vertices
                break;
            }
        }
    }

    // Si hay un punto seleccionado
    if (point >= 0) {
        // Cambia el vertice seleccionado a la posicion del raton
        x[point] = ngn->input->mouse.x;
        y[point] = ngn->input->mouse.y;
        // Dibuja el marcador de vertices
        canvas->Circle(x[point], y[point], SNAP, 0xFF0000FF, SNAP, true);
    }

    // Dibuja el poligono
    for (uint32_t n = 0; n < VERTEX; n ++) canvas->Line(x[n], y[n], x[((n + 1) % VERTEX)], y[((n + 1) % VERTEX)], 0xFFFFFFFF);

}



/*** Renderiza la escena ***/
void Demo::Render() {

    // Renderiza el fondo
    ngn->render->Texture(bg);

    // Renderiza el canvas
    ngn->render->Canvas(canvas);

    // Renderiza el cursor del raton en la posicion del raton
    ngn->render->Sprite(cursor, ngn->input->mouse.x, ngn->input->mouse.y);

}
