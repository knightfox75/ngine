/******************************************************************************

    Ejemplo de render de una escena a una textura
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.19.0-stable o superior
	(c) 2016 - 2025 by Cesar Rincon "NightFox"
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
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Punteros a nullptr
    man_gfx = nullptr;
    man[0].sprite = nullptr;
    man[1].sprite = nullptr;
    bg_gfx = nullptr;
    bg = nullptr;
    render_texture = nullptr;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Destruye los sprites
    delete man[0].sprite; man[0].sprite = nullptr;
    delete man[1].sprite; man[1].sprite = nullptr;
    delete man_gfx; man_gfx = nullptr;

    // Destruye el fondo
    delete bg; bg = nullptr;
    delete bg_gfx; bg_gfx = nullptr;

    // Destruye el render
    delete render_texture; render_texture = nullptr;

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

    // Caga de archivos
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
        // Render de la escena
        Render();

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

    // Carga el fondo
    bg_gfx = ngn->load->TiledBg("data/running_man_bg.tbg");
    if (!bg_gfx) return false;

    // Carga los sprites
    man_gfx = ngn->load->Sprite("data/man_running.spr");
    if (!man_gfx) return false;

    // Carga correcta
    return true;

}



/*** Crea la escena ***/
void Demo::Create() {

    // Crea la textura del render
    render_texture = new NGN_Texture(SCR_WIDTH, SCR_HEIGHT, 0, 0);
    scale = 1.0f;
    rotation = 0.0f;

    // Crea el fondo
    bg = new NGN_TiledBg(bg_gfx, 0, 0);

    // Crea el sprite
    man[0].sprite = new NGN_Sprite(man_gfx);
    man[0].position = {640.0f, 532.0f};
    man[1].sprite = new NGN_Sprite(man_gfx);
    man[1].position = {640.0f, 180.0f};

    // Crea las animaciones
    man[0].sprite->AddAnimation("idle", 0, 0, 0, 0);      // Idle
    man[0].sprite->AddAnimation("fwd", 1, 8, 1, 4);       // Adelante
    man[0].sprite->AddAnimation("bck", 8, 1, 8, 8);       // Atras
    man[0].sprite->SetAnimation("idle");                  // Selecciona la animacion por defecto
    man[1].sprite->AddAnimation("idle", 0, 0, 0, 0);      // Idle
    man[1].sprite->AddAnimation("fwd", 1, 8, 1, 4);       // Adelante
    man[1].sprite->AddAnimation("bck", 1, 8, 1, 8);       // Atras
    man[1].sprite->SetAnimation("idle");                  // Selecciona la animacion por defecto

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Movimieto del personaje
    if (ngn->input->key_ARROW_RIGHT->held && (man[0].position.x < 1120.0f)) {
        man[0].position.x += man[0].speed[0];
        man[0].sprite->SetAnimation("fwd");
        man[1].position.x += man[1].speed[0];
        man[1].sprite->SetAnimation("fwd");
        man[1].sprite->flip_h = false;
    } else if (ngn->input->key_ARROW_LEFT->held && (man[0].position.x > 192.0f)) {
        man[0].position.x += man[0].speed[1];
        man[0].sprite->SetAnimation("bck");
        man[1].position.x += man[1].speed[1];
        man[1].sprite->SetAnimation("bck");
        man[1].sprite->flip_h = true;
    } else {
        man[0].sprite->SetAnimation("idle");
        man[1].sprite->SetAnimation("idle");
    }

    // Aplicalo
    man[0].sprite->Position(man[0].position);
    man[0].sprite->PlayAnimation();
    man[1].sprite->Position(man[1].position);
    man[1].sprite->PlayAnimation();

    // Zoom de camara
    if (ngn->input->key_ARROW_UP->held) {
        scale += 0.01f;
        if (scale > 1.5f) scale = 1.5f;
    }
    if (ngn->input->key_ARROW_DOWN->held) {
        scale -= 0.01f;
        if (scale < 1.0f) scale = 1.0f;
    }

    // Rotacion de la camara
    if (ngn->input->key_Q->held) {
        rotation -= 0.3f;
        if (rotation < -15.0f) rotation = -15.0f;
    }
    if (ngn->input->key_W->held) {
        rotation += 0.3f;
        if (rotation > 15.0f) rotation = 15.0f;
    }

    // Calcula el zoom escalando la textura
    render_texture->Scale(scale);
    float x = (SCR_WIDTH / 2) - (((float)(SCR_WIDTH / 2)) * scale);
    float y = (SCR_HEIGHT / 2) - (((float)(SCR_HEIGHT / 2)) * scale);
    render_texture->Position(x, y);
    render_texture->rotation = rotation;

}



/*** Render de los graficos ***/
void Demo::Render() {

    // Renderiza a una textura
    ngn->render->RenderToTexture(render_texture);
    ngn->render->TiledBg(bg);
    ngn->render->Sprite(man[0].sprite);
    ngn->render->Sprite(man[1].sprite);

    // Renderiza la textura en la pantalla
    ngn->render->RenderToScreen();
    ngn->render->Texture(render_texture);

}
