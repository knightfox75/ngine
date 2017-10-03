/******************************************************************************

    N'gine Lib for C++
    *** Version 0.1.0-alpha ***
    Meotodos de entrada

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2017 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
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

/*** PREPROCESADOR ***/
#define __INPUT_DEBUG false



/*** Includes ***/
// C++
#include <cstdio>
#include <iostream>
#include <vector>

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

    if (held) {
        if (last) {
            down = false;
        } else {
            down = true;
        }
    } else {
        down = false;
        if (last) {
            up = true;
        } else {
            up = false;
        }
    }

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

    key_ARROW_UP = new NGN_Key(); keylist.push_back(key_ARROW_UP);
    key_ARROW_DOWN = new NGN_Key(); keylist.push_back(key_ARROW_DOWN);
    key_ARROW_LEFT = new NGN_Key(); keylist.push_back(key_ARROW_LEFT);
    key_ARROW_RIGHT = new NGN_Key(); keylist.push_back(key_ARROW_RIGHT);

    // Teclas especiales
    key_ANY_KEY = new NGN_Key();

}



// Realiza la lectura del teclado
void NGN_Input::ReadKeyboard() {

    const Uint8* kb = SDL_GetKeyboardState(NULL);

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

    key_ARROW_UP->held = kb[SDL_SCANCODE_UP];
    key_ARROW_DOWN->held = kb[SDL_SCANCODE_DOWN];
    key_ARROW_LEFT->held = kb[SDL_SCANCODE_LEFT];
    key_ARROW_RIGHT->held = kb[SDL_SCANCODE_RIGHT];

}



/*** Borra las instancias de todas las teclas ***/
void NGN_Input::RemoveKeys() {

    uint32_t n = 0;
    for (n = 0; n < keylist.size(); n ++) {
        delete keylist[n];
        keylist[n] = NULL;
    }
    keylist.clear();

    // Teclas especiales
    delete key_ANY_KEY; key_ANY_KEY = NULL;

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
    m = SDL_GetMouseState(&mouse.x, &mouse.y);

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

    // Numer inicial de game controllers
    controllers = 0;

    // Inicializa la lista de controladores
    controller.clear();
    controller.reserve(GAME_CONTROLLERS);

    // Datos por defecto de la lista
    GameControllerReset();

}



/*** Reinicio de los controladores ***/
void NGN_Input::GameControllerReset() {

    for (uint32_t i = 0; i < controller.capacity(); i ++) {
        controller[i].available = false;
        controller[i].joy = NULL;
        controller[i].id = -1;
        controller[i].axis_number = 0;
        controller[i].button_number = 0;
        controller[i].pov_available = false;
        for (uint32_t a = 0; a < GAME_CONTROLLER_AXIS; a ++) controller[i].axis[a] = 0.0f;
        for (uint32_t b = 0; b < GAME_CONTROLLER_BUTTONS; b ++) controller[i].button[b].Reset();
        controller[i].pov = 0;
        controller[i].pov_up.held = false;
        controller[i].pov_down.held = false;
        controller[i].pov_left.held = false;
        controller[i].pov_right.held = false;
    }

}



/*** Actualiza los game controllers ***/
void NGN_Input::UpdateGameController() {

    // Numero actual de game controllers
    int32_t gc = SDL_NumJoysticks();

    // Hot plug del game controller
    // Si ha cambiado el numero de controlladores conectados, reinicia la lista
    if (gc != controllers) {
        // Cierra los controladores abiertos actualmente
        for (uint32_t i = 0; i < controller.capacity(); i ++) if (controller[i].joy != NULL) SDL_JoystickClose(controller[i].joy);
        // Reinicia la lista de controladores
        GameControllerReset();
        controllers = gc;
        // Si hay controladores disponibles, actualizalos
        if (gc > 0) {
            for (int32_t i = 0; i < gc; i ++) {
                // Controlador disponible?
                controller[i].joy = SDL_JoystickOpen(i);
                if (controller[i].joy != NULL) {
                    // Guarda los parametros de este joystick
                    controller[i].available = true;
                    controller[i].id = SDL_JoystickInstanceID(controller[i].joy);
                    controller[i].axis_number = SDL_JoystickNumAxes(controller[i].joy);
                    if (controller[i].axis_number > GAME_CONTROLLER_AXIS) controller[i].axis_number = GAME_CONTROLLER_AXIS;
                    controller[i].button_number = SDL_JoystickNumButtons(controller[i].joy);
                    if (controller[i].button_number > GAME_CONTROLLER_BUTTONS) controller[i].button_number = GAME_CONTROLLER_BUTTONS;
                    if (SDL_JoystickNumHats(controller[i].joy) > 0) controller[i].pov_available = true;
                }
            }
        }
    }

    // Debug de los AXIS disponibles
    #if __INPUT_DEBUG == true
    if (gc > 0) {
        for (int32_t i = 0; i < gc; i ++) {
            std::cout << "ID" << i << "|";
            for (int32_t a = 0; a < controller[i].axis_number; a ++) {
                std::cout << "A" << a << ":" << std::fixed << std::setw(5) << std::setprecision(2) << controller[i].axis[a] << "|";
            }
            //std::cout << std::endl;
        }
    }
    #endif


    // Lectura de los botones de los game controllers
    if (gc > 0) {
        for (int32_t i = 0; i < gc; i ++) {
            // Si el game controller esta disponible
            if (controller[i].available) {
                // Actualiza los botones de cada game controller
                for (int32_t b = 0; b < controller[i].button_number; b ++) {
                    controller[i].button[b].held = SDL_JoystickGetButton(controller[i].joy, b);
                    controller[i].button[b].Update();
                }
                // Actualiza el POV si este existe
                if (controller[i].pov_available) {
                    controller[i].pov = SDL_JoystickGetHat(controller[i].joy, 0);
                    controller[i].pov_up.held = (controller[i].pov & 0x01) ? true:false;
                    controller[i].pov_up.Update();
                    controller[i].pov_right.held = (controller[i].pov & 0x02) ? true:false;
                    controller[i].pov_down.Update();
                    controller[i].pov_down.held = (controller[i].pov & 0x04) ? true:false;
                    controller[i].pov_left.Update();
                    controller[i].pov_left.held = (controller[i].pov & 0x08) ? true:false;
                    controller[i].pov_right.Update();
                }
            }
        }
    }


    // Debug de los botones y crucetas disponibles
    #if __INPUT_DEBUG == true
    if (gc > 0) {
        for (int32_t i = 0; i < gc; i ++) {
            //std::cout << "ID_" << i << "|>>|";
            for (int32_t b = 0; b < controller[i].button_number; b ++) {
                std::cout << "B" << b << ":" << controller[i].button[b].held << "|";
            }
            if (controller[i].pov_available) {
                std::cout << "P:" << (int32_t)controller[i].pov;
            }
            std::cout << std::endl;
        }
    }
    #endif



}
