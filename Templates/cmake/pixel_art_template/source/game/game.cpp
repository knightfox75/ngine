/******************************************************************************

    Pixel Art Project Template: Nucleo del juego

    Proyecto iniciado el xx de xx del xx
    (c) xxxx by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.20.0+10th-anniversary o superior
	(c) 2016 - 2026 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
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
#include "game.h"
#include "../system/defines.h"
#include "../system/settings.h"



/*** Constructor de la clase ***/
Game::Game() {

    // Singletons (Adquisicion de instancias compartidas)
    shared_data = SharedData::GetInstance();
    soundtrack = Soundtrack::GetInstance();
    input = Input::GetInstance();

    /// DEBUG - Dummy texture ///
    dummy = nullptr;

}



/*** Destructor de la clase ***/
Game::~Game() {

    /// DEBUG - Dummy texture ///
    delete dummy; dummy = nullptr;

    // Borra los datos
    ngn->resources->Clear(RS_SHARED);

    // Singletons (anular referencias compartidas)
    input = nullptr;
    soundtrack = nullptr;
    shared_data = nullptr;

}



/*** Al iniciar el modulo ***/
bool Game::Start() {

    // Carga de archivos de recursos
    if (!Load()) return false;

    // Crea los elementos del juego
    Create();

    // Inicia la maquina de estados
    state = next_state = STATE.fade_in;

    // Procesos correctos
    return true;

}



/*** Ejecucion del modulo ***/
uint32_t Game::Run() {

    // Resultado
    uint32_t r = FLAG_ZERO;

    // Bucle de ejecucion
    while (!r) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();

        // Actualizacion de procesos (logica)
        r |= Update();
        // Se ha solicitado por parte del usuario salir del programa?
        r |= ApplicationQuit();

        // Render de los elementos graficos
        Render();
        // Actualiza la BGM
        soundtrack->Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

        // Procesos al finalizar el frame
        LateUpdate();

    }

    // Devuelve el resultado
    if (r & FLAG_RUN_QUIT) {                // Si el FLAG de salir del programa esta activo
        return GAME_FLAG.exit;
    } else if (r & FLAG_RUN_TITLE) {        // Si el FLAG de volver al titulo esta activo
        return GAME_FLAG.title;
    } else {                                // Si el FLAG no se reconoce, sal del programa
        return GAME_FLAG.exit;
    }

}



/*** Carga de los recursos ***/
bool Game::Load() {

    // Recursos compartidos del juego
    if (!ngn->resources->Load(RS_SHARED, "data/game/shared.txt")) return false;

    // Carga correcta
    return true;

}



/*** Crea los elementos del juego ***/
void Game::Create() {

    /// - Crea la textura de prueba - ///
    dummy = new NGN_Texture(RS_SHARED, "dummy", 0, 0);

    // Ajusta las mascaras
    shared_data->asset.black_texture->alpha = 0xFF;
    shared_data->asset.black_texture->visible = true;

}



/*** Logica del juego ***/
uint32_t Game::Update() {

    // Resultado
    uint32_t r = FLAG_ZERO;

    // Metodos de entrada
    input->Update();

    // Maquina de estados
    switch (state) {

        // Fade IN del menu de opciones
        case STATE.fade_in:
            shared_data->asset.black_texture->alpha -= FADE_SPEED;
            if (shared_data->asset.black_texture->alpha < 0x00) {
                shared_data->asset.black_texture->alpha = 0x00;
                shared_data->asset.black_texture->visible = false;
                next_state = STATE.run;
            }
            break;

        // Ejecucion del menu de opciones
        case STATE.run:
            // Si se pulsa ESC, vuelve al titulo
            if (ngn->input->key_ESC->down) {
                soundtrack->SetFadeOut(true);
                shared_data->asset.black_texture->visible = true;
                next_state = STATE.fade_out;;
            }
            break;

        // Fade OUT del menu de opciones
        case STATE.fade_out:
            shared_data->asset.black_texture->alpha += FADE_SPEED;
            if (shared_data->asset.black_texture->alpha > 0xFF) {
                shared_data->asset.black_texture->alpha = 0xFF;
                next_state = STATE.exit;
            }
            // Volumen de la musica
            if (soundtrack->GetFadeOut()) soundtrack->SetVolumeTo((((0xFF - shared_data->asset.black_texture->alpha) * 100) / 0xFF));
            break;

        // Devuelve el resultado de la ejecucion
        case STATE.exit:
            if (soundtrack->GetFadeOut()) soundtrack->Stop();
            r = FLAG_RUN_TITLE;
            break;

    }

    // Siguiente estado
    state = next_state;

    // Resultado
    return r;

}



/*** Render de los elementos graficos ***/
void Game::Render() {

    /// - Render de la textura de prueba - ///
    ngn->render->Texture(dummy);

    // Render de las mascaras
    ngn->render->Texture(shared_data->asset.black_texture);

}



/*** Actualizacion de los procesos post-render ***/
void Game::LateUpdate() {

}



/*** Salida del usuario ***/
uint32_t Game::ApplicationQuit() {

    // Valor a devolver
    uint32_t r = FLAG_ZERO;

    // Si se pulsa la X de la ventana o ALT+F4
    if (ngn->system->quit) r |= FLAG_RUN_QUIT;

    // Devuelve el resultado
    return r;

}
