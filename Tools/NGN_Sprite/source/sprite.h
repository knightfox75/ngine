/******************************************************************************

    Conversor de PNG a Sprite (.spr) para N'gine
    - Nucleo del programa -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Sprite is under MIT License

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



#ifndef TILE_MAP_H_INCLUDED
#define TILE_MAP_H_INCLUDED



/*** Includes ***/
// C++
#include <vector>
#include <string>
// Proyecto
#include "defines.h"
#include "message.h"
#include "fs_manager.h"
#include "convert_to_sprite.h"



/*** Declaracion de la clase ***/
class Sprite {

    public:

        // Constructor
        Sprite(int32_t argc, char* args[]);

        // Destructor
        ~Sprite();

        /*** Propiedades ***/



        /*** Metodos ***/
        int32_t Run();          // Programa principal


    private:

        /*** Propiedades ***/

        // Flags de los argumentos
        struct argument {
            bool state;
            std::string value;
        };
        argument arg_help;              // Ayuda
        argument arg_in_file;           // Archivo de origen
        argument arg_out_file;          // Archivo de salida
        argument arg_width;             // Tama�o del tile
        argument arg_height;            // Nivel de optimizacion
        argument arg_strip;             // Generacion de un PNG con la tira de fotogramas


        // Argumentos introducidos
        int32_t argument_count;                             // Numero de argumentos en la linea de comandos
        std::vector<std::string> argument_list;             // Lista con los argumentos proporcionados

        // Parametros de la conversion
        struct {
            std::string in_file;            // Archivo de entrada
            std::string out_file;           // Archivo de salida
            int32_t width;                  // Tama�o del tile
            int32_t height;                 // Nivel de optimizacion
            bool generate_strip;            // Generar la tira de fotogramas?
        } parameter;


        /*** Objetos ***/
        Message* msg;                   // Gestor de mensages
        FsManager* fs;                  // Gestor de archivos del sistema
        ConvertToSprite* png2sprite;    // Conversor de PNG a Sprite


        /*** Metodos para la lectura de parametros***/
        int32_t CheckArguments();       // Analiza la lista de argumentos proporcionados
        bool ValidateParameters();      // Valida los parametros de entrada
        void Report();                  // Resumen de los datos


        /*** Metodos de depuracion ***/
        void DebugArgList();            // Imprime la lista de argumentos introducidos

};



#endif // TILE_MAP_H_INCLUDED
