/******************************************************************************

    Ejemplo del uso del sonido
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 0.11.0-a o superior

    Requiere GCC 7.3.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.10) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG
    (c) 2005 - 2016 by Lode Vandevenne
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
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros a los datos
    bg_data = NULL;
    sfx_data = NULL;

    // Inicializa los punteros a los objetos
    bg = NULL;
    bgm = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Deten la musica si es necesario
    ngn->sound->CloseMusic(bgm);
    bgm = NULL;

    // Borra los objetos
    delete bg; bg = NULL;

    // Borra los datos
    delete bg_data; bg_data = NULL;
    delete sfx_data; sfx_data = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Selecciona el modo grafico
    #if defined (OS_WINDOWS)
        if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, SCR_MODE_WINDOWS, BILINEAR_FILTER, VSYNC)) return false;
    #elif defined (OS_LINUX)
        if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    #endif

    // Esconde el cursor del raton
    ngn->graphics->ShowMouse(SHOW_MOUSE);

    // Contador de FPS
    ngn->system->fps_counter = FPS_COUNTER;

    // Fuerza la actualizacion de la pantalla
    ngn->graphics->Update();
    #if defined (OS_LINUX)
        ngn->graphics->full_screen = SCR_MODE_LINUX;
    #endif

    // Muestra la version de la libreria en la consola
    #if defined (MODE_DEBUG)
        std::cout << ngn->system->GetVersion() << std::endl;
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
    if (bg_data == NULL) return false;

    // Carga los sfx
    sfx_data = ngn->load->AudioClip("data/voice.wav");
    if (sfx_data == NULL) return false;

    // Carga el stream de musica
    bgm = ngn->sound->OpenMusic("data/pululate.ogg", false, 50);

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
