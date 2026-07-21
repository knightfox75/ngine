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



/*** Includes ***/

// C++
#include <cstdio>

// Libreria
#include "ngn.h"



/*** Puntero de la instancia a nullptr ***/
NGN_Gpio* NGN_Gpio::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
NGN_Gpio* NGN_Gpio::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Gpio();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Gpio::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Contructor ***/
NGN_Gpio::NGN_Gpio() {

    available = false;

    #if defined (TARGET_RASPBERRY)
        chip = nullptr;
        for (uint8_t k = 0; k < MAX_PINS; k ++) lines[k] = nullptr;
    #endif

    for (uint8_t k = 0; k < MAX_PINS; k ++) {
        pin_mode[k] = NGN_GpioMode::UNDEFINED;
    }

}



/*** Destructor ***/
NGN_Gpio::~NGN_Gpio() {

    #if defined (TARGET_RASPBERRY)

        // Libera todas las lineas reservadas
        ReleaseAllPins();

        // Cierra el chip GPIO si estaba abierto
        if (chip) {
            gpiod_chip_close(chip);
            chip = nullptr;
        }

    #endif

}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Gpio::BootUp() {

    #if defined (TARGET_RASPBERRY)

        // Intenta abrir el chip GPIO principal de la Raspberry Pi
        chip = gpiod_chip_open_by_name(chip_name);

        // Segun si se tiene exito o no...
        if (chip) {
            available = true;
        } else {
            available = false;
            ngn->log->Message("[NGN_Gpio] error: unable to open " + std::string(chip_name));
        }

    #else

        // Fuera de Raspberry Pi, este modulo nunca esta disponible
        available = false;

    #endif

}



/*** Configura el modo de un pin ***/
bool NGN_Gpio::PinMode(uint8_t pin, NGN_GpioMode mode) {

    #if defined (TARGET_RASPBERRY)

        // Si no esta disponible, sal
        if (!available) return false;

        // Si el pin no es valido, sal
        if (!ValidPin(pin)) return false;

        // Si el pin ya tenia una linea reservada, liberala antes de reconfigurarlo
        if (lines[pin]) {
            gpiod_line_release(lines[pin]);
            lines[pin] = nullptr;
        }

        // Intenta acceder al pin solicitado mediante un puntero temporal
        gpiod_line* line = gpiod_chip_get_line(chip, pin);
        if (!line) {
            ngn->log->Message("[NGN_Gpio] error: unable to get line for pin " + std::to_string(pin));
            return false;
        }


        // Guarda el resultado de la operacion
        int32_t result = -1;

        // Segun el modo solicitado...
        switch (mode) {

            case NGN_GpioMode::OUTPUT:
                result = gpiod_line_request_output(line, consumer_name, 0);
                break;

            case NGN_GpioMode::INPUT:
                result = gpiod_line_request_input(line, consumer_name);
                break;

            case NGN_GpioMode::INPUT_PULLUP:
                result = gpiod_line_request_input_flags(line, consumer_name, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
                break;

            case NGN_GpioMode::INPUT_PULLDOWN:
                result = gpiod_line_request_input_flags(line, consumer_name, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_DOWN);
                break;

            default:
                break;

        }

        // Si no se ha podido reconfigurar, indica el error
        if (result < 0) {
            ngn->log->Message("[NGN_Gpio] error: unable to request pin " + std::to_string(pin));
            pin_mode[pin] = NGN_GpioMode::UNDEFINED;
            return false;
        }

        // Guarda el modo
        pin_mode[pin] = mode;

        // Guarda la linea ya reservada, lista para leer/escribir
        lines[pin] = line;

        return true;

    #else

        // NOOP fuera de RPi
        (void)pin; (void)mode;
        return false;

    #endif

}



/*** Escribe un valor digital en un pin ya configurado como OUTPUT ***/
bool NGN_Gpio::DigitalWrite(uint8_t pin, bool value) {

    #if defined (TARGET_RASPBERRY)

        // Si no esta disponible, sal
        if (!available) return false;
        // Si el pin no es valido, sal
        if (!ValidPin(pin)) return false;

        // Si la linea no esta configurada, sal
        if (!lines[pin]) {
            ngn->log->Message("[NGN_Gpio] error: pin " + std::to_string(pin) + " has no PinMode set");
            return false;
        }

        // Si el modo es incorrecto, sal
        if (pin_mode[pin] != NGN_GpioMode::OUTPUT) return false;

        // Intenta establecer el valor y guarda el resultado de la operacion
        int32_t result = gpiod_line_set_value(lines[pin], (value ? 1 : 0));

        return (result == 0);

    #else

        // NOOP fuera de la RPi
        (void)pin; (void)value;
        return false;

    #endif

}



/*** Lee el valor digital de un pin ya configurado como INPUT ***/
bool NGN_Gpio::DigitalRead(uint8_t pin) {

    #if defined (TARGET_RASPBERRY)

        // Si no esta disponible, sal
        if (!available) return false;
        // Si el pin no es valido, sal
        if (!ValidPin(pin)) return false;

        // Si la linea no esta configurada, sal
        if (!lines[pin]) {
            ngn->log->Message("[NGN_Gpio] error: pin " + std::to_string(pin) + " has no PinMode set");
            return false;
        }

        // Si el modo es incorrecto, sal
        if (
            (pin_mode[pin] != NGN_GpioMode::INPUT)
            &&
            (pin_mode[pin] != NGN_GpioMode::INPUT_PULLUP)
            &&
            (pin_mode[pin] != NGN_GpioMode::INPUT_PULLDOWN)
        ) return false;

        // Intenta la lectura y guarda el resultado de la operacion
        int32_t result = gpiod_line_get_value(lines[pin]);

        return (result == 1);

    #else

        // NOOP fuera de la RPi
        (void)pin;
        return false;

    #endif

}



/*** Libera un pin previamente reservado ***/
void NGN_Gpio::ReleasePin(uint8_t pin) {

    #if defined (TARGET_RASPBERRY)

        // Si el pin no es valido, sal
        if (!ValidPin(pin)) return;

        // Si la linea del pin esta declarada, liberala
        if (lines[pin]) {
            gpiod_line_release(lines[pin]);
            lines[pin] = nullptr;
            pin_mode[pin] = NGN_GpioMode::UNDEFINED;
        }

    #else

        // NOOP fuera de la RPi
        (void)pin;

    #endif

}



/*** Libera todos los pines reservados hasta el momento ***/
void NGN_Gpio::ReleaseAllPins() {

    #if defined (TARGET_RASPBERRY)

        for (uint8_t k = 0; k < MAX_PINS; k ++) {
            if (lines[k]) {
                gpiod_line_release(lines[k]);
                lines[k] = nullptr;
                pin_mode[k] = NGN_GpioMode::UNDEFINED;
            }
        }

    #endif

}



/*** Indica si el modulo GPIO se ha inicializado correctamente ***/
bool NGN_Gpio::IsAvailable() {

    return available;

}



/*** Comprueba si el numero de pin esta dentro del rango valido ***/
bool NGN_Gpio::ValidPin(uint8_t pin) {

    return (pin < MAX_PINS);

}
