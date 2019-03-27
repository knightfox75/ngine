/******************************************************************************

    N'gine Lib for C++
    *** Version 0.8.0-alpha-WIP2 ***
    Meotodos de entrada

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2019 by Cesar Rincon "NightFox"
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



#ifndef NGN_INPUT_H_INCLUDED
#define NGN_INPUT_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>
#include <string>


/*** Defines ***/
#define GAME_CONTROLLERS 8              // Numero maximo de game controllers
#define GAME_CONTROLLER_AXIS 8          // Numero maximo de axis por game controller
#define GAME_CONTROLLER_BUTTONS 20      // Numero maximo de botones por game controller



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

        // Contructor
        NGN_Input();

        // Destructor
        ~NGN_Input();



    private:

        // Define las propiedades del game controller
        struct controller_data {
            bool available;                                 // El controlador esta disponible?
            SDL_Joystick* joy;                              // Puntero a la informacion del joy
            int32_t id;                                     // ID de la instancia de este joy
            std::string name;                               // Nombre del JoyStick
            int32_t axis_number;                            // Numero de axis disponibles
            float axis[GAME_CONTROLLER_AXIS];               // Axis
            int32_t button_number;                          // Numero de botones disponibles
            NGN_Key button[GAME_CONTROLLER_BUTTONS];        // Botones
            bool pov_available;                             // POV (Cruceta digital) disponible?
            uint8_t pov;                                    // Resultado del POV (BITMASK de 4 bits)
            NGN_Key pov_up;                                 // POV como teclas cursor (Arriba)
            NGN_Key pov_down;                               // POV como teclas cursor (Abajo)
            NGN_Key pov_left;                               // POV como teclas cursor (Izquierda)
            NGN_Key pov_right;                              // POV como teclas cursor (Derecha)
        };

        // Lista de controladores disponibles
        struct controller_list_data {
            std::string name;   // ID del controlador
            int32_t slot;       // Slot de la lista asignado
        };
        std::vector<controller_list_data> controller_list;




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
        controller_data controller[GAME_CONTROLLERS];
        int32_t controllers;



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
