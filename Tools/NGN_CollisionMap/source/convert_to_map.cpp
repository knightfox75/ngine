/******************************************************************************

    Conversor de PNG a Mapa de Colisiones (.map) para N'gine
    - Convierte un archivo PNG en tiles -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Mapa de Colisiones is under MIT License

	Copyright (c) 2016-2023 by Cesar Rincon "NightFox"

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
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

// LodePNG
#include "lodepng/lodepng.h"

// Includes del proyecto
#include "convert_to_map.h"
#include "defines.h"



/*** Constructor ***/
ConvertToMap::ConvertToMap() {

    map_width = 0;                  // Ancho del mapa
    map_height = 0;                 // Altura del mapa
    size_of_tile = 0;               // tamaño del tile
    pal_length = 0;                 // tamaño de la paleta
    tileset_length = 0;             // tamaño del tileset
    map_length = 0;                 // tamaño del mapa

    palette.clear();
    tiles.clear();
    tmap.clear();

    bitmap.clear();

}



/*** Destructor ***/
ConvertToMap::~ConvertToMap() {

    bitmap.clear();

    palette.clear();
    tiles.clear();
    tmap.clear();

}



/*** Convierte un archivo PNG en tiles ***/
bool ConvertToMap::Convert(
    std::string in_file,        // Archivo PNG a convertir
    std::string out_file,       // Nombre base de los archivos de salida
    uint32_t tile_size          // tamaño del tile
) {

    // Guarda los parametros
    size_of_tile = tile_size;

    // Prepara el buffer para los pixeles de la imagen
    std::vector<uint8_t> png_pixels;
    png_pixels.clear();

    // Intenta abrir y decodificar el archivo PNG
    if (!ReadPng(in_file, png_pixels)) return false;

    // Convierte el PNG a BITMAP + PALETA
    if (!GenerateBitmap(png_pixels)) return false;
    png_pixels.clear();

    // Genera un tilemap con los datos del bitmap
    GenerateTileMap();

    // Guarda en el archivo empaquetado los datos generados
    bool r = WriteFile(out_file);

    // Elimina los buffers
    bitmap.clear();
    palette.clear();
    tiles.clear();
    tmap.clear();

    // Conversion correcta
    return r;

}



/*** Convierte la imagen en bitmap + paleta ***/
bool ConvertToMap::GenerateBitmap(std::vector<uint8_t> &data) {

    // Prepara los bufferes de destino
    bitmap.clear();
    bitmap.resize((map_width * map_height), 0);

    // Prepara los buffers temporales
    std::vector<uint32_t> t_palette;
    t_palette.clear();
    t_palette.resize(256, 0);

    // Variables de uso temporal
    uint32_t pixel;         // Alamacena el valor del pixel
    int32_t id = 0;         // Contenedor de ID de la paleta
    uint32_t pal_id = 0;    // Posicion actual en la paleta
    uint32_t offset = 0;    // Posicion actual en el mapa

    // Marcador de progreso
    uint64_t max_pixels = (map_width * map_height);
    uint64_t current_pixel = 0;
    uint64_t percent = 0, old_percent = 0;
    char s[16];
    std::cout << std::endl;
    std::cout << "Generating bitmap & palette..." << std::endl;

    // Identifica el color de cada pixel e identificalo en la paleta, de no existir, a�adelo
    for (uint32_t y = 0; y < map_height; y ++) {
        for (uint32_t x = 0; x < map_width; x ++) {
            // Color del pixel actual
            offset = ((y * map_width) + x);
            pixel = GetPixel(data, (offset << 2));
            // Reset de ID
            id = -1;
            // Si el color coincide con alguno de la paleta...
            for (uint32_t c = 0; c < t_palette.size(); c ++) {
                if (pixel == t_palette[c]) {
                    id = c;
                    break;
                }
            }
            // Si es un nuevo color, indicalo
            if (id < 0) {
                if (pal_id > 255) {
                    std::cout << "Image format not compatible. The image must be indexed at 256 colors." << std::endl;
                    bitmap.clear();
                    t_palette.clear();
                    return false;
                }
                // Guarda el nuevo color
                t_palette[pal_id] = pixel;
                // Guarda el pixel
                bitmap[offset] = (0xff & ((uint8_t)(pal_id)));
                // Siguente PAL ID
                pal_id ++;
            } else {
                // Guarda el Pixel
                bitmap[offset] = (0xff & ((uint8_t)(id)));
            }
            // Informacion del progreso
            current_pixel ++;
            percent = ((current_pixel * 1000) / max_pixels);
            if (percent != old_percent) {
                sprintf(s, "%05.1f%%", ((float)percent / 10.0f));
                std::cout << "\x0d" << s << " ";
                old_percent = percent;
            }
            //std::cout << "OFFSET:" << offset << "   COLOR:" << pixel << " ID:" << (int32_t)_id << "  BITMAP:" << (int32_t)bitmap[offset] << std::endl;
        }
    }

    // Mensage del finde del progreso
    std::cout << "done!" << std::endl;
    std::cout << "Bitmap generation completed." << std::endl;
    std::cout << "Palette generation completed. The palette contains " << pal_id << " colors." << std::endl;

    // Guarda el tamaño del buffer de la paleta
    pal_length = pal_id;

    // Copia la paleta temporal a la final
    palette.clear();
    palette.resize(pal_length, 0);
    for (uint32_t n = 0; n < pal_length; n ++) palette[n] = t_palette[n];
    t_palette.clear();

    // Todo Ok
    return true;

}



/*** Genera una mapa de tiles a partir del bitmap ***/
void ConvertToMap::GenerateTileMap() {

    // Aviso de ajuste del tamaño de tiles
    std::cout << std::endl;
    if (((map_width % size_of_tile) != 0) || (map_height % size_of_tile) != 0) {
        std::cout << "WARNING: Image size doesn't fits the tile size." << std::endl;
        std::cout << "The size of the Tilemap will be adjusted to the closest right size." << std::endl;
    }

    // Prepara el buffer para almacenar el mapa de tiles
    uint32_t width = (map_width / size_of_tile);
    if ((map_width % size_of_tile) != 0) width ++;
    uint32_t height = (map_height / size_of_tile);
    if ((map_height % size_of_tile) != 0) height ++;

    // Crea el buffer para el mapa
    map_length = (width * height);
    tmap.clear();
    tmap.resize(map_length, 0);

    // Crea el buffer temporal para el tileset
    uint32_t tile_length = (size_of_tile * size_of_tile);
    uint32_t l = (width * height * tile_length);
    std::vector<uint8_t> t_tiles;   // Buffer para almacenar los pixeles de los tiles
    t_tiles.clear();
    t_tiles.resize(l, 0);

    // Crea el buffer temporal para el tile actual a analizar
    std::vector<uint8_t> t_tl;
    t_tl.clear();

    // Control de tile nuevo
    bool new_tile = false, miss_match = false;
    uint32_t last_tile = 0;
    uint32_t offset = 0;
    uint32_t t_num = 0;

    // Datos para el indicador de progreso
    uint64_t max_tiles = (height * width);
    uint64_t current_tile = 0;
    uint64_t percent = 0, old_percent = 0;
    char s[16];
    std::cout << "Generating a tilemap of " << width << "x" << height <<" tiles." << std::endl;

    // Recorre todo el mapa analizando los tiles
    for (uint32_t ty = 0; ty < height; ty ++) {
        for (uint32_t tx = 0; tx < width; tx ++) {
            // Reset del flag
            new_tile = true;
            // Copia un tile del bitmap
            GetTile(t_tl, tx, ty);
            // Comparalo con todos los tiles previamente guardados
            for (uint32_t i = 0; i < last_tile; i ++) {
                offset = (i * tile_length);
                miss_match = false;
                for (uint32_t n = 0; n < tile_length; n ++) {
                    if (t_tl[n] != t_tiles[(offset + n)]) {
                        miss_match = true;
                        break;
                    }
                }
                // Si no hay discrepancia, a�ade el tile actual al mapa
                if (!miss_match) {
                    t_num = ((ty * width) + tx);
                    tmap[t_num] = i;
                    new_tile = false;
                    break;
                }
            }
            // Si no se encuentra coincidencia, a�ade el tile
            if (new_tile) {
                t_num = ((ty * width) + tx);
                tmap[t_num] = last_tile;
                offset = (last_tile * tile_length);
                for (uint32_t n = 0; n < tile_length; n ++) t_tiles[(offset + n)] = t_tl[n];
                last_tile ++;
            }
            // Informacion del progreso
            current_tile ++;
            percent = ((current_tile * 1000) / max_tiles);
            if (percent != old_percent) {
                sprintf(s, "%05.1f%%", ((float)percent / 10.0f));
                std::cout << "\x0d" << s << " ";
                old_percent = percent;
            }
        }
    }

    // Copia el tileset final al buffer de destino
    tileset_length = (last_tile * tile_length);
    tiles.clear();
    tiles.resize(tileset_length, 0);
    for (uint32_t n = 0; n < tileset_length; n ++) tiles[n] = t_tiles[n];

    // Borra los buffers temporales
    t_tl.clear();
    t_tiles.clear();

    // Resultado
    std::cout << "done!" << std::endl;
    std::cout << "Tilemap with " << last_tile << " tiles created successfully." << std::endl << std::endl;

}



/*** Graba el archivo empaquetado ***/
bool ConvertToMap::WriteFile(std::string filename) {

    // Guarda la informacion para la cabecera del archivo
    FileHeader header;
    memset((void*)&header, 0, sizeof(header));
    header.version = VERSION;
    strncpy(header.magic, MAGIC_STRING.c_str(), MAGIC_STRING.length());
    header.width = map_width;                                                       // tamaño del mapa en pixeles
    header.height = map_height;
    header.tile_size = size_of_tile;                                                // tamaño del tile
    header.pal_length = pal_length;                                                 // n� de elementos de la paleta (*4)
    header.tileset_length = tileset_length;                                         // n� de elementos del tileset
    header.map_length = map_length;                                                 // n� de elementos del mapa (*4)

    // Genera el nombres de archivo
    std::string fname = filename + MAP_EXTENSION;
    const char* map_filename = fname.c_str();

    // Graba el archivo principal
    std::ofstream file;
    file.open(map_filename, std::ofstream::out | std::ofstream::binary);
    if (file.is_open()) {
        file.write((char*)&header, sizeof(header));                                 // Cabecera
        file.write((char*)&palette[0], (palette.capacity() * sizeof(int32_t)));    // Paleta
        file.write((char*)&tiles[0], tiles.capacity());                             // Tiles
        file.write((char*)&tmap[0], (tmap.capacity() * sizeof(int32_t)));          // Mapa de tiles
        file.close();
        std::cout << "File " << fname << " successfully saved." << std::endl;
    } else {
        std::cout << "Error saving " << fname << "." << std::endl;
        return false;
    }

    // Todo correcto
    return true;

}



/*** Lee un archivo .PNG y coloca los pixeles en un buffer ***/
bool ConvertToMap::ReadPng(std::string filename, std::vector<uint8_t> &data) {

    // Variables
    uint32_t width = 0, height = 0;     // tamaño del archivo cargado

    // Prepara el buffer temporal
    std::vector<uint8_t> png_data;
    png_data.clear();

    // Prepara el buffer para los datos decodificados
    data.clear();

    // Carga el archivo PNG
    if (lodepng::load_file(png_data, filename) != 0) {
        std::cout << "Error loading " << filename << "." << std::endl;
        png_data.clear();
        return false;
    }

    // Si se ha cargado correctamente, decodifica la imagen
    if (lodepng::decode(data, width, height, png_data) != 0) {
        std::cout << "Error decoding " << filename << "." << std::endl;
        png_data.clear();
        data.clear();
        return false;
    }

    // Guarda el tamaño del mapa
    map_width = width;
    map_height = height;

    // Borra el buffer temporal
    png_data.clear();

    // Fin de la funcion de carga
    return true;

}



/*** Lee un pixel del buffer especificado ***/
uint32_t ConvertToMap::GetPixel(std::vector<uint8_t> &data, uint32_t offset) {

    // Variables
    uint32_t pixel = 0;

    // Lee los bits del pixel
    /*if ((offset + 3) < data.capacity()) {*/
    pixel = data[(offset + 3)] | (data[(offset + 2)] << 8) | (data[(offset + 1)] << 16) | (data[offset] << 24);
    /*} else {
        pixel.r = pixel.g = pixel.b = pixel.a = 0;
        std::cout << "Pixel read out of range." << std::endl;
    }*/

    return pixel;

}



/*** Obten un tile del bitmap ***/
void ConvertToMap::GetTile(std::vector<uint8_t> &data, uint32_t pos_x, uint32_t pos_y) {

    // Prepara el buffer de salida
    data.clear();
    data.resize(size_of_tile * size_of_tile);
    // Calcula los puntos de corte
    uint32_t start_x = (pos_x * size_of_tile);
    uint32_t end_x = (start_x + size_of_tile);
    uint32_t start_y = (pos_y * size_of_tile);
    uint32_t end_y = (start_y + size_of_tile);
    // Copia los datos del buffer al tile
    uint32_t px = 0;
    uint32_t offset = 0;
    for (uint32_t y = start_y; y < end_y; y ++) {
        for (uint32_t x = start_x; x < end_x; x ++) {
            if ((y < map_height) && (x < map_width)) {
                offset = ((map_width * y) + x);
                data[px] = bitmap[offset];
            } else {
                data[px] = 0;
            }
            px ++;
        }
    }

}
