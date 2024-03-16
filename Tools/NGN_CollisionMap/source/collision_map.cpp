/******************************************************************************

    Conversor de PNG a Mapa de Colisiones (.map) para N'gine
    - Nucleo del programa -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Mapa de Colisiones is under MIT License

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



/*** Includes ***/

// C++
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


// Includes del proyecto
#include "collision_map.h"
#include "defines.h"
#include "message.h"
#include "convert_to_map.h"




/*** Constructor ***/
CollisionMap::CollisionMap(int32_t argc, char* args[]) {


    // Guarda el numero de argumentos
    argument_count = argc;

    // General al lista de argumentos
    argument_list.clear();
    argument_list.resize(argument_count);
    for (int32_t i = 0; i < argument_count; i ++) argument_list[i] = args[i];

    // Parametros
    parameter.in_file = "";
    parameter.out_file = "";
    parameter.tile_size = 0;

    // Crea los objetos adicionales
    msg = new Message(argument_list[0]);
    fs = new FsManager();
    png2map = new ConvertToMap();

    // Flags de argumentos
    arg_help.state = false; arg_help.value = "";
    arg_in_file.state = false; arg_in_file.value = "";
    arg_out_file.state = false; arg_out_file.value = "";
    arg_tile_size.state = false; arg_tile_size.value = "";

}



/*** Destructor ***/
CollisionMap::~CollisionMap() {

    // Elimina la lista de argumentos
    argument_list.clear();

    // Elimina los objetos adicionales
    delete png2map; png2map = NULL;
    delete fs; fs = NULL;
    delete msg; msg = NULL;

}



/*** Programa principal ***/
int32_t CollisionMap::Run() {

    // Resultado de la ejecucion
    int32_t r = ERR_CODE_OK;

    // Texto de cabecera
    msg->AppHeader();

    // Analiza la lista de argumentos
    int32_t arglist = CheckArguments();
    //DebugArgList();

    // Analisis de los posibles errores en la linea de comandos / peticion de ayuda
    if (arglist == 0) {
        // Si no se ha especificado ningun argumento
        msg->QuestionMarkForHelp();
    } else if (arglist < 0) {
        // Uso incorrecto
        msg->UseError();
        msg->UserManual();
        r = ERR_CODE_ARG_INVALID;
    } else if (arg_help.state) {
        // Manual en pantalla
        msg->UserManual();
    } else {
        // Valida los parametros de entrada
        if (ValidateParameters()) {
            Report();
            if (
                !png2map->Convert(
                parameter.in_file,
                parameter.out_file,
                parameter.tile_size
                )
            ) r = ERR_CODE_CONVERSION_FAILURE;
        } else {
            msg->QuestionMarkForHelp();
            r = ERR_CODE_ARG_INVALID;
        }
    }

    // Resultado de la ejecucion
    return r;

}



/*** Analiza la lista de argumentos proporcionados ***/
int32_t CollisionMap::CheckArguments() {

    // Si no hay argumentos, muestra el texto de ayuda
    if (argument_count == 1) return 0;

    // Si el numero de argumentos es incorrecto
    if ((argument_count < ARG_MIN_NUM) || (argument_count > ARG_MAX_NUM)) return -1;

    // Flag de argumentos validos
    bool arg = false;
    bool valid = false;

    // Analiza la lista de argumentos
    for (uint32_t i = 1; i < argument_list.size(); i ++) {

        valid = false;

        if (argument_list[i] == ARG_HELP) {                         // Texto de ayuda

            if (arg_help.state) return -1;
            arg_help.state = true;
            valid = true;

        } else if (argument_list[i] == ARG_OUT_FILE) {              // Archivo de salida

            if (arg_out_file.state) return -1;
            if ((i + 1) >= argument_list.size()) return -1;
            arg_out_file.state = true;
            i ++;
            arg_out_file.value = argument_list[i];
            valid = true;

        } else if (argument_list[i] == ARG_TILE_SIZE) {             // tamaño del tile

            if (arg_tile_size.state) return -1;
            if ((i + 1) >= argument_list.size()) return -1;
            arg_tile_size.state = true;
            i ++;
            arg_tile_size.value = argument_list[i];
            valid = true;

        } else if (i == 1) {

            if (arg_in_file.state) return -1;
            arg_in_file.state = true;
            arg_in_file.value = argument_list[i];
            valid = true;

        }

        // Si el argumento no se reconoce...
        if (!valid) return -1;

        // Argumento valido?
        arg |= valid;

    }

    // Indica los argumentos se han leido correctamente
    return (arg) ? 1:-1;

}



/*** Valida los parametros de entrada ***/
bool CollisionMap::ValidateParameters() {

    // Resultado
    bool r = true;

    // Verifica si el archivo de origen existe
    if (!fs->CheckIfFileExist(arg_in_file.value)) {
        std::cout << arg_in_file.value << " file not found." << std::endl;
        r = false;
    } else {
        parameter.in_file = arg_in_file.value;
    }


    // Nombre base de los archivos de salida
    if (arg_out_file.state) {
        parameter.out_file = arg_out_file.value;
    } else {
        parameter.out_file = parameter.in_file.substr(0, (parameter.in_file.length() - 4));
    }


    // tamaño del tile
    if (arg_tile_size.state) {
        parameter.tile_size = std::stoi(arg_tile_size.value);
        if ((parameter.tile_size < MIN_TILE_SIZE) || (parameter.tile_size > MAX_TILE_SIZE)) {
            std::cout << "Tile size out of range." << std::endl;
            r = false;
        }
    } else {
        parameter.tile_size = DEFAULT_TILE_SIZE;
    }

    // Parametros validados
    return r;

}



/*** Resumen de los datos recopilados ***/
void CollisionMap::Report() {

    // Informacion de los parametros introducidos
    std::cout << "Output filename: " << parameter.out_file << MAP_EXTENSION << std::endl;
    std::cout << "Tile size: " << parameter.tile_size << "x" << parameter.tile_size << " pixels." << std::endl;

}



/*** DEBUG: Imprime la lista de argumentos introducidos ***/
void CollisionMap::DebugArgList() {

    std::cout << std::endl;
    std::cout << "Argument list" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Executable name: " << argument_list[0] << std::endl;
    std::cout << "Input file: " << arg_in_file.state << " " << arg_in_file.value << std::endl;
    std::cout << ARG_HELP << " " << arg_help.state << " " << arg_help.value << std::endl;
    std::cout << ARG_OUT_FILE << " " << arg_out_file.state << " " << arg_out_file.value << std::endl;
    std::cout << ARG_TILE_SIZE << " " << arg_tile_size.state << " " << arg_tile_size.value << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::endl;

}
