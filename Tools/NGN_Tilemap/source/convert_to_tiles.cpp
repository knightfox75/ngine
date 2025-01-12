/******************************************************************************

    Conversor de PNG a Fondo de Tiles (.tbg) para N'gine
    - Convierte un archivo PNG en tiles -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016-2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Fondo de Tiles is under MIT License

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
#include "convert_to_tiles.h"
#include "defines.h"



/*** Constructor ***/
ConvertToTiles::ConvertToTiles() {

    size_of_tile = 0;
    optimize = 0;
    extra_files = false;

    bg_width = 0;
    bg_height = 0;
    map_width = 0;
    map_height = 0;
    out_width = 0;
    out_height = 0;
    tileset_length = 0;

    tiles.clear();
    tmap.clear();
    img_tile.clear();
    map_tile.clear();
    buffer.clear();

}



/*** Destructor ***/
ConvertToTiles::~ConvertToTiles() {

    tiles.clear();
    tmap.clear();
    img_tile.clear();
    map_tile.clear();
    buffer.clear();

}



/*** Convierte un archivo PNG en tiles ***/
bool ConvertToTiles::Convert(
    std::string in_file,        // Archivo PNG a convertir
    std::string out_file,       // Nombre base de los archivos de salida
    uint32_t tile_size,         // Tama�o del tile
    uint32_t op_level,          // Nivel de optimizacion
    bool ex_files               // Generar archivos adicionales?
) {

    // Guarda los parametros
    size_of_tile = tile_size;
    optimize = op_level;
    extra_files = ex_files;

    // Prepara el buffer para los pixeles de la imagen
    std::vector<uint8_t> png_pixels;
    png_pixels.clear();

    // Intenta abrir y decodificar el archivo PNG
    if (!ReadPng(in_file, png_pixels)) return false;

    // Convierte la imagen cargada a un conjunto de TILES + MAPA, guardado los datos en sus correspondientes buffers
    GenerateTileset(png_pixels);

    // Guarda en el archivo empaquetado los datos generados
    bool r = WriteFile(out_file);

    // Elimina los datos de todos los buffers usados
    tiles.clear();
    tmap.clear();
    img_tile.clear();
    map_tile.clear();
    buffer.clear();

    // Conversion correcta
    return r;

}



/*** Convierte la imagen en tiles + mapa ***/
void ConvertToTiles::GenerateTileset(std::vector<uint8_t> &data) {

    // Inicializa las variables
    tileset_length = 0;

    // Variables
    uint32_t img_x = 0, img_y = 0, total_tiles = 0, idx = 0, t = 0;

    // Borra los buffers a usar
    buffer.clear();
    tiles.clear();
    tmap.clear();

    // Reserva el espacio maximo para el buffer y los contenedores de tiles
    buffer.resize((((map_width * map_height) << 2) + ((size_of_tile * size_of_tile) << 2)), 0);
    img_tile.resize(((size_of_tile * size_of_tile) << 2), 0);
    map_tile.resize(((size_of_tile * size_of_tile) << 2), 0);

    // Marcador...
    uint64_t max_tiles = ((map_width / size_of_tile) * (map_height / size_of_tile));  // Total de tiles
    uint64_t percent = 0, old_percent = 0;
    char s[16];
    std::cout << "Optimizing " << max_tiles << " tiles..." << std::endl;

    // Reserva el espacio para el mapa de tiles
    tmap.resize((max_tiles << 2), 0);

    // Tile vacio al inicio del tileset
    std::vector<uint8_t> empty_tile;
    empty_tile.resize(((size_of_tile * size_of_tile) << 2), 0);
    for (uint32_t n = 0; n < empty_tile.capacity(); n += 4) {
        // Codifica el pixel
        empty_tile[n] = 255;
        empty_tile[(n + 1)] = 255;
        empty_tile[(n + 2)] = 255;
        empty_tile[(n + 3)] = 0;
    }
    // Genera el primer tile del mapa vacio, como referencia
    PutTile(empty_tile, buffer, 0, (tileset_length * size_of_tile), size_of_tile);
    tileset_length ++;
    empty_tile.clear();


    // Convierte toda la imagen a una tira de tiles
    for (img_y = 0; img_y <= (map_height - size_of_tile); img_y += size_of_tile) {
        for (img_x = 0; img_x <= (map_width - size_of_tile); img_x += size_of_tile) {
            // Obten un tile
            GetTile(img_tile, data, img_x, img_y, bg_width, bg_height);
            // Comparalo con los tiles existentes y a�adelo si no existe. Registra el resultado en el mapa
            t = OptimizeTiles();
            // Codifica el WORD con la info del tile
            tmap[idx] = (t & 0x000000ff); idx ++;
            tmap[idx] = ((t & 0x0000ff00) >> 8); idx ++;
            tmap[idx] = ((t & 0x00ff0000) >> 16); idx ++;
            tmap[idx] = ((t & 0xff000000) >> 24); idx ++;
            // Contador de tiles
            total_tiles ++;
            // Informacion del progreso
            percent = ((total_tiles * 1000) / max_tiles);
            if (percent != old_percent) {
                sprintf(s, "%05.1f%%", ((float)percent / 10.0f));
                std::cout << "\x0d" << s << " ";
                old_percent = percent;
            }
        }
    }

    std::cout << "done!" << std::endl;
    std::cout << tileset_length << " tiles from " << total_tiles << " generated." << std::endl;
    //std::cout << "Tile strip image size: " << size_of_tile << "x" << (tileset_length * size_of_tile) << " pixels." << std::endl;

    // Calcula el tama�o de la imagen de destino, para que sea lo mas cuadrada posible
    double sq = sqrt(tileset_length);                                // Calcula el ancho
    uint32_t w = ceil(sq);
    out_width = (w * size_of_tile);
    uint32_t h = ceil((double)tileset_length / (double)w);         // Calcula el alto
    out_height = (h * size_of_tile);

    // Genera la imagen de destino vacia
    tiles.resize(((out_width * out_height) << 2), 0);  // * 4

    // Copia la tira de tiles a la imagen de destino
    img_x = img_y = 0;
    for (uint32_t t = 0; t < tileset_length; t ++) {
        // Carga el tile a copiar
        GetTile(img_tile, buffer, 0, (t * size_of_tile), size_of_tile, (tileset_length * size_of_tile));
        // Y escribelo en la imagen de destino
        PutTile(img_tile, tiles, img_x, img_y, out_width);
        // Incremente las coordenadas de destino
        img_x += size_of_tile;
        if (img_x >= out_width) {
            img_x = 0;
            img_y += size_of_tile;
        }
    }

    //out_width = size_of_tile;
    //out_height = strip_height;
    std::cout << "Output tileset size is " << out_width << "x" << out_height << " pixels." << std::endl;
    //std::cout << "Tiles " << t_num << "   " << "Real " << (w * h) << " " << w << " " << h << std::endl;


    //std::cout << "Header size " << sizeof(header) << std::endl;

    //*** Test ***//


}



/*** Graba el archivo empaquetado ***/
bool ConvertToTiles::WriteFile(std::string filename) {

    // Prepara el buffer temporal
    std::vector<uint8_t> tileset_png;
    tileset_png.clear();

    // Intenta codificar el Tileset a PNG
    if (lodepng::encode(tileset_png, tiles, out_width, out_height) != 0) {
        std::cout << "Error encoding the tileset image data." << std::endl;
        tileset_png.clear();
        return false;
    }

    // Guarda la informacion para la cabecera del archivo
    FileHeader header;
    memset((void*)&header, 0, sizeof(header));
    // Datos de version
    header.version = VERSION;
    strncpy(header.magic, MAGIC_STRING.c_str(), MAGIC_STRING.length());
    // Datos del fondo original
    header.bg_width = bg_width;
    header.bg_height = bg_height;
    // Datos del mapa
    header.map_width = map_width;
    header.map_height = map_height;
    header.map_length = tmap.capacity();
    // Datos del tileset
    header.tileset_width = out_width;
    header.tileset_height = out_height;
    header.tileset_length = tileset_png.capacity();
    header.tile_size = size_of_tile;

    // Genera el nombres de archivo
    std::string fname = filename + TBG_EXTENSION;
    const char* tilemap_filename = fname.c_str();

    // Graba el archivo principal
    std::ofstream file;
    file.open(tilemap_filename, std::ofstream::out | std::ofstream::binary);
    if (file.is_open()) {
        file.write((char*)&header, sizeof(header));                     // Cabecera
        file.write((char*)&tileset_png[0], tileset_png.capacity());     // Tileset en PNG
        file.write((char*)&tmap[0], tmap.capacity());                   // Mapa
        file.close();
        std::cout << "File " << fname << " successfully saved." << std::endl;
    } else {
        std::cout << "Error saving " << fname << "." << std::endl;
        return false;
    }

    // Elimia los datos del PNG del tileset
    tileset_png.clear();

    // Si no se requieren los archivos adicionales, termina
    if (!extra_files) return true;

    // Guarda el tileset en PNG
    fname = (filename + TILESET_EXTENSION);
    if (!WritePng(fname, tiles, out_width, out_height)) return false;

    // Guarda el mapa en formato binario
    fname = (filename + MAP_EXTENSION);
    const char* map_filename = fname.c_str();
    file.open(map_filename, std::ofstream::out | std::ofstream::binary);
    if (file.is_open()) {
        file.write((char*)&tmap[0], tmap.capacity());
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
bool ConvertToTiles::ReadPng(std::string filename, std::vector<uint8_t> &data) {

    // Variables
    uint32_t width = 0, height = 0;     // Tama�o del archivo cargado

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

    // Guarda los tama�os de la imagen a convertir
    bg_width = width;
    bg_height = height;

    // Calcula el tama�o del mapa necesario
    if ((width % size_of_tile) == 0) {
        map_width = width;
    } else {
        map_width = ((((uint32_t)(width / size_of_tile)) + 1) * size_of_tile);
    }
    if ((height % size_of_tile) == 0) {
        map_height = height;
    } else {
        map_height = ((((uint32_t)(height / size_of_tile)) + 1) * size_of_tile);
    }

    // Informacion del archivo cargado
    std::cout << "File " << filename << " loaded & decoded successfully." << std::endl;
    std::cout << "Image size is " << width << "x" << height << " pixels." << std::endl;
    // Aviso de tama�o no exacto
    if (((width % size_of_tile) != 0) || ((height % size_of_tile) != 0)) {
        std::cout << std::endl;
        std::cout << "WARNING: Image size doesn't fits the tile size." << std::endl;
        std::cout << "Half tiles will be filled with blank space." << std::endl;
        std::cout << "New image size is " << map_width << "x" << map_height << " pixels." << std::endl;
        std::cout << std::endl;
    }

    // Limpia el buffer temporal
    png_data.clear();

    // Fin de la funcion de carga
    return true;

}



/*** Graba un buffer de pixeles en un archivo PNG ***/
bool ConvertToTiles::WritePng(std::string filename, std::vector<uint8_t> &data, uint32_t width, uint32_t height) {

    // Prepara el buffer temporal
    std::vector<uint8_t> png_data;
    png_data.clear();

    // Intenta codificar los pixeles del buffer a PNG
    if (lodepng::encode(png_data, data, width, height) != 0) {
        std::cout << "Error encoding the .PNG image data." << std::endl;
        png_data.clear();
        return false;
    }

    // Intenta guardar el archivo .PNG
    if (lodepng::save_file(png_data, filename) != 0) {
        std::cout << "Error saving " << filename << " file." << std::endl;
        png_data.clear();
        return false;
    }

    // Grabacion correcta
    std::cout << "File " << filename << " successfully saved." << std::endl;
    png_data.clear();
    return true;

}



/*** Lee un pixel del buffer especificado ***/
RgbaPixel ConvertToTiles::GetPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w) {

    // Variables
    RgbaPixel pixel;

    // Calcula el offset
    uint32_t offset = (((y * w) + x) << 2);     // * 4

    // Lee los bits del pixel
    /*if ((offset + 3) < data.capacity()) {*/
        pixel.r = data[offset];
        pixel.g = data[(offset + 1)];
        pixel.b = data[(offset + 2)];
        pixel.a = data[(offset + 3)];
    /*} else {
        pixel.r = pixel.g = pixel.b = pixel.a = 0;
        std::cout << "Pixel read out of range." << std::endl;
    }*/

    return pixel;

}



/*** Escribe el pixel en el buffer especificado ***/
void ConvertToTiles::PutPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w, RgbaPixel pixel) {

    // Variables
    uint32_t offset = (((y * w) + x) << 2);     // * 4

    /*if ((offset + 3) < data.capacity()) {*/
        data[offset] = pixel.r;
        data[(offset + 1)] = pixel.g;
        data[(offset + 2)] = pixel.b;
        data[(offset + 3)] = pixel.a;
    /*} else {
        std::cout << "Pixel write out of range." << std::endl;
    }*/

}



/*** Genera un tile del buffer especificado ***/
void ConvertToTiles::GetTile(std::vector<uint8_t> &tl, std::vector<uint8_t> &img, uint32_t x, uint32_t y, uint32_t max_w, uint32_t max_h) {

    // Borra el buffer de salida
    //tl.clear();
    //tl.resize(((size_of_tile * size_of_tile) << 2)); // * 4

    // Lee el tile del buffer de origen y guardalo en el de destino
    uint32_t o_offset = 0, d_offset = 0;
    for (uint32_t oy = y; oy < (y + size_of_tile); oy ++) {
        for (uint32_t ox = x; ox < (x + size_of_tile); ox ++) {
            // Calcula el offset
            // Si esta fuera de rango...
            if ((ox >= max_w) || (oy >= max_h)) {
                // Pixel vacio
                tl[d_offset] = 255; d_offset ++;
                tl[d_offset] = 255; d_offset ++;
                tl[d_offset] = 255; d_offset ++;
                tl[d_offset] = 0; d_offset ++;
            } else {
                /*if ((o_offset + 3) < img.capacity()) {*/
                    // Copia los 4 bytes de cada pixel
                    o_offset = (((oy * max_w) + ox) << 2);
                    for (uint32_t i = 0; i < 4; i ++) {
                        tl[d_offset] = img[(o_offset + i)];
                        d_offset ++;
                    }
                /*} else {
                    std::cout << "Tile read overflow at " << ox << "x" << oy << std::endl;
                }*/
            }
        }
    }

}



/*** Escribe un tile en el buffer especificado ***/
void ConvertToTiles::PutTile(std::vector<uint8_t> &tl, std::vector<uint8_t> &img, uint32_t x, uint32_t y, uint32_t w) {

    // Lee el tile del buffer de origen y guardalo en el de destino
    uint32_t o_offset = 0, d_offset = 0;
    for (uint32_t dy = y; dy < (y + size_of_tile); dy ++) {
        for (uint32_t dx = x; dx < (x + size_of_tile); dx ++) {
            d_offset = (((dy * w) + dx) << 2);
            /*if ((d_offset + 3) < img.capacity()) {*/
                // Copia los 4 bytes de cada pixel
                for (uint32_t i = 0; i < 4; i ++) {
                    img[(d_offset + i)] = tl[o_offset];
                    o_offset ++;
                }
            /*} else {
                std::cout << "Tile write overflow." << dx << "x" << dy << std::endl;
            }*/
        }
    }

}


/*** Realiza la optimizacion de tiles unicos ***/
uint32_t ConvertToTiles::OptimizeTiles() {

    // Variables
    uint32_t i = 0;
    int32_t add_tile = 0, tile_id = 0;

    // Verifica si es el primer tile del set
    if (tileset_length > 0) {

        // Si hay mas tiles, verifica que no exista
        for (i = 0; i < tileset_length; i ++) {
            GetTile(map_tile, buffer, 0, (i * size_of_tile), size_of_tile, (tileset_length * size_of_tile));
            // Copia el id del tile actual del Tileset
            tile_id = i;
            // Comparacion de tiles
            if (CompareNormal()) {
                add_tile = 1;
                break;
            } else if (CompareFlipH()) {
                add_tile = 2;
                break;
            } else if (CompareFlipV()) {
                add_tile = 4;
                break;
            } else if (Compare180deg()) {
                add_tile = 8;
                break;
            } else if (Compare90degCW()) {
                add_tile = 16;
                break;
            } else if (Compare90degACW()) {
                add_tile = 32;
                break;
            }
        }

    }

    // A�ade el tile si es necesario
    if (add_tile == 0) {
        PutTile(img_tile, buffer, 0, (tileset_length * size_of_tile), size_of_tile);
        tile_id = tileset_length;
        tileset_length ++;
        // Al ser un tile a�adido, por defecto no tiene ni flip ni rotacion
        add_tile = 1;
    }

    // Calcula el valor de devolver para a�adir al mapa
    // [8bits FLIP &| ROTATION] [24bits TILE ID]
    tile_id |= (add_tile << 24);

    // Devuelve la ID del tile coincidente
    return tile_id;

}



/*** Compara si 2 tiles son identicos (img_tile vs map_tile) ***/
bool ConvertToTiles::CompareNormal() {

    //bool r = true;              // Son iguales?
    uint32_t i = 0;

    // Compara todos los pixels de las tiles
    for (i = 0; i < img_tile.capacity(); i ++) {
        if (img_tile[i] != map_tile[i]) {
            //r = false;
            //break;
            return false;
        }
    }

    //return r;
    return true;

}



/*** Compara si 2 tiles estan en espejo horizontal (H FLIP) ***/
bool ConvertToTiles::CompareFlipH() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 2) return false;

    uint32_t x, y, _x;
    uint32_t last = size_of_tile - 1;
    RgbaPixel p, _p;

    // Compara todos los pixels de las tiles
    for (y = 0; y < size_of_tile; y ++) {
        for (x = 0; x < size_of_tile; x ++) {
            _x = (last - x);
            p = GetPixel(img_tile, x, y, size_of_tile);
            _p = GetPixel(map_tile, _x, y, size_of_tile);
            if (
                (p.r != _p.r)
                ||
                (p.g != _p.g)
                ||
                (p.b != _p.b)
                ||
                (p.a != _p.a)
                ) {
                //r = false;
                //break;
                return false;
            }
        }
    }

    //return r;
    return true;

}



/*** Compara si 2 tiles estan en espejo vertical (V FLIP) ***/
bool ConvertToTiles::CompareFlipV() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 2) return false;

    uint32_t x, y, _y;
    uint32_t last = size_of_tile - 1;
    RgbaPixel p, _p;

    // Compara todos los pixels de las tiles
    for (y = 0; y < size_of_tile; y ++) {
        for (x = 0; x < size_of_tile; x ++) {
            _y = (last - y);
            p = GetPixel(img_tile, x, y, size_of_tile);
            _p = GetPixel(map_tile, x, _y, size_of_tile);
            if (
                (p.r != _p.r)
                ||
                (p.g != _p.g)
                ||
                (p.b != _p.b)
                ||
                (p.a != _p.a)
                ) {
                //r = false;
                //break;
                return false;
            }
        }
    }

    //return r;
    return true;

}



/*** Compara si 2 tiles estan en espejo completo (H & V FLIP) ***/
bool ConvertToTiles::Compare180deg() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 3) return false;

    uint32_t x, y, _x, _y;
    uint32_t last = size_of_tile - 1;
    RgbaPixel p, _p;

    // Compara todos los pixels de las tiles
    for (y = 0; y < size_of_tile; y ++) {
        for (x = 0; x < size_of_tile; x ++) {
            _x = (last - x);
            _y = (last - y);
            p = GetPixel(img_tile, x, y, size_of_tile);
            _p = GetPixel(map_tile, _x, _y, size_of_tile);
            if (
                (p.r != _p.r)
                ||
                (p.g != _p.g)
                ||
                (p.b != _p.b)
                ||
                (p.a != _p.a)
                ) {
                //r = false;
                //break;
                return false;
            }
        }
    }

    //return r;
    return true;

}



/*** Compara si 2 tiles estan rotadas 90� a la derecha (90� Clock Wise) ***/
bool ConvertToTiles::Compare90degCW() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 3) return false;

    uint32_t x, y, _x, _y;
    uint32_t last = size_of_tile - 1;
    RgbaPixel p, _p;

    // Compara todos los pixels de las tiles
    for (y = 0; y < size_of_tile; y ++) {
        for (x = 0; x < size_of_tile; x ++) {
            _x = (last - y);
            _y = x;
            p = GetPixel(img_tile, x, y, size_of_tile);
            _p = GetPixel(map_tile, _x, _y, size_of_tile);
            if (
                (p.r != _p.r)
                ||
                (p.g != _p.g)
                ||
                (p.b != _p.b)
                ||
                (p.a != _p.a)
                ) {
                //r = false;
                //break;
                return false;
            }
        }
    }

    //return r;
    return true;

}



/*** Compara si 2 tiles estan rotadas 90� a la izquierda (90� Anti Clock Wise) ***/
bool ConvertToTiles::Compare90degACW() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 3) return false;

    uint32_t x, y, _x, _y;
    uint32_t last = size_of_tile - 1;
    RgbaPixel p, _p;

    // Compara todos los pixels de las tiles
    for (y = 0; y < size_of_tile; y ++) {
        for (x = 0; x < size_of_tile; x ++) {
            _x = y;
            _y = (last - x);
            p = GetPixel(img_tile, x, y, size_of_tile);
            _p = GetPixel(map_tile, _x, _y, size_of_tile);
            if (
                (p.r != _p.r)
                ||
                (p.g != _p.g)
                ||
                (p.b != _p.b)
                ||
                (p.a != _p.a)
                ) {
                //r = false;
                //break;
                return false;
            }
        }
    }

    //return r;
    return true;

}
