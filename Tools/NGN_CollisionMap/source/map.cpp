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

    // Borra los buffers de memoria
    png.clear();
    raw.clear();
    bitmap.clear();
    palette.clear();

}



/*** Convierte el archivo a un Sprite Sheet del tamaño solicitado ***/
int32_t CollisionMap::ConvertPng() {

    // Intenta cargar el archivo PNG, si hay error sal e informa.
    if (ReadPNG(input_filename, raw) > 0) return 1;

    // Genera el mapa de colisiones
    if (GenerateMap() > 0) return 1;

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

    // Graba el archivo principal
    std::ofstream file;
    sprintf(f, "%s", basename);
    file.open(f, std::ofstream::out | std::ofstream::binary);
    if (file.is_open()) {
        file.write((char*)&header, sizeof(header));                                     // Cabecera
        file.write((char*)&palette[0], (palette.capacity() * sizeof(uint32_t)));    // Paleta
        file.write((char*)&bitmap[0], bitmap.capacity());                               // Bitmap
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
    pixel = data[offset] | (data[(offset + 1)] << 8) | (data[(offset + 2)] << 16) | (data[(offset + 3)] << 24);
    /*} else {
        pixel.r = pixel.g = pixel.b = pixel.a = 0;
        std::cout << "Pixel read out of range." << std::endl;
    }*/

    return pixel;

}



/*** Genera el mapa de colisiones ***/
int32_t CollisionMap::GenerateMap() {

    // Medidas del mapa
    out_width = in_width;
    out_height = in_height;

    // Actualiza la informacion de la cabecera
    header.version = VERSION;                           // Version del programa
    sprintf(header.magic, MAGIC_STRING);         // Magic String
    header.width = out_width;
    header.height = out_height;
    for (uint32_t i = 0; i < sizeof(header.reserve); i ++) header.reserve[i] = 0;    // Espacio reservado

    // Prepara los bufferes de destino
    bitmap.clear();
    bitmap.resize((out_width * out_height), 0);
    palette.clear();
    palette.resize(256, 0);

    // Variables de uso temporal
    uint32_t pixel;         // Alamacena el valor del pixel
    int32_t id = 0;                 // Contenedor de ID de la paleta
    uint32_t pal_id = 0;    // Posicion actual en la paleta
    uint32_t offset = 0;    // Posicion actual en el mapa


    // Identifica el color de cada pixel e identificalo en la paleta, de no existir, añadelo
    for (uint32_t y = 0; y < out_height; y ++) {
        for (uint32_t x = 0; x < out_width; x ++) {
            // Color del pixel actual
            offset = ((y * out_width) + x);
            pixel = GetPixel(raw, (offset << 2));
            // Reset de ID
            id = -1;
            // Si el color coincide con alguno de la paleta...
            for (uint32_t c = 0; c < palette.size(); c ++) {
                if (pixel == palette[c]) {
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
                palette[pal_id] = pixel;
                // Guarda el pixel
                bitmap[offset] = (0xff & ((uint8_t)(pal_id)));
                // Siguente PAL ID
                pal_id ++;
            } else {
                // Guarda el Pixel
                bitmap[offset] = (0xff & ((uint8_t)(id)));
            }
            //std::cout << "OFFSET:" << offset << "   COLOR:" << pixel << " ID:" << (int32_t)_id << "  BITMAP:" << (int32_t)bitmap[offset] << std::endl;
        }
    }


    // Todo Ok
    return 0;


}
