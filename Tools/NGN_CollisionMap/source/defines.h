/******************************************************************************

    Conversor de PNG a Mapa de Colisiones (.map) para N'gine
    - Definiciones -

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

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
const int32_t ERR_CODE_CONVERSION_FAILURE = 0x03;


/*** Version de N'gine ***/
const int32_t NGN_VERSION_MAJOR = 1;                                        // Version mayor
const int32_t NGN_VERSION_MINOR = 11;                                       // Version menor
const int32_t NGN_VERSION_PATCH = 0;                                        // Version parche
const std::string NGN_VERSION_METADATA = "stable";                          // Version metadatos


/*** Version del programa ***/
const std::string PROGRAM_NAME = "N'gine PNG to Collision Map converter";   // Nombre del programa
const std::string CC_YEARS = "2016-2023";                                   // Años en el texto CREATIVE COMMONS
const int32_t PROGRAM_VERSION_MAJOR = 2;                                    // Version mayor
const int32_t PROGRAM_VERSION_MINOR = 0;                                    // Version menor
const int32_t PROGRAM_VERSION_PATCH = 0;                                    // Version parche
const std::string PROGRAM_VERSION_METADATA = "stable";                      // Version metadatos
const std::string MAGIC_STRING = "NGN COLLISION MAP";    	                // Magic string
const uint8_t MS_LENGTH = 17;                                               // Logitud del magic string
const uint8_t VERSION = 2;                                                  // Version del archivo


/*** Textos recurrentes ***/
const std::string TEXT_DASHED_LINE = "------------------------------------------------------------";


/*** Listado de argumentos disponibles ***/
const int32_t ARG_MIN_NUM = 2;                          // Numero minimo de argumentos
const int32_t ARG_MAX_NUM = 7;                          // Numero maximo de argumentos
const std::string ARG_HELP = "-?";                      // Ayuda
const std::string ARG_OUT_FILE = "-o";                  // Define el archivo de salida
const std::string ARG_TILE_SIZE = "-ts";                // Define el tamaño del tile

/*** Parametros de los argumentos ***/
const int32_t MIN_TILE_SIZE = 8;                        // Tamaño minimo del tile
const int32_t MAX_TILE_SIZE = 1024;                     // Tamaño maximo del tile
const int32_t DEFAULT_TILE_SIZE = 32;                   // Tamaño por defecto del tile

/*** Extensiones ***/
const std::string MAP_EXTENSION = ".map";               // Extension de archivo: Archivo tilemap


/*** Estructuras personalizadas ***/

// Cabecera del archivo
struct FileHeader {
    uint8_t version;                // Version del programa de conversion
    char magic[32];                 // Magic String
    uint32_t width;                 // Ancho del mapa
    uint32_t height;                // Alto del mapa
    uint32_t tile_size;             // Tamaño del tile
    uint32_t pal_length;            // Tamaño de los datos (nº de elementos) de la paleta
    uint32_t tileset_length;        // Tamaño de los datos (nº de elementos) del tileset
    uint32_t map_length;            // Tamaño de los datos (nº de elementos) del mapa
    uint8_t reserve[256];           // Posible uso futuro
};


#endif // DEFINES_H_INCLUDED
