/******************************************************************************

    N'gine Lib for C++
    *** Version 1.21.0-wip0x03 ***
    Funciones de carga de archivos

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016 - 2026 by Cesar Rincon "NightFox"

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



#ifndef NGN_LOAD_H_INCLUDED
#define NGN_LOAD_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <string>
// Includes de SDL TTF
#include <SDL_ttf.h>
// Includes de la libreria
#include "ngn_filesystem.h"


/*** Defines ***/
static const std::string MAGIC_STRING_TBG = "NGN TILEDBG";
static const std::string MAGIC_STRING_SPR = "NGN SPRITE";
static const std::string MAGIC_STRING_CMAP = "NGN COLLISION MAP";
static const uint32_t COLLISION_MAP_MIN_TILE_SIZE = 8;
static const uint32_t COLLISION_MAP_MAX_TILE_SIZE = 1024;




/*** Definicion de la clase ***/
class NGN_Load {

   // Public
    public:

        // Devuelve la instancia
        static NGN_Load* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


        // Carga de texturas
        NGN_TextureData* Texture(std::string filepath);

        // Carga un fondo tileado
        NGN_TiledBgData* TiledBg(std::string filepath);

        // Carga un sprite
        NGN_SpriteData* Sprite(std::string filepath);

        // Carga un mapa de colisiones
        NGN_CollisionMapData* CollisionMap(std::string filepath);

        // Carga un archivo de audio en RAM
        NGN_AudioClipData* AudioClip(std::string filepath);

        // Carga y convierte una fuente TTF al formato de la libreria
        NGN_TextFont* TrueTypeFont(
            std::string filepath,                   // Archivo a cargar
            uint32_t height,                        // Altura de la fuente (en pixeles)
            bool antialias = true,                  // Antialias?
            uint32_t font_color = 0xFFFFFF,         // Color base
            uint32_t outline = 0,                   // Borde? (en pixeles)
            uint32_t outline_color = 0x000000       // Color del borde
        );


        // Carga una imagen PNG como RAW
        NGN_RawImage* PngAsRaw(std::string filepath);

        // Carga un fotograma de un sprite como RAW
        NGN_RawImage* SpriteAsRaw(std::string filepath, uint32_t frame = 0);

        // Carga los fotogramas de un sprite como un vector de RAWs
        bool SpriteAsRawVector(
            std::string filepath,                       // Archivo a cargar
            std::vector<NGN_RawImage*> &raw_frames,     // Vector de destino con los frames
            uint32_t first_frame = 0,                   // Frame inicial (0 por defecto)
            uint32_t last_frame = NGN_DEFAULT_VALUE     // Frame final (ultimo por defecto)
        );

        // Metodos para cargar archivos de texto
        std::string TextFile(std::string filepath);                                     // Primera sobrecarga
        bool TextFile(std::string filepath, std::vector<std::string> &text_lines);      // Segunda sobrecarga

        // Metodo para cargar un archivo desde de el origen predeterminado, en caso de estar encriptado, se desencriptara.
        int32_t LoadFile(std::string filepath, std::vector<uint8_t> &data);

        // Establece el disco como el origen de datos
        void SetDisk();

        // Establece un archivo empaquetado como el origen de datos
        bool SetPackage(std::string pkg_file, std::string key = "");

        // Devuelve si se esta usando actualmente un archivo de empaquetado de datos
        bool PackageEnabled();


    // Private
    private:

        // Contructor
        NGN_Load();

        // Destructor
        ~NGN_Load();

        // Puntero de memoria a la instancia
        static NGN_Load* instance;


        // Objeto para el acceso al sistema de archivos
        NGN_FileSystem* file_system;

        // Guarda el origen de los datos (Disco o empaquetado)
        bool use_package;


        // Carga los datos de un sprite
        bool LoadSpriteData(std::string filepath, std::vector<uint8_t> &img_pixels, NGN_SpriteData* spr);


};



#endif // NGN_LOAD_H_INCLUDED
