/******************************************************************************

    Conversor de PNG a Mapa de Colisiones (.map) para N'gine
	- Gestor de sistema de archivos del sistema -

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

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
