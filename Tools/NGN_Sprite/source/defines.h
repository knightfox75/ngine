/******************************************************************************

    Conversor de PNG a Sprite (.spr) para N'gine
    - Definiciones -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Sprite is under MIT License

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
const std::string PROGRAM_NAME = "N'gine PNG to Sprite converter";          // Nombre del programa
const std::string CPR_YEARS = "2016-2024";                                  // Años en el texto de CopyRight
const int32_t PROGRAM_VERSION_MAJOR = 2;                                    // Version mayor
const int32_t PROGRAM_VERSION_MINOR = 0;                                    // Version menor
const int32_t PROGRAM_VERSION_PATCH = 2;                                    // Version parche
const std::string PROGRAM_VERSION_METADATA = "stable";                      // Version metadatos
const std::string MAGIC_STRING = "NGN SPRITE";    	                        // Magic string
const uint8_t MS_LENGTH = 10;                                               // Logitud del magic string
const uint8_t VERSION = 2;                                                  // Version del archivo


/*** Textos recurrentes ***/
const std::string TEXT_DASHED_LINE = "--------------------------------------------------------------------------------";


/*** Listado de argumentos disponibles ***/
const int32_t ARG_MIN_NUM = 2;                          // Numero minimo de argumentos
const int32_t ARG_MAX_NUM = 10;                         // Numero maximo de argumentos
const std::string ARG_HELP = "-?";                      // Ayuda
const std::string ARG_WIDTH = "-w";                     // Ancho del fotograma
const std::string ARG_HEIGHT = "-h";                    // Alto del fotograma
const std::string ARG_OUT_FILE = "-o";                  // Define el archivo de salida
const std::string ARG_STRIP = "-s";                     // Genera un PNG con la tira de fotogramas


/*** Extensiones ***/
const std::string SPR_EXTENSION = ".spr";               // Extension de archivo: Archivo tilemap
const std::string STRIP_EXTENSION = "_strip.png";       // Extension de archivo: Tileset en PNG


/*** Parametros de funcionamiento ***/
const int32_t MIN_FRAME_SIZE = 1;           // Tamaño minimo del fotograma
const int32_t MAX_FRAME_SIZE = 8192;        // Tamaño maximo del fotograma



/*** Estructuras personalizadas ***/

// Cabecera del archivo
struct FileHeader {
    uint8_t version;                // Version del programa de conversion
    char magic[32];                 // Magic String
    uint32_t sheet_width;           // Tamaño del sheet completo
    uint32_t sheet_height;
    uint32_t frame_width;           // Tamaño del frame
    uint32_t frame_height;
    uint32_t total_frames;          // Numero total de frames de la imagen
    uint8_t reserve[256];           // Posible uso futuro
};


#endif // DEFINES_H_INCLUDED
