/******************************************************************************

    N'gine Lib for SDL2
    Conversor de PNG a TILES + MAP [Funciones]

    Proyecto iniciado el 11 de Febrero del 2016
    (cc) 2016 - 2017 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG
    (c) 2005 - 2016 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



#ifndef TILEMAP_H_INCLUDED
#define TILEMAP_H_INCLUDED


/*** Defines ***/
#define VERSION             2                   // Version del archivo
#define MAGIC_STRING        "NGN TILEDBG"       // Magic String
#define DEFAULT_TILE_SIZE   32                  // Tamaño por defecto del TILE



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
    uint8_t version;                // Version del programa de mapeado
    char magic[32];                 // Magic string
    uint32_t bg_width;              // Ancho del fondo (imagen original)
    uint32_t bg_height;             // Alto del fondo (imagen original)
    uint32_t tileset_width;         // Ancho del Tileset en pixeles
    uint32_t tileset_height;        // Ancho del Tileset en pixeles
    uint32_t map_width;             // Ancho del mapa (segun los tiles) en pixeles
    uint32_t map_height;            // Alto del mapa (segun los tiles) en pixeles
    uint32_t tile_size;             // Tamaño del tile
    uint32_t tileset_length;        // Tamaño de los datos del tileset
    uint32_t map_length;            // Tamaño de los datos del mapa
    char reserve[256];              // Posible uso futuro
};


/*** Clase para la conversion del archivo PNG ***/
class PngToTiles {

    // Segmento Publico
    public:

        // Conctructor
        PngToTiles();

        // Destructor
        ~PngToTiles();


        // Propiedades publicas
        std::string input_filename;     // Fichero de entrada
        std::string output_filename;    // Nombre base del archivo de salida
        uint32_t size_of_tile;      // Tamaño del tile [16x16 por defecto]
        bool extra_files;               // Generar archivos adicionales?
        uint32_t optimize;          // Nivel de optimizacion


        // Convierte el archivo
        int32_t ConvertPng();





    // Segmento privado
    private:

        /*** Propiedades ***/

        // Define los vectores de memoria
        std::vector<uint8_t> png;                 // Buffer para almacenar la imagen PNG
        std::vector<uint8_t> raw;                 // Buffer para almacenar los pixeles de la imagen de entrada
        std::vector<uint8_t> out;                 // Buffer para almacenar la imagen de salida

        std::vector<uint8_t> tiles;               // Buffer para almacenar los tiles del tileset
        std::vector<uint8_t> tmap;                 // Buffer para almacenar el mapa

        std::vector<uint8_t> img_tile;            // Almacena un tile de la imagen de Origen
        std::vector<uint8_t> map_tile;            // Almacena un tile del mapa de destino

        std::vector<uint8_t> buffer;              // Buffer temporal de datos

        // Parametros de las imagenes
        uint32_t out_width, out_height;             // Tamaño del archivo de salida
        uint32_t bg_width, bg_height;               // Tamaño real del fondo
        uint32_t map_width, map_height;             // Tamaño del mapa a generar
        uint32_t tileset_length;                    // Numero de tiles unicos del tileset

        // Cabecera del archivo de salida
        file_header header;


        /*** Metodos ***/

        // Lee el archivo PNG y almacena los pixeles de la imagen en el buffer
        int32_t ReadPNG(std::string filename, std::vector<uint8_t> &data);

        // Graba el buffer a un archivo PNG
        int32_t WritePNG(std::string filename, std::vector<uint8_t> &data);

        // Convierte la imagen en tiles + mapa
        void GenerateTileset();

        // Graba el archivo empaquetado
        int32_t WriteFile(std::string filename);

        // Lee un pixel del buffer especificado
        rgba_pixel GetPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w);

        // Escribe el pixel en el buffer especificado
        void PutPixel(std::vector<uint8_t> &data, uint32_t x, uint32_t y, uint32_t w, rgba_pixel pixel);

        // Lee un tile del buffer especificado
        void GetTile(std::vector<uint8_t> &tl, std::vector<uint8_t> &img, uint32_t x, uint32_t y, uint32_t max_w, uint32_t max_h);

        // Escribe un tile en el buffer especificado
        void PutTile(std::vector<uint8_t> &tl, std::vector<uint8_t> &img, uint32_t x, uint32_t y, uint32_t w);

        // Realiza la optimizacion de tiles unicos
        uint32_t OptimizeTiles();

        // Compara si 2 tiles son identicos (img_tile vs map_tile)
        bool CompareNormal();

        // Compara si 2 tiles estan en espejo horizontal (H FLIP)
        bool CompareFlipH();

        // Compara si 2 tiles estan en espejo vertical (V FLIP)
        bool CompareFlipV();

        // Compara si 2 tiles estan en espejo completo (H & V FLIP)
        bool Compare180deg();

        // Compara si 2 tiles estan rotadas 90º a la derecha (90º Clock Wise)
        bool Compare90degCW();

        // Compara si 2 tiles estan rotadas 90º a la izquierda (90º Anti Clock Wise)
        bool Compare90degACW();

};



#endif // TILEMAP_H_INCLUDED
