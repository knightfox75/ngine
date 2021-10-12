/******************************************************************************

    N'gine Lib for C++
    *** Version 1.6.0-beta ***
    Funciones del sistema de archivos

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
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



#ifndef NGN_FILESYSTEM_H_INCLUDED
#define NGN_FILESYSTEM_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <string>
#include <vector>



/*** Declaracion de la clase ***/
class NGN_FileSystem {

    public:

        // Constructor
        NGN_FileSystem();

        // Destructor
        ~NGN_FileSystem();

        // Metodo para cargar un archivo desde un archivo empaquetado a un buffer temporal en RAM
        int32_t LoadFileFromPakage(std::string filepath, std::vector<uint8_t> &buffer);

        // Establece el uso de un archivo empaquetado
        bool SetPackage(std::string pkg_file, std::string key);



    private:

        // Datos del archivo empaquetado
        std::string package_file;           // Ruta al archivo empaquetado
        std::string package_key;            // Clave de encriptado

        // Recuerda si ya se ha abierto con exito el paquete
        std::string current_package_file;   // Ruta al archivo empaquetado

        // Define la cabecera del archivo empaquetado
        struct FileHeader {
            uint8_t version;                // Version del programa
            char magic[32];                 // Magic String (MAX 32 chars)
            uint32_t fat_offset;            // Punto de inicio de la FAT
            uint32_t fat_size;              // Tamaño de la FAT
            uint32_t data_offset;           // Punto de unicio de los datos
            uint32_t data_size;             // Tamaño de los datos
            uint8_t reserve[256];           // Reservado para usos futuros (256 por defecto)
        };


        // Cabecera del archivo
        FileHeader file_header;             // Encabezado del archivo

        // Magic string
        const std::string MAGIC_STRING = "NGN FILE SYSTEM";
        // Version del programa de empaquetado
        const uint8_t VERSION = 2;


        //Define la estructura de un nodo de la FAT
        // Tamaño del checksum
        static const uint8_t CHK_SIZE = 32;
        // Estructura del nodo
        struct FatNode {
            uint32_t next_node_offset;          // Posicion del siguiente nodo
            uint32_t path_length;               // Numero de caracteres de la ruta
            uint32_t file_offset;               // Posicion del archivo
            uint32_t file_size;                 // Tamaño del archivo
            uint8_t checksum[CHK_SIZE];         // Checksum del archivo
            std::string file_name;              // Nombre del archivo (ruta incluida)
        };
        // Tamaño de los datos estaticos del nodo
        static const uint32_t NODE_SIZE = ((sizeof(uint32_t) * 4) + (sizeof(uint8_t) * CHK_SIZE));

        // Tabla de asignacion de archivos
        std::vector<FatNode> fat;               // Lista con los archivos a gestionar
        uint32_t fat_size;                      // Tamaño de la tabla de asignacion de archivos (FAT)
        uint32_t data_size;                     // Tamaño de los datos

        /*

        Estructura de la tabla de asignacion de archivos

        [(uint8_t)(uint8_t)(uint8_t)(uint8_t)]      {next_node_offset}      // Posicion del siguiente nodo
        [(uint8_t)(uint8_t)(uint8_t)(uint8_t)]      {path_length}           // Longitud en caracteres del nombre de archivo
        [(uint8_t)(uint8_t)(uint8_t)(uint8_t)]      {file_offset}           // Posicion del archivo
        [(uint8_t)(uint8_t)(uint8_t)(uint8_t)]      {file_size}             // Tamaño del archivo
        [(uint8_t)(uint8_t)(uint8_t)...      ]      {file_name}             // Nombre del archivo (varia en longitud)

        */

        // Almacena los valores de la clave de encriptacion
        std::vector<uint8_t> key_values;


        // Lee el encabezado del archivo
        int32_t ReadPackageHeader();
        // Crea una tabla de asignacion de archivos de un archivo empaquetado
        int32_t CreateFatFromPackage();

        // Desencriptado de los datos contenidos en un vector
        int32_t DecryptData(std::vector<uint8_t> &data);
        // Convierte la clave a un vector de valores numericos
        void KeyToValues();
        // Rota un valor n veces a la izquierda
        uint8_t RotateLeft(uint8_t value, uint32_t positions);
        // Rota un valor n veces a la derecha
        uint8_t RotateRight(uint8_t value, uint32_t positions);

        // Checksum del archivo
        std::vector<uint8_t> Checksum(std::vector<uint8_t> &data);

};



#endif // NGN_FILESYSTEM_H_INCLUDED
