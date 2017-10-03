/******************************************************************************

    N'gine Lib for C++
    Conversor de PNG a Mapa de Colisiones (.map)

    Proyecto iniciado el 8 de Abril del 2016
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

// CollisionMap class
#include "map.h"




/*** Mensages de error ***/
void ErrorMsg() {
    std::cout << "Command line error." << std::endl;
    std::cout << "Usage: NGN_CollisionMap INPUT_FILE.PNG [-o filename]" << std::endl;
    std::cout << "-o filename   Name for the output file, without extension (.map will be added automaticaly)." << std::endl;
}




/*** Main ***/
int32_t main(int32_t argc, char* args[]) {

    // Variables
    std::string in_file = "";       // Nombre del archivo de entrada
    std::string out_file = "";      // Nombre base de los archivos de salida

    // Texto de bienvenida
    std::cout << std::endl << "N'gine PNG to Collision Map converter." << std::endl << "(cc) 2016 - 2017 by Cesar Rincon." << std::endl << "http://www.nightfoxandco.com" << std::endl << "contact@nightfoxandco.com" << std::endl << std::endl;

    // Variables de control
    bool _out_file = false;

    // Verifica la linea de comandos
    if ((argc < 2) || (argc > 4)) {     // Nº de argumentos incorrecto

        ErrorMsg();
        return 1;

    } else {

        // Archivo de entrada
        in_file = args[1];
        if ((in_file.length() < 5) || (out_file.length() > 240)) {
            ErrorMsg();
            return 1;
        }

        // Analiza los argumentos extra
        for (int32_t i = 2; i < argc; i ++) {

            if (strcmp(args[i], "-o") == 0) {      // Comando -O
                if (((i + 1) < argc) && !_out_file) {
                    _out_file = true;
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
            }
        }
    }
    std::cout << std::endl;

    // Nombre del archivo de salida
    if (out_file == "") out_file = in_file;
    out_file += ".map";


    /*** Parametros aceptados, crea el objeto principal ***/
    CollisionMap* cmap = new CollisionMap();

    // Registra los parametros del mapa a convertir
    cmap->input_filename = in_file;
    cmap->output_filename = out_file;

    // Carga y convierte la imagen PNG en un mapa de colisiones
    cmap->ConvertPng();

    /*** Proceso completado, destruye el objeto principal ***/
    delete cmap;


    // Fin del programa
    return 0;

}
