/******************************************************************************

    N'gine Lib for C++
    *** Version 1.14.0-stable ***
    Funciones de sistema

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2023 by Cesar Rincon "NightFox"

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal	in the Software without restriction,
	including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be
	included in all	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER	LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************************/



/*** Includes ***/
// C++
#include <cstdio>
#include <iostream>
#include <ctime>

// SDL
#include <SDL.h>
#include <SDL_ttf.h>

// SFML
#include <SFML/Config.hpp>

// LodePNG
#include "lodepng/lodepng.h"

// Libreria
#include "ngn.h"



/*** Puntero de la instancia a NULL ***/
NGN_System* NGN_System::instance = NULL;



/*** Metodo para crear/obtener la instancia ***/
NGN_System* NGN_System::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_System();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_System::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = NULL;
    }

}



/*** Contructor ***/
NGN_System::NGN_System() {

    // Inicializa los flags por defecto
    quit = false;
    fps_counter = false;
    _focus = true;

    // Tiempo delta
    _delta_time = SDL_GetTicks();
    delta_time = 0.0f;

    // Inicia el seed del random
    srand(time(NULL));

}



/*** Destructor ***/
NGN_System::~NGN_System() {
}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_System::BootUp() {
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

            // Evento
            case SDL_WINDOWEVENT:
                switch (sdl_event.window.event) {
                    case SDL_WINDOWEVENT_SHOWN:
                    case SDL_WINDOWEVENT_EXPOSED:
                    case SDL_WINDOWEVENT_MOVED:
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    case SDL_WINDOWEVENT_MAXIMIZED:
                    case SDL_WINDOWEVENT_RESTORED:
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        ngn->graphics->force_redaw |= true;
                        break;
                }
                switch (sdl_event.window.event) {
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                        _focus = true;
                        break;
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                        _focus = false;
                        break;
                }
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

    std::string version = "";

    version += "----------------------------------------\n";

    // N'gine
    version += "N'gine version ";
    version += ngn->toolbox->Int2String(NGN_VERSION_MAJOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(NGN_VERSION_MINOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(NGN_VERSION_PATCH, 1, "0");
    version += "-";
    version += NGN_VERSION_METADATA;
    version += "\n";

    // SDL2
    SDL_version sdl_version;
    SDL_GetVersion(&sdl_version);
    version += "SDL2 version ";
    version += ngn->toolbox->Int2String(sdl_version.major, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(sdl_version.minor, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(sdl_version.patch, 1, "0");
    version += "\n";

    // SDL_ttf
    const SDL_version* ttf = TTF_Linked_Version();
    version += "SDL_ttf version ";
    version += ngn->toolbox->Int2String(ttf->major, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(ttf->minor, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(ttf->patch, 1, "0");
    version += "\n";

    // SFML
    version += "SFML version ";
    version += ngn->toolbox->Int2String(SFML_VERSION_MAJOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(SFML_VERSION_MINOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(SFML_VERSION_PATCH, 1, "0");
    version += "\n";

    // LodePNG
    std::string lode(LODEPNG_VERSION_STRING);
    version += "LodePNG version ";
    version += lode;
    version += "\n";

    version += "----------------------------------------";

    return version;

}



/*** Devuelve el estado del foco actual de la aplicacion ***/
bool NGN_System::GetApplicationFocus() {

    return _focus;

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

    // Redibujado de la pantalla
    ngn->graphics->force_redaw = false;

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
