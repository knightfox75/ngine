/******************************************************************************

    N'gine Lib for C++
    *** Version 1.12.0-stable ***
    Funciones de sistema

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
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



#ifndef NGN_SYSTEM_H_INCLUDED
#define NGN_SYSTEM_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11




/*** Definicion de la clase ***/
class NGN_System {

   // Public
    public:

        // Devuelve la instancia
        static NGN_System* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();

        // Inicializa la libreria
        bool Init();

        // Gestor de eventos de SDL
        SDL_Event sdl_event;
        void EventUpdate();

        // Version de N'gine (a string)
        std::string GetVersion();

        // Flags de sistema
        bool quit;                      // Se solicita la salida de la aplicacion

        // Tiempo delta
        float delta_time;

        // Contador de FPS por consola de debug habilitado
        bool fps_counter;

        // Estado del foco de la aplicacion
        bool GetApplicationFocus();


    // Private
    private:

        // Contructor
        NGN_System();

        // Destructor
        ~NGN_System();

        // Puntero de memoria a la instancia
        static NGN_System* instance;

        // Resetea los flags y variables antes de leer el evento
        void ResetFlags();

        // Lectura del evento del mouse [SDL_MOUSEWHEEL]
        void MouseWheel();

        // Lectura del evento del mouse [SDL_MOUSEMOTION]
        void MouseMotion();

        // Lectura del evento del Joystick [SDL_JOYAXISMOTION]
        void JoyAxisMotion();

        // Tiempo delta
        uint32_t _delta_time;

        // Estado del foco de la aplicacion
        bool _focus;

};



#endif // NGN_SYSTEM_H_INCLUDED
