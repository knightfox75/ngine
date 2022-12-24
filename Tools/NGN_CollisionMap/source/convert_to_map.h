/******************************************************************************

    Conversor de PNG a Mapa de Colisiones (.map) para N'gine
    - Convierte un archivo PNG en tiles -

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



#ifndef CONVERT_TO_MAP_H_INCLUDED
#define CONVERT_TO_MAP_H_INCLUDED



/*** Includes ***/
// C++
#include <vector>
#include <string>
// Proyecto
#include "defines.h"



/*** Declaracion de la clase ***/
class ConvertToMap {

    public:

        // Constructor
        ConvertToMap();

        // Destructor
        ~ConvertToMap();

        /*** Propiedades ***/



        /*** Metodos ***/

        // Convierte un archivo PNG en tiles
        bool Convert(
            std::string in_file,        // Archivo PNG a convertir
            std::string out_file,       // Nombre base de los archivos de salida
            uint32_t tile_size          // Tamaño del tile
        );


    private:

        /*** Propiedades ***/

        // Parametros del mapa
        uint32_t map_width;                 // Ancho del mapa
        uint32_t map_height;                // Altura del mapa
        uint32_t size_of_tile;              // Tamaño del tile
        uint32_t pal_length;                // Tamaño de la paleta
        uint32_t tileset_length;            // Tamaño del tileset
        uint32_t map_length;                // Tamaño del mapa

        // Define los vectores de memoria para el archivo de guardado
        std::vector<uint32_t> palette;      // Buffer para almacenar la paleta de colores
        std::vector<uint8_t> tiles;         // Buffer para almacenar los pixeles de los tiles
        std::vector<uint32_t> tmap;         // Buffer para almacenar el mapa de tiles

        // Define los vectores de memoria para datos temporales
        std::vector<uint8_t> bitmap;        // Buffer para almacenar el mapa de pixeles


        /*** Metodos ***/

        // Genera un bitmap + paleta a partir de la imagen
        bool GenerateBitmap(std::vector<uint8_t> &data);

        // Genera una mapa de tiles a partir del bitmap
        void GenerateTileMap();

        // Graba el archivo empaquetado
        bool WriteFile(std::string filename);

        // Lee el archivo .PNG y almacena los pixeles de la imagen en el buffer
        bool ReadPng(std::string filename, std::vector<uint8_t> &data);

        // Lee un pixel del buffer especificado
        uint32_t GetPixel(std::vector<uint8_t> &data, uint32_t offset);

        // Lee un tile del buffer especificado
        void GetTile(std::vector<uint8_t> &data, uint32_t pos_x, uint32_t pos_y);

};



#endif // CONVERT_TO_MAP_H_INCLUDED
