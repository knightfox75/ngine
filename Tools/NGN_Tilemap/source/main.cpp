/******************************************************************************

    N'gine Lib for C++
    Conversor de PNG a TILES + MAP

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG
    (c) 2005 - 2020 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



/*** Includes ***/

// C++
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

// LodePNG
#include "lodepng/lodepng.h"

// Tielmap
#include "tilemap.h"



/*** Version de N'gine ***/
static const int32_t NGN_VERSION_MAJOR = 0;                 // Version mayor
static const int32_t NGN_VERSION_MINOR = 12;                // Version menor
static const int32_t NGN_VERSION_PATCH = 0;                 // Version parche
static const std::string NGN_VERSION_METADATA = "a";        // Version metadatos
/*** Version del programa ***/
static const int32_t PROGRAM_VERSION_MAJOR = 0;             // Version mayor
static const int32_t PROGRAM_VERSION_MINOR = 3;             // Version menor
static const int32_t PROGRAM_VERSION_PATCH = 2;             // Version parche
static const std::string PROGRAM_VERSION_METADATA = "a";    // Version metadatos



/*** Mensages de error ***/
void ErrorMsg() {
    std::cout << "Command line error." << std::endl;
    std::cout << "Usage: NGN_Tilemap INPUT_FILE.PNG [-o filename] [-ts xx] [-l x] [-ef]" << std::endl;
    std::cout << "-o filename   Name for the output file, without extension (.tbg will be added automaticaly)." << std::endl;
    std::cout << "-ts xx        Size of the squared tile in pixels (From 8 to 1024, 32 pixels by default)." << std::endl;
    std::cout << "-lv x         Optimization level   [1] Duplicates   [2] Flips (Default)   [3] Rotations" << std::endl;
    std::cout << "-ef           Generates extra files with the tileset in PNG and the map in binnary formats." << std::endl << std::endl;
}




/*** Main ***/
int32_t main(int32_t argc, char* args[]) {

    // Variables
    std::string in_file = "";       // Nombre del archivo de entrada
    std::string out_file = "";      // Nombre base de los archivos de salida
    uint32_t ts = 32;               // Tamaño del tile
    uint32_t lv = 2;                // Nivel de optimizacion
    bool ef = false;                // Generar archivos extra?

    // Variables de control
    bool _filename = false;
    bool _ts = false;
    bool _lv = false;
    bool _ef = false;
    bool _err = false;

    // Texto de bienvenida
    std::string ngn_version = std::to_string(NGN_VERSION_MAJOR) + "." + std::to_string(NGN_VERSION_MINOR) + "." + std::to_string(NGN_VERSION_PATCH) + "-" + NGN_VERSION_METADATA;
    std::string program_version = std::to_string(PROGRAM_VERSION_MAJOR) + "." + std::to_string(PROGRAM_VERSION_MINOR) + "." + std::to_string(PROGRAM_VERSION_PATCH) + "-" + NGN_VERSION_METADATA;
    std::cout <<
    std::endl << "N'gine PNG to TiledBG converter. (v" << program_version << ")." <<
    std::endl << "For N'gine Version " << ngn_version << " or higher." <<
    std::endl << "(cc) 2016 - 2020 by Cesar Rincon." <<
    std::endl << "https://nightfoxandco.com" <<
    std::endl << "contact@nightfoxandco.com" <<
    std::endl << std::endl;

    // Verifica la linea de comandos
    if ((argc < 2) || (argc > 10)) {     // Nº de argumentos incorrecto

        ErrorMsg();
        return 1;

    } else {              // Analiza los argumentos extra

        // Archivo de entrada
        in_file = args[1];
        if ((in_file.length() < 5) || (in_file.length() > 240)) {
            ErrorMsg();
            return 1;
        }

        for (int32_t i = 2; i < argc; i ++) {
            _err = true;
            if (strcmp(args[i], "-o") == 0) {      // Comando -O
                if (((i + 1) < argc) && !_filename) {
                    _filename = true;
                    _err = false;
                    out_file = args[(i + 1)];
                    i ++;
                    if ((out_file.length() < 1) || (out_file.length() > 240)) {
                        ErrorMsg();
                        return 1;
                    }
                } else {
                    ErrorMsg();
                    return 1;
                }
            } else if (strcmp(args[i], "-ts") == 0) {      // Comando -TS
                if (((i + 1) < argc) && !_ts) {
                    _ts = true;
                    _err = false;
                    ts = atoi(args[(i + 1)]);
                    i ++;
                    if ((ts < 8) || (ts > 1024)) {
                        ErrorMsg();
                        return 1;
                    }
                } else {
                    ErrorMsg();
                    return 1;
                }
            } else if (strcmp(args[i], "-lv") == 0) {      // Comando -LV
                if (((i + 1) < argc) && !_lv) {
                    _lv = true;
                    _err = false;
                    lv = atoi(args[(i + 1)]);
                    i ++;
                    if ((lv < 1) || (lv > 3)) {
                        ErrorMsg();
                        return 1;
                    }
                } else {
                    ErrorMsg();
                    return 1;
                }
            } else if (strcmp(args[i], "-ef") == 0) {       // Comando -EF
                if (!_ef) {
                    ef = _ef = true;
                    _err = false;
                } else {
                    ErrorMsg();
                    return 1;
                }
            }

        }
        // Error de uso incorrecto de argumentos
        if (_err) {
            ErrorMsg();
            return 1;
        }
    }
    std::cout << std::endl;

    // Nombre del archivo de salida
    if (out_file == "") out_file = in_file.substr(0, (in_file.length() - 4));

    // Crea el objeto de la libreria
    PngToTiles* tm = new PngToTiles();

    // Aplica los parametros por defecto
    tm->input_filename = in_file;
    tm->output_filename = out_file;
    tm->size_of_tile = ts;
    tm->optimize = lv;
    tm->extra_files = ef;

    // Informacion de los parametros introducidos
    std::cout << "Output filename: " << tm->output_filename << ".tbg" << std::endl;
    std::cout << "Tile size: " << tm->size_of_tile << "x" << tm->size_of_tile << " pixels." << std::endl;
    std::cout << "Optimization level: ";
    switch (tm->optimize) {
        case 1:
            std::cout << "Only duplicates." << std::endl;
            break;
        case 2:
            std::cout << "Duplicates & flips." << std::endl;
            break;
        case 3:
            std::cout << "Duplicates, flips & rotations." << std::endl;
            break;
    }
    std::cout << "Generation of extra files ";
    if (tm->extra_files) {
        std::cout << "enabled." << std::endl;
    } else {
        std::cout << "disabled." << std::endl;
    }
    std::cout << std::endl;

    // Convierte la imagen solicitada
    tm->ConvertPng();

    // Elimina la instancia
    delete tm;

    return 0;

}
