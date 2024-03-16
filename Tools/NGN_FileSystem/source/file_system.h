/******************************************************************************

    Sistema de empaquetado de archivos para N'gine
    - Sistema de archivos -

    Proyecto iniciado el 14 de Febrero del 2021
    (c) 2021 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	Sistema de empaquetado de archivos para N'gine is under MIT License

	Copyright (c) 2016 - 2024 by Cesar Rincon "NightFox"

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



#ifndef FILE_SYSTEM_H_INCLUDED
#define FILE_SYSTEM_H_INCLUDED



/*** Includes ***/
// C++
#include <vector>
#include <string>
// Proyecto
#include "defines.h"
#include "message.h"
#include "fs_manager.h"
#include "cryptography.h"



/*** Declaracion de la clase ***/
class FileSystem {

    public:

        // Constructor
        FileSystem(int32_t argc, char* args[]);

        // Destructor
        ~FileSystem();

        /*** Propiedades ***/



        /*** Metodos ***/
        int32_t Run();          // Programa principal


    private:

        /*** Propiedades ***/

        // Flags de los argumentos
        struct argument {
            bool state;
            std::string value;
        };
        argument arg_help;          // Ayuda
        argument arg_in_dir;        // Directorio de origen
        argument arg_out_dir;       // Directorio de destino
        argument arg_in_file;       // Archivo de origen
        argument arg_out_file;      // Archivo de destino
        argument arg_key;           // Clave de encriptado

        // Argumentos introducidos
        int32_t argument_count;                             // Numero de argumentos en la linea de comandos
        std::vector<std::string> argument_list;             // Lista con los argumentos proporcionados

        // Tabla de asignacion de archivos
        std::vector<FatNode> fat;                           // Lista con los archivos a gestionar
        uint32_t fat_size;                                  // Tama�o de la tabla de asignacion de archivos (FAT)
        uint32_t data_size;                                 // Tama�o de los datos

        // Cabecera del archivo
        FileHeader file_header;                             // Encabezado del archivo


        /*** Objetos ***/
        Message* msg;                                       // Gestor de mensages
        FsManager* fs;                                      // Gestor del sistema de archivos
        Cryptography* crypto;                               // Funciones de encriptado de datos


        /*** Metodos para la lectura de parametros***/
        int32_t CheckArguments();                           // Analiza la lista de argumentos proporcionados

        /*** Metodos para la creacion de un archivo empaquetado ***/
        int32_t CreateFatFromDir();                         // Crea una tabla de asignacion de archivos desde el directorio de entrada
        int32_t CreatePackage();                            // Crea un archivo con los datos empaquetados
        void GenerateFileHeader();                          // Genera el encabezado del archivo empaquetado
        int32_t WritePackageFile(std::string filepath);     // Graba el archivo con los datos

        /*** Metodos para la lectura de un archivo empaquetado ***/
        int32_t CreateFatFromPackage();                     // Crea una tabla de asignacion de archivos de un archivo empaquetado
        int32_t ExtractPackage();                           // Extrae los archivos de un empaquetado a un directorio
        int32_t ReadFileHeader();                           // Lee el encabezado del archivo

        /*** Metodos adicionales ***/
        std::vector<uint8_t> Checksum(std::vector<uint8_t> &data);

        /*** Metodos de depuracion ***/
        void DebugArgList();                                // Imprime la lista de argumentos introducidos
        void DebugShowFat();                                // Imprime el contenido actual de la FAT

};



#endif // FILE_SYSTEM_H_INCLUDED
