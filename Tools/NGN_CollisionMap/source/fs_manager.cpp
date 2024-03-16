/******************************************************************************

    Conversor de PNG a Mapa de Colisiones (.map) para N'gine
	- Gestor de sistema de archivos del sistema -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Mapa de Colisiones is under MIT License

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



/*** Includes ***/

// C++
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
// C
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
// Includes del proyecto
#include "fs_manager.h"
#include "defines.h"



/*** Constructor ***/
FsManager::FsManager() {

}



/*** Destructor ***/
FsManager::~FsManager() {

}



/*** Crea una lista de archivos de un directorio concreto ***/
std::vector<std::string> FsManager::GetListOfFiles(std::string path) {

    // Crea una lista temporal
    std::vector<std::string> file_list;
    file_list.clear();

    // Lista recursiva
    if (path.length() > 0) GetFilesRecursive(path, file_list);

    // Devuelve la lista creada
    return file_list;

}



/*** Crea un directorio a partir de un path, si este no existe ***/
int32_t FsManager::MakePath(std::string path) {

    // Caracter de referencia
    std::string getchar = "";

    // Genera una lista de directorios
    std::vector<std::string> directory;
    directory.clear();
    for (uint32_t i = 0; i < path.length(); i ++) {
        getchar = path.at(i);
        if (getchar == "/") {
            directory.push_back(path.substr(0, i));
        }
    }

    // Genera un path sin el nombre de archivo
    for (uint32_t i = 0; i < directory.size(); i ++) {
        if (directory[i] == ".") continue;
        if (i == (directory.size() - 1)) std::cout << "Attempting to create [" << directory[i] << "]" << std::endl;
        const char* p = directory[i].c_str();
        #if defined (OS_WINDOWS)
            mkdir(p);
        #elif defined (OS_LINUX)
            mkdir(p, 0777);
        #else
            mkdir(p, 0777);
        #endif
    }

    // Devuelve el resultado
    return directory.size();

}



/*** Crea una lista recursiva de los archivos ***/
void FsManager::GetFilesRecursive(std::string path, std::vector<std::string> &files) {

    // Convierte la ruta de archivo a constante
    const char* _path = path.c_str();

    // Estructura de datos de "dirent"
    struct dirent* dp;
    // Variable de control de los metodos
    DIR *dir;

    // Intenta abrir el directorio
    dir = opendir(_path);
    // Si no puedes, asume que es un archivo y registralo
    if (!dir) {
        if (CheckIfFileExist(path)) files.push_back(path);
        return;
    }

    // Variables
    std::string file_name = "";

    // Lee el directorio hasta que te quedes sin datos
    while ((dp = readdir(dir)) != NULL) {

        // Nombre del archivo actual
        file_name = dp->d_name;

        // Si es el indicador de raiz (.) o subir un nivel (..), saltatelo
        if ((file_name == ".") || (file_name == "..")) continue;

        // Genera una nueva ruta, por si es un directorio y no un archivo
        std::string new_path = "";
        if (path.substr(0, 1) != ".") new_path += (path + "/");
        new_path += dp->d_name;

        // Recursividad
        GetFilesRecursive(new_path, files);

    }

    // Cierra el directorio
    closedir(dir);

}



/*** Verifica que el archivo existe ***/
bool FsManager::CheckIfFileExist(std::string path) {

    // Convierte la ruta de archivo a constante
    const char* _path = path.c_str();

    // Intenta abrir el archivo en modo binario
    std::ifstream file;
    file.open(_path, std::ifstream::in | std::ifstream::binary);

    // Segun si has podido o no abrirlo...
    if (file.is_open()) {
        file.close();
        return true;
    } else {
        return false;
    }

}
