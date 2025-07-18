/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0-wip_0x03 ***
    Meotodos de entrada

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2025 by Cesar Rincon "NightFox"

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

/*** PREPROCESADOR ***/
#define __INPUT_DEBUG false



/*** Includes ***/
// C++
#include <cstdio>
#include <iostream>
#include <vector>
#include <cmath>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"

// Debug
#if __INPUT_DEBUG == true
    #include <iomanip>
#endif



/******************************************************************************

    Prototipo de datos para las teclas

******************************************************************************/

/*** Prototipo de tecla (Clase NGN_Key) ***/

/*** Constructor ***/
NGN_Key::NGN_Key() {

    // Inicializa la tecla
    Reset();

}



/*** Destructor ***/
NGN_Key::~NGN_Key() {
}



/*** Actualiza la tecla ***/
void NGN_Key::Update() {

    down = held & (!last);
    up = (!held) & last;

    last = held;

}



/*** Reinicia la tecla ***/
void NGN_Key::Reset() {

    up = false;
    down = false;
    held = false;
    last = false;

}



/******************************************************************************

    Metodos de entrada del teclado, mouse, game controller...

******************************************************************************/

/*** Metodos de entrada (Clase NGN_Input) ***/


/*** Puntero de la instancia a nullptr ***/
NGN_Input* NGN_Input::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
NGN_Input* NGN_Input::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Input();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Input::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Constructor ***/
NGN_Input::NGN_Input() {

    // Crea la lista de teclas
    AddKeys();

    // Inicializa el mouse
    MouseInit();

    // Inicializa los game controllers
    GameControllerInit();

}



/*** Destructor ***/
NGN_Input::~NGN_Input() {

    // Elimina las instancias de las teclas
    RemoveKeys();

    // Cierra los JOYS abiertos
    GameControllerClose();

}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Input::BootUp() {
}



/*** Actualiza el estado de los dispositivos de entrada ***/
void NGN_Input::Update() {

    // Teclado
    UpdateKeys();

    // Raton
    UpdateMouse();

    // Game Controllers
    UpdateGameController();

}



/*** Actualiza el estado de todas las teclas ***/
void NGN_Input::UpdateKeys() {

    uint32_t n = 0;

    // Lee el estado del teclado
    ReadKeyboard();

    // Actualiza el estado de las teclas
    key_ANY_KEY->held = false;
    for (n = 0; n < keylist.size(); n ++) {
        keylist[n]->Update();
        key_ANY_KEY->held |= keylist[n]->held;
    }
    key_ANY_KEY->Update();

}



/*** Crea la lista de teclas ***/
void NGN_Input::AddKeys() {

    keylist.clear();

    key_1 = new NGN_Key(); keylist.push_back(key_1);
    key_2 = new NGN_Key(); keylist.push_back(key_2);
    key_3 = new NGN_Key(); keylist.push_back(key_3);
    key_4 = new NGN_Key(); keylist.push_back(key_4);
    key_5 = new NGN_Key(); keylist.push_back(key_5);
    key_6 = new NGN_Key(); keylist.push_back(key_6);
    key_7 = new NGN_Key(); keylist.push_back(key_7);
    key_8 = new NGN_Key(); keylist.push_back(key_8);
    key_9 = new NGN_Key(); keylist.push_back(key_9);
    key_0 = new NGN_Key(); keylist.push_back(key_0);

    key_Q = new NGN_Key(); keylist.push_back(key_Q);
    key_W = new NGN_Key(); keylist.push_back(key_W);
    key_E = new NGN_Key(); keylist.push_back(key_E);
    key_R = new NGN_Key(); keylist.push_back(key_R);
    key_T = new NGN_Key(); keylist.push_back(key_T);
    key_Y = new NGN_Key(); keylist.push_back(key_Y);
    key_U = new NGN_Key(); keylist.push_back(key_U);
    key_I = new NGN_Key(); keylist.push_back(key_I);
    key_O = new NGN_Key(); keylist.push_back(key_O);
    key_P = new NGN_Key(); keylist.push_back(key_P);

    key_A = new NGN_Key(); keylist.push_back(key_A);
    key_S = new NGN_Key(); keylist.push_back(key_S);
    key_D = new NGN_Key(); keylist.push_back(key_D);
    key_F = new NGN_Key(); keylist.push_back(key_F);
    key_G = new NGN_Key(); keylist.push_back(key_G);
    key_H = new NGN_Key(); keylist.push_back(key_H);
    key_J = new NGN_Key(); keylist.push_back(key_J);
    key_K = new NGN_Key(); keylist.push_back(key_K);
    key_L = new NGN_Key(); keylist.push_back(key_L);

    key_Z = new NGN_Key(); keylist.push_back(key_Z);
    key_X = new NGN_Key(); keylist.push_back(key_X);
    key_C = new NGN_Key(); keylist.push_back(key_C);
    key_V = new NGN_Key(); keylist.push_back(key_V);
    key_B = new NGN_Key(); keylist.push_back(key_B);
    key_N = new NGN_Key(); keylist.push_back(key_N);
    key_M = new NGN_Key(); keylist.push_back(key_M);

    key_SPACE = new NGN_Key(); keylist.push_back(key_SPACE);
    key_ESC = new NGN_Key(); keylist.push_back(key_ESC);
    key_RETURN = new NGN_Key(); keylist.push_back(key_RETURN);
    key_TAB = new NGN_Key(); keylist.push_back(key_TAB);
    key_BACK_SPACE = new NGN_Key(); keylist.push_back(key_BACK_SPACE);

    key_ARROW_UP = new NGN_Key(); keylist.push_back(key_ARROW_UP);
    key_ARROW_DOWN = new NGN_Key(); keylist.push_back(key_ARROW_DOWN);
    key_ARROW_LEFT = new NGN_Key(); keylist.push_back(key_ARROW_LEFT);
    key_ARROW_RIGHT = new NGN_Key(); keylist.push_back(key_ARROW_RIGHT);

    key_LEFT_CONTROL = new NGN_Key(); keylist.push_back(key_LEFT_CONTROL);
    key_RIGHT_CONTROL = new NGN_Key(); keylist.push_back(key_RIGHT_CONTROL);
    key_LEFT_SHIFT = new NGN_Key(); keylist.push_back(key_LEFT_SHIFT);
    key_RIGHT_SHIFT = new NGN_Key(); keylist.push_back(key_RIGHT_SHIFT);
    key_LEFT_ALT = new NGN_Key(); keylist.push_back(key_LEFT_ALT);
    key_RIGHT_ALT = new NGN_Key(); keylist.push_back(key_RIGHT_ALT);

    key_F1 = new NGN_Key(); keylist.push_back(key_F1);
    key_F2 = new NGN_Key(); keylist.push_back(key_F2);
    key_F3 = new NGN_Key(); keylist.push_back(key_F3);
    key_F4 = new NGN_Key(); keylist.push_back(key_F4);
    key_F5 = new NGN_Key(); keylist.push_back(key_F5);
    key_F6 = new NGN_Key(); keylist.push_back(key_F6);
    key_F7 = new NGN_Key(); keylist.push_back(key_F7);
    key_F8 = new NGN_Key(); keylist.push_back(key_F8);
    key_F9 = new NGN_Key(); keylist.push_back(key_F9);
    key_F10 = new NGN_Key(); keylist.push_back(key_F10);
    key_F11 = new NGN_Key(); keylist.push_back(key_F11);
    key_F12 = new NGN_Key(); keylist.push_back(key_F12);

    key_INSERT = new NGN_Key(); keylist.push_back(key_INSERT);
    key_DELETE = new NGN_Key(); keylist.push_back(key_DELETE);
    key_HOME = new NGN_Key(); keylist.push_back(key_HOME);
    key_END = new NGN_Key(); keylist.push_back(key_END);
    key_PAGE_UP = new NGN_Key(); keylist.push_back(key_PAGE_UP);
    key_PAGE_DOWN = new NGN_Key(); keylist.push_back(key_PAGE_DOWN);

    key_GRAVE = new NGN_Key(); keylist.push_back(key_GRAVE);
    key_MINUS = new NGN_Key(); keylist.push_back(key_MINUS);
    key_EQUAL = new NGN_Key(); keylist.push_back(key_EQUAL);
    key_LEFT_BRACKET = new NGN_Key(); keylist.push_back(key_LEFT_BRACKET);
    key_RIGHT_BRACKET = new NGN_Key(); keylist.push_back(key_RIGHT_BRACKET);
    key_APOSTROPHE = new NGN_Key(); keylist.push_back(key_APOSTROPHE);
    key_BACK_SLASH = new NGN_Key(); keylist.push_back(key_BACK_SLASH);
    key_COMMA = new NGN_Key(); keylist.push_back(key_COMMA);
    key_PERIOD = new NGN_Key(); keylist.push_back(key_PERIOD);
    key_SLASH = new NGN_Key(); keylist.push_back(key_SLASH);

    key_PRINT_SCREEN = new NGN_Key(); keylist.push_back(key_PRINT_SCREEN);
    key_SCROLL_LOCK = new NGN_Key(); keylist.push_back(key_SCROLL_LOCK);
    key_PAUSE = new NGN_Key(); keylist.push_back(key_PAUSE);

    nkp_SLASH = new NGN_Key(); keylist.push_back(nkp_SLASH);
    nkp_ASTERISK = new NGN_Key(); keylist.push_back(nkp_ASTERISK);
    nkp_MINUS = new NGN_Key(); keylist.push_back(nkp_MINUS);
    nkp_7 = new NGN_Key(); keylist.push_back(nkp_7);
    nkp_8 = new NGN_Key(); keylist.push_back(nkp_8);
    nkp_9 = new NGN_Key(); keylist.push_back(nkp_9);
    nkp_4 = new NGN_Key(); keylist.push_back(nkp_4);
    nkp_5 = new NGN_Key(); keylist.push_back(nkp_5);
    nkp_6 = new NGN_Key(); keylist.push_back(nkp_6);
    nkp_1 = new NGN_Key(); keylist.push_back(nkp_1);
    nkp_2 = new NGN_Key(); keylist.push_back(nkp_2);
    nkp_3 = new NGN_Key(); keylist.push_back(nkp_3);
    nkp_0 = new NGN_Key(); keylist.push_back(nkp_0);
    nkp_PERIOD = new NGN_Key(); keylist.push_back(nkp_PERIOD);
    nkp_RETURN = new NGN_Key(); keylist.push_back(nkp_RETURN);
    nkp_PLUS = new NGN_Key(); keylist.push_back(nkp_PLUS);

    // Teclas especiales
    key_ANY_KEY = new NGN_Key();

}



// Realiza la lectura del teclado
void NGN_Input::ReadKeyboard() {

    const uint8_t* kb = SDL_GetKeyboardState(nullptr);

    key_1->held = kb[SDL_SCANCODE_1];
    key_2->held = kb[SDL_SCANCODE_2];
    key_3->held = kb[SDL_SCANCODE_3];
    key_4->held = kb[SDL_SCANCODE_4];
    key_5->held = kb[SDL_SCANCODE_5];
    key_6->held = kb[SDL_SCANCODE_6];
    key_7->held = kb[SDL_SCANCODE_7];
    key_8->held = kb[SDL_SCANCODE_8];
    key_9->held = kb[SDL_SCANCODE_9];
    key_0->held = kb[SDL_SCANCODE_0];

    key_Q->held = kb[SDL_SCANCODE_Q];
    key_W->held = kb[SDL_SCANCODE_W];
    key_E->held = kb[SDL_SCANCODE_E];
    key_R->held = kb[SDL_SCANCODE_R];
    key_T->held = kb[SDL_SCANCODE_T];
    key_Y->held = kb[SDL_SCANCODE_Y];
    key_U->held = kb[SDL_SCANCODE_U];
    key_I->held = kb[SDL_SCANCODE_I];
    key_O->held = kb[SDL_SCANCODE_O];
    key_P->held = kb[SDL_SCANCODE_P];

    key_A->held = kb[SDL_SCANCODE_A];
    key_S->held = kb[SDL_SCANCODE_S];
    key_D->held = kb[SDL_SCANCODE_D];
    key_F->held = kb[SDL_SCANCODE_F];
    key_G->held = kb[SDL_SCANCODE_G];
    key_H->held = kb[SDL_SCANCODE_H];
    key_J->held = kb[SDL_SCANCODE_J];
    key_K->held = kb[SDL_SCANCODE_K];
    key_L->held = kb[SDL_SCANCODE_L];

    key_Z->held = kb[SDL_SCANCODE_Z];
    key_X->held = kb[SDL_SCANCODE_X];
    key_C->held = kb[SDL_SCANCODE_C];
    key_V->held = kb[SDL_SCANCODE_V];
    key_B->held = kb[SDL_SCANCODE_B];
    key_N->held = kb[SDL_SCANCODE_N];
    key_M->held = kb[SDL_SCANCODE_M];

    key_SPACE->held = kb[SDL_SCANCODE_SPACE];
    key_ESC->held = kb[SDL_SCANCODE_ESCAPE];
    key_RETURN->held = kb[SDL_SCANCODE_RETURN];
    key_TAB->held = kb[SDL_SCANCODE_TAB];
    key_BACK_SPACE->held = kb[SDL_SCANCODE_BACKSPACE];

    key_ARROW_UP->held = kb[SDL_SCANCODE_UP];
    key_ARROW_DOWN->held = kb[SDL_SCANCODE_DOWN];
    key_ARROW_LEFT->held = kb[SDL_SCANCODE_LEFT];
    key_ARROW_RIGHT->held = kb[SDL_SCANCODE_RIGHT];

    key_LEFT_CONTROL->held = kb[SDL_SCANCODE_LCTRL];
    key_RIGHT_CONTROL->held = kb[SDL_SCANCODE_RCTRL];
    key_LEFT_SHIFT->held = kb[SDL_SCANCODE_LSHIFT];
    key_RIGHT_SHIFT->held = kb[SDL_SCANCODE_RSHIFT];
    key_LEFT_ALT->held = kb[SDL_SCANCODE_LALT];
    key_RIGHT_ALT->held = kb[SDL_SCANCODE_RALT];

    key_F1->held = kb[SDL_SCANCODE_F1];
    key_F2->held = kb[SDL_SCANCODE_F2];
    key_F3->held = kb[SDL_SCANCODE_F3];
    key_F4->held = kb[SDL_SCANCODE_F4];
    key_F5->held = kb[SDL_SCANCODE_F5];
    key_F6->held = kb[SDL_SCANCODE_F6];
    key_F7->held = kb[SDL_SCANCODE_F7];
    key_F8->held = kb[SDL_SCANCODE_F8];
    key_F9->held = kb[SDL_SCANCODE_F9];
    key_F10->held = kb[SDL_SCANCODE_F10];
    key_F11->held = kb[SDL_SCANCODE_F11];
    key_F12->held = kb[SDL_SCANCODE_F12];

    key_INSERT->held = kb[SDL_SCANCODE_INSERT];
    key_DELETE->held = kb[SDL_SCANCODE_DELETE];
    key_HOME->held = kb[SDL_SCANCODE_HOME];
    key_END->held = kb[SDL_SCANCODE_END];
    key_PAGE_UP->held = kb[SDL_SCANCODE_PAGEUP];
    key_PAGE_DOWN->held = kb[SDL_SCANCODE_PAGEDOWN];

    key_BACK_SLASH->held = kb[SDL_SCANCODE_GRAVE];
    key_MINUS->held = kb[SDL_SCANCODE_MINUS];
    key_EQUAL->held = kb[SDL_SCANCODE_EQUALS];
    key_LEFT_BRACKET->held = kb[SDL_SCANCODE_LEFTBRACKET];
    key_RIGHT_BRACKET->held = kb[SDL_SCANCODE_RIGHTBRACKET];
    key_APOSTROPHE->held = kb[SDL_SCANCODE_APOSTROPHE];
    key_BACK_SLASH->held = kb[SDL_SCANCODE_BACKSLASH];
    key_COMMA->held = kb[SDL_SCANCODE_COMMA];
    key_PERIOD->held = kb[SDL_SCANCODE_PERIOD];
    key_SLASH->held = kb[SDL_SCANCODE_SLASH];

    key_PRINT_SCREEN->held = kb[SDL_SCANCODE_PRINTSCREEN];
    key_SCROLL_LOCK->held = kb[SDL_SCANCODE_SCROLLLOCK];
    key_PAUSE->held = kb[SDL_SCANCODE_PAUSE];

    nkp_SLASH->held = kb[SDL_SCANCODE_KP_DIVIDE];
    nkp_ASTERISK->held = kb[SDL_SCANCODE_KP_MULTIPLY];
    nkp_MINUS->held = kb[SDL_SCANCODE_KP_MINUS];
    nkp_7->held = kb[SDL_SCANCODE_KP_7];
    nkp_8->held = kb[SDL_SCANCODE_KP_8];
    nkp_9->held = kb[SDL_SCANCODE_KP_9];
    nkp_4->held = kb[SDL_SCANCODE_KP_4];
    nkp_5->held = kb[SDL_SCANCODE_KP_5];
    nkp_6->held = kb[SDL_SCANCODE_KP_6];
    nkp_1->held = kb[SDL_SCANCODE_KP_1];
    nkp_2->held = kb[SDL_SCANCODE_KP_2];
    nkp_3->held = kb[SDL_SCANCODE_KP_3];
    nkp_0->held = kb[SDL_SCANCODE_KP_0];
    nkp_PERIOD->held = kb[SDL_SCANCODE_KP_PERIOD];
    nkp_RETURN->held = kb[SDL_SCANCODE_KP_ENTER];
    nkp_PLUS->held = kb[SDL_SCANCODE_KP_PLUS];

}



/*** Borra las instancias de todas las teclas ***/
void NGN_Input::RemoveKeys() {

    uint32_t n = 0;
    for (n = 0; n < keylist.size(); n ++) {
        delete keylist[n];
        keylist[n] = nullptr;
    }
    keylist.clear();

    // Teclas especiales
    delete key_ANY_KEY; key_ANY_KEY = nullptr;

}



/*** Inicializa el mouse ***/
void NGN_Input::MouseInit() {

    mouse.LB.Reset();
    mouse.MB.Reset();
    mouse.RB.Reset();
    mouse.x = 0;
    mouse.y = 0;
    mouse.wheel_x = 0;
    mouse.wheel_y = 0;
    mouse.raw_x = 0;
    mouse.raw_y = 0;

}



/*** Realiza la lectura del estado del mouse ***/
void NGN_Input::UpdateMouse() {

    // Variable de lectura
    uint32_t m = 0;

    // Lectura de la informacion del mouse segun SDL
    Vector2I32 pos = {0, 0};
    m = SDL_GetMouseState(&pos.x, &pos.y);
    // Escala las coordenadas al area visible
    Vector2 spos = {(float)pos.x, (float)pos.y};
    spos = ngn->graphics->ScaleAndFitCoordinates(spos);
    mouse.x = std::round(spos.x);
    mouse.y = std::round(spos.y);
    // Limita las coordenadas al area visible
    if (mouse.x < 0) mouse.x = 0;
    if (mouse.y < 0) mouse.y = 0;
    if (mouse.x >= ngn->graphics->native_w) mouse.x = (ngn->graphics->native_w - 1);
    if (mouse.y >= ngn->graphics->native_h) mouse.y = (ngn->graphics->native_h - 1);

    // Botones
    mouse.LB.held = m & (1 << 0);
    mouse.MB.held = m & (1 << 1);
    mouse.RB.held = m & (1 << 2);

    // Actualiza el estado de los botones
    mouse.LB.Update();
    mouse.MB.Update();
    mouse.RB.Update();

    /*
    Las lecturas de la rueda y el RAW se realizan en el SYSTEM
    */

}




/*** Inicializa los game controllers ***/
void NGN_Input::GameControllerInit() {

    // Numero inicial de game controllers
    controllers = 0;

    // Lista de controladores conectados varia
    controller_list.clear();

    // Datos por defecto de la lista
    for (uint8_t i = 0; i < GAME_CONTROLLERS; i ++) GameControllerReset(i);

}



/*** Cierra los controladores abiertos ***/
void NGN_Input::GameControllerClose() {

    // Cierra los controladores de la lista
    for (uint32_t i = 0; i < GAME_CONTROLLERS; i ++) {
        if (!controller[i].available) continue;
        if (!SDL_GameControllerGetAttached(controller[i].gamepad)) continue;
        // Si soporta rumble, cancela cualquier efecto en curso
        if (controller[i].rumble_available) SDL_GameControllerRumble(controller[i].gamepad, 0, 0, 0);
        // Cierra el controlador
        SDL_GameControllerClose(controller[i].gamepad);
        // Informa de la eliminacion
        #if defined (MODE_DEBUG)
            // Imprime la info del controlador eliminado
            std::string txt = "[NGN_Input info]\nThe <";
            txt += controller[i].name;
            txt += "> with ID <";
            txt += ngn->toolbox->Int2String(controller[i].device_id, 1, "0");
            txt += "> at slot [";
            txt += ngn->toolbox->Int2String(i, 1, "0");
            txt += "] has been successfully closed.";
            ngn->log->Message(txt);
        #endif
        // Reinicialo
        GameControllerReset(i);
    }

    // Limpia la lista de controladores
    controller_list.clear();

}



/*** Reinicio de los controladores ***/
void NGN_Input::GameControllerReset(uint8_t idx) {

    controller[idx].available = false;
    controller[idx].device_id = -1;
    controller[idx].name = "";
    for (uint32_t a = 0; a < GAME_CONTROLLER_AXIS; a ++) controller[idx].axis[a] = 0.0f;
    for (uint32_t b = 0; b < GAME_CONTROLLER_BUTTONS; b ++) controller[idx].button[b].Reset();
    controller[idx].pov = 0;
    controller[idx].pov_up.Reset();
    controller[idx].pov_down.Reset();
    controller[idx].pov_left.Reset();
    controller[idx].pov_right.Reset();
    controller[idx].dpad.up.Reset();
    controller[idx].dpad.down.Reset();
    controller[idx].dpad.left.Reset();
    controller[idx].dpad.right.Reset();
    controller[idx].any_button.Reset();
    controller[idx].any_axis.Reset();
    controller[idx].activity.Reset();
    controller[idx].rumble_available = false;


}



/*** Actualiza los game controllers ***/
void NGN_Input::UpdateGameController() {

    // Numero actual de game controllers
    int32_t gc = SDL_NumJoysticks();

    // Hot plug del game controller
    // Si ha cambiado el numero de controlladores conectados, reinicia la lista
    if (gc > controllers) {
        AddControllers(gc);
        controllers = gc;
    } else if (gc < controllers) {
        RemoveControllers();
        controllers = gc;
    }

    // Lectura de los botones de los game controllers
    for (int32_t i = 0; i < GAME_CONTROLLERS; i ++) {
        // Si el game controller no esta disponible, lectura del siguiente
        if (!controller[i].available) continue;
        // Estados "any" y actividad del game controller
        controller[i].any_button.held = false;
        controller[i].any_axis.held = false;
        controller[i].activity.held = false;
        // Actualiza los botones del game controller
        for (int32_t b = 0; b < GAME_CONTROLLER_BUTTONS; b ++) {
            controller[i].button[b].Update();
            controller[i].any_button.held |= controller[i].button[b].held;
        }
        // Actualiza el estado del D-PAD
        controller[i].dpad.up.Update();
        controller[i].dpad.down.Update();
        controller[i].dpad.left.Update();
        controller[i].dpad.right.Update();
        // Registra cambios en el D-PAD para el estado "any"
        controller[i].any_axis.held |= controller[i].dpad.up.held;
        controller[i].any_axis.held |= controller[i].dpad.down.held;
        controller[i].any_axis.held |= controller[i].dpad.left.held;
        controller[i].any_axis.held |= controller[i].dpad.right.held;
        // Actualiza el POV en base al D-PAD (legacy)
        controller[i].pov_up.held = controller[i].dpad.up.held;
        controller[i].pov_down.held = controller[i].dpad.down.held;
        controller[i].pov_left.held = controller[i].dpad.left.held;
        controller[i].pov_right.held = controller[i].dpad.right.held;
        // Actualiza el POV (legacy)
        controller[i].pov_up.Update();
        controller[i].pov_down.Update();
        controller[i].pov_left.Update();
        controller[i].pov_right.Update();
        // Mascara de bits para el POV (legacy)
        controller[i].pov = 0;
        if (controller[i].dpad.up.held) controller[i].pov |= 0x01;
        if (controller[i].dpad.right.held) controller[i].pov |= 0x02;
        if (controller[i].dpad.down.held) controller[i].pov |= 0x04;
        if (controller[i].dpad.left.held) controller[i].pov |= 0x08;
        // Registra los cambios de los axis para el estado "any"
        for (uint32_t a = 0; a < GAME_CONTROLLER_AXIS; a ++) {
            controller[i].any_axis.held |= (std::abs(controller[i].axis[a]) > XBOX_AXIS_DEADZONE) ? true:false;
        }
        // Actualiza los estados "any" y "activity" del controlador
        controller[i].activity.held |= controller[i].any_button.held;
        controller[i].activity.held |= controller[i].any_axis.held;
        controller[i].any_button.Update();
        controller[i].any_axis.Update();
        controller[i].activity.Update();
    }

}



/*** Añade los nuevos controladores a la lista ***/
void NGN_Input::AddControllers(int32_t gc) {

    // Variables
    SDL_GameController* gamepad = nullptr;     // Puntero a la instancia
    bool found = false;                     // Control de busqueda
    ControllerListData gamepad_data;        // Temporal para añadirlo a la lista
    std::string name = "";                  // Nombre del gamepad
    std::string serial_id = "";             // numero de serie
    int32_t device_id = -1;                 // ID de instancia del dispositivo

    // Recorre la lista de controladores conectados actualmente
    for (int32_t idx = 0; idx < gc; idx ++) {

        // Intenta abrir el controlador
        gamepad = SDL_GameControllerOpen(idx);

        // Si falla la apertura, intentalo con el siguiente
        if (!gamepad) continue;

        // Nombre e ID del Gamepad
        device_id = SDL_GameControllerGetPlayerIndex(gamepad);
        name = std::string(SDL_GameControllerName(gamepad));

        // Busca si ese GAMEPAD ya esta en la lista
        found = false;
        for (uint32_t i = 0; i < controller_list.size(); i ++) {
            // Si lo encuentras, indicalo y sal
            if ((controller_list[i].device_id == device_id) && (controller_list[i].name == name)) {
                found = true;
                break;
            }
        }
        // Si esta en la lista, analiza el siguiente ID conectado
        if (found) continue;

        // Si no se encuentra, añadelo a la lista en el primer slot disponible
        for (int32_t i = 0; i < GAME_CONTROLLERS; i ++) {

            // Si el slot en uso (controlador disponible, busca en el siguiente)
            if (controller[i].available) continue;

            // Guarda los parametros de este joystick
            controller[i].available = true;
            controller[i].gamepad = gamepad;
            controller[i].device_id = device_id;
            controller[i].name = name;
            controller[i].rumble_available = SDL_GameControllerHasRumble(gamepad);

            #if defined (MODE_DEBUG)
                // Imprime la info del controlador añadido
                std::string txt = "[NGN_Input info]\nThe <";
                txt += name;
                txt += "> with ID <";
                txt += ngn->toolbox->Int2String(device_id, 1, "0");
                txt += "> has been connected to slot [";
                txt += ngn->toolbox->Int2String(i, 1, "0");
                txt += "]. This device ";
                if (!controller[i].rumble_available) txt += "doesn't ";
                txt += "supports rumble.";
                ngn->log->Message(txt);
            #endif

            // Guarda este game pad en la lista de controladores disponibles
            gamepad_data.name = name;
            gamepad_data.device_id = device_id;
            gamepad_data.slot = i;
            controller_list.push_back(gamepad_data);

            // Sal del proceso despues de buscar un slot libre y asignarlo de ser posible
            break;

        }

    }

}



/*** Elimina los controladores desconectados de la lista ***/
void NGN_Input::RemoveControllers() {

    // Variables
    bool repeat = false;                    // Flag para repetir el proceso
    uint8_t slot = 0;                       // ID en la lista de joysticks

    // Elimina los gamepads no conectados
    do {
        repeat = false;
        for (uint32_t i = 0; i < controller_list.size(); i ++) {
            slot = controller_list[i].slot;
            // Si el Gamepad no esta disponible...
            if (!SDL_GameControllerGetAttached(controller[slot].gamepad)) {
                // Si soporta rumble, cancela cualquier efecto en curso
                if (controller[slot].rumble_available) SDL_GameControllerRumble(controller[slot].gamepad, 0, 0, 0);
                // Cierra el controlador
                SDL_GameControllerClose(controller[slot].gamepad);
                // Reinicialo
                GameControllerReset(slot);
                // Informa de la eliminacion
                #if defined (MODE_DEBUG)
                    // Imprime la info del controlador eliminado
                    std::string txt = "[NGN_Input info]\nThe <";
                    txt += controller_list[i].name;
                    txt += "> with ID <";
                    txt += ngn->toolbox->Int2String(controller_list[i].device_id, 1, "0");
                    txt += "> has been disconnected from slot [";
                    txt += ngn->toolbox->Int2String(slot, 1, "0");
                    txt += "].";
                    ngn->log->Message(txt);
                #endif
                // Y eliminalo de la lista de controladores conectados
                controller_list.erase(controller_list.begin() + i);
                // Marca para repetir
                repeat = true;
                // Aborta
                break;
            }
        }
    } while (repeat);

}



/*** Efecto simple de "rumble" en el controlador ***/
int32_t NGN_Input::ControllerRumble(uint32_t controller_id, float intensity, uint32_t duration) {

    // Fuera de rango
    if (controller_id >= GAME_CONTROLLERS) return -1;
    // Game controller disponible
    if (!controller[controller_id].available) return -1;
    // Rumble disponible
    if (!controller[controller_id].rumble_available) return -1;
    // Rango de fuerza incorrecto
    if ((intensity < 0.0f) || (intensity > 1.0f)) return -1;

    // Aplica el efecto rumble
    uint16_t i = (uint16_t)(32767.0f * intensity);
    return SDL_GameControllerRumble(controller[controller_id].gamepad, i, i, duration);

}
