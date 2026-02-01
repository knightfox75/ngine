/******************************************************************************

    Ejemplo del uso del sonido
    Archivo de Demo (Programa)

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
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros a los datos
    bg_data = nullptr;
    sfx_data = nullptr;

    // Inicializa los punteros a los objetos
    bg = nullptr;
    bgm = nullptr;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Deten la musica si es necesario
    ngn->sound->CloseMusic(bgm);
    bgm = nullptr;

    // Borra los objetos
    delete bg; bg = nullptr;

    // Borra los datos
    delete bg_data; bg_data = nullptr;
    delete sfx_data; sfx_data = nullptr;

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

    //  Carga los archivos necesarios
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



/*** Carga los archivos necesarios ***/
bool Demo::Load() {

    // Carga los graficos
    bg_data = ngn->load->Texture("data/sound_help.png");
    if (bg_data == nullptr) return false;

    // Carga los sfx
    sfx_data = ngn->load->AudioClip("data/voice.wav");
    if (sfx_data == nullptr) return false;

    // Carga el stream de musica
    bgm = ngn->sound->OpenMusic("data/pululate.ogg", false, 50);
    if (!bgm) return false;

    // Carga correcta
    return true;

}



/*** Crea el escenario ***/
void Demo::CreateStage() {

    // Crea el fondo
    bg = new NGN_Texture(bg_data, 0, 0);

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Musica y sfx
    Music();
    Sfx();

    // Render de los graficos
    Render();

}



/*** Render de los graficos ***/
void Demo::Render() {

    // Fondo
    ngn->render->Texture(bg);

}



/*** Control de la musica ***/
void Demo::Music() {

    // Variables
    float pitch = 0.0f;

    // Play de la musica
    if (ngn->input->key_Q->down) ngn->sound->PlayMusic(bgm, ngn->sound->MusicGetVolume(bgm), true);

    // Stop de la musica
    if (ngn->input->key_W->down) ngn->sound->StopMusic(bgm);

    // Pause de la musica
    if (ngn->input->key_E->down) ngn->sound->PauseMusic(bgm);

    // Resume de la musica
    if (ngn->input->key_R->down) ngn->sound->ResumeMusic(bgm);


    // Volumen de la musica
    if (ngn->input->key_A->held) {
        ngn->sound->MusicVolume(bgm, (ngn->sound->MusicGetVolume(bgm) - 1));
    }
    if (ngn->input->key_S->held) {
        ngn->sound->MusicVolume(bgm, (ngn->sound->MusicGetVolume(bgm) + 1));
    }

    // Pitch de la musica
    if (ngn->input->key_D->held) {
        pitch = ngn->sound->MusicGetPitch(bgm);
        pitch -= 0.01f;
        if (pitch < 0.5) pitch = 0.5f;
        ngn->sound->MusicPitch(bgm, pitch);
    }
    if (ngn->input->key_F->held) {
        pitch = ngn->sound->MusicGetPitch(bgm);
        pitch += 0.01f;
        if (pitch > 2.0f) pitch = 2.0f;
        ngn->sound->MusicPitch(bgm, pitch);
    }

    // Reset de la Musica
    if (ngn->input->key_1->down) {
        ngn->sound->MusicPitch(bgm, 1.0f);
        ngn->sound->MusicVolume(bgm, 50);
    }

}



/*** Efectos de sonido ***/
void Demo::Sfx() {

    // Canal izquierdo
    if (ngn->input->key_Z->down) ngn->sound->PlaySfx(sfx_data, 100, -100, false);
    // Ambos canales (centro)
    if (ngn->input->key_X->down) ngn->sound->PlaySfx(sfx_data, 100, 0, false);
    // Canal derecho
    if (ngn->input->key_C->down) ngn->sound->PlaySfx(sfx_data, 100, 100, false);

}
