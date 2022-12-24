/******************************************************************************

    Conversor de PNG a Sprite (.spr) para N'gine
    - ConvertToSprite -

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



#ifndef CONVERT_TO_SPRITE_H_INCLUDED
#define CONVERT_TO_SPRITE_H_INCLUDED



/*** Includes ***/
// C++
#include <vector>
#include <string>
// Proyecto
#include "defines.h"



/*** Declaracion de la clase ***/
class ConvertToSprite {

    public:

        // Constructor
        ConvertToSprite();

        // Destructor
        ~ConvertToSprite();

        /*** Propiedades ***/


        /*** Metodos ***/
        // Convierte un archivo PNG en un Sprite
        bool Convert(
            std::string in_file,        // Archivo PNG a convertir
            std::string out_file,       // Nombre base de los archivos de salida
            uint32_t frame_w,           // Tamaño del tile
            uint32_t frame_h,           // Nivel de optimizacion
            bool strip                  // Generar archivos adicionales?
        );


    private:

        /*** Propiedades ***/

        // Parametros de las imagenes
        uint32_t in_width, in_height;               // Tamaño del archivo de entrada
        uint32_t out_width, out_height;             // Tamaño del archivo de salida

        // Parametros de los fotogramas
        uint32_t frame_width;                       // Tamaño del frame
        uint32_t frame_height;
        uint32_t total_frames;

        // Parametros adicionales
        bool sprite_strip;

        // Buffers
        std::vector<uint8_t> sheet;                 // Buffer para almacenar la composicion del Sprite Sheet


        /*** Metodos ***/

        // Corta y almacena los diferentes fotogramas del sprite
        void CutFrames(std::vector<uint8_t> &data);

        // Graba el archivo empaquetado
        bool WriteFile(std::string filename);

        // Lee el archivo .PNG y almacena los pixeles de la imagen en el buffer
        bool ReadPng(std::string filename, std::vector<uint8_t> &data);
        // Graba un buffer de pixeles en un archivo PNG
        bool WritePng(std::string filename, std::vector<uint8_t> &data, uint32_t width, uint32_t height);

};



#endif // CONVERT_TO_SPRITE_H_INCLUDED
