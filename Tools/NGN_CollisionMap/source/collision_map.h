/******************************************************************************

    Conversor de PNG a Mapa de Colisiones (.map) para N'gine
    - Nucleo del programa -

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



#ifndef COLLISION_MAP_H_INCLUDED
#define COLLISION_MAP_H_INCLUDED



/*** Includes ***/
// C++
#include <vector>
#include <string>
// Proyecto
#include "defines.h"
#include "message.h"
#include "fs_manager.h"
#include "convert_to_map.h"



/*** Declaracion de la clase ***/
class CollisionMap {

    public:

        // Constructor
        CollisionMap(int32_t argc, char* args[]);

        // Destructor
        ~CollisionMap();

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
        argument arg_tile_size;         // Tamaño del tile

        // Argumentos introducidos
        int32_t argument_count;                             // Numero de argumentos en la linea de comandos
        std::vector<std::string> argument_list;             // Lista con los argumentos proporcionados

        // Parametros de la conversion
        struct {
            std::string in_file;            // Archivo de entrada
            std::string out_file;           // Archivo de salida
            int32_t tile_size;              // Tamaño del tile
        } parameter;


        /*** Objetos ***/
        Message* msg;                   // Gestor de mensages
        FsManager* fs;                  // Gestor de archivos del sistema
        ConvertToMap* png2map;          // Conversor de PNG a mapa de colision


        /*** Metodos para la lectura de parametros***/
        int32_t CheckArguments();       // Analiza la lista de argumentos proporcionados
        bool ValidateParameters();      // Valida los parametros de entrada
        void Report();                  // Resumen de los datos


        /*** Metodos de depuracion ***/
        void DebugArgList();            // Imprime la lista de argumentos introducidos

};



#endif // COLLISION_MAP_H_INCLUDED
