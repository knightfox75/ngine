/******************************************************************************

    N'gine Lib for C++
    *** Version 1.23.0+stable ***
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



/*** Constructor ***/
NGN_Gpio::NGN_Gpio() {

    available = false;

    #if defined (TARGET_RASPBERRY)
        chip = nullptr;
        for (uint8_t k = 0; k < MAX_PORTS; k ++) requests[k] = nullptr;
    #endif

    for (uint8_t k = 0; k < MAX_PORTS; k ++) {
        port_mode[k] = NGN_GPIO_MODE.undefined;
    }

}



/*** Destructor ***/
NGN_Gpio::~NGN_Gpio() {

    #if defined (TARGET_RASPBERRY)

        // Libera todas las lineas reservadas
        ReleaseAllPorts();

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

        // Si el chip ya estaba abierto, cierralo primero
        if (chip) {
            gpiod_chip_close(chip);
            chip = nullptr;
        }

        // Intenta abrir el dispositivo del chip GPIO principal (libgpiod v2)
        chip = gpiod_chip_open(chip_path);

        // Segun si se tiene exito o no...
        if (chip) {
            available = true;
        } else {
            available = false;
            ngn->log->Message("[NGN_Gpio] error: unable to open " + std::string(chip_path));
        }

    #else

        // Fuera de Raspberry Pi, este modulo nunca esta disponible
        available = false;

    #endif

}



/*** Configura el modo de un puerto ***/
bool NGN_Gpio::SetPortMode(uint8_t port_id, uint8_t mode) {

    #if defined (TARGET_RASPBERRY)

        // Si no esta disponible, sal
        if (!available) return false;

        // Si el puerto no es valido, sal
        if (!ValidPort(port_id)) return false;

        // Si el puerto ya tenia una solicitud reservada, liberala antes de reconfigurarlo
        if (requests[port_id]) {
            gpiod_line_request_release(requests[port_id]);
            requests[port_id] = nullptr;
        }

        // Crea los ajustes de linea
        gpiod_line_settings* settings = gpiod_line_settings_new();
        if (!settings) {
            ngn->log->Message("[NGN_Gpio] error: unable to allocate line settings");
            return false;
        }

        // Configura la direccion y el bias segun el modo solicitado
        switch (mode) {

            case NGN_GPIO_MODE.output:
                gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
                gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);
                break;

            case NGN_GPIO_MODE.input:
                gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
                gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_DISABLED);
                break;

            case NGN_GPIO_MODE.input_pullup:
                gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
                gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP);
                break;

            case NGN_GPIO_MODE.input_pulldown:
                gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
                gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_DOWN);
                break;

            default:
                gpiod_line_settings_free(settings);
                return false;

        }

        // Crea la configuracion de linea y añade los ajustes para el pin solicitado
        gpiod_line_config* line_cfg = gpiod_line_config_new();
        if (!line_cfg) {
            gpiod_line_settings_free(settings);
            ngn->log->Message("[NGN_Gpio] error: unable to allocate line config");
            return false;
        }

        uint32_t offset = port_id;
        if (gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings) != 0) {
            gpiod_line_config_free(line_cfg);
            gpiod_line_settings_free(settings);
            ngn->log->Message("[NGN_Gpio] error: unable to add line settings for port_id " + std::to_string(port_id));
            return false;
        }

        // Crea la configuracion de la solicitud y establece el nombre del consumidor
        gpiod_request_config* req_cfg = gpiod_request_config_new();
        if (!req_cfg) {
            gpiod_line_config_free(line_cfg);
            gpiod_line_settings_free(settings);
            ngn->log->Message("[NGN_Gpio] error: unable to allocate request config");
            return false;
        }
        gpiod_request_config_set_consumer(req_cfg, consumer_name);

        // Realiza la solicitud de la linea
        gpiod_line_request* request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

        // Libera los objetos de configuracion temporales
        gpiod_request_config_free(req_cfg);
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);

        // Si no se ha podido reservar, indica el error
        if (!request) {
            ngn->log->Message("[NGN_Gpio] error: unable to request port_id " + std::to_string(port_id));
            port_mode[port_id] = NGN_GPIO_MODE.undefined;
            return false;
        }

        // Guarda la solicitud y el modo
        requests[port_id] = request;
        port_mode[port_id] = mode;

        return true;

    #else

        // NOOP fuera de Raspberry Pi
        (void)port_id; (void)mode;
        return false;

    #endif

}



// Configura el tiempo del filtro de "rebote" de un puerto cuando esta en modo INPUT
bool NGN_Gpio::SetDebounce(uint8_t port_id, uint32_t microseconds) {

    #if defined (TARGET_RASPBERRY)

        // Si no esta disponible, sal
        if (!available) return false;

        // Si el puerto no es valido, sal
        if (!ValidPort(port_id)) return false;

        // Si la linea no esta configurada, sal
        if (!requests[port_id]) {
            ngn->log->Message("[NGN_Gpio] error: port_id " + std::to_string(port_id) + " has no PortMode set");
            return false;
        }

        // Si el modo es incorrecto, sal
        if (
            (port_mode[port_id] != NGN_GPIO_MODE.input)
            &&
            (port_mode[port_id] != NGN_GPIO_MODE.input_pullup)
            &&
            (port_mode[port_id] != NGN_GPIO_MODE.input_pulldown)
        ) return false;

        // Crea los ajustes con la configuracion actual y el nuevo debounce
        gpiod_line_settings* settings = gpiod_line_settings_new();
        if (!settings) return false;
        gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);
        
        // Segun el modo...
        switch (port_mode[port_id]) {
            case NGN_GPIO_MODE.input_pullup:
                gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP);
                break;
            case NGN_GPIO_MODE.input_pulldown:
                gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_DOWN);
                break;
            default:
                gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_DISABLED);
                break;
        }

        // Establece el tiempo de filtrado de rebote en microsegundos
        gpiod_line_settings_set_debounce_period_us(settings, microseconds);

        // Añade los ajustes a la configuracion de linea
        gpiod_line_config* line_cfg = gpiod_line_config_new();
        if (!line_cfg) {
            gpiod_line_settings_free(settings);
            return false;
        }
        uint32_t offset = port_id;
        gpiod_line_config_add_line_settings(line_cfg, &offset, 1, settings);

        // Aplica la reconfiguracion sobre la solicitud activa
        int32_t result = gpiod_line_request_reconfigure_lines(requests[port_id], line_cfg);

        // Libera los objetos temporales
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);

        // Devuelve TRUE en caso de exito (0). Si hay un error (-1), devuelve FALSE
        return (result == 0);

    #else

        // NOOP fuera de la RPi
        (void)port_id; (void)microseconds;
        return false;

    #endif

}


// Configura el mismo tiempo de "rebote" a todos los puertos configurados como INPUT
bool NGN_Gpio::SetDebounceAll(uint32_t microseconds) {

    #if defined (TARGET_RASPBERRY)

        bool result = true;

        for (uint8_t k = 0; k < MAX_PORTS; k ++) {

            // Si es un modo de entrada valido, aplica el filtro
            switch (GetPortMode(k)) {

                case NGN_GPIO_MODE.input:
                case NGN_GPIO_MODE.input_pullup:
                case NGN_GPIO_MODE.input_pulldown:
                    result &= SetDebounce(k, microseconds);
                    break;

                default:
                    // Ignora puertos de salida, no configurados o reservados (no es error)
                    break;

            }

        }

        // Devuelve el resultado
        return result;

    #else

        // NOOP fuera de la RPi
        (void)microseconds;
        return false;

    #endif

}



/*** Escribe un valor digital en un puerto ya configurado como OUTPUT ***/
bool NGN_Gpio::DigitalWrite(uint8_t port_id, bool value) {

    #if defined (TARGET_RASPBERRY)

        // Si no esta disponible, sal
        if (!available) return false;

        // Si el puerto no es valido, sal
        if (!ValidPort(port_id)) return false;

        // Si la linea no esta configurada, sal
        if (!requests[port_id]) {
            ngn->log->Message("[NGN_Gpio] error: port_id " + std::to_string(port_id) + " has no PortMode set");
            return false;
        }

        // Si el modo es incorrecto, sal
        if (port_mode[port_id] != NGN_GPIO_MODE.output) return false;

        // Intenta establecer el valor y guarda el resultado de la operacion
        gpiod_line_value val = value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
        int32_t result = gpiod_line_request_set_value(requests[port_id], port_id, val);

        // Devuelve TRUE en caso de exito (0). Si hay un error (-1), devuelve FALSE
        return (result == 0);

    #else

        // NOOP fuera de la RPi
        (void)port_id; (void)value;
        return false;

    #endif

}



/*** Lee el valor digital de un puerto ya configurado como INPUT ***/
bool NGN_Gpio::DigitalRead(uint8_t port_id) {

    #if defined (TARGET_RASPBERRY)

        // Si no esta disponible, sal
        if (!available) return false;
        
        // Si el puerto no es valido, sal
        if (!ValidPort(port_id)) return false;

        // Si la linea no esta configurada, sal
        if (!requests[port_id]) {
            ngn->log->Message("[NGN_Gpio] error: port_id " + std::to_string(port_id) + " has no PortMode set");
            return false;
        }

        // Si el modo es incorrecto, sal
        if (
            (port_mode[port_id] != NGN_GPIO_MODE.input)
            &&
            (port_mode[port_id] != NGN_GPIO_MODE.input_pullup)
            &&
            (port_mode[port_id] != NGN_GPIO_MODE.input_pulldown)
        ) return false;

        // Intenta la lectura y guarda el resultado de la operacion
        gpiod_line_value val = gpiod_line_request_get_value(requests[port_id], port_id);

        // Devuelve TRUE solo en caso de valor HIGH (1). En los casos de LOW (0) o ERROR (-1), devuelve FALSE
        return (val == GPIOD_LINE_VALUE_ACTIVE);

    #else

        // NOOP fuera de la RPi
        (void)port_id;
        return false;

    #endif

}



/*** Libera un puerto previamente reservado ***/
void NGN_Gpio::ReleasePort(uint8_t port_id) {

    #if defined (TARGET_RASPBERRY)

        // Si el puerto no es valido, sal
        if (!ValidPort(port_id)) return;

        // Si la solicitud del puerto esta declarada, liberala
        if (requests[port_id]) {
            gpiod_line_request_release(requests[port_id]);
            requests[port_id] = nullptr;
            port_mode[port_id] = NGN_GPIO_MODE.undefined;
        }

    #else

        // NOOP fuera de la RPi
        (void)port_id;

    #endif

}



/*** Libera todos los puertos reservados hasta el momento ***/
void NGN_Gpio::ReleaseAllPorts() {

    #if defined (TARGET_RASPBERRY)

        for (uint8_t k = 0; k < MAX_PORTS; k ++) ReleasePort(k);

    #endif

}



// Devuelve el modo actual de un puerto
uint8_t NGN_Gpio::GetPortMode(uint8_t port_id) {

    // Si el puerto no es valido, devuelve indefinido
    if (!ValidPort(port_id)) return NGN_GPIO_MODE.undefined;

    // Devuelve el modo actual del puerto
    return port_mode[port_id];

}



/*** Indica si el modulo GPIO se ha inicializado correctamente ***/
bool NGN_Gpio::IsAvailable() {

    return available;

}



/*** Comprueba si el numero de puerto esta dentro del rango valido ***/
bool NGN_Gpio::ValidPort(uint8_t port_id) {

    // Si se sale de rango, indica que no esta disponible
    if (port_id >= MAX_PORTS) return false;
    
    // Si el puerto esta en la lista de reservados, tampoco esta disponible
    for (uint8_t k = 0; k < RESERVED_PORTS_NUMBER; k ++) {
        if (port_id == RESERVED_PORTS_LIST[k]) return false;
    }

    // Puerto disponible
    return true;

}