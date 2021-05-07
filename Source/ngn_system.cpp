/******************************************************************************

    N'gine Lib for C++
    *** Version 1.5.0-wip4 ***
    Funciones de sistema

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


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
// C++
#include <cstdio>
#include <iostream>
#include <ctime>

// SDL
#include <SDL.h>
#include <SDL_ttf.h>

// Libreria
#include "ngn.h"



/*** Contructor ***/
NGN_System::NGN_System() {

    // Inicializa los flags por defecto
    quit = false;
    fps_counter = false;

    // Tiempo delta
    _delta_time = SDL_GetTicks();
    delta_time = 0.0f;

    // Inicia el seed del random
    srand(time(NULL));

}



/*** Destructor ***/
NGN_System::~NGN_System() {
}



/*** Inicializa el engine SDL ***/
bool NGN_System::Init() {

    // Intenta iniciar SDL con las opciones por defecto
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) < 0) {
        ngn->log->Message("[NGN_System error] SDL initialization failed.");
        return false;
    }

    // Returns result
    return true;

}



/*** Gestor de eventos de SDL ***/
void NGN_System::EventUpdate() {

    // Tiempo delta
    delta_time = (((float)(SDL_GetTicks() - _delta_time)) / 1000.0f);
    _delta_time = SDL_GetTicks();

    // Resetea todos los flags de evento
    ResetFlags();

    // Lee todos los eventos
    while (SDL_PollEvent(&sdl_event) != 0) {

        // Segun el tipo de evento
        switch (sdl_event.type) {

            // Quit
            case SDL_QUIT:
                quit = true;
                break;

            // Evento de la rueda del mouse
            case SDL_MOUSEWHEEL:
                MouseWheel();
                break;

            // Evento de movimiento del mouse
            case SDL_MOUSEMOTION:
                MouseMotion();
                break;

            // Evento del movimiento de los axis del joystick
            case SDL_JOYAXISMOTION:
                JoyAxisMotion();
                break;

            default:
                break;

        }

    }

    // Actualiza el estado de los dispositvos de entrada
    ngn->input->Update();

}



/*** Devuelve un string con la version actual de N'gine ***/
std::string NGN_System::GetVersion() {

    std::string version = "N'gine version ";
    version += ngn->toolbox->Int2String(NGN_VERSION_MAJOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(NGN_VERSION_MINOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(NGN_VERSION_PATCH, 1, "0");
    version += "-";
    version += NGN_VERSION_METADATA;

    return version;

}



/*** Resetea los flags y variables antes de leer el evento ***/
void NGN_System::ResetFlags() {

    // Quit
    quit = false;

    // Mouse wheel
    ngn->input->mouse.wheel_x = 0;
    ngn->input->mouse.wheel_y = 0;

    // Mouse RAW
    ngn->input->mouse.raw_x = 0;
    ngn->input->mouse.raw_y = 0;

}



/*** Lectura del evento del mouse [SDL_MOUSEWHEEL] ***/
void NGN_System::MouseWheel() {

    ngn->input->mouse.wheel_x = sdl_event.wheel.x;
    ngn->input->mouse.wheel_y = sdl_event.wheel.y;

}



/*** Lectura del evento del mouse [SDL_MOUSEMOTION] ***/
void NGN_System::MouseMotion() {

    ngn->input->mouse.raw_x = sdl_event.motion.xrel;
    ngn->input->mouse.raw_y = sdl_event.motion.yrel;

}



/*** Lectura del evento del Joystick [SDL_JOYAXISMOTION] ***/
void NGN_System::JoyAxisMotion() {

    if (ngn->input->controllers <= 0) return;

    for (int32_t i = 0; i < GAME_CONTROLLERS; i ++) {
        if (ngn->input->controller[i].available) {
            if (sdl_event.jaxis.which == ngn->input->controller[i].id) {
                if (sdl_event.jaxis.axis < ngn->input->controller[i].axis_number) {
                    ngn->input->controller[i].axis[sdl_event.jaxis.axis] = (sdl_event.jaxis.value < 0) ? ((float)sdl_event.jaxis.value / 32768.0f):((float)sdl_event.jaxis.value / 32767.0f);
                }
            }
        }
    }

}
