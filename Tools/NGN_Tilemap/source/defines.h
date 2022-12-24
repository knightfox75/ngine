/******************************************************************************

    Conversor de PNG a Fondo de Tiles (.tbg) para N'gine
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
const std::string PROGRAM_NAME = "N'gine PNG to Tile Map converter";        // Nombre del programa
const std::string CC_YEARS = "2016-2023";                                   // Años en el texto CREATIVE COMMONS
const int32_t PROGRAM_VERSION_MAJOR = 2;                                    // Version mayor
const int32_t PROGRAM_VERSION_MINOR = 0;                                    // Version menor
const int32_t PROGRAM_VERSION_PATCH = 0;                                    // Version parche
const std::string PROGRAM_VERSION_METADATA = "stable";                      // Version metadatos
const std::string MAGIC_STRING = "NGN TILEDBG";    	                        // Magic string
const uint8_t MS_LENGTH = 11;                                               // Logitud del magic string
const uint8_t VERSION = 2;                                                  // Version del archivo


/*** Textos recurrentes ***/
const std::string TEXT_DASHED_LINE = "------------------------------------------------------------";


/*** Listado de argumentos disponibles ***/
const int32_t ARG_MIN_NUM = 2;                          // Numero minimo de argumentos
const int32_t ARG_MAX_NUM = 10;                         // Numero maximo de argumentos
const std::string ARG_HELP = "-?";                      // Ayuda
const std::string ARG_OUT_FILE = "-o";                  // Define el archivo de salida
const std::string ARG_TILE_SIZE = "-ts";                // Define el tamaño del tile
const std::string ARG_OP_LEVEL = "-lv";                 // Nivel de optimizacion
const std::string ARG_EXTRA_FILES = "-ef";              // Genera los archivos de salida adicionales

/*** Parametros de los argumentos ***/
const int32_t MIN_TILE_SIZE = 8;                        // Tamaño minimo del tile
const int32_t MAX_TILE_SIZE = 1024;                     // Tamaño maximo del tile
const int32_t DEFAULT_TILE_SIZE = 32;                   // Tamaño por defecto del tile
const int32_t DEFAULT_OP_LEVEL = 2;                     // Valor de optimizacion por defecto

/*** Extensiones ***/
const std::string TBG_EXTENSION = ".tbg";               // Extension de archivo: Archivo tilemap
const std::string TILESET_EXTENSION = "_tileset.png";   // Extension de archivo: Tileset en PNG
const std::string MAP_EXTENSION = "_map.bin";           // Extension de archivo: Mapa en formato binario



/*** Estructuras personalizadas ***/

// Pixel RGBA
struct RgbaPixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// Cabecera del archivo
struct FileHeader {
    uint8_t version;                // Version del programa de mapeado
    char magic[32];                 // Magic string
    uint32_t bg_width;              // Ancho del fondo (imagen original)
    uint32_t bg_height;             // Alto del fondo (imagen original)
    uint32_t tileset_width;         // Ancho del Tileset en pixeles
    uint32_t tileset_height;        // Ancho del Tileset en pixeles
    uint32_t map_width;             // Ancho del mapa (segun los tiles) en pixeles
    uint32_t map_height;            // Alto del mapa (segun los tiles) en pixeles
    uint32_t tile_size;             // Tamaño del tile
    uint32_t tileset_length;        // Tamaño de los datos del tileset
    uint32_t map_length;            // Tamaño de los datos del mapa
    char reserve[256];              // Posible uso futuro
};


#endif // DEFINES_H_INCLUDED
