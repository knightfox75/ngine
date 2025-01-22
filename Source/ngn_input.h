/******************************************************************************

    N'gine Lib for C++
    *** Version 1.18.1-stable ***
    Meotodos de entrada

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2024 by Cesar Rincon "NightFox"

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



#ifndef NGN_INPUT_H_INCLUDED
#define NGN_INPUT_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>
#include <string>



/******************************************************************************

    Definicion de las constantes para referirse a las
    propiedades de los controladores

******************************************************************************/


/*** Define las propiedades de los controladores ***/
static const int32_t GAME_CONTROLLERS = 8;              // Numero maximo de game controllers
static const int32_t GAME_CONTROLLER_AXIS = 5;          // Numero maximo de axis por game controller
static const int32_t GAME_CONTROLLER_BUTTONS = 11;      // Numero maximo de botones por game controller

/*** Definiciones por defecto de X-INPUT (XBOX Controller) ***/
static const uint32_t XBOX_BUTTON_A = 0;                // Botones del PAD de XBOX [11]
static const uint32_t XBOX_BUTTON_B = 1;
static const uint32_t XBOX_BUTTON_X = 2;
static const uint32_t XBOX_BUTTON_Y = 3;
static const uint32_t XBOX_BUTTON_L = 4;
static const uint32_t XBOX_BUTTON_R = 5;
static const uint32_t XBOX_BUTTON_BACK = 6;
static const uint32_t XBOX_BUTTON_START = 7;
static const uint32_t XBOX_BUTTON_STICK_L = 8;
static const uint32_t XBOX_BUTTON_STICK_R = 9;
static const uint32_t XBOX_BUTTON_XBOX = 10;
static const uint32_t XBOX_STICK_L_AXIS_X = 0;          // Axis analogicos [5]
static const uint32_t XBOX_STICK_L_AXIS_Y = 1;
static const uint32_t XBOX_STICK_R_AXIS_X = 2;
static const uint32_t XBOX_STICK_R_AXIS_Y = 3;
static const uint32_t XBOX_TRIGGER_AXIS = 4;            // Gatillos analogicos
static const float XBOX_AXIS_DEADZONE = 0.35f;          // Zona muerta de los axis
static const float XBOX_AXIS_IGNORE = 0.01f;            // Zona donde se ignora la entrada



/******************************************************************************

    Prototipo de datos para las teclas

******************************************************************************/

/*** Prototipo de tecla (Clase NGN_Key) ***/
class NGN_Key {

    // Public
    public:

        // Contructor
        NGN_Key();

        // Destructor
        ~NGN_Key();

        // Estados de la tecla
        bool down;
        bool held;
        bool up;

        // Actualiza la tecla
        void Update();

        // Reinicia la tecla
        void Reset();


    // Private
    private:

        bool last;

};



/******************************************************************************

    Metodos de entrada del teclado, mouse, game controller...

******************************************************************************/

/*** Metodos de entrada (Clase NGN_Input) ***/
class NGN_Input {

    public:

        // Devuelve la instancia
        static NGN_Input* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


    private:

        // Contructor
        NGN_Input();

        // Destructor
        ~NGN_Input();

        // Puntero de memoria a la instancia
        static NGN_Input* instance;


        // Define las propiedades del game controller
        struct ControllerData {
            bool available;                                 // El controlador esta disponible?
            SDL_GameController* gamepad;                    // Puntero a la informacion del game pad
            int32_t device_id;                              // ID de instancia
            std::string name;                               // Nombre del game pad
            float axis[GAME_CONTROLLER_AXIS];               // Axis
            NGN_Key button[GAME_CONTROLLER_BUTTONS];        // Botones
            uint8_t pov;                                    // Resultado del POV (BITMASK de 4 bits)
            NGN_Key pov_up;                                 // POV como teclas cursor (Arriba)
            NGN_Key pov_down;                               // POV como teclas cursor (Abajo)
            NGN_Key pov_left;                               // POV como teclas cursor (Izquierda)
            NGN_Key pov_right;                              // POV como teclas cursor (Derecha)
            struct {                                        // D-PAD (Conocido como POV en los joysticks)
                NGN_Key up;
                NGN_Key down;
                NGN_Key left;
                NGN_Key right;
            } dpad;
            NGN_Key any_button;                             // Cualquier boton
            NGN_Key any_axis;                               // Cualquier axis (analogicos, gatillos o D-PAD)
            NGN_Key activity;                               // Actividad del mando (cualquier boton o axis)
            bool rumble_available;                          // Dispone de efecto "rumble"
        };

        // Lista de controladores disponibles
        struct ControllerListData {
            std::string name;           // Nombre del controlador
            int32_t device_id;          // ID unica de hardware
            int32_t slot;               // Slot de la lista asignado
        };
        std::vector<ControllerListData> controller_list;




    public:

        // Define la lista de teclas
        NGN_Key *key_1, *key_2, *key_3, *key_4, *key_5, *key_6, *key_7, *key_8, *key_9, *key_0;
        NGN_Key *key_Q, *key_W, *key_E, *key_R, *key_T, *key_Y, *key_U, *key_I, *key_O, *key_P;
        NGN_Key *key_A, *key_S, *key_D, *key_F, *key_G, *key_H, *key_J, *key_K, *key_L;
        NGN_Key *key_Z, *key_X, *key_C, *key_V, *key_B, *key_N, *key_M;
        NGN_Key *key_SPACE, *key_ESC, *key_RETURN, *key_TAB, *key_BACK_SPACE;
        NGN_Key *key_ARROW_UP, *key_ARROW_DOWN, *key_ARROW_LEFT, *key_ARROW_RIGHT;
        NGN_Key *key_LEFT_CONTROL, *key_RIGHT_CONTROL, *key_LEFT_SHIFT, *key_RIGHT_SHIFT, *key_LEFT_ALT, *key_RIGHT_ALT;
        NGN_Key *key_F1, *key_F2, *key_F3, *key_F4, *key_F5, *key_F6, *key_F7, *key_F8, *key_F9, *key_F10, *key_F11, *key_F12;
        NGN_Key *key_INSERT, *key_DELETE, *key_HOME, *key_END, *key_PAGE_UP, *key_PAGE_DOWN;

        // Teclas de simbolos
        NGN_Key *key_GRAVE, *key_MINUS, *key_EQUAL;
        NGN_Key *key_LEFT_BRACKET, *key_RIGHT_BRACKET;
        NGN_Key *key_SEMICLON, *key_APOSTROPHE, *key_BACK_SLASH;
        NGN_Key *key_COMMA, *key_PERIOD, *key_SLASH;

        // Teclas especiales
        NGN_Key *key_PRINT_SCREEN, *key_SCROLL_LOCK, *key_PAUSE;

        // Bloque numerico
        NGN_Key *nkp_SLASH, *nkp_ASTERISK, *nkp_MINUS;
        NGN_Key *nkp_7, *nkp_8, *nkp_9;
        NGN_Key *nkp_4, *nkp_5, *nkp_6;
        NGN_Key *nkp_1, *nkp_2, *nkp_3;
        NGN_Key *nkp_0, *nkp_PERIOD, *nkp_RETURN, *nkp_PLUS;

        // Teclas adicionales
        NGN_Key* key_ANY_KEY;


        // Define las propiedades del raton
        struct {
            int32_t x;
            int32_t y;
            NGN_Key LB;
            NGN_Key MB;
            NGN_Key RB;
            int32_t wheel_x;
            int32_t wheel_y;
            int32_t raw_x;
            int32_t raw_y;
        } mouse;


        // Lista de game controllers
        ControllerData controller[GAME_CONTROLLERS];
        int32_t controllers;

        // Efecto simple de "rumble" para el controlador
        int32_t ControllerRumble(uint32_t controller_id, float intensity, uint32_t duration);


        // Actualiza el estado de los dispositivos de entrada
        void Update();



    private:

        // Crea la lista de teclas
        std::vector<NGN_Key*> keylist;

        // Crea la lista de teclas
        void AddKeys();

        // Realiza la lectura del teclado
        void ReadKeyboard();

        // Actualiza el estado de todas las teclas
        void UpdateKeys();

        // Borra las instancias de todas las teclas
        void RemoveKeys();



        // Inicializa el mouse
        void MouseInit();

        // Realiza una lectura del estado del mouse
        void UpdateMouse();



        // Inicializa los game controllers
        void GameControllerInit();

        // Cierra todos los controladores
        void GameControllerClose();

        // Reinicia la lista de controladores
        void GameControllerReset(uint8_t idx);

        // Actualiza la lista de game controllers
        void UpdateGameController();

        // Añade controladores a la lista
        void AddControllers(int32_t gc);

        // Quita controladores de la lista
        void RemoveControllers();

};







#endif // NGN_INPUT_H_INCLUDED
