/******************************************************************************

    N'gine Lib for SDL2
    Conversor de PNG a TILES + MAP

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2017 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG
    (c) 2005 - 2016 by Lode Vandevenne
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
    std::string filename = "";      // Nombre base de los archivos de salida
    uint32_t ts = 32;               // Tamaño del tile
    uint32_t lv = 2;                // Nivel de optimizacion
    bool ef = false;                // Generar archivos extra?

    // Texto de bienvenida
    std::cout << std::endl << "N'gine PNG to TiledBG converter." << std::endl << "(cc) 2016 - 2017 by Cesar Rincon." << std::endl << "http://www.nightfoxandco.com" << std::endl << "contact@nightfoxandco.com" << std::endl << std::endl;

    // Verifica la linea de comandos
    if ((argc < 2) || (argc > 10)) {     // Nº de argumentos incorrecto
        ErrorMsg();
        return 1;
    } else if (argc > 2) {              // Analiza los argumentos extra
        bool _filename = false;
        bool _ts = false;
        bool _lv = false;
        bool _ef = false;
        for (int32_t i = 2; i < argc; i ++) {

            if (strcmp(args[i], "-o") == 0) {      // Comando -O
                if (((i + 1) < argc) && !_filename) {
                    _filename = true;
                    filename = args[(i + 1)];
                    i ++;
                    if ((filename.length() < 1) || (filename.length() > 240)) {
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
                } else {
                    ErrorMsg();
                    return 1;
                }
            }

        }
    }
    std::cout << std::endl;

    // Crea el objeto de la libreria
    PngToTiles* tm = new PngToTiles();

    // Aplica los parametros por defecto
    tm->input_filename = args[1];
    if (filename != "" ){
        tm->output_filename = filename;
    } else {
        tm->output_filename = args[1];
    }
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
