/******************************************************************************

    Project Template: Metodos de entrada

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
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "input.h"



/*** Puntero de la instancia a nullptr ***/
Input* Input::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
Input* Input::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new Input();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void Input::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Constructor de la clase ***/
Input::Input() {

    // Prepara la entrada
    Reset();

}



/*** Destructor de la clase ***/
Input::~Input() {
}



/*** Procesos iniciales despues de crear la instancia ***/
void Input::BootUp() {
}



/*** Actualiza la entrada ***/
void Input::Update() {

    // Uso general

    // UP
    up.held =
        ngn->input->key_ARROW_UP->held
        |
        ngn->input->key_W->held
        |
        ngn->input->controller[0].dpad.up.held
        |
        (ngn->input->controller[0].axis[XBOX_STICK_L_AXIS_Y] < -XBOX_AXIS_DEADZONE)
    ;
    UpdateKey(up);

    // DOWN
    down.held =
        ngn->input->key_ARROW_DOWN->held
        |
        ngn->input->key_S->held
        |
        ngn->input->controller[0].dpad.down.held
        |
        (ngn->input->controller[0].axis[XBOX_STICK_L_AXIS_Y] > XBOX_AXIS_DEADZONE)
    ;
    UpdateKey(down);

    // LEFT
    left.held =
        ngn->input->key_ARROW_LEFT->held
        |
        ngn->input->key_A->held
        |
        ngn->input->controller[0].dpad.left.held
        |
        (ngn->input->controller[0].axis[XBOX_STICK_L_AXIS_X] < -XBOX_AXIS_DEADZONE)
    ;
    UpdateKey(left);

    // RIGHT
    right.held =
        ngn->input->key_ARROW_RIGHT->held
        |
        ngn->input->key_D->held
        |
        ngn->input->controller[0].dpad.right.held
        |
        (ngn->input->controller[0].axis[XBOX_STICK_L_AXIS_X] > XBOX_AXIS_DEADZONE)
    ;
    UpdateKey(right);

    // ACCEPT
    accept.held =
        ngn->input->key_SPACE->held
        |
        ngn->input->key_RETURN->held
        |
        ngn->input->controller[0].button[XBOX_BUTTON_A].held
        |
        ngn->input->controller[0].button[XBOX_BUTTON_START].held
    ;
    UpdateKey(accept);

    // Cancel
    cancel.held =
        ngn->input->key_ESC->held
        |
        ngn->input->controller[0].button[XBOX_BUTTON_B].held
    ;
    UpdateKey(cancel);


    // Especificas del gameplay

}



/*** Reinicia TODAS las teclas ***/
void Input::Reset() {

    ResetKey(up);
    ResetKey(down);
    ResetKey(left);
    ResetKey(right);
    ResetKey(accept);
    ResetKey(cancel);

}



/*** Reinicia una tecla concreta ***/
void Input::ResetKey(KeyState &key) {

    key.up = false;
    key.down = false;
    key.held = false;
    key.last = false;

}



/*** Actualiza el estado de una tecla ***/
void Input::UpdateKey(KeyState &key) {

    key.down = !key.last & key.held;
    key.up = key.last & !key.held;
    key.last = key.held;

}
