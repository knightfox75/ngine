/******************************************************************************

    N'gine Lib for C++
    *** Version 1.18.1-stable ***
    Funciones del gestor de recursos

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



#ifndef NGN_RESOURCES_H_INCLUDED
#define NGN_RESOURCES_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <vector>
#include <string>
// Includes de la libreria
#include "ngn_defines.h"



/*** FLAG's de configuracion ***/
// #define RESOURCES_DEBUG


/*** Declaracion de la clase ***/
class NGN_Resources {


    public:

        // Devuelve la instancia
        static NGN_Resources* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


    private:

        // Prototipo de datos: Texturas
        struct TextureT {
            NGN_TextureData* data;
            std::string id;
        };

        // Prototipo de datos: Fondos de tiles
        struct TiledbgT {
            NGN_TiledBgData* data;
            std::string id;
        };

        // Prototipo de datos: Sprites
        struct SpriteT {
            NGN_SpriteData* data;
            std::string id;
        };

        // Prototipo de datos: Mapas de collisiones
        struct CmapT {
            NGN_CollisionMapData* data;
            std::string id;
        };

        // Prototipo de datos: SFX
        struct SfxT {
            NGN_AudioClipData* data;
            std::string id;
        };

        // Prototipo de datos: TXT (Archivos de texto)
        struct TxtT {
            std::vector<std::string> data;
            std::string id;
        };

        // Prototipo de datos: Tipografias
        struct TypefaceT {
            NGN_TextFont* data;
            std::string id;
        };

        // Estructura de recursos
        struct ResourcesT {

            std::vector<TextureT> texture;
            std::vector<TiledbgT> tiledbg;
            std::vector<SpriteT> sprite;
            std::vector<CmapT> cmap;
            std::vector<SfxT> sfx;
            std::vector<TxtT> txt;
            std::vector<TypefaceT> typeface;

            std::string name;

        };

        // Repositoios de datos
        std::vector<ResourcesT> repository;


    public:

        // Crea un nuevo repositorio, si este ya existe, ignora la orden
        void AddRepository(std::string repo_name);

        // Elimina un repositorio, si este no existe, ignora la orden
        void RemoveRepository(std::string repo_name);

        // Elimina el contenido actual del repositorio indicado
        void Clear(std::string repo_name);

        // Carga una lista de recursos en el repositorio indicado, si no existe, devuelve un error
        bool Load(std::string repo_name, std::string filelist);

        // Busca y devuelve el recurso solicitado del repositorio especificado
        NGN_TextureData* GetTexture(std::string repo_name, std::string resource_name, bool err = true);
        NGN_TiledBgData* GetTiledbg(std::string repo_name, std::string resource_name, bool err = true);
        NGN_SpriteData* GetSprite(std::string repo_name, std::string resource_name, bool err = true);
        NGN_CollisionMapData* GetCmap(std::string repo_name, std::string resource_name, bool err = true);
        NGN_AudioClipData* GetSfx(std::string repo_name, std::string resource_name, bool err = true);
        std::vector<std::string> GetTxt(std::string repo_name, std::string resource_name, bool err = true);
        NGN_TextFont* GetTypeface(std::string repo_name, std::string resource_name, bool err = true);

        // Busca y elimina el recurso solicitado del repositorio especificado
        bool RemoveTexture(std::string repo_name, std::string resource_name);
        bool RemoveTiledbg(std::string repo_name, std::string resource_name);
        bool RemoveSprite(std::string repo_name, std::string resource_name);
        bool RemoveCmap(std::string repo_name, std::string resource_name);
        bool RemoveSfx(std::string repo_name, std::string resource_name);
        bool RemoveTxt(std::string repo_name, std::string resource_name);
        bool RemoveTypeface(std::string repo_name, std::string resource_name);



    private:

        // Constructor de la clase
        NGN_Resources();

        // Destructor de la clase
        ~NGN_Resources();

        // Puntero de memoria a la instancia
        static NGN_Resources* instance;

        // Devuelve el ID del repositorio solicitado
        int8_t GetRepositoryId(std::string repo_name);

        // Lista de identificadores de tipos de archivo
        const struct {
            std::string texture = "TEX";        // Texturas (.png)
            std::string tiledbg = "TBG";        // Fondos de tiles (.tbg)
            std::string sprite = "SPR";         // Sprites (.spr)
            std::string cmap = "MAP";           // Mapas de colisiones (.map)
            std::string sfx = "SFX";            // Efectos de sonido (.wav .ogg)
            std::string txt = "TXT";            // Archivos de texto (.txt)
            std::string typeface = "FNT";       // Fuentes tipograficas (.ttf .otf)
        } FILETYPE_ID;

        // Tabla de asignacion de archivos
        struct FatT {
            std::string filetype;
            std::string id;
            std::string filename;
            struct {
                std::string height;
                std::string antialias;
                std::string base_color;
                std::string outline;
                std::string outline_color;
            } typeface;
        };
        const std::string CSV_CHAR = ";";

        // Prepara la tabla de asignacion de archivos
        bool ParseFileTable(std::vector<std::string> &text, std::vector<FatT> &fat);

        // Caracteres de control
        const std::string COMMENT_CHAR = "#";

        // Numero maximo de repositorios
        const int8_t MAX_REPOSITORIES = 127;

};



#endif // NGN_RESOURCES_H_INCLUDED
