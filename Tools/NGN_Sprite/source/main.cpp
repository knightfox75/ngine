/******************************************************************************

    N'gine Lib for C++
    Conversor de PNG a Sprite sheet (.spr)

    Proyecto iniciado el 3 de Marzo del 2016
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

// Sprite class
#include "sprite.h"




/*** Mensages de error ***/
void ErrorMsg() {
    std::cout << "Command line error." << std::endl;
    std::cout << "Usage: NGN_Sprite INPUT_FILE.PNG -w xx -h xx [-o filename] [-s]" << std::endl;
    std::cout << "-w xx         Width of the sprite frame." << std::endl;
    std::cout << "-h xx         Height of the sprite frame." << std::endl;
    std::cout << "-o filename   Name for the output file, without extension (.spr will be added automaticaly)." << std::endl;
    std::cout << "-s            Generates a PNG file with the sprite strip." << std::endl << std::endl;
}




/*** Main ***/
int32_t main(int32_t argc, char* args[]) {

    // Variables
    std::string in_file = "";       // Nombre del archivo de entrada
    std::string out_file = "";      // Nombre base de los archivos de salida
    int32_t width = -1;                 // Ancho del frame
    int32_t height = -1;                // Altura del frame

    // Variables de control
    bool _out_file = false;
    bool _width = false;
    bool _height = false;
    bool _strip = false;
    bool _err = false;

    // Texto de bienvenida
    std::cout <<
    std::endl << "N'gine PNG to Sprite converter. (v0.3.0-a)." <<
    std::endl << "For N'gine Version 0.6.0-a or higher." <<
    std::endl << "(cc) 2016 - 2018 by Cesar Rincon." <<
    std::endl << "http://www.nightfoxandco.com" <<
    std::endl << "contact@nightfoxandco.com" <<
    std::endl << std::endl;

    // Verifica la linea de comandos
    if ((argc < 2) || (argc > 9)) {     // Nº de argumentos incorrecto

        ErrorMsg();
        return 1;

    } else {

        // Archivo de entrada
        in_file = args[1];
        if ((in_file.length() < 5) || (in_file.length() > 240)) {
            ErrorMsg();
            return 1;
        }

        // Analiza los argumentos extra
        for (int32_t i = 2; i < argc; i ++) {
            _err = true;
            if (strcmp(args[i], "-o") == 0) {      // Comando -O
                if (((i + 1) < argc) && !_out_file) {
                    _out_file = true;
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
            } else if (strcmp(args[i], "-w") == 0) {      // Comando -W
                if (((i + 1) < argc) && !_width) {
                    _width = true;
                    _err = false;
                    width = atoi(args[(i + 1)]);
                    i ++;
                    if (width < 1) {
                        ErrorMsg();
                        return 1;
                    }
                } else {
                    ErrorMsg();
                    return 1;
                }
            } else if (strcmp(args[i], "-h") == 0) {      // Comando -H
                if (((i + 1) < argc) && !_height) {
                    _height = true;
                    _err = false;
                    height = atoi(args[(i + 1)]);
                    i ++;
                    if (height < 1) {
                        ErrorMsg();
                        return 1;
                    }
                } else {
                    ErrorMsg();
                    return 1;
                }
            } else if (strcmp(args[i], "-s") == 0) {    // Comando -S
                _strip = true;
                _err = false;
            }
        }
        // Error de uso incorrecto de argumentos
        if (_err) {
            ErrorMsg();
            return 1;
        }
    }
    std::cout << std::endl;

    // Analiza si hay el minimo de parametros requeridos introducidos correctamente
    if (!_width || !_height) {
        ErrorMsg();
        return 1;
    }

    // Nombre del archivo de salida
    if (out_file == "") out_file = in_file.substr(0, (in_file.length() - 4));


    /*** Parametros aceptados, crea el objeto principal ***/
    SpriteSheet* spr = new SpriteSheet();

    // Registra los parametros del Sprite a convertir
    spr->input_filename = in_file;
    spr->output_filename = out_file;
    spr->frame_width = width;
    spr->frame_height = height;
    if (_strip) spr->strip = true;

    // Carga y convierte la imagen PNG en una tira de sprites
    spr->ConvertPng();

    /*** Proceso completado, destruye el objeto principal ***/
    delete spr;


    // Fin del programa
    return 0;

}
