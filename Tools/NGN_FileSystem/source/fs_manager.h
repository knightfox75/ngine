/******************************************************************************

    N'gine Lib for C++
    Sistema de archivos
	- Gestor de sistema de archivos del sistema -

    Proyecto iniciado el 14 de Febrero del 2021
    (cc) 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

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


    private:

        /*** Propiedades ***/


        /*** Metodos ***/

        // Metodo recursivo para listar todos los archivos de todos los directorios
        void GetFilesRecursive(std::string path, std::vector<std::string> &files);
        bool CheckIfFileExist(std::string path);

};



#endif // FS_MANAGER_H_INCLUDED
