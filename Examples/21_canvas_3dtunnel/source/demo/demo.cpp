/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso del Canvas: Tunel 3D (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.12.0-stable o superior

    Requiere GCC 11.3.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.26.3) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.20.2) - 64-bits
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

    // Prepara los punteros
    bg = NULL;      // Canvas del fondo

    // Vector de datos
    deep.clear();

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Elimina los canvas
    delete bg; bg = NULL;       // Fondo

    // Vacia el vector de datos
    deep.clear();

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

	// Muestra la version de la libreria en la consola
	#if defined (MODE_DEBUG)
		ngn->log->Message(ngn->system->GetVersion() + " started successfully!");
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


    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Crea el canvas del fondo
    bg = new NGN_Canvas();      // Pantalla completa

    // Calculos del tunel
    origin.top = 0;
    origin.bottom = (SCR_HEIGHT - 1);
    origin.left = 0;
    origin.right = (SCR_WIDTH - 1);
    w = (SCR_WIDTH / deep_size);
    h = (SCR_HEIGHT/ deep_size);
    tunnel_bg.top = (h / 2);
    tunnel_bg.bottom = (h / 2);
    tunnel_bg.left = (w / 2);
    tunnel_bg.right = (w / 2);

    // Crea un buffer de profundidad
    float z = 1.0f;
    zst_point = 1.0f;
    do {
        z *= deep_step;
        if (z > z_size) continue;
        deep.push_back(z);
        //std::cout << z << std::endl;
    } while (z < z_size);

    // Mouse delta
    delta_mouse.x = (float)ngn->input->mouse.x;
    delta_mouse.y = (float)ngn->input->mouse.y;

}



/*** Logica del programa ***/
void Demo::Logic() {

    // Borra el canvas
    bg->Cls();

    // Dibuja el tunel
    DrawTunnel();

    // Movimiento
    zst_point *= tunnel_speed;
    if (zst_point >= deep_step) zst_point -= (deep_step - 1.0f);
    float z = zst_point;
    for (uint32_t i = 0; i < deep.size(); i ++) {
        z *= deep_step;
        deep[i] = z;
    }

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Render del canvas
    ngn->render->Canvas(bg);

}



/*** Dibuja el tunel ***/
void Demo::DrawTunnel() {

    // Coordenadas del mouse
    delta_mouse.x += (((float)ngn->input->mouse.x - delta_mouse.x) / mouse_speed);
    delta_mouse.y += (((float)ngn->input->mouse.y - delta_mouse.y) / mouse_speed);
    int32_t mx = std::round(delta_mouse.x);
    int32_t my = std::round(delta_mouse.y);
    if (mx < w) mx = w;
    if (mx > ((int32_t)SCR_WIDTH - w)) mx = ((int32_t)SCR_WIDTH - w);
    if (my < h) my = h;
    if (my > ((int32_t)SCR_HEIGHT - h)) my = ((int32_t)SCR_HEIGHT - h);

    // Calculo del fondo del tunel
    int32_t x1 = (mx - tunnel_bg.left);
    int32_t y1 = (my - tunnel_bg.top);
    int32_t x2 = (mx + tunnel_bg.right);
    int32_t y2 = (my + tunnel_bg.bottom);

    // Calculo de las lineas de profundidad
    float _x1, _x2, _y1, _y2;
    _x1 = (float)std::abs(x1 - origin.left);
    _x2 = (float)std::abs(x2 - origin.right);
    _y1 = (float)std::abs(y1 - origin.top);
    _y2 = (float)std::abs(y2 - origin.bottom);

    int32_t lx1, ly1, lx2, ly2;         // Vertices del marco actual
    int32_t ox1, oy1, ox2, oy2;         // Vertices del marco anterior
    uint32_t r = 0, g = 0, b = 0;       // Componentes del color
    uint32_t color = 0;                 // Color

    ox1 = origin.left;                  // Define el primer marco fuera del punto de vista
    ox2 = origin.right;
    oy1 = origin.top;
    oy2 = origin.bottom;

    // Segmentos
    for (int32_t i = ((int32_t)deep.size() - 1); i >= 0; i --) {
        // Calculo del color
        g = (std::ceil((deep[i] * (float)0xDF) / z_size) + 0x20);
        b = (0xFF - g);
        color = (r << 24) | (g << 16) | (b << 8) | 0xFF;
        // Calculos del marco actual
        lx1 = std::round((float)x1 - ((_x1 * deep[i]) / z_size));
        ly1 = std::round((float)y1 - ((_y1 * deep[i]) / z_size));
        lx2 = std::round((float)x2 + ((_x2 * deep[i]) / z_size));
        ly2 = std::round((float)y2 + ((_y2 * deep[i]) / z_size));
        // Dibuja las lineas
        DrawLines(ox1, oy1, ox2, oy2, lx1, ly1, lx2, ly2, color);
        // Guarda los valores para la siguiente linea
        ox1 = lx1;
        ox2 = lx2;
        oy1 = ly1;
        oy2 = ly2;
    }

}


/*** Dibuja las lineas de una seccion ***/
void Demo::DrawLines(
    int32_t ox1, int32_t oy1,
    int32_t ox2, int32_t oy2,
    int32_t lx1, int32_t ly1,
    int32_t lx2, int32_t ly2,
    uint32_t color
) {
    // Vertices
    bg->Line(ox1, oy1, lx1, ly1, color);
    bg->Line(ox2, oy1, lx2, ly1, color);
    bg->Line(ox1, oy2, lx1, ly2, color);
    bg->Line(ox2, oy2, lx2, ly2, color);
    // Segmentos intermedios
    int32_t tx1 = 0, ty1 = 0, tx2 = 0, ty2 = 0;
    for (int i = 1; i < segments_number; i ++) {
        // Superior
        tx1 = std::round((((float)std::abs(ox1 - ox2) / (float)segments_number) * (float)i) + (float)ox1);
        ty1 = oy1;
        tx2 = std::round((((float)std::abs(lx1 - lx2) / (float)segments_number) * (float)i) + (float)lx1);
        ty2 = ly1;
        bg->Line(tx1, ty1, tx2, ty2, color);
        // Inferior
        tx1 = std::round((((float)std::abs(ox1 - ox2) / (float)segments_number) * (float)i) + (float)ox1);
        ty1 = oy2;
        tx2 = std::round((((float)std::abs(lx1 - lx2) / (float)segments_number) * (float)i) + (float)lx1);
        ty2 = ly2;
        bg->Line(tx1, ty1, tx2, ty2, color);
        // Izquierda
        tx1 = ox1;
        ty1 = std::round((((float)std::abs(oy1 - oy2) / (float)segments_number) * (float)i) + (float)oy1);
        tx2 = lx1;
        ty2 = std::round((((float)std::abs(ly1 - ly2) / (float)segments_number) * (float)i) + (float)ly1);
        bg->Line(tx1, ty1, tx2, ty2, color);
        // Derecha
        tx1 = ox2;
        ty1 = std::round((((float)std::abs(oy1 - oy2) / (float)segments_number) * (float)i) + (float)oy1);
        tx2 = lx2;
        ty2 = std::round((((float)std::abs(ly1 - ly2) / (float)segments_number) * (float)i) + (float)ly1);
        bg->Line(tx1, ty1, tx2, ty2, color);
    }
    // Marco
    bg->Box(lx1, ly1, lx2, ly2, color);
}

