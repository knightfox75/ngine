/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0-wip_0x02 ***
    Funciones de mensages de depuracion

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2025 by Cesar Rincon "NightFox"

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
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

// Libreria
#include "ngn.h"


/*** Puntero de la instancia a nullptr ***/
NGN_Log* NGN_Log::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
NGN_Log* NGN_Log::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Log();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Log::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Constructor ***/
NGN_Log::NGN_Log() {

    _log_file = "";
    _log_enabled = false;

}



/*** Destructor ***/
NGN_Log::~NGN_Log() {

    if (file.is_open()) file.close();

}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Log::BootUp() {
}



/*** Abre el archivo de registro ***/
bool NGN_Log::OpenLogFile(std::string log_file, bool overwrite) {

    // Protecciones
    if (log_file == "") return false;
    if (_log_enabled) return false;

    // Guarda la configuracion
    _log_file = log_file;
    _log_enabled = true;

    // Nombre del archivo de registro
    const char* _filepath = log_file.c_str();

    // Si el archivo ya existe...
    if (ngn->disk->CheckFile(log_file) >= 0) {
        if (overwrite) {
            // Sobreescribelo
            file.open(_filepath, std::fstream::out | std::fstream::trunc);
        } else {
            // Adjunta la informacion a continuacion
            file.open(_filepath, std::fstream::out | std::fstream::app);
        }
        return true;
    }


    // Si no existe, intenta crear uno de nuevo
    file.open(_filepath, std::fstream::out | std::fstream::trunc);

    // Si no puedes abrir el archivo, intenta crear antes la ruta
    if (!file.is_open()) {
        // Intenta generar la estructura de directorios, en caso de no existir
        ngn->disk->MakePath(log_file);
        // Y vuelve a intentar abrir el archivo para su escritura
        file.open(_filepath, std::fstream::out | std::fstream::trunc);
        // Si continua sin poder abrirse, sal informando del error
        if (!file.is_open()) {
            Message("[NGN_Log error] Can't create <" + log_file + "> file.");
            _log_enabled = false;
            return false;
        }
    }

    // Devuelve el resultado de la operacion
    return true;

}



/*** Cierra el archivo de registro ***/
void NGN_Log::CloseLogFile() {

    if (file.is_open()) file.close();
    _log_enabled = false;

}



/*** Procesa el mensage ***/
void NGN_Log::Message(std::string msg) {

    std::string time_stamp = GetTimeStamp();

    // Muestra el mensage por la consola de texto
    std::cout << time_stamp << " ";
    std::cout << msg << std::endl;

    // Si el archivo de registro esta habilitado, intenta registrarlo en el
    if (!_log_enabled) return;
    file << time_stamp << " ";
    file << msg << std::endl;

}



/*** Genera la marca de tiempo ***/
std::string NGN_Log::GetTimeStamp() {

    // Acceso a la fecha actual del sistema
    time_t current_time;
    struct  tm* datetime;
    current_time = time(nullptr);
    datetime = localtime(&current_time);

    // Genera el texto
    std::string time_stamp = "[";

    time_stamp += ngn->toolbox->Int2String((datetime->tm_year + 1900), 4, "0");
    time_stamp += "-";
    time_stamp += ngn->toolbox->Int2String((datetime->tm_mon + 1), 2, "0");
    time_stamp += "-";
    time_stamp += ngn->toolbox->Int2String(datetime->tm_mday, 2, "0");
    time_stamp += " ";
    time_stamp += ngn->toolbox->Int2String(datetime->tm_hour, 2, "0");
    time_stamp += ":";
    time_stamp += ngn->toolbox->Int2String(datetime->tm_min, 2, "0");
    time_stamp += ":";
    time_stamp += ngn->toolbox->Int2String(datetime->tm_sec, 2, "0");
    time_stamp += "]";

    // Devuelve la marca de tiempo
    return time_stamp;

}
