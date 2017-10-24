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
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

// LodePNG
#include "lodepng/lodepng.h"

// CollisionMap class
#include "map.h"



/*** Constructor ***/
CollisionMap::CollisionMap() {

}



/*** Destructor ***/
CollisionMap::~CollisionMap() {

    // Borra los buffers de memoria temporales
    png.clear();
    raw.clear();
    bitmap.clear();
    // Borra los buffers de archivo
    palette.clear();
    tiles.clear();
    tmap.clear();
    // Borra los buffers temporales
    t_palette.clear();
    t_tiles.clear();
    t_tl.clear();

}



/*** Convierte el archivo a un Sprite Sheet del tamaño solicitado ***/
int32_t CollisionMap::ConvertPng() {

    // Intenta cargar el archivo PNG, si hay error sal e informa.
    if (ReadPNG(input_filename, raw) > 0) return 1;
    // Genera el Bitmap de colisiones
    if (GenerateBitmap() > 0) return 1;
    // Genera los tiles y el mapa
    GenerateMap();
    // Genera la cabecera del archivo
    GenerateHeader();
    // Graba los archivos de salida
    if (WriteFile(output_filename) > 0) return 1;
    // Devuelve el resultado
    return 0;

}



/*** Lee el archivo PNG y almacena los pixeles de la imagen en el buffer ***/
int32_t CollisionMap::ReadPNG(std::string filename, std::vector<uint8_t> &data) {

    // Borra los buffers
    png.clear();

    // Carga el archivo PNG
    uint32_t _error = lodepng::load_file(png, filename);

    // Si se ha cargado correctamente, decodifica la imagen
    if (_error == 0) {
        _error = lodepng::decode(raw, in_width, in_height, png);
    } else {
        std::cout << "Error loading " << filename << "." << std::endl;
        png.clear();
        return 1;
    }

    // Verifica que la decodificacion sea la correcta
    if (_error == 0) {
        std::cout << "File " << filename << " loaded & decoded successfully." << std::endl;
        std::cout << "Image size is " << in_width << "x" << in_height << " pixels." << std::endl;
        png.clear();
        return 0;
    } else {
        std::cout << "Error decoding " << filename << "." << std::endl;
        png.clear();
        return 1;
    }


}



/*** Graba el archivo empaquetado ***/
int32_t CollisionMap::WriteFile(std::string filename) {

    // Variables
    uint32_t i = 0;

    for (i = 0; i < sizeof(header.reserve); i ++) header.reserve[i] = 0;

    // Genera el nombre de archivo
    char f[256];
    char basename[256];
    for (i = 0; i < filename.length(); i ++) basename[i] = filename[i];     // Nombre base
    basename[i] = '\0';     // Terminador

    // Espaciado en el texto
    std::cout << std::endl;

    // Graba el archivo principal
    sprintf(f, "%s.map", basename);
    std::ofstream file;
    file.open(f, std::ofstream::out | std::ofstream::binary);
    if (file.is_open()) {
        file.write((char*)&header, sizeof(header));                                 // Cabecera
        file.write((char*)&palette[0], (palette.capacity() * sizeof(int32_t)));    // Paleta
        file.write((char*)&tiles[0], tiles.capacity());                             // Tiles
        file.write((char*)&tmap[0], (tmap.capacity() * sizeof(int32_t)));          // Mapa de tiles
        file.close();
        std::cout << "File " << f << " successfully saved." << std::endl;
    } else {
        std::cout << "Error saving " << f << "." << std::endl;
        return 1;
    }

    // Todo correcto
    return 0;

}




/*** Lee un pixel del buffer especificado ***/
uint32_t CollisionMap::GetPixel(std::vector<uint8_t> &data, uint32_t offset) {

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



/*** Genera el mapa de colisiones ***/
int32_t CollisionMap::GenerateBitmap() {

    // Medidas del mapa
    out_width = in_width;
    out_height = in_height;

    // Prepara los bufferes de destino
    bitmap.clear();
    bitmap.resize((out_width * out_height), 0);
    t_palette.clear();
    t_palette.resize(256, 0);

    // Variables de uso temporal
    uint32_t pixel;         // Alamacena el valor del pixel
    int32_t id = 0;         // Contenedor de ID de la paleta
    uint32_t pal_id = 0;    // Posicion actual en la paleta
    uint32_t offset = 0;    // Posicion actual en el mapa

    // Marcador de progreso
    uint64_t max_pixels = (out_height * out_width);
    uint64_t current_pixel = 0;
    uint64_t percent = 0, old_percent = 0;
    char s[16];
    std::cout << std::endl;
    std::cout << "Generating bitmap & palette..." << std::endl;

    // Identifica el color de cada pixel e identificalo en la paleta, de no existir, añadelo
    for (uint32_t y = 0; y < out_height; y ++) {
        for (uint32_t x = 0; x < out_width; x ++) {
            // Color del pixel actual
            offset = ((y * out_width) + x);
            pixel = GetPixel(raw, (offset << 2));
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
                    return 1;
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
    std::cout << "Palette generated with " << pal_id << " colors." << std::endl;

    // Guarda el tamaño del buffer de la paleta
    pal_length = pal_id;

    // Copia la paleta temporal a la final
    palette.clear();
    palette.resize(pal_length, 0);
    for (uint32_t n = 0; n < pal_length; n ++) palette[n] = t_palette[n];
    t_palette.clear();

    // Todo Ok
    return 0;

}



/*** Genera la cabecera del archivo ***/
void CollisionMap::GenerateHeader() {

    // Actualiza la informacion de la cabecera
    header.version = VERSION;                           // Version del programa
    sprintf(header.magic, MAGIC_STRING);                // Magic String
    header.width = out_width;                           // Tamaño del mapa en pixeles
    header.height = out_height;
    header.tile_size = tile_size;                       // Tamaño del tile
    header.pal_length = pal_length;                     // nº de elementos de la paleta (*4)
    header.tileset_length = tileset_length;             // nº de elementos del tileset
    header.map_length = map_length;                     // nº de elementos del mapa (*4)
    for (uint32_t i = 0; i < sizeof(header.reserve); i ++) header.reserve[i] = 0;    // Espacio reservado

}



/*** Genera el tileset y el mapa ***/
void CollisionMap::GenerateMap() {

    // Prepara el buffer para almacenar el mapa de tiles
    uint32_t width = (out_width / tile_size);
    if ((out_width % tile_size) != 0) width ++;
    uint32_t height = (out_height / tile_size);
    if ((out_height % tile_size) != 0) height ++;

    // Aviso de ajuste del tamaño de tiles
    std::cout << std::endl;
    if (((out_width % tile_size) != 0) || (out_height % tile_size)) {
        std::cout << "WARNING: Image size doesn't fits the tile size." << std::endl;
        std::cout << "Tilemap size will be adjusted to the right size." << std::endl;
    }

    // Datos para el indicador de progreso
    uint64_t max_tiles = (height * width);
    uint64_t current_tile = 0;
    uint64_t percent = 0, old_percent = 0;
    char s[16];
    std::cout << "Generating a tilemap of " << width << "x" << height <<" tiles." << std::endl;

    // Crea el buffer para el mapa
    map_length = (width * height);
    tmap.clear();
    tmap.resize(map_length, 0);

    // Crea el buffer temporal para el tileset
    uint32_t tile_length = (tile_size * tile_size);
    uint32_t l = (width * height * tile_length);
    t_tiles.clear();
    t_tiles.resize(l, 0);

    // Control de tile nuevo
    bool new_tile = false, miss_match = false;
    uint32_t last_tile = 0;
    uint32_t offset = 0;
    uint32_t t_num = 0;

    // Recorre todo el mapa analizando los tiles
    for (uint32_t ty = 0; ty < height; ty ++) {
        for (uint32_t tx = 0; tx < width; tx ++) {
            // Reset del flag
            new_tile = true;
            // Copia un tile del bitmap
            GetTile(tx, ty);
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
                // Si no hay discrepancia, añade el tile actual al mapa
                if (!miss_match) {
                    t_num = ((ty * width) + tx);
                    tmap[t_num] = i;
                    new_tile = false;
                    break;
                }
            }
            // Si no se encuentra coincidencia, añade el tile
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
    t_tiles.clear();

    // Resultado
    std::cout << "done!" << std::endl;
    std::cout << "Tilemap with " << last_tile << " tiles created successfully." << std::endl;

}



/*** Obten un tile del bitmap ***/
void CollisionMap::GetTile(uint32_t pos_x, uint32_t pos_y) {

    // Prepara el buffer de salida
    t_tl.clear();
    t_tl.resize(tile_size * tile_size);
    // Calcula los puntos de corte
    uint32_t start_x = (pos_x * tile_size);
    uint32_t end_x = (start_x + tile_size);
    uint32_t start_y = (pos_y * tile_size);
    uint32_t end_y = (start_y + tile_size);
    // Copia los datos del buffer al tile
    uint32_t px = 0;
    uint32_t offset = 0;
    for (uint32_t y = start_y; y < end_y; y ++) {
        for (uint32_t x = start_x; x < end_x; x ++) {
            if ((y < out_height) && (x < out_width)) {
                offset = ((out_width * y) + x);
                t_tl[px] = bitmap[offset];
            } else {
                t_tl[px] = 0;
            }
            px ++;
        }
    }

}
