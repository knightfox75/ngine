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



#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED



/*** Defines ***/
static const int32_t VERSION = 2;                       // Version del archivo
static const std::string MAGIC_STRING = "NGN SPRITE";   // Magic string



/*** Includes ***/
#include <vector>
#include <string>



/*** Estructuras personalizadas ***/

// Pixel RGBA
struct rgba_pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// Cabecera del archivo
struct file_header {
    uint8_t version;              // Version del programa de conversion
    char magic[32];                     // Magic String
    uint32_t sheet_width;           // Tamaño del sheet completo
    uint32_t sheet_height;
    uint32_t frame_width;           // Tamaño del frame
    uint32_t frame_height;
    uint32_t total_frames;          // Numero total de frames de la imagen
    uint8_t reserve[256];                  // Posible uso futuro
};



/*** Declaracion de la clase ***/
class SpriteSheet {

    // Public
    public:

        // Constructor
        SpriteSheet();

        // Destructor
        ~SpriteSheet();

        /*** Propiedades ***/

        std::string input_filename;     // Fichero de entrada
        std::string output_filename;    // Nombre base del archivo de salida
        uint32_t frame_width;       // Tamaño del frame
        uint32_t frame_height;

        bool strip;                     // Debe generarse el PNG con el Strip?


        /*** Metodos ***/

        // Convierte el archivo
        int32_t ConvertPng();



    // Private
    private:

        /*** Propiedades ***/

        // Define los vectores de memoria
        std::vector<uint8_t> png;                 // Buffer para almacenar la imagen PNG
        std::vector<uint8_t> raw;                 // Buffer para almacenar los pixeles de la imagen de entrada
        std::vector<uint8_t> sheet;               // Buffer para almacenar la composicion del Sprite Sheet
        std::vector<uint8_t> out;                 // Buffer para almacenar la imagen de salida

        // Parametros de las imagenes
        uint32_t in_width, in_height;               // Tamaño del archivo de entrada
        uint32_t out_width, out_height;             // Tamaño del archivo de salida

        // Cabecera del archivo de salida
        file_header header;


        /*** Metodos ***/

        // Lee el archivo PNG y almacena los pixeles de la imagen en el buffer
        int32_t ReadPNG(std::string filename, std::vector<uint8_t> &data);

        // Graba el buffer a un archivo PNG
        int32_t WritePNG(std::string filename, std::vector<uint8_t> &data);

        // Graba el archivo empaquetado
        int32_t WriteFile(std::string filename);

        // Lee un pixel del buffer especificado
        rgba_pixel GetPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w);

        // Escribe el pixel en el buffer especificado
        void PutPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w, rgba_pixel pixel);

        // Corta la imagen de izquierda a derecha y de arriba a abajo, en bloques del tamaño del frame
        void GenerateSpriteSheet();


};



#endif // SPRITE_H_INCLUDED
