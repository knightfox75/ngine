/******************************************************************************

    N'gine Lib for C++
    *** Version 1.21.0-wip0x02 ***
    Funciones de sistema

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016 - 2026 by Cesar Rincon "NightFox"

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



/*** Puntero de la instancia a nullptr ***/
NGN_System* NGN_System::instance = nullptr;



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
        instance = nullptr;
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
    srand(time(nullptr));

}



/*** Destructor ***/
NGN_System::~NGN_System() {
}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_System::BootUp() {
}



/*** Inicializa el engine SDL ***/
bool NGN_System::Init() {

    // Flags segun el target
    #if defined (TARGET_RG35XX)
        uint32_t sdl_flags = (SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
    #else
        uint32_t sdl_flags = (SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER);
    #endif

    // Intenta iniciar SDL con los flags seleccionados
    if (SDL_Init(sdl_flags) < 0) {
        std::string err_text = "[NGN_System error] SDL initialization failed: ";
        err_text += std::string(SDL_GetError());
        ngn->log->Message(err_text);
        return false;
    }

    // Returns result
    return true;

}



/*** Gestor de eventos de SDL ***/
void NGN_System::EventUpdate() {

    // Prepara la pantalla para renderizar el frame actual
    ngn->graphics->PrepareFrame();

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

            // Evento de los axis del GamePad
            case SDL_CONTROLLERAXISMOTION:
                GamePadAxisMotion();
                break;

            // Evento de los botones del GamePad
            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
                GamePadButtons();
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
                        ngn->graphics->force_redraw |= true;
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

    std::string version = "\n";

    version += "--------------------------------------------------\n";
    version += " Core Libs\n";
    version += "--------------------------------------------------\n";

    // N'gine
    version += "  N'gine version: ";
    version += ngn->toolbox->Int2String(NGN_VERSION_MAJOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(NGN_VERSION_MINOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(NGN_VERSION_PATCH, 1, "0");
    version += NGN_VERSION_MD_CHAR;
    version += NGN_VERSION_METADATA;
    version += "\n";

    // SDL2
    SDL_version sdl_version;
    SDL_GetVersion(&sdl_version);
    version += "  SDL2 version: ";
    version += ngn->toolbox->Int2String(sdl_version.major, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(sdl_version.minor, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(sdl_version.patch, 1, "0");
    version += "\n";

    // SDL_ttf
    const SDL_version* ttf = TTF_Linked_Version();
    version += "  SDL_ttf version: ";
    version += ngn->toolbox->Int2String(ttf->major, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(ttf->minor, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(ttf->patch, 1, "0");
    version += "\n";

    // SFML
    version += "  SFML version: ";
    version += ngn->toolbox->Int2String(SFML_VERSION_MAJOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(SFML_VERSION_MINOR, 1, "0");
    version += ".";
    version += ngn->toolbox->Int2String(SFML_VERSION_PATCH, 1, "0");
    version += "\n";

    // LodePNG
    std::string lode(LODEPNG_VERSION_STRING);
    version += "  LodePNG version: ";
    version += lode;
    version += "\n";

    // Renderer info
    version += "--------------------------------------------------\n";
    version += " Renderer Information\n";
    version += "--------------------------------------------------\n";
    SDL_RendererInfo renderer_info;
    SDL_GetRendererInfo(ngn->graphics->renderer, &renderer_info);
    version += "  Driver: ";
    version += std::string(renderer_info.name);
    version += "\n";
    version += "  Max texture size: ";
    version += std::to_string(renderer_info.max_texture_width) + "x" + std::to_string(renderer_info.max_texture_height);
    version += "\n";

    version += "--------------------------------------------------\n";

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
    ngn->graphics->force_redraw = false;

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



/*** Lectura del evento del GamePad [SDL_CONTROLLERAXISMOTION] ***/
void NGN_System::GamePadAxisMotion() {

    if (ngn->input->controllers <= 0) return;

    float axis = 0.0f;
    const float int2float = 32767.0f;

    const uint32_t axis_id[4] = {
        XBOX_STICK_L_AXIS_X,
        XBOX_STICK_L_AXIS_Y,
        XBOX_STICK_R_AXIS_X,
        XBOX_STICK_R_AXIS_Y
    };
    const SDL_GameControllerAxis sdl_id[4] = {
        SDL_CONTROLLER_AXIS_LEFTX,
        SDL_CONTROLLER_AXIS_LEFTY,
        SDL_CONTROLLER_AXIS_RIGHTX,
        SDL_CONTROLLER_AXIS_RIGHTY
    };

    for (int32_t i = 0; i < GAME_CONTROLLERS; i ++) {

        if (!ngn->input->controller[i].available) continue;

        // Sticks izquierdo y derecho
        for (int32_t n = 0; n < 4; n ++) {
            axis = ((float)SDL_GameControllerGetAxis(ngn->input->controller[i].gamepad, sdl_id[n]) / int2float);
            if (std::abs(axis) < XBOX_AXIS_IGNORE) {
                axis = 0.0f;
            } else if (axis < -1.0f) {
                axis = -1.0f;
            } else if (axis > 1.0f) {
                axis = 1.0f;
            }
            ngn->input->controller[i].axis[axis_id[n]] = axis;
        }

        // Gatillos
        axis = ((float)(SDL_GameControllerGetAxis(ngn->input->controller[i].gamepad, SDL_CONTROLLER_AXIS_TRIGGERLEFT) - SDL_GameControllerGetAxis(ngn->input->controller[i].gamepad, SDL_CONTROLLER_AXIS_TRIGGERRIGHT)) / int2float);
        if (std::abs(axis) < XBOX_AXIS_IGNORE) {
            axis = 0.0f;
        } else if (axis < -1.0f) {
            axis = -1.0f;
        } else if (axis > 1.0f) {
            axis = 1.0f;
        }
        ngn->input->controller[i].axis[XBOX_TRIGGER_AXIS] = axis;

    }

}



/*** Lectura del evento del GamePad [SDL_CONTROLLERBUTTONDOWN & SDL_CONTROLLERBUTTONUP] ***/
void NGN_System::GamePadButtons() {


    if (ngn->input->controllers <= 0) return;


    for (int32_t i = 0; i < GAME_CONTROLLERS; i ++) {

        if (!ngn->input->controller[i].available) continue;

        ngn->input->controller[i].button[XBOX_BUTTON_A].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_A) > 0) ? true:false;
        ngn->input->controller[i].button[XBOX_BUTTON_B].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_B) > 0) ? true:false;
        ngn->input->controller[i].button[XBOX_BUTTON_X].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_X) > 0) ? true:false;
        ngn->input->controller[i].button[XBOX_BUTTON_Y].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_Y) > 0) ? true:false;

        ngn->input->controller[i].button[XBOX_BUTTON_L].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_LEFTSHOULDER) > 0) ? true:false;
        ngn->input->controller[i].button[XBOX_BUTTON_R].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) > 0) ? true:false;

        ngn->input->controller[i].button[XBOX_BUTTON_BACK].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_BACK) > 0) ? true:false;
        ngn->input->controller[i].button[XBOX_BUTTON_START].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_START) > 0) ? true:false;

        ngn->input->controller[i].button[XBOX_BUTTON_STICK_L].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_LEFTSTICK) > 0) ? true:false;
        ngn->input->controller[i].button[XBOX_BUTTON_STICK_R].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_RIGHTSTICK) > 0) ? true:false;

        ngn->input->controller[i].button[XBOX_BUTTON_XBOX].held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_GUIDE) > 0) ? true:false;

        ngn->input->controller[i].dpad.up.held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_DPAD_UP) > 0) ? true:false;
        ngn->input->controller[i].dpad.down.held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_DPAD_DOWN) > 0) ? true:false;
        ngn->input->controller[i].dpad.left.held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_DPAD_LEFT) > 0) ? true:false;
        ngn->input->controller[i].dpad.right.held = (SDL_GameControllerGetButton(ngn->input->controller[i].gamepad, SDL_CONTROLLER_BUTTON_DPAD_RIGHT) > 0) ? true:false;

    }

}
