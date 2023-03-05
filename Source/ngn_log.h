/******************************************************************************

    N'gine Lib for C++
    *** Version 1.13.0-win_0x01 ***
    Funciones de mensages de depuracion

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



#ifndef NGN_LOG_H_INCLUDED
#define NGN_LOG_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <string>
#include <fstream>



/*** Declaracion de la clase ***/
class NGN_Log {

    public:

        // Devuelve la instancia
        static NGN_Log* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();

        // Abre el archivo de registro
        bool OpenLogFile(std::string log_file, bool overwrite = true);

        // Cierra el archivo de registro
        void CloseLogFile();

        // Mensage
        void Message(std::string msg);


    private:

        // Constructor
        NGN_Log();

        // Destructor
        ~NGN_Log();

        // Puntero de memoria a la instancia
        static NGN_Log* instance;

        std::string _log_file;  // Nombre del archivo de registro
        bool _log_enabled;      // Archivo de registro habilitado

        std::fstream file;      // Stream de gestion del archivo de registro

        // Genera la marca de tiempo actual
        std::string GetTimeStamp();

};



#endif // NGN_LOG_H_INCLUDED
