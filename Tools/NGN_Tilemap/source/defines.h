/******************************************************************************

    Conversor de PNG a Fondo de Tiles (.tbg) para N'gine
    - Definiciones -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016-2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Fondo de Tiles is under MIT License

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

#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED



/*** Includes ***/
// C++
#include <cstdint>
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
const std::string CPR_YEARS = "2016-2025";                                  // Años en el texto de CopyRight
const int32_t PROGRAM_VERSION_MAJOR = 2;                                    // Version mayor
const int32_t PROGRAM_VERSION_MINOR = 0;                                    // Version menor
const int32_t PROGRAM_VERSION_PATCH = 3;                                    // Version parche
const std::string PROGRAM_VERSION_METADATA = "stable";                      // Version metadatos
const std::string MAGIC_STRING = "NGN TILEDBG";    	                        // Magic string
const uint8_t MS_LENGTH = 11;                                               // Logitud del magic string
const uint8_t VERSION = 2;                                                  // Version del archivo


/*** Textos recurrentes ***/
const std::string TEXT_DASHED_LINE = "--------------------------------------------------------------------------------";


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
