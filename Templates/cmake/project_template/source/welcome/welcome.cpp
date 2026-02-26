/******************************************************************************

    Project Template: Pantallas de bienvenida

    Proyecto iniciado el 13 de Noviembre del 2022
    (c) 2022-2023 by Cesar Rincon "NightFox"
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
// Includes de la libreria NGN
#include <ngn.h>
// Includes del proyecto
#include "welcome.h"
#include "../system/defines.h"
#include "../system/settings.h"



/*** Constructor de la clase ***/
Welcome::Welcome() {

    // Singletons
    input = Input::GetInstance();

    // Inicializa los punteros a objetos
    bg = nullptr;
    voice = nullptr;

}



/*** Destructor de la clase ***/
Welcome::~Welcome() {

    // Borra los objetos
    delete bg; bg = nullptr;

    // Anula los punteros
    voice = nullptr;

    // Borra los datos
    ngn->resources->Clear(RS_LOCAL);

    // Singletons
    input = nullptr;

}



/*** Al iniciarse el objeto ***/
bool Welcome::Start() {

    // Carga los archivos
    if (!Load()) return false;

    // Inicia la maquina de estados
    state = next_state = STATE.start_delay;

    // Variables de control
    timer = 0;

    // Inicializacion correcta
    return true;

}



/*** Ejecucion del objeto ***/
uint32_t Welcome::Run() {

    // Resultado
    uint32_t r = FLAG_ZERO;

    // Bucle de ejecucion
    while (!r) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos

        // Logica del programa
        Update();
        // Render de los graficos
        Render();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

        // Control del bucle
        if (ngn->system->quit) {
            r |= FLAG_RUN_QUIT;     // Si se pulsa la [X] de la ventana o ALT + F4, sal de programa
        } else if (state == STATE.exit) {
            r |= FLAG_RUN_TITLE;    // Si termina, ejecuta el modulo del titulo
        }

    }

    // Devuelve el resultado
    if (r & FLAG_RUN_QUIT) {                // Si el FLAG de salir del programa esta activo
        return WELCOME_FLAG.exit;
    } else if (r & FLAG_RUN_TITLE) {        // Si el FLAG de volver al titulo esta activo
        return WELCOME_FLAG.title;
    } else {                                // Si el FLAG no se reconoce, sal del programa
        return WELCOME_FLAG.exit;
    }

}



/*** Carga los archivos ***/
bool Welcome::Load() {

    // Carga los recursos
    if (!ngn->resources->Load(RS_LOCAL, "data/welcome/welcome.txt")) return false;

    // Carga correcta
    return true;

}



/*** Logica del modulo ***/
void Welcome::Update() {

    // Metodos de entrada
    input->Update();

    // Estados
    switch (state) {

        // Espera inicial
        case STATE.start_delay:
            timer ++;
            if (timer >= START_DELAY) next_state = STATE.ngine_create;
            break;

        // Crea el logo N'GINE
        case STATE.ngine_create:
            // Crea el fondo
            bg = new NGN_Texture(RS_LOCAL, "ngine_logo", 0, 0);
            // Transparente
            bg->alpha = 0;
            // Lanza el logo
            next_state = STATE.ngine_in;
            break;

        // N'GINE Fade in
        case STATE.ngine_in:
            bg->alpha += FADE_SPEED;
            if (bg->alpha >= 0xFF) {
                bg->alpha = 0xFF;
                // Voz
                voice = ngn->sound->PlaySfx(RS_LOCAL, "ngine_voice");
                // Salta de estado
                timer = 0;
                next_state = STATE.ngine_wait;
            }
            // Si se pulsa cualquier tecla, salta al fade-out
            if (input->accept.down || input->cancel.down) next_state = STATE.ngine_out;
            break;

        // N'GINE wait
        case STATE.ngine_wait:
            if (!ngn->sound->SfxIsAlive(voice)) {
                timer ++;
                if ((timer >= WAIT_NGINE) || input->accept.down || input->cancel.down) next_state = STATE.ngine_out;
            }
            break;

        // N'GINE Fade out
        case STATE.ngine_out:
            // Fade
            bg->alpha -= FADE_SPEED;
            if (bg->alpha <= 0) {
                bg->alpha = 0;
                voice = nullptr;
                timer = 0;
                // Destruye el logo
                next_state = STATE.exit;
            }
            break;


        // Error trap
        default:
            break;


    }

    // Actualiza el estado
    state = next_state;

}



/*** Render de los graficos ***/
void Welcome::Render() {

    // Render del fondo
    if (bg != nullptr) ngn->render->Texture(bg);

}
