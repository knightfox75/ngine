/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Sonido

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere SDL2 (2.0.5 or higher)
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.4.1 or higher)
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

/*** Defines ***/
#define WINDOW_TITLE "N'GINE Sound example."
#define SCR_WIDTH   1280
#define SCR_HEIGHT  720



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
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT)) return false;

    // Esconde el cursor del raton
    ngn->graphics->ShowMouse(false);

    // Debug?
    ngn->system->fps_counter = false;

    // Fuerza la actualizacion de la pantalla
    ngn->graphics->Update();

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
void Demo::Run() {

    // Control del loop
    bool loop = true;

    // Loop principal
    while (loop) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos

        // Actualizacion del programa
        Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

        // Control del bucle principal
        loop &= !ngn->system->quit;                                 // Si se pulsa la [X] de la ventana
        loop &= !ngn->input->key_ESC->down;                         // O se pulsa la tecla [ESC] sal del bucle de ejecucion

    }

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
