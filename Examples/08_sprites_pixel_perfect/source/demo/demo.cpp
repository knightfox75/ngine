/******************************************************************************

    Ejemplo de colisiones "pixel perfect" entre sprites
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

    // Punteros de datos
    bg_data = NULL;
    star_data = NULL;
    cake_data = NULL;

    // Punteros de objetos
    bg = NULL;
    star = NULL;
    cake = NULL;

    // Variables
    zoom.x = zoom.y = 1.0f;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg; bg = NULL;
    delete star; star = NULL;
    delete cake; cake = NULL;

    // Borra los datos
    delete bg_data; bg_data = NULL;
    delete star_data; star_data = NULL;
    delete cake_data; cake_data = NULL;

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
    CreateStage();

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



/*** Carga los archivos necesarios ***/
bool Demo::Load() {

    // Fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (bg_data == NULL) return false;

    // Sprites
    star_data = ngn->load->Sprite("data/red_star.spr");
    if (star_data == NULL) return false;
    cake_data = ngn->load->Sprite("data/cake.spr");
    if (cake_data == NULL) return false;

    // Carga correcta
    return true;

}



/*** Crea la escena ***/
void Demo::CreateStage() {

    // Crea el fondo
    bg = new NGN_Texture(bg_data, 0, 0);
    // Crea el sprite de la tarta
    cake = new NGN_Sprite(cake_data, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));
    cake->frame = 0;
    // Crea el sprite de la estrella
    star = new NGN_Sprite(star_data, 0, 0);

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    Logic();        // Logica del programa
    Render();       // Renderiza la escena

}



/*** Logica del programa ***/
void Demo::Logic() {

    // Mueve la estrella con el raton
    star->Position(ngn->input->mouse.x, ngn->input->mouse.y);

    // Rotacion
    if (ngn->input->key_ARROW_RIGHT->held) cake->Rotate(2.0f);
    if (ngn->input->key_ARROW_LEFT->held) cake->Rotate(-2.0f);
    if (ngn->input->key_Q->down) cake->rotation = 0.0f;

    // Zoom Global
    if (ngn->input->key_ARROW_UP->held) {
        zoom.x += 0.02f;
        zoom.y += 0.02f;
        if (zoom.x > 4.0f) zoom.x = 4.0f;
        if (zoom.y > 4.0f) zoom.y = 4.0f;
    }
    if (ngn->input->key_ARROW_DOWN->held) {
        zoom.x -= 0.02f;
        zoom.y -= 0.02f;
        if (zoom.x < 0.25f) zoom.x = 0.25f;
        if (zoom.y < 0.25f) zoom.y = 0.25f;
    }
    // Zoom X
    if (ngn->input->key_D->held) {
        zoom.x += 0.02f;
        if (zoom.x > 4.0f) zoom.x = 4.0f;
    }
    if (ngn->input->key_A->held) {
        zoom.x -= 0.02f;
        if (zoom.x < 0.25f) zoom.x = 0.25f;
    }
    // Zoom Y
    if (ngn->input->key_W->held) {
        zoom.y += 0.02f;
        if (zoom.y > 4.0f) zoom.y = 4.0f;
    }
    if (ngn->input->key_S->held) {
        zoom.y -= 0.02f;
        if (zoom.y < 0.25f) zoom.y = 0.25f;
    }
    if (ngn->input->key_E->down) zoom.x = zoom.y = 1.0f;
    cake->Scale(zoom.x, zoom.y);

    // Dependiendo de si hay o no colision
    if (ngn->collisions->PixelPerfect(star, cake)) {
        cake->frame = 1;
    } else {
        cake->frame = 0;
    }

}



/*** Renderiza la escena ***/
void Demo::Render() {

    ngn->render->Texture(bg);       // Fondo
    ngn->render->Sprite(cake);      // Sprite de la tarta
    ngn->render->Sprite(star);      // Sprite de la estrella

}
