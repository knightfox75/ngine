/******************************************************************************

    N'gine Lib for C++
    Conversor de PNG a Sprite sheet (.spr)

    Proyecto iniciado el 3 de Marzo del 2016
    (cc) 2016 - 2019 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG
    (c) 2005-2016 by Lode Vandevenne
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

// Sprite class
#include "sprite.h"



/*** Constructor ***/
SpriteSheet::SpriteSheet() {

    // Inicializa variables
    strip = false;

}



/*** Destructor ***/
SpriteSheet::~SpriteSheet() {

    // Borra los buffers de memoria
    png.clear();
    raw.clear();
    sheet.clear();
    out.clear();

}



/*** Convierte el archivo a un Sprite Sheet del tamaño solicitado ***/
int32_t SpriteSheet::ConvertPng() {

    // Intenta cargar el archivo PNG, si hay error sal e informa.
    if (ReadPNG(input_filename, raw) > 0) return 1;

    // Genera la plantilla
    GenerateSpriteSheet();

    // Graba los archivos de salida
    if (WriteFile(output_filename) > 0) return 1;

    // Devuelve el resultado
    return 0;

}



/*** Lee el archivo PNG y almacena los pixeles de la imagen en el buffer ***/
int32_t SpriteSheet::ReadPNG(std::string filename, std::vector<uint8_t> &data) {

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
        // Aviso de tamaño no exacto
        if (((in_width % frame_width) != 0) || ((in_height % frame_height) != 0)) {
            std::cout << "WARNING: Image size doesn't fits the sprite frame size." << std::endl;
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
int32_t SpriteSheet::WritePNG(std::string filename, std::vector<uint8_t> &data) {

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



/*** Graba el archivo empaquetado ***/
int32_t SpriteSheet::WriteFile(std::string filename) {

    // Variables
    uint32_t i = 0;

    // Intenta codificar el Tileset a PNG
    out.clear();
    if (lodepng::encode(out, sheet, out_width, out_height) > 0) return 1;

    for (i = 0; i < sizeof(header.reserve); i ++) header.reserve[i] = 0;

    // Genera el nombre de archivo
    char f[256];
    char basename[256];
    for (i = 0; i < filename.length(); i ++) basename[i] = filename[i];     // Nombre base
    basename[i] = '\0';     // Terminador

    //std::cout << filename << std::endl;
    //std::cout << basename << std::endl;

    // Graba el archivo principal
    sprintf(f, "%s.spr", basename);
    std::ofstream file;
    file.open(f, std::ofstream::out | std::ofstream::binary);
    if (file.is_open()) {
        file.write((char*)&header, sizeof(header));         // Cabecera
        file.write((char*)&out[0], out.capacity());         // Sprite sheet en PNG
        file.close();
        std::cout << "File " << f << " successfully saved." << std::endl;
    } else {
        std::cout << "Error saving " << f << "." << std::endl;
        return 1;
    }

    // Si es necesario, guarda el archivo PNG extra
    if (strip) {
        sprintf(f, "%s_sheet.png", basename);
        if (WritePNG(f, sheet) > 0) return 1;
    }

    // Todo correcto
    return 0;

}




/*** Lee un pixel del buffer especificado ***/
rgba_pixel SpriteSheet::GetPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w) {

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
void SpriteSheet::PutPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w, rgba_pixel pixel) {

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




/*** Genera la plantilla de frames del sprite ***/
void SpriteSheet::GenerateSpriteSheet() {

    // Variables
    uint32_t img_x = 0, img_y = 0;      // Punto de corte
    uint32_t _frames_w = (uint32_t)floor(in_width / frame_width);       // Numero de columnas de frames
    uint32_t _frames_h = (uint32_t)floor(in_height / frame_height);     // Numero de filas de frames
    uint32_t _total_frames = (_frames_w * _frames_h);                       // Numero total de frames
    uint32_t _current_frame = 0;

    // Calcula el tamaño del archivo de salida
    out_width = frame_width;
    out_height = (_total_frames * frame_height);

    // Prepara los bufferes a usar
    sheet.clear();
    sheet.resize(((out_width * out_height) << 2), 0);  // * 4

    // Marcador
    std::cout << "This image contains " << _total_frames << " frames of " << frame_width << "x" << frame_height << " pixels." << std::endl;

    // Variables del control de corte del frame
    uint32_t src_pointer = 0, dst_pointer = 0;
    uint32_t cut_x = 0, cut_y = 0;
    uint32_t cut_start_x = 0, cut_end_x = 0, cut_start_y = 0, cut_end_y = 0;

    // Corta la imagen en una tira de frames
    for (img_y = 0; img_y <= (in_height - frame_height); img_y += frame_height) {
            // Calcula el punto de corte (Y)
            cut_start_y = img_y; cut_end_y = (cut_start_y + frame_height);
        for (img_x = 0; img_x <= (in_width - frame_width); img_x += frame_width) {
            // Contador de frames cortados
            _current_frame ++;
            std::cout << "\x0d" << "Cutting off " << _current_frame << " of " << _total_frames << " frames... ";
            // Calcula el punto de corte (X)
            cut_start_x = img_x; cut_end_x = (cut_start_x + frame_width);
            // Bucle de corte
            for (cut_y = cut_start_y; cut_y < cut_end_y; cut_y ++) {
                for (cut_x = cut_start_x; cut_x < cut_end_x; cut_x ++) {
                    // Calcula el puntero de lectura
                    src_pointer = (((cut_y * in_width) + cut_x) << 2);
                    // Copia los 4 bytes del pixel de origen al buffer de destino
                    for (uint32_t n = src_pointer; n < (src_pointer + 4); n ++) {
                        sheet[dst_pointer] = raw[n];
                        dst_pointer ++;
                    }
                }
            }
        }
    }

    // Ok
    std::cout << "done!" << std::endl;
    std::cout << "Output sprite sheet size is " << out_width << "x" << out_height << " pixels." << std::endl;

    // Actualiza la informacion de la cabecera
    header.version = VERSION;                               // Version del programa
    sprintf(header.magic, "%s", MAGIC_STRING.c_str());      // Magic String
    header.sheet_width = out_width;                         // Dimensiones del strip
    header.sheet_height = out_height;
    header.frame_width = frame_width;                       // Dimensiones del frame
    header.frame_height = frame_height;
    header.total_frames = _total_frames;                    // Numero de frames del sprite
    for (uint32_t i = 0; i < sizeof(header.reserve); i ++) header.reserve[i] = 0;    // Espacio reservado

}
