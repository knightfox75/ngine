/******************************************************************************

    N'gine Lib for C++
    *** Version 1.15.0-RC1 ***
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

        // Lectura del evento del GamePad [SDL_CONTROLLERAXISMOTION]
        void GamePadAxisMotion();

        // Lectura del evento del GamePad [SDL_CONTROLLERBUTTONDOWN]
        void GamePadButtons();

        // Tiempo delta
        uint32_t _delta_time;

        // Estado del foco de la aplicacion
        bool _focus;

};



#endif // NGN_SYSTEM_H_INCLUDED
