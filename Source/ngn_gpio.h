/******************************************************************************

    N'gine Lib for C++
    *** Version 1.23.0-wip_0x02 ***
    GPIO - Acceso a los pines de entrada/salida de Raspberry PI

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



#ifndef NGN_GPIO_H_INCLUDED
#define NGN_GPIO_H_INCLUDED



/*** Includes ***/

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11

// Solo en Raspberry Pi: Requiere "libgpiod"
// El resto de plataformas compilan la clase igualmente, pero como no-op
#if defined (TARGET_RASPBERRY)
    #include <gpiod.h>
#endif



/*** Define el Modo de un pin, igual que en Arduino ***/
enum class NGN_GpioMode : uint8_t {
    INPUT = 0x00,
    OUTPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    UNDEFINED = 0xFF
};



/*** Declaracion de la clase de acceso al GPIO ***/
class NGN_Gpio {

    public:

        // Devuelve la instancia
        static NGN_Gpio* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();

        // Numero maximo de pines GPIO validos (numeracion BCM, Raspberry Pi 4)
        static const uint8_t MAX_PINS = 28;


        /*** Entrada / salida digital ***/

        // Configura el modo de un pin (INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN)
        bool PinMode(uint8_t pin, NGN_GpioMode mode);

        // Escribe un valor digital (true = HIGH, false = LOW) en un pin ya configurado como OUTPUT
        bool DigitalWrite(uint8_t pin, bool value);

        // Lee el valor digital de un pin ya configurado como INPUT
        bool DigitalRead(uint8_t pin);

        // Libera un pin previamente reservado, dejandolo disponible de nuevo
        void ReleasePin(uint8_t pin);

        // Libera todos los pines reservados hasta el momento
        void ReleaseAllPins();


        // Indica si el backend de GPIO se ha inicializado correctamente
        // (siempre sera "false" fuera de Raspberry Pi)
        bool IsAvailable();


    private:

        // Constructor de la clase
        NGN_Gpio();

        // Destructor de la clase
        ~NGN_Gpio();

        // Puntero de memoria a la instancia
        static NGN_Gpio* instance;

        // Comprueba si el numero de pin esta dentro del rango valido
        bool ValidPin(uint8_t pin);

        // Flag de disponibilidad del modulo
        bool available;

        // Guarda el modo de capa linea
        NGN_GpioMode pin_mode[MAX_PINS];

        #if defined (TARGET_RASPBERRY)

            // Nombre del chip GPIO principal de la Raspberry Pi (gpiochip0)
            const char* chip_name = "gpiochip0";

            // Puntero al chip GPIO abierto
            gpiod_chip* chip;

            // Lineas GPIO reservadas, indexadas directamente por numero de pin (BCM)
            // nullptr = pin sin PinMode() asignado todavia
            gpiod_line* lines[MAX_PINS];

            // Consumidor que se registra al reservar cada linea (aparece en herramientas como gpioinfo)
            const char* consumer_name = "ngine";

        #endif

};



#endif // NGN_GPIO_H_INCLUDED
