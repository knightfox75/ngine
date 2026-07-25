/******************************************************************************

    Conversor de PNG a Fondo de Tiles Indexado (atlas + paleta)
    - Convierte un archivo PNG en tiles -

    Proyecto iniciado el 1 de Mayo del 2026
    (c) 2016-2026 by Cesar Rincon "NightFox"
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
    palette_colors = 0;

    atlas_tiles.clear();
    background_map.clear();
    atlas_palette.clear();
    img_tile.clear();
    map_tile.clear();
    buffer.clear();

}



/*** Destructor ***/
ConvertToTiles::~ConvertToTiles() {

    atlas_tiles.clear();
    background_map.clear();
    atlas_palette.clear();
    img_tile.clear();
    map_tile.clear();
    buffer.clear();

}



/*** Convierte un archivo PNG en tiles ***/
bool ConvertToTiles::Convert(
    std::string in_file,        // Archivo PNG a convertir
    std::string out_file,       // Nombre base de los archivos de salida
    uint32_t tile_size,         // Tamaño del tile
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
    if (!ReadPng(in_file, png_pixels, atlas_palette)) return false;

    // Convierte la imagen cargada a un conjunto de TILES + MAPA, guardado los datos en sus correspondientes buffers
    GenerateTileset(png_pixels);

    // Guarda en el archivo empaquetado los datos generados
    bool r = WriteFile(out_file);

    // Elimina los datos de todos los buffers usados
    atlas_tiles.clear();
    background_map.clear();
    atlas_palette.clear();
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
    atlas_tiles.clear();
    background_map.clear();

    // Reserva el espacio maximo para el buffer y los contenedores de tiles
    buffer.assign(((map_width * map_height) + (size_of_tile * size_of_tile)), 0);
    img_tile.assign((size_of_tile * size_of_tile), 0);
    map_tile.assign((size_of_tile * size_of_tile), 0);

    // Marcador...
    uint64_t max_tiles = ((map_width / size_of_tile) * (map_height / size_of_tile));  // Total de tiles
    uint64_t percent = 0, old_percent = 0;
    char s[16];
    std::cout << "Optimizing " << max_tiles << " tiles..." << std::endl;

    // Reserva el espacio para el mapa de tiles
    background_map.resize((max_tiles << 2), 0);

    // Tile vacio al inicio del tileset
    std::vector<uint8_t> empty_tile;
    empty_tile.assign((size_of_tile * size_of_tile), 0);
    // Genera el primer tile del mapa vacio, como referencia
    PutTile(empty_tile, buffer, 0, (tileset_length * size_of_tile), size_of_tile);
    tileset_length ++;
    empty_tile.clear();


    // Convierte toda la imagen a una tira de tiles
    for (img_y = 0; img_y <= (map_height - size_of_tile); img_y += size_of_tile) {
        for (img_x = 0; img_x <= (map_width - size_of_tile); img_x += size_of_tile) {
            // Obten un tile
            GetTile(img_tile, data, img_x, img_y, bg_width, bg_height);
            // Comparalo con los tiles existentes y añadelo si no existe. Registra el resultado en el mapa
            t = OptimizeTiles();
            // Codifica el WORD con la info del tile
            background_map[idx] = (t & 0x000000ff); idx ++;
            background_map[idx] = ((t & 0x0000ff00) >> 8); idx ++;
            background_map[idx] = ((t & 0x00ff0000) >> 16); idx ++;
            background_map[idx] = ((t & 0xff000000) >> 24); idx ++;
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
    atlas_tiles.resize((out_width * out_height), 0);

    // Copia la tira de tiles a la imagen de destino
    img_x = img_y = 0;
    for (uint32_t t = 0; t < tileset_length; t ++) {
        // Carga el tile a copiar
        GetTile(img_tile, buffer, 0, (t * size_of_tile), size_of_tile, (tileset_length * size_of_tile));
        // Y escribelo en la imagen de destino
        PutTile(img_tile, atlas_tiles, img_x, img_y, out_width);
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

    // Guarda la informacion para la cabecera del archivo de Atlas + mapa
    BackgroundFileHeader background_header;
    memset((void*)&background_header, 0, sizeof(background_header));
    // Datos de version
    background_header.version = VERSION;
    strncpy(background_header.magic, BG_MAGIC_STRING.c_str(), BG_MAGIC_STRING.length());
    // Datos del fondo original
    background_header.bg_width = bg_width;
    background_header.bg_height = bg_height;
    // Datos del mapa
    background_header.map_width = map_width;
    background_header.map_height = map_height;
    background_header.map_length = background_map.size();
    // Datos del tileset
    background_header.tileset_width = out_width;
    background_header.tileset_height = out_height;
    background_header.tileset_length = atlas_tiles.size();
    background_header.tile_size = size_of_tile;

    // Genera el nombres de archivo
    std::string fname = (filename + BACKGROUND_EXTENSION);
    const char* tilemap_filename = fname.c_str();

    // Graba el archivo de Atlas + Mapa
    std::ofstream file;
    file.open(tilemap_filename, std::ofstream::out | std::ofstream::binary);
    if (file.is_open()) {
        file.write((char*)&background_header, sizeof(background_header));                             // Cabecera
        file.write((char*)&atlas_tiles[0], atlas_tiles.size());             // Atlas en RAW
        file.write((char*)&background_map[0], background_map.size());       // Mapa
        file.close();
        std::cout << "File " << fname << " successfully saved." << std::endl;
    } else {
        std::cout << "Error saving " << fname << "." << std::endl;
        return false;
    }


    // Guarda la informacion para la cabecera del archivo de paleta
    PaletteFileHeader palette_header;
    memset((void*)&palette_header, 0, sizeof(palette_header));
    // Datos de version
    palette_header.version = VERSION;
    strncpy(palette_header.magic, PAL_MAGIC_STRING.c_str(), PAL_MAGIC_STRING.length());
    // Colores en la paleta
    palette_header.colors = palette_colors;

    // Genera el nombres de archivo
    fname = (filename + PALETTE_EXTENSION);
    const char* palette_filename = fname.c_str();

    // Graba el archivo de paleta
    file.open(palette_filename, std::ofstream::out | std::ofstream::binary);
    if (file.is_open()) {
        file.write((char*)&palette_header, sizeof(palette_header));     // Cabecera
        file.write((char*)&atlas_palette[0], atlas_palette.size());     // Paleta
        file.close();
        std::cout << "File " << fname << " successfully saved." << std::endl;
    } else {
        std::cout << "Error saving " << fname << "." << std::endl;
        return false;
    }



    // Si no se requieren los archivos adicionales, termina
    if (!extra_files) return true;

    // Prepara el buffer temporal para el tileset
    std::cout << "Exporting atlas PNG..." << std::endl;
    std::vector<uint8_t> tileset_raw_data;
    tileset_raw_data.assign((atlas_tiles.size() << 2), 0);

    // Recrea el tileset
    for (uint32_t k = 0; k < atlas_tiles.size(); k ++) {
        uint32_t idx = (atlas_tiles[k] << 2);
        uint32_t offset = (k << 2);
        tileset_raw_data[offset] = atlas_palette[idx];
        tileset_raw_data[(offset + 1)] = atlas_palette[(idx + 1)];
        tileset_raw_data[(offset + 2)] = atlas_palette[(idx + 2)];
        tileset_raw_data[(offset + 3)] = atlas_palette[(idx + 3)];
    }

    // Guarda el tileset en PNG
    fname = (filename + VISUAL_ATLAS_EXTENSION);
    if (!WritePng(fname, tileset_raw_data, out_width, out_height)) {
        std::cout << "Error saving " << fname << "." << std::endl;
        tileset_raw_data.clear();
        return false;
    }

    // Paso de limpieza
    tileset_raw_data.clear();

    // Guarda la paleta en un PNG
    std::cout << "Exporting palette PNG..." << std::endl;
    const uint32_t swatches_per_row = 16;                       // 16 casillas de ancho
    const uint32_t swatches_per_column = 16;                    // 16 casillas de alto
    const uint32_t swatch_size = 16;                            // 16 píxeles por casilla
    const uint32_t img_w = (swatches_per_row * swatch_size);    // 256 píxeles
    const uint32_t img_h = (swatches_per_column * swatch_size); // 256 píxeles (para 256 colores)
    const uint32_t img_pixels = (img_w * img_h);

    // Prepara el buffer (RGBA = 4 bytes por píxel)
    std::vector<uint8_t> palette_raw_data;
    palette_raw_data.assign((img_pixels << 2), 0);

    // Recore la paleta actual
    for (uint32_t k = 0; k < palette_colors; k ++) {

        // Coordenadas de la esquina superior izquierda de la casilla actual
        uint32_t start_x = ((k % swatches_per_row) * swatch_size);
        uint32_t start_y = ((k / swatches_per_row) * swatch_size);

        // Extraemos el color de la paleta (RGBA)
        uint32_t pal_idx = (k << 2);
        uint8_t r = atlas_palette[pal_idx];
        uint8_t g = atlas_palette[(pal_idx + 1)];
        uint8_t b = atlas_palette[(pal_idx + 2)];
        uint8_t a = atlas_palette[(pal_idx + 3)];

        // Dibujamos la casilla de 16x16
        for (uint32_t dy = 0; dy < swatch_size; dy ++) {
            for (uint32_t dx = 0; dx < swatch_size; dx ++) {
                // Calculo de la posicion absoluta en la imagen final
                uint32_t px = (start_x + dx);
                uint32_t py = (start_y + dy);
                // Indice en el buffer RGBA de destino ((py * ancho_imagen + px) * 4)
                uint32_t target_idx = (((py * img_w) + px) << 2);
                // Asigna el color
                palette_raw_data[target_idx] = r;
                palette_raw_data[(target_idx + 1)] = g;
                palette_raw_data[(target_idx + 2)] = b;
                palette_raw_data[(target_idx + 3)] = a;
            }
        }
    }

    // Guarda la paleta en PNG
    fname = (filename + VISUAL_PALETTE_EXTENSION);
    if (!WritePng(fname, palette_raw_data, img_w, img_h)) {
        std::cout << "Error saving " << fname << "." << std::endl;
    }

    // Paso de limpieza
    palette_raw_data.clear();

    // Todo correcto
    return true;

}



/*** Lee un archivo .PNG y coloca los pixeles en un buffer ***/
bool ConvertToTiles::ReadPng(std::string filename, std::vector<uint8_t> &pixels, std::vector<uint8_t> &palette) {

    // Variables
    uint32_t width = 0, height = 0;     // Tamaño del archivo cargado

    // Carga el archivo PNG en bruto
    std::vector<uint8_t> png_data;
    png_data.clear();
    if (lodepng::load_file(png_data, filename) != 0) {
        std::cout << "Error loading " << filename << "." << std::endl;
        return false;
    }

    // Configura LodePNG para NO expandir a RGBA: queremos los indices raw
    LodePNGState state;
    lodepng_state_init(&state);
    state.decoder.color_convert = 0;    // Desactiva la conversion de color

    // Decodifica el PNG: "data" contendra los indices uint8, "state" la paleta
    uint32_t w, h;
    uint8_t* raw_buffer = nullptr;
    uint32_t error = lodepng_decode(
        &raw_buffer, &w, &h,
        &state,
        png_data.data(), png_data.size()
    );
    png_data.clear();

    // Si la decodificacion ha fallado
    if (error != 0) {
        std::cout << "Error decoding " << filename << ": " << lodepng_error_text(error) << std::endl;
        if (raw_buffer) free(raw_buffer);
        lodepng_state_cleanup(&state);
        return false;
    }

    // Verifica que el PNG esta en modo indexado (8bpp con paleta)
    if (state.info_png.color.colortype != LCT_PALETTE) {
        std::cout << "Error: " << filename << " is not an indexed PNG." << std::endl;
        free(raw_buffer);
        lodepng_state_cleanup(&state);
        return false;
    }

    // Verifica que la paleta sea valida
    uint32_t pal_size = (uint32_t)state.info_png.color.palettesize;
    if (pal_size > 256) {
        std::cout << "Error: " << filename << " palette overflow (>256 colors)." << std::endl;
        free(raw_buffer);
        lodepng_state_cleanup(&state);
        return false;
    }
    palette_colors = pal_size;

    // Asigna los tamaños
    width  = (uint32_t)w;
    height = (uint32_t)h;

    // Copia los indices al vector de salida
    pixels.assign(raw_buffer, raw_buffer + (width * height));

    // Extrae la paleta RGBA (fija 256 entradas)
    palette.assign((256 << 2), 0);      // 256 * 4
    for (uint32_t k = 0; k < pal_size; k ++) {
        uint32_t idx = (k << 2);
        palette[idx] = state.info_png.color.palette[(k << 2)];
        palette[(idx + 1)] = state.info_png.color.palette[((k << 2) + 1)];
        palette[(idx + 2)] = state.info_png.color.palette[((k << 2) + 2)];
        palette[(idx + 3)] = state.info_png.color.palette[((k << 2) + 3)];
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

    // Informacion del archivo cargado
    std::cout << "File " << filename << " loaded & decoded successfully." << std::endl;
    std::cout << "Image size is " << width << "x" << height << " pixels." << std::endl;
    // Aviso de tamaño no exacto
    if (((width % size_of_tile) != 0) || ((height % size_of_tile) != 0)) {
        std::cout << std::endl;
        std::cout << "WARNING: Image size doesn't fits the tile size." << std::endl;
        std::cout << "Half tiles will be filled with blank space." << std::endl;
        std::cout << "New image size is " << map_width << "x" << map_height << " pixels." << std::endl;
        std::cout << std::endl;
    }

    // Paso de limpieza
    free(raw_buffer);
    lodepng_state_cleanup(&state);

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
uint8_t ConvertToTiles::GetPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w) {

    return data[((y * w) + x)];

}



/*** Escribe el pixel en el buffer especificado ***/
void ConvertToTiles::PutPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w, uint8_t pixel) {

    data[((y * w) + x)] = pixel;

}



/*** Genera un tile del buffer especificado ***/
void ConvertToTiles::GetTile(std::vector<uint8_t> &tl, std::vector<uint8_t> &img, uint32_t x, uint32_t y, uint32_t max_w, uint32_t max_h) {

    // Lee el tile del buffer de origen y guardalo en el de destino
    uint32_t o_offset = 0, d_offset = 0;
    for (uint32_t oy = y; oy < (y + size_of_tile); oy ++) {
        for (uint32_t ox = x; ox < (x + size_of_tile); ox ++) {
            // Si esta fuera de rango...
            if ((ox >= max_w) || (oy >= max_h)) {
                // Pixel vacio
                tl[d_offset] = 0;
                d_offset ++;
            } else {
                o_offset = ((oy * max_w) + ox);
                tl[d_offset] = img[o_offset];
                d_offset ++;
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
            d_offset = ((dy * w) + dx);
            img[d_offset] = tl[o_offset];
            o_offset ++;
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
bool ConvertToTiles::CompareNormal() {

    //bool r = true;              // Son iguales?
    uint32_t i = 0;

    // Compara todos los pixels de las tiles
    for (i = 0; i < img_tile.size(); i ++) {
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
    uint8_t p, _p;

    // Compara todos los pixels de las tiles
    for (y = 0; y < size_of_tile; y ++) {
        for (x = 0; x < size_of_tile; x ++) {
            _x = (last - x);
            p = GetPixel(img_tile, x, y, size_of_tile);
            _p = GetPixel(map_tile, _x, y, size_of_tile);
            if (p != _p) return false;
        }
    }

    return true;

}



/*** Compara si 2 tiles estan en espejo vertical (V FLIP) ***/
bool ConvertToTiles::CompareFlipV() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 2) return false;

    uint32_t x, y, _y;
    uint32_t last = size_of_tile - 1;
    uint8_t p, _p;

    // Compara todos los pixels de las tiles
    for (y = 0; y < size_of_tile; y ++) {
        for (x = 0; x < size_of_tile; x ++) {
            _y = (last - y);
            p = GetPixel(img_tile, x, y, size_of_tile);
            _p = GetPixel(map_tile, x, _y, size_of_tile);
            if (p != _p) return false;
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
    uint8_t p, _p;

    // Compara todos los pixels de las tiles
    for (y = 0; y < size_of_tile; y ++) {
        for (x = 0; x < size_of_tile; x ++) {
            _x = (last - x);
            _y = (last - y);
            p = GetPixel(img_tile, x, y, size_of_tile);
            _p = GetPixel(map_tile, _x, _y, size_of_tile);
            if (p != _p) return false;
        }
    }

    //return r;
    return true;

}



/*** Compara si 2 tiles estan rotadas 90º a la derecha (90º Clock Wise) ***/
bool ConvertToTiles::Compare90degCW() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 3) return false;

    uint32_t x, y, _x, _y;
    uint32_t last = size_of_tile - 1;
    uint8_t p, _p;

    // Compara todos los pixels de las tiles
    for (y = 0; y < size_of_tile; y ++) {
        for (x = 0; x < size_of_tile; x ++) {
            _x = (last - y);
            _y = x;
            p = GetPixel(img_tile, x, y, size_of_tile);
            _p = GetPixel(map_tile, _x, _y, size_of_tile);
            if (p != _p) return false;
        }
    }

    //return r;
    return true;

}



/*** Compara si 2 tiles estan rotadas 90º a la izquierda (90º Anti Clock Wise) ***/
bool ConvertToTiles::Compare90degACW() {

    //bool r = true;          // Son iguales?

    // Nivel de optimizacion
    if (optimize < 3) return false;

    uint32_t x, y, _x, _y;
    uint32_t last = size_of_tile - 1;
    uint8_t p, _p;

    // Compara todos los pixels de las tiles
    for (y = 0; y < size_of_tile; y ++) {
        for (x = 0; x < size_of_tile; x ++) {
            _x = y;
            _y = (last - x);
            p = GetPixel(img_tile, x, y, size_of_tile);
            _p = GetPixel(map_tile, _x, _y, size_of_tile);
            if (p != _p) return false;
        }
    }

    //return r;
    return true;

}
