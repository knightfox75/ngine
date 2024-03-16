/******************************************************************************

    Conversor de PNG a Fondo de Tiles (.tbg) para N'gine
    - Convierte un archivo PNG en tiles -

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



#ifndef CONVERT_TO_TILES_H_INCLUDED
#define CONVERT_TO_TILES_H_INCLUDED



/*** Includes ***/
// C++
#include <vector>
#include <string>
// Proyecto
#include "defines.h"



/*** Declaracion de la clase ***/
class ConvertToTiles {

    public:

        // Constructor
        ConvertToTiles();

        // Destructor
        ~ConvertToTiles();

        /*** Propiedades ***/



        /*** Metodos ***/

        // Convierte un archivo PNG en tiles
        bool Convert(
            std::string in_file,        // Archivo PNG a convertir
            std::string out_file,       // Nombre base de los archivos de salida
            uint32_t tile_size,         // Tama�o del tile
            uint32_t op_level,          // Nivel de optimizacion
            bool ex_files               // Generar archivos adicionales?
        );


    private:

        /*** Propiedades ***/

        // Parametros de funcionamiento
        uint32_t size_of_tile;                      // Tama�o del tile [32x32 por defecto]
        uint32_t optimize;                          // Nivel de optimizacion
        bool extra_files;                           // Generar archivos adicionales?

        // Parametros de las imagenes
        uint32_t bg_width, bg_height;               // Tama�o real del fondo
        uint32_t map_width, map_height;             // Tama�o del mapa a generar
        uint32_t out_width, out_height;             // Tama�o del archivo de salida
        uint32_t tileset_length;                    // Numero de tiles unicos del tileset

        // Buffers de datos
        std::vector<uint8_t> tiles;                 // Buffer para almacenar los tiles del tileset
        std::vector<uint8_t> tmap;                  // Buffer para almacenar el mapa
        std::vector<uint8_t> img_tile;              // Almacena un tile de la imagen de Origen
        std::vector<uint8_t> map_tile;              // Almacena un tile del mapa de destino
        std::vector<uint8_t> buffer;                // Buffer temporal de datos


        /*** Metodos ***/

        // Convierte la imagen en tiles + mapa
        void GenerateTileset(std::vector<uint8_t> &data);

        // Graba el archivo empaquetado
        bool WriteFile(std::string filename);

        // Lee el archivo .PNG y almacena los pixeles de la imagen en el buffer
        bool ReadPng(std::string filename, std::vector<uint8_t> &data);
        // Graba un buffer de pixeles en un archivo PNG
        bool WritePng(std::string filename, std::vector<uint8_t> &data, uint32_t width, uint32_t height);

        // Lee un pixel del buffer especificado
        RgbaPixel GetPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w);
        // Escribe el pixel en el buffer especificado
        void PutPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w, RgbaPixel pixel);

        // Lee un tile del buffer especificado
        void GetTile(std::vector<uint8_t> &tl, std::vector<uint8_t> &img, uint32_t x, uint32_t y, uint32_t max_w, uint32_t max_h);
        // Escribe un tile en el buffer especificado
        void PutTile(std::vector<uint8_t> &tl, std::vector<uint8_t> &img, uint32_t x, uint32_t y, uint32_t w);

        // Realiza la optimizacion de tiles unicos
        uint32_t OptimizeTiles();
        // Compara si 2 tiles son identicos (img_tile vs map_tile)
        bool CompareNormal();
        // Compara si 2 tiles estan en espejo horizontal (H FLIP)
        bool CompareFlipH();
        // Compara si 2 tiles estan en espejo vertical (V FLIP)
        bool CompareFlipV();
        // Compara si 2 tiles estan en espejo completo (H & V FLIP)
        bool Compare180deg();
        // Compara si 2 tiles estan rotadas 90� a la derecha (90� Clock Wise)
        bool Compare90degCW();
        // Compara si 2 tiles estan rotadas 90� a la izquierda (90� Anti Clock Wise)
        bool Compare90degACW();

};



#endif // CONVERT_TO_TILES_H_INCLUDED
