/******************************************************************************

    N'gine Lib for C++
    *** Version 1.18.0-wip_0x01 ***
    Funciones de acceso al disco

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2024 by Cesar Rincon "NightFox"

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



#ifndef NGN_DISK_H_INCLUDED
#define NGN_DISK_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <string>
#include <vector>



/*** Declaracion de la clase ***/
class NGN_Disk {

    // Clases con acceso a los miembros restringidos de esta clase
    friend class NGN_Graphics;
    friend class NGN_Load;
    friend class NGN_Log;

    public:

        // Devuelve la instancia
        static NGN_Disk* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


        // Lee un archivo en formato binario desde el disco y almacenalo en un buffer en RAM
        int32_t ReadBinaryFile(std::string filepath, std::vector<uint8_t> &buffer);

        // Escribe un archivo en formato binario al disco desde un buffer en RAM
        int32_t WriteBinaryFile(std::string filepath, std::vector<uint8_t> &buffer);

        // Lee un archivo en formato de texto desde el disco y almacenalo en un string
        std::string ReadTextFile(std::string filepath);                                 // Primera sobrecarga
        bool ReadTextFile(std::string filepath, std::vector<std::string> &lines);       // Segunda sobrecarga

        // Escribe un archivo en formato de texto al disco desde un string
        int32_t WriteTextFile(std::string filepath, std::string text, bool append = false);

        // Verifica si un archivo existe
        int32_t CheckFile(std::string path);


    private:

        // Constructor
        NGN_Disk();

        // Destructor
        ~NGN_Disk();

        // Puntero de memoria a la instancia
        static NGN_Disk* instance;


        // Genera una estructura de directorios
        int32_t MakePath(std::string path);

};



#endif // NGN_DISK_H_INCLUDED
