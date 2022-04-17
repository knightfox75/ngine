/******************************************************************************

    N'gine Lib for C++
    *** Version 1.8.0-stable ***
    Funciones de mensages de depuracion

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2022 by Cesar Rincon "NightFox"
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



/*** Includes ***/
// C++
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

// Libreria
#include "ngn.h"



/*** Constructor ***/
NGN_Log::NGN_Log() {

    _log_file = "";
    _log_enabled = false;

}



/*** Destructor ***/
NGN_Log::~NGN_Log() {

    if (file.is_open()) file.close();

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
    current_time = time(NULL);
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
