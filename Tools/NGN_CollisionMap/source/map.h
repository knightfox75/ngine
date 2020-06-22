/******************************************************************************

    N'gine Lib for C++
    Conversor de PNG a Mapa de Colisiones (.map)

    Proyecto iniciado el 8 de Abril del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG
    (c) 2005 - 2020 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED



/*** Includes ***/
#include <vector>
#include <string>



/*** Defines ***/
static const int32_t VERSION = 2;                               // Version del archivo
static const std::string MAGIC_STRING = "NGN COLLISION MAP";    // Magic string



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
    uint8_t version;                // Version del programa de conversion
    char magic[32];                 // Magic String
    uint32_t width;                 // Ancho del mapa
    uint32_t height;                // Alto del mapa
    uint32_t tile_size;             // Tamaño del tile
    uint32_t pal_length;            // Tamaño de los datos (nº de elementos) de la paleta
    uint32_t tileset_length;        // Tamaño de los datos (nº de elementos) del tileset
    uint32_t map_length;            // Tamaño de los datos (nº de elementos) del mapa
    uint8_t reserve[256];           // Posible uso futuro
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
        uint32_t tile_size;             // Tamaño del tile


        /*** Metodos ***/

        // Convierte el archivo
        int32_t ConvertPng();



    // Private
    private:

        /*** Propiedades ***/

        // Define los vectores de memoria temporales
        std::vector<uint8_t> png;           // Buffer para almacenar la imagen PNG
        std::vector<uint8_t> raw;           // Buffer para almacenar los pixeles de la imagen de entrada
        std::vector<uint8_t> bitmap;        // Buffer para almacenar el mapa de pixeles
        // Define los vectores de memoria para el archivo de guardado
        std::vector<uint32_t> palette;      // Buffer para almacenar la paleta de colores
        std::vector<uint8_t> tiles;         // Buffer para almacenar los pixeles de los tiles
        std::vector<uint32_t> tmap;         // Buffer para almacenar el mapa de tiles
        // Define los vectores de memoria temporales
        std::vector<uint32_t> t_palette;    // Buffer para almacenar la paleta de colores
        std::vector<uint8_t> t_tiles;       // Buffer para almacenar los pixeles de los tiles
        std::vector<uint8_t> t_tl;          // Buffer para almacenar un tile

        // Parametros de las imagenes
        uint32_t in_width, in_height;               // Tamaño del archivo de entrada
        uint32_t out_width, out_height;             // Tamaño del archivo de salida

        // Parametros del mapa
        uint32_t pal_length;
        uint32_t tileset_length;
        uint32_t map_length;

        // Cabecera del archivo de salida
        file_header header;


        /*** Metodos ***/

        // Lee el archivo PNG y almacena los pixeles de la imagen en el buffer
        int32_t ReadPNG(std::string filename, std::vector<uint8_t> &data);

        // Graba el archivo empaquetado
        int32_t WriteFile(std::string filename);

        // Lee un pixel del buffer especificado
        uint32_t GetPixel(std::vector<uint8_t> &data, uint32_t offset);

        // Genera el la paleta y el bitmap de colisiones
        int32_t GenerateBitmap();

        // Genera la cabecera del archivo
        void GenerateHeader();

        // Genera los tiles y el mapa
        void GenerateMap();

        // Obten un tile del bitmap
        void GetTile(uint32_t pos_x, uint32_t pos_y);

};



#endif // MAP_H_INCLUDED
