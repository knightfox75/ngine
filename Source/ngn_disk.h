/******************************************************************************

    N'gine Lib for C++
    *** Version 1.13.0-win_0x02 ***
    Funciones de acceso al disco

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
