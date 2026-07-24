/******************************************************************************

    N'gine Lib for C++
    *** Version 1.23.0-wip_0x03 ***
    GPIO - Acceso a los puertos de entrada/salida de Raspberry PI

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

// Solo en Raspberry Pi: Requiere "libgpiod" (v2)
// El resto de plataformas compilan la clase igualmente, pero como NO-OP
#if defined (TARGET_RASPBERRY)
    #include <gpiod.h>
#endif



/*** Define el Modo de un puerto, igual que en Arduino ***/
const struct {
    enum {
        input = 0x00,
        output,
        input_pullup,
        input_pulldown,
        undefined = 0xFF
    };
} NGN_GPIO_MODE;



/*** Declaracion de la clase de acceso al GPIO ***/
class NGN_Gpio {

    public:

        // Devuelve la instancia
        static NGN_Gpio* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();

        // Configura el modo de un puerto (INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN)
        bool PortMode(uint8_t port_id, uint8_t mode);

        // Escribe un valor digital (true = HIGH, false = LOW) en un puerto ya configurado como OUTPUT
        bool DigitalWrite(uint8_t port_id, bool value);

        // Lee el valor digital de un puerto ya configurado como INPUT
        bool DigitalRead(uint8_t port_id);

        // Libera un puerto previamente reservado, dejandolo disponible de nuevo
        void ReleasePort(uint8_t port_id);

        // Libera todos los puertos reservados hasta el momento
        void ReleaseAllPorts();

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

        // Numero maximo de puertos GPIO validos (numeracion BCM)
        static const uint8_t MAX_PORTS = 28;
        static const uint8_t RESERVED_PORTS_NUMBER = 2;
        const uint8_t RESERVED_PORTS_LIST[RESERVED_PORTS_NUMBER] = {0, 1};

        // Comprueba si el numero de puerto esta dentro del rango valido
        bool ValidPort(uint8_t port_id);

        // Flag de disponibilidad del modulo
        bool available;

        // Guarda el modo de cada puerto
        uint8_t port_mode[MAX_PORTS];

        #if defined (TARGET_RASPBERRY)

            // Ruta al dispositivo del chip GPIO principal de la Raspberry Pi (libgpiod v2)
            const char* chip_path = "/dev/gpiochip0";

            // Puntero al chip GPIO abierto
            gpiod_chip* chip;

            // Solicitudes de lineas GPIO reservadas, indexadas por numero de puerto (BCM)
            // nullptr = puerto sin PortMode() asignado todavia
            gpiod_line_request* requests[MAX_PORTS];

            // Consumidor que se registra al reservar cada linea (aparece en herramientas como gpioinfo)
            const char* consumer_name = "ngine";

        #endif

};



#endif // NGN_GPIO_H_INCLUDED