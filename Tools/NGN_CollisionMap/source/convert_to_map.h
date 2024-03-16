/******************************************************************************

    Conversor de PNG a Mapa de Colisiones (.map) para N'gine
    - Convierte un archivo PNG en tiles -

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
            uint32_t tile_size          // tamaño del tile
        );


    private:

        /*** Propiedades ***/

        // Parametros del mapa
        uint32_t map_width;                 // Ancho del mapa
        uint32_t map_height;                // Altura del mapa
        uint32_t size_of_tile;              // tamaño del tile
        uint32_t pal_length;                // tamaño de la paleta
        uint32_t tileset_length;            // tamaño del tileset
        uint32_t map_length;                // tamaño del mapa

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
