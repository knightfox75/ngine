/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0+10th-anniversary ***
    Funciones de mensages de depuracion

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
