/******************************************************************************

    Conversor de PNG a Sprite (.spr) para N'gine
    - ConvertToSprite -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Sprite is under MIT License

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
#include "convert_to_sprite.h"
#include "defines.h"



/*** Constructor ***/
ConvertToSprite::ConvertToSprite() {

    in_width = 0;
    in_height = 0;
    out_width = 0;
    out_height = 0;

    frame_width = 0;
    frame_height = 0;
    total_frames = 0;

    sprite_strip = false;

    sheet.clear();

}



/*** Destructor ***/
ConvertToSprite::~ConvertToSprite() {

    sheet.clear();

}


/*** Convierte un archivo PNG en un Sprite ***/
bool ConvertToSprite::Convert(
    std::string in_file,        // Archivo PNG a convertir
    std::string out_file,       // Nombre base de los archivos de salida
    uint32_t frame_w,           // Tama�o del tile
    uint32_t frame_h,           // Nivel de optimizacion
    bool strip                  // Generar archivos adicionales?
) {

    // Prepara el buffer para los pixeles de la imagen
    std::vector<uint8_t> png_pixels;
    png_pixels.clear();

    // Intenta abrir y decodificar el archivo PNG
    if (!ReadPng(in_file, png_pixels)) return false;

    // Verifica que el tama�o de imagen coincida con los datos del tama�o de fotograma
    if (
        (frame_w > in_width)
        ||
        (frame_h > in_height)
        ||
        ((in_width % frame_w) != 0)
        ||
        ((in_height % frame_h) != 0)
    ) {
        std::cout << "Image size doesn't fits the sprite frame size." << std::endl;
        png_pixels.clear();
        return false;
    }

    // Guarda los parametros del fotograma
    frame_width = frame_w;
    frame_height = frame_h;

    // Guarda los datos adicionales
    sprite_strip = strip;

    // Corta el PNG en los diversos fotogramas que contiene y colocalos en una tira
    CutFrames(png_pixels);

    // Guarda en el archivo empaquetado los datos generados
    bool r = WriteFile(out_file);

    // Vacia los buffers usados
    sheet.clear();
    png_pixels.clear();

    // Conversion con exito
    return r;

}


/*** Corta y almacena los diferentes fotogramas del sprite ***/
void ConvertToSprite::CutFrames(std::vector<uint8_t> &data) {

    // Variables
    uint32_t img_x = 0, img_y = 0;                                          // Punto de corte
    uint32_t _frames_w = (uint32_t)std::floor(in_width / frame_width);      // Numero de columnas de frames
    uint32_t _frames_h = (uint32_t)std::floor(in_height / frame_height);    // Numero de filas de frames
    uint32_t _total_frames = (_frames_w * _frames_h);                       // Numero total de frames
    uint32_t _current_frame = 0;                                            // Fotograma actual

    // Calcula el tama�o del archivo de salida
    out_width = frame_width;
    out_height = (_total_frames * frame_height);
    // Y guarda el numero total de frames
    total_frames = _total_frames;

    // Prepara los bufferes a usar
    sheet.clear();
    sheet.resize(((out_width * out_height) << 2), 0);   // * 4

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
                        sheet[dst_pointer] = data[n];
                        dst_pointer ++;
                    }
                }
            }
        }
    }

    // Ok
    std::cout << "done!" << std::endl;
    std::cout << "Output sprite sheet size is " << out_width << "x" << out_height << " pixels." << std::endl;

}



/*** Graba el archivo empaquetado ***/
bool ConvertToSprite::WriteFile(std::string filename) {

    // Prepara el buffer temporal
    std::vector<uint8_t> spritesheet_png;
    spritesheet_png.clear();

    // Intenta codificar el Tileset a PNG
    if (lodepng::encode(spritesheet_png, sheet, out_width, out_height) != 0) {
        std::cout << "Error encoding the spritesheet image data." << std::endl;
        spritesheet_png.clear();
        return false;
    }

    // Guarda la informacion para la cabecera del archivo
    FileHeader header;
    memset((void*)&header, 0, sizeof(header));
    header.version = VERSION;                                                       // Version del programa
    strncpy(header.magic, MAGIC_STRING.c_str(), MAGIC_STRING.length());             // Magic String
    header.sheet_width = out_width;                                                 // Dimensiones del strip
    header.sheet_height = out_height;
    header.frame_width = frame_width;                                               // Dimensiones del frame
    header.frame_height = frame_height;
    header.total_frames = total_frames;                                             // Numero de frames del sprite

    // Genera el nombre de archivo
    std::string fname = filename + SPR_EXTENSION;
    const char* sprite_filename = fname.c_str();

    // Graba el archivo principal
    std::ofstream file;
    file.open(sprite_filename, std::ofstream::out | std::ofstream::binary);
    if (file.is_open()) {
        file.write((char*)&header, sizeof(header));                             // Cabecera
        file.write((char*)&spritesheet_png[0], spritesheet_png.capacity());     // Sprite sheet en PNG
        file.close();
        std::cout << "File " << fname << " successfully saved." << std::endl;
    } else {
        std::cout << "Error saving " << fname << "." << std::endl;
        return false;
    }

    // Elimia los datos del PNG del spritesheet
    spritesheet_png.clear();

    // Si no se requiere el archivo de tira de fotogramas, termina
    if (!sprite_strip) return true;

    // Guarda el tileset en PNG
    fname = (filename + STRIP_EXTENSION);
    if (!WritePng(fname, sheet, out_width, out_height)) return false;

    // Todo correcto
    return true;

}



/*** Lee un archivo .PNG y coloca los pixeles en un buffer ***/
bool ConvertToSprite::ReadPng(std::string filename, std::vector<uint8_t> &data) {

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
    in_width = width;
    in_height = height;

    // Borra el buffer temporal
    png_data.clear();

    // Fin de la funcion de carga
    return true;

}



/*** Graba un buffer de pixeles en un archivo PNG ***/
bool ConvertToSprite::WritePng(std::string filename, std::vector<uint8_t> &data, uint32_t width, uint32_t height) {

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
