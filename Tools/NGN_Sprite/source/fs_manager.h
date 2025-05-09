/******************************************************************************

    Conversor de PNG a Sprite (.spr) para N'gine
	- Gestor de sistema de archivos del sistema -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Sprite is under MIT License

	Copyright (c) 2016 - 2025 by Cesar Rincon "NightFox"

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



#ifndef FS_MANAGER_H_INCLUDED
#define FS_MANAGER_H_INCLUDED



/*** Includes ***/
// C++
#include <vector>
#include <string>
// Proyecto
#include "defines.h"



/*** Declaracion de la clase ***/
class FsManager {

    public:

        // Constructor
        FsManager();

        // Destructor
        ~FsManager();

        /*** Propiedades ***/



        /*** Metodos ***/

        // Crea una lista con los archivos contenidos en un directorio
        std::vector<std::string> GetListOfFiles(std::string path);

        // Crea un directorio a partir de un path, si este no existe
        int32_t MakePath(std::string path);


        // Verifica si un archivo existe
        bool CheckIfFileExist(std::string path);


    private:

        /*** Propiedades ***/


        /*** Metodos ***/

        // Metodo recursivo para listar todos los archivos de todos los directorios
        void GetFilesRecursive(std::string path, std::vector<std::string> &files);

};



#endif // FS_MANAGER_H_INCLUDED
