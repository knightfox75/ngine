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



#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED



/*** Defines ***/
#define VERSION             1                           // Version del archivo
#define MAGIC_STRING        "NGN COLLISION MAP"         // Magic string



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
    uint32_t width;                 // Ancho del mapa
    uint32_t height;                // Alto del mapa
    uint8_t reserve[256];                  // Posible uso futuro
};



/*** Declaracion de la clase ***/
class CollisionMap {

    // Public
    public:

        // Constructor
        CollisionMap();

        // Destructor
        ~CollisionMap();

        /*** Propiedades ***/

        std::string input_filename;     // Fichero de entrada
        std::string output_filename;    // Nombre base del archivo de salida


        /*** Metodos ***/

        // Convierte el archivo
        int32_t ConvertPng();



    // Private
    private:

        /*** Propiedades ***/

        // Define los vectores de memoria
        std::vector<uint8_t> png;                 // Buffer para almacenar la imagen PNG
        std::vector<uint8_t> raw;                 // Buffer para almacenar los pixeles de la imagen de entrada
        std::vector<uint8_t> bitmap;              // Buffer para almacenar el mapa de pixeles
        std::vector<uint32_t> palette;              // Buffer para almacenar la paleta de colores

        // Parametros de las imagenes
        uint32_t in_width, in_height;               // Tamaño del archivo de entrada
        uint32_t out_width, out_height;             // Tamaño del archivo de salida

        // Cabecera del archivo de salida
        file_header header;


        /*** Metodos ***/

        // Lee el archivo PNG y almacena los pixeles de la imagen en el buffer
        int32_t ReadPNG(std::string filename, std::vector<uint8_t> &data);


        // Graba el archivo empaquetado
        int32_t WriteFile(std::string filename);

        // Lee un pixel del buffer especificado
        uint32_t GetPixel(std::vector<uint8_t> &data, uint32_t offset);

        // Genera el mapa de colisiones
        int32_t GenerateMap();


};



#endif // MAP_H_INCLUDED
