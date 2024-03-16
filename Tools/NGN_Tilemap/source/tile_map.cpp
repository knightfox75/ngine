/******************************************************************************

    Conversor de PNG a Fondo de Tiles (.tbg) para N'gine
    - Nucleo del programa -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016-2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Fondo de Tiles is under MIT License

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
#include "tile_map.h"
#include "defines.h"
#include "message.h"
#include "convert_to_tiles.h"




/*** Constructor ***/
TileMap::TileMap(int32_t argc, char* args[]) {


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
    parameter.optimization_level = 0;
    parameter.generate_extra_files = false;

    // Crea los objetos adicionales
    msg = new Message(argument_list[0]);
    fs = new FsManager();
    png2tiles = new ConvertToTiles();

    // Flags de argumentos
    arg_help.state = false; arg_help.value = "";
    arg_in_file.state = false; arg_in_file.value = "";
    arg_out_file.state = false; arg_out_file.value = "";
    arg_tile_size.state = false; arg_tile_size.value = "";
    arg_op_level.state = false; arg_op_level.value = "";
    arg_extra_files.state = false; arg_extra_files.value = "";


}



/*** Destructor ***/
TileMap::~TileMap() {

    // Elimina la lista de argumentos
    argument_list.clear();

    // Elimina los objetos adicionales
    delete png2tiles; png2tiles = NULL;
    delete fs; fs = NULL;
    delete msg; msg = NULL;

}



/*** Programa principal ***/
int32_t TileMap::Run() {

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
                !png2tiles->Convert(
                parameter.in_file,
                parameter.out_file,
                parameter.tile_size,
                parameter.optimization_level,
                parameter.generate_extra_files
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
int32_t TileMap::CheckArguments() {

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

        } else if (argument_list[i] == ARG_TILE_SIZE) {             // Tama�o del tile

            if (arg_tile_size.state) return -1;
            if ((i + 1) >= argument_list.size()) return -1;
            arg_tile_size.state = true;
            i ++;
            arg_tile_size.value = argument_list[i];
            valid = true;

        } else if (argument_list[i] == ARG_OP_LEVEL) {              // Nivel de optimizacion

            if (arg_op_level.state) return -1;
            if ((i + 1) >= argument_list.size()) return -1;
            arg_op_level.state = true;
            i ++;
            arg_op_level.value = argument_list[i];
            valid = true;

        } else if (argument_list[i] == ARG_EXTRA_FILES) {           // Genera archivos adicionales

            if (arg_extra_files.state) return -1;
            arg_extra_files.state = true;
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
bool TileMap::ValidateParameters() {

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


    // Tama�o del tile
    if (arg_tile_size.state) {
        parameter.tile_size = std::stoi(arg_tile_size.value);
        if ((parameter.tile_size < MIN_TILE_SIZE) || (parameter.tile_size > MAX_TILE_SIZE)) {
            std::cout << "Tile size out of range." << std::endl;
            r = false;
        }
    } else {
        parameter.tile_size = DEFAULT_TILE_SIZE;
    }

    // Nivel de optimizacion
    if (arg_op_level.state) {
        parameter.optimization_level = std::stoi(arg_op_level.value);
        if ((parameter.optimization_level < 1) || (parameter.optimization_level > 3)) {
            std::cout << "Optimization level out of range." << std::endl;
            r = false;
        }
    } else {
        parameter.optimization_level = DEFAULT_OP_LEVEL;
    }

    // Generacion de archivos adicionales
    parameter.generate_extra_files = arg_extra_files.state;

    // Parametros validados
    return r;

}



/*** Resumen de los datos recopilados ***/
void TileMap::Report() {

    // Informacion de los parametros introducidos
    std::cout << "Output filename: " << parameter.out_file << TBG_EXTENSION << std::endl;
    std::cout << "Tile size: " << parameter.tile_size << "x" << parameter.tile_size << " pixels." << std::endl;
    std::cout << "Optimization level: ";
    switch (parameter.optimization_level) {
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
    std::cout << "Generation of extra files: ";
    if (parameter.generate_extra_files) {
        std::cout << "enabled." << std::endl;
    } else {
        std::cout << "disabled." << std::endl;
    }
    std::cout << std::endl;

}



/*** DEBUG: Imprime la lista de argumentos introducidos ***/
void TileMap::DebugArgList() {

    std::cout << std::endl;
    std::cout << "Argument list" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Executable name: " << argument_list[0] << std::endl;
    std::cout << "Input file: " << arg_in_file.state << " " << arg_in_file.value << std::endl;
    std::cout << ARG_HELP << " " << arg_help.state << " " << arg_help.value << std::endl;
    std::cout << ARG_OUT_FILE << " " << arg_out_file.state << " " << arg_out_file.value << std::endl;
    std::cout << ARG_TILE_SIZE << " " << arg_tile_size.state << " " << arg_tile_size.value << std::endl;
    std::cout << ARG_OP_LEVEL << " " << arg_op_level.state << " " << arg_op_level.value << std::endl;
    std::cout << ARG_EXTRA_FILES << " " << arg_extra_files.state << " " << arg_extra_files.value << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::endl;

}
