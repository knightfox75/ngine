/******************************************************************************

    Sistema de empaquetado de archivos para N'gine
    - Definiciones -

    Proyecto iniciado el 14 de Febrero del 2021
    (cc) 2021 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

******************************************************************************/

#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED



/*** Includes ***/
// C++
#include <string>



/*** Codigos de ERROR ***/
const int32_t ERR_CODE_OK = 0x00;
const int32_t ERR_CODE_BOOTFAILURE = 0x01;
const int32_t ERR_CODE_ARG_INVALID = 0x02;
const int32_t ERR_CODE_INDIR_EMPTY = 0x03;
const int32_t ERR_CODE_OUTFILE_FAILS = 0x04;
const int32_t ERR_CODE_EXTRACT_FAILS = 0x05;
const int32_t ERR_CODE_INFILE_EMPTY = 0x06;


/*** Version de N'gine ***/
const int32_t NGN_VERSION_MAJOR = 1;                                 // Version mayor
const int32_t NGN_VERSION_MINOR = 11;                                // Version menor
const int32_t NGN_VERSION_PATCH = 0;                                 // Version parche
const std::string NGN_VERSION_METADATA = "stable";                   // Version metadatos


/*** Version del programa ***/
const std::string PROGRAM_NAME = "N'gine file system manager";       // Nombre del programa
const std::string CC_YEARS = "2021-2023";                            // Años en el texto CREATIVE COMMONS
const int32_t PROGRAM_VERSION_MAJOR = 1;                             // Version mayor
const int32_t PROGRAM_VERSION_MINOR = 3;                             // Version menor
const int32_t PROGRAM_VERSION_PATCH = 0;                             // Version parche
const std::string PROGRAM_VERSION_METADATA = "stable";               // Version metadatos
const std::string MAGIC_STRING = "NGN FILE SYSTEM";    	            // Magic string
const uint8_t MS_LENGTH = 15;                                        // Logitud del magic string
const uint8_t VERSION = 2;                                           // Version del archivo


/*** Textos recurrentes ***/
const std::string TEXT_DASHED_LINE = "------------------------------------------------------------";


/*** Listado de argumentos disponibles ***/
const int32_t ARG_MIN_NUM = 2;                      // Numero minimo de argumentos
const int32_t ARG_MAX_NUM = 7;                      // Numero maximo de argumentos
const std::string ARG_HELP = "-?";                  // Ayuda
const std::string ARG_IN_DIR = "-idir";             // Define el directorio de origen
const std::string ARG_OUT_DIR = "-odir";            // Define el directorio de destino
const std::string ARG_IN_FILE = "-ifile";           // Define el archivo de origen
const std::string ARG_OUT_FILE = "-ofile";          // Define el archivo de destino
const std::string ARG_KEY = "-key";                 // Define la clave de encriptado


/*** Estructuras de datos ***/
const uint8_t CHK_SIZE = 32;
struct FatNode {
    uint32_t next_node_offset;          // Posicion del siguiente nodo
    uint32_t path_length;               // Numero de caracteres de la ruta
    uint32_t file_offset;               // Posicion del archivo
    uint32_t file_size;                 // Tamaño del archivo
    uint8_t checksum[CHK_SIZE];         // Checksum del archivo
    std::string file_name;              // Nombre del archivo (ruta incluida)
};

// Tamaño de los datos estaticos del nodo
const uint32_t NODE_SIZE = ((sizeof(uint32_t) * 4) + (sizeof(uint8_t) * CHK_SIZE));

/*

    Estructura de la tabla de asignacion de archivos

    [(uint8_t)(uint8_t)(uint8_t)(uint8_t)]      {next_node_offset}      // Posicion del siguiente nodo
    [(uint8_t)(uint8_t)(uint8_t)(uint8_t)]      {path_length}           // Longitud en caracteres del nombre de archivo
    [(uint8_t)(uint8_t)(uint8_t)(uint8_t)]      {file_offset}           // Posicion del archivo
    [(uint8_t)(uint8_t)(uint8_t)(uint8_t)]      {file_size}             // Tamaño del archivo
    [(uint8_t)(uint8_t)(uint8_t)...      ]      {file_name}             // Nombre del archivo (varia en longitud)

*/


/*** Cabecera del archivo empaquetado ***/
struct FileHeader {
    uint8_t version;                // Version del programa
    char magic[32];                 // Magic String (MAX 32 chars)
    uint32_t fat_offset;            // Punto de inicio de la FAT
    uint32_t fat_size;              // Tamaño de la FAT
    uint32_t data_offset;           // Punto de unicio de los datos
    uint32_t data_size;             // Tamaño de los datos
    uint8_t reserve[256];           // Reservado para usos futuros (256 por defecto)
};



#endif // DEFINES_H_INCLUDED
