/******************************************************************************

    N'gine Lib for C++
    Conversor de PNG a TILES + MAP [Funciones]

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG
    (c) 2005 - 2020 by Lode Vandevenne
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

// Tilemap
#include "tilemap.h"



/*** Contructor ***/
PngToTiles::PngToTiles() {
}



/*** Destructor ***/
PngToTiles::~PngToTiles() {

    // Borra todos los vectores de memoria
    png.clear();
    raw.clear();
    out.clear();
    tiles.clear();
    tmap.clear();
    img_tile.clear();
    map_tile.clear();
    buffer.clear();

}



/*** Convierte el archivo ***/
int32_t PngToTiles::ConvertPng() {

    // Intenta cargar el archivo PNG, si hay error sal e informa.
    if (ReadPNG(input_filename, raw) > 0) return 1;

    // Convierte la imagen a tiles
    GenerateTileset();

    // Graba los archivos de salida
    if (WriteFile(output_filename) > 0) return 1;

    return 0;

}



/*** Lee el archivo PNG y almacena los pixeles de la imagen en el buffer ***/
int32_t PngToTiles::ReadPNG(std::string filename, std::vector<uint8_t> &data) {

    // Variables
    uint32_t width, height;     // Tamaño del archivo cargado

    // Borra los buffers
    png.clear();

    // Carga el archivo PNG
    uint32_t _error = lodepng::load_file(png, filename);

    // Si se ha cargado correctamente, decodifica la imagen
    if (_error == 0) {
        _error = lodepng::decode(raw, width, height, png);
    } else {
        std::cout << "Error loading " << filename << "." << std::endl;
        png.clear();
        return 1;
    }

    // Guarda los tamaños de la imagen a convertir
    bg_width = width;
    bg_height = height;

    // Calcula el tamaño del mapa necesario
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


    // Verifica que la decodificacion sea la correcta
    if (_error == 0) {
        std::cout << "File " << filename << " loaded & decoded successfully." << std::endl;
        std::cout << "Image size is " << width << "x" << height << " pixels." << std::endl;
        // Aviso de tamaño no exacto
        if (((width % size_of_tile) != 0) || ((height % size_of_tile) != 0)) {
            std::cout << std::endl;
            std::cout << "WARNING: Image size doesn't fits the tile size." << std::endl;
            std::cout << "Half tiles will be filled with blank spaces." << std::endl;
            std::cout << "New image size is " << map_width << "x" << map_height << " pixels." << std::endl;
            std::cout << std::endl;
        }
        png.clear();
        return 0;
    } else {
        std::cout << "Error decoding " << filename << "." << std::endl;
        png.clear();
        return 1;
    }


}


/*** Graba el buffer a un archivo PNG ***/
int32_t PngToTiles::WritePNG(std::string filename, std::vector<uint8_t> &data) {

    // Borra los bufferes
    out.clear();

    uint32_t _error = lodepng::encode(out, data, out_width, out_height);

    // Si se ha codificado correctamente, graba el archivo
    if (_error == 0) {
        _error = lodepng::save_file(out, filename);
        if (_error == 0) {
            std::cout << "File " << filename << " successfully saved." << std::endl;
            out.clear();
            return 0;
        } else {
            std::cout << "Error saving " << filename << "." << std::endl;
            out.clear();
            return 1;
        }
    } else {
        std::cout << "Error encoding the RAW data." << std::endl;
        out.clear();
        return 1;
    }

}



/*** Lee un pixel del buffer especificado ***/
rgba_pixel PngToTiles::GetPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w) {

    // Variables
    rgba_pixel pixel;

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
void PngToTiles::PutPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w, rgba_pixel pixel) {

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



/*** Convierte la imagen en tiles + mapa ***/
void PngToTiles::GenerateTileset() {

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
            GetTile(img_tile, raw, img_x, img_y, bg_width, bg_height);
            // Comparalo con los tiles existentes y añadelo si no existe. Registra el resultado en el mapa
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

    // Calcula el tamaño de la imagen de destino, para que sea lo mas cuadrada posible
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
int32_t PngToTiles::WriteFile(std::string filename) {

    // Proteccion de path
    if (filename.length() >= 244) {
        std::cout << "Error saving " << filename << "." << std::endl;
        std::cout << "Path too long." << std::endl;
        return 1;
    }

    // Variables
    uint32_t i = 0;

    // Intenta codificar el Tileset a PNG
    out.clear();
    if (lodepng::encode(out, tiles, out_width, out_height) > 0) return 1;


    // Guarda la informacion para la cabecera del archivo

    header.version = VERSION;

    sprintf(header.magic, "%s", MAGIC_STRING.c_str());

    header.bg_width = bg_width;
    header.bg_height = bg_height;

    header.map_width = map_width;
    header.map_height = map_height;
    header.map_length = tmap.capacity();

    header.tileset_width = out_width;
    header.tileset_height = out_height;
    header.tileset_length = out.capacity();
    header.tile_size = size_of_tile;

    for (i = 0; i < sizeof(header.reserve); i ++) header.reserve[i] = 0;

    // Genera el nombre de archivo
    char f[256];
    char basename[244];
    for (i = 0; i < filename.length(); i ++) basename[i] = filename[i];     // Nombre base
    basename[i] = '\0';     // Terminador

    //std::cout << filename << std::endl;
    //std::cout << basename << std::endl;

    // Graba el archivo principal
    sprintf(f, "%s.tbg", basename);
    std::ofstream file;
    file.open(f, std::ofstream::out | std::ofstream::binary);
    if (file.is_open()) {
        file.write((char*)&header, sizeof(header));         // Cabecera
        file.write((char*)&out[0], out.capacity());         // Tileset en PNG
        file.write((char*)&tmap[0], tmap.capacity());       // Mapa
        file.close();
        std::cout << "File " << f << " successfully saved." << std::endl;
    } else {
        std::cout << "Error saving " << f << "." << std::endl;
        return 1;
    }


    // Si se han pedido los archivos extra...
    if (extra_files) {

        // Graba la imagen con el tileset
        sprintf(f, "%s_tileset.png", basename);
        if (WritePNG(f, tiles) > 0) return 1;

        // Graba el mapa del tileset en formato binario
        sprintf(f, "%s_map.bin", basename);
        file.open(f, std::ofstream::out | std::ofstream::binary);
        if (file.is_open()) {
            file.write((char*)&tmap[0], tmap.capacity());
            file.close();
            std::cout << "File " << f << " successfully saved." << std::endl;
        } else {
            std::cout << "Error saving " << f << "." << std::endl;
            return 1;
        }

    }

    // Todo correcto
    return 0;

}



/*** Genera un tile del buffer especificado ***/
void PngToTiles::GetTile(std::vector<uint8_t> &tl, std::vector<uint8_t> &img, uint32_t x, uint32_t y, uint32_t max_w, uint32_t max_h) {

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
void PngToTiles::PutTile(std::vector<uint8_t> &tl, std::vector<uint8_t> &img, uint32_t x, uint32_t y, uint32_t w) {

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
uint32_t PngToTiles::OptimizeTiles() {

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

    // Añade el tile si es necesario
    if (add_tile == 0) {
        PutTile(img_tile, buffer, 0, (tileset_length * size_of_tile), size_of_tile);
        tile_id = tileset_length;
        tileset_length ++;
        // Al ser un tile añadido, por defecto no tiene ni flip ni rotacion
        add_tile = 1;
    }

    // Calcula el valor de devolver para añadir al mapa
    // [8bits FLIP &| ROTATION] [24bits TILE ID]
    tile_id |= (add_tile << 24);

    // Devuelve la ID del tile coincidente
    return tile_id;

}



/*** Compara si 2 tiles son identicos (img_tile vs map_tile) ***/
bool PngToTiles::CompareNormal() {

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
bool PngToTiles::CompareFlipH() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 2) return false;

    uint32_t x, y, _x;
    uint32_t last = size_of_tile - 1;
    rgba_pixel p, _p;

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
bool PngToTiles::CompareFlipV() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 2) return false;

    uint32_t x, y, _y;
    uint32_t last = size_of_tile - 1;
    rgba_pixel p, _p;

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
bool PngToTiles::Compare180deg() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 3) return false;

    uint32_t x, y, _x, _y;
    uint32_t last = size_of_tile - 1;
    rgba_pixel p, _p;

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



/*** Compara si 2 tiles estan rotadas 90º a la derecha (90º Clock Wise) ***/
bool PngToTiles::Compare90degCW() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 3) return false;

    uint32_t x, y, _x, _y;
    uint32_t last = size_of_tile - 1;
    rgba_pixel p, _p;

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



/*** Compara si 2 tiles estan rotadas 90º a la izquierda (90º Anti Clock Wise) ***/
bool PngToTiles::Compare90degACW() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 3) return false;

    uint32_t x, y, _x, _y;
    uint32_t last = size_of_tile - 1;
    rgba_pixel p, _p;

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

