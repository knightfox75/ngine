/******************************************************************************

    Conversor de PNG a Fondo de Tiles (.tbg) para N'gine
    - Convierte un archivo PNG en tiles -

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

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
            uint32_t tile_size,         // Tamaño del tile
            uint32_t op_level,          // Nivel de optimizacion
            bool ex_files               // Generar archivos adicionales?
        );


    private:

        /*** Propiedades ***/

        // Parametros de funcionamiento
        uint32_t size_of_tile;                      // Tamaño del tile [32x32 por defecto]
        uint32_t optimize;                          // Nivel de optimizacion
        bool extra_files;                           // Generar archivos adicionales?

        // Parametros de las imagenes
        uint32_t bg_width, bg_height;               // Tamaño real del fondo
        uint32_t map_width, map_height;             // Tamaño del mapa a generar
        uint32_t out_width, out_height;             // Tamaño del archivo de salida
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
        // Compara si 2 tiles estan rotadas 90º a la derecha (90º Clock Wise)
        bool Compare90degCW();
        // Compara si 2 tiles estan rotadas 90º a la izquierda (90º Anti Clock Wise)
        bool Compare90degACW();

};



#endif // CONVERT_TO_TILES_H_INCLUDED
