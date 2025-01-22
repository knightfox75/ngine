/******************************************************************************

    N'gine Lib for C++
    *** Version 1.18.1-stable ***
    Definiciones de prototipos

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2024 by Cesar Rincon "NightFox"

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



#ifndef NGN_DEFINES_H_INCLUDED
#define NGN_DEFINES_H_INCLUDED


/*** Includes C++ ***/
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>


/*** Includes ***/
#include <SFML/Audio.hpp>



/*** Version de N'gine ***/
static const int32_t NGN_VERSION_MAJOR = 1;                     // Version mayor
static const int32_t NGN_VERSION_MINOR = 18;                    // Version menor
static const int32_t NGN_VERSION_PATCH = 1;                     // Version parche
static const std::string NGN_VERSION_METADATA = "stable";       // Version metadatos

/*** Definiciones generales ***/
static const int32_t NGN_DEFAULT_VALUE = 0x7FFFFFFF;            // Valor de "defecto"
static const uint32_t NGN_FPS_LIMIT = 60;                       // Limite del motor de render en FPS
/*** Definiciones de modos de pantalla ***/
static const int8_t NGN_SCR_WINDOW = 1;                         // "Modo ventana x1"
static const int8_t NGN_SCR_WINDOW_X2 = 2;                      // "Modo ventana x2"
static const int8_t NGN_SCR_WINDOW_X3 = 3;                      // "Modo ventana x3"
static const int8_t NGN_SCR_WINDOW_X4 = 4;                      // "Modo ventana x4"
static const int8_t NGN_SCR_WINDOW_FULL = 0;                    // "Modo ventana a pantalla completa"
static const int8_t NGN_SCR_FULLSCREEN = -1;                    // "Modo pantalla completa"
/*** Definiciones de constantes matematicas ***/
static const float PI = 3.14159265f;                            // Definicion del numero PI
static const float DEG2RAD = (PI / 180.0f);                     // Constante de conversion Grados a Radianes
static const float RAD2DEG = (180.0f / PI);                     // Constante de conversion Radianes a Grados
/*** Definiciones de modos de mezcla de color ***/
static const SDL_BlendMode NGN_BLENDMODE_NONE = SDL_BLENDMODE_NONE;          // Sin mezcla
static const SDL_BlendMode NGN_BLENDMODE_ALPHA = SDL_BLENDMODE_BLEND;        // Por nivel de alpha
static const SDL_BlendMode NGN_BLENDMODE_ADDITIVE = SDL_BLENDMODE_ADD;       // Mezcla aditiva
static const SDL_BlendMode NGN_BLENDMODE_MODULATE = SDL_BLENDMODE_MOD;       // Modulacion por color
/*** Definiciones de los modos de mascara ***/
static const uint8_t NGN_MASKMODE_CUTOUT = 0;       // Recorta el origen con la forma de la mascara
static const uint8_t NGN_MASKMODE_HOLLOW = 1;       // Extrae la mascara del origien



/*** Prototipos de datos ***/

struct Size2 {
    float width;
    float height;
};


struct Size2I32 {
    int32_t width;
    int32_t height;
};


struct Size2I64 {
    int64_t width;
    int64_t height;
};


struct Rgba {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};



/*** Propiedades obsoletas ***/

struct CanvasPoint {
    int32_t x;
    int32_t y;
    int32_t color;
};


struct CanvasLine {
    int32_t x1;
    int32_t y1;
    int32_t x2;
    int32_t y2;
    int32_t color;
};



/*** Prototipo de TEXTURA [NGN_TextureData] ***/
class NGN_TextureData {

    // Public
    public:

        // Constructor
        NGN_TextureData();

        // Destructor
        ~NGN_TextureData();

        SDL_Texture* gfx;               // Textura
        int32_t width;                  // Ancho de la textura
        int32_t height;                 // Alto de la textura


    // Private
    private:

};



/*** Prototipo de datos del FONDO TILEADO [NGN_TiledBgData] ***/
class NGN_TiledBgData {

    // Public
    public:

        // Constructor
        NGN_TiledBgData();

        // Destructor
        ~NGN_TiledBgData();

        // Cabecera del archivo
        struct {
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
        } header;

        // Contenido del fondo
        std::vector<SDL_Texture*> tiles;    // Almacena el Tileset
        std::vector<uint8_t> tmap;          // Almacena el mapa

    // Private
    private:

};



/*** Prototipo de datos de los SPRITES ANIMADOS [NGN_SpriteData] ***/
class NGN_SpriteData {

    // Public
    public:

        // Constructor
        NGN_SpriteData();

        // Destructor
        ~NGN_SpriteData();

        // Cabecera del archivo
        struct {
            uint8_t version;                // Version del programa de conversion
            char magic[32];                 // Magic string
            uint32_t sheet_width;           // Tamaño del sheet completo
            uint32_t sheet_height;
            uint32_t frame_width;           // Tamaño del frame
            uint32_t frame_height;
            uint32_t total_frames;          // Numero total de frames de la imagen
            uint8_t reserve[256];           // Posible uso futuro
        } header;


        // Contenido del fondo
        std::vector<SDL_Texture*> gfx;                   // Almacena el grafico de todos los frames

    // Private
    private:

};



/*** Prototipo de datos de los MAPAS DE COLISIONES [NGN_CollisionMapData] ***/
class NGN_CollisionMapData {

    // Public
    public:

        // Contructor
        NGN_CollisionMapData();

        // Destructor
        ~NGN_CollisionMapData();


        // Cabecera del archivo
        struct {
            uint8_t version;                // Version del programa de conversion
            char magic[32];                 // Magic String
            uint32_t width;                 // Ancho del mapa
            uint32_t height;                // Alto del mapa
            uint32_t tile_size;             // Tamaño del tile
            uint32_t pal_length;            // Tamaño de los datos (n� de elementos) de la paleta
            uint32_t tileset_length;        // Tamaño de los datos (n� de elementos) del tileset
            uint32_t map_length;            // Tamaño de los datos (n� de elementos) del mapa
            uint8_t reserve[256];           // Posible uso futuro
        } header;

        std::vector<uint32_t> palette;      // Buffer para almacenar la paleta de colores
        std::vector<uint8_t> tiles;         // Buffer para almacenar los pixeles de los tiles
        std::vector<uint32_t> tmap;         // Buffer para almacenar el mapa de tiles

        uint32_t tile_bytes;                // Tamaño de cada tile en bytes
        uint32_t tiles_row_width;           // Tamaño en pixeles de una fila de tiles


    // Private
    private:



};



/*** Prototipo de datos de los CLIPS DE SONIDO [NGN_AudioClipData] ***/
class NGN_AudioClipData {

    // Public
    public:

        // Constructor
        NGN_AudioClipData();

        // Destructor
        ~NGN_AudioClipData();

        // Buffer con el sonido
        sf::SoundBuffer data;

    // Private
    private:

};



/*** Prototipo de datos de las fuentes de texto ***/
class NGN_TextFont {

    // Public
    public:

        // Constructor
        NGN_TextFont();

        // Destructor
        ~NGN_TextFont();

        // Vector contenedor de los 256 caracteres de la fuente
        std::vector<NGN_TextureData*> character;

        // Alto de fila de la fuente cargada
        uint32_t height;

        // Separacion entre lineas
        uint32_t  line_spacing;

    // Private
    private:

};



/*** Prototipo de datos de las imagenes en RAW [NGN_RawImage] ***/
class NGN_RawImage {

    // Public
    public:

        // Constructor
        NGN_RawImage();

        // Destructor
        ~NGN_RawImage();

        // Buffer de pixeles
        std::vector<uint8_t> pixels;

        // Datos de la imagen
        uint32_t width;             // Ancho de la imagen
        uint32_t height;            // Alto de la imagen


    // Private
    private:

};



/*** Prototipo de datos de una superficie [NGN_RendererSurface] ***/
class NGN_RendererSurface {

    // Public
    public:

        // Constructor
        NGN_RendererSurface();

        // Destructor
        ~NGN_RendererSurface();

        // Superficie
        SDL_Surface* surface;

        int32_t width;          // Ancho de la superficie
        int32_t height;         // Alto de la superficie


    // Private
    private:

};



#endif // NGN_DEFINES_H_INCLUDED
