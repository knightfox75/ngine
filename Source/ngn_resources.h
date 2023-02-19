/******************************************************************************

    N'gine Lib for C++
    *** Version 1.12.0-stable ***
    Funciones del gestor de recursos

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


    N'gine se distribuye bajo la licencia CREATIVE COMMONS
    "Attribution-NonCommercial 4.0 International"
    https://creativecommons.org/licenses/by-nc/4.0/

    You are free to:

        - Share
        copy and redistribute the material in any medium or format.
        - Adapt
        remix, transform, and build upon the material.

        The licensor cannot revoke these freedoms as long as you follow
        the license terms.

    Under the following terms:

        - Attribution
        You must give appropriate credit, provide a link to the license,
        and indicate if changes were made. You may do so in any reasonable
        manner, but not in any way that suggests the licensor endorses you
        or your use.

        - NonCommercial
        You may not use the material for commercial purposes.

        - No additional restrictions
        You may not apply legal terms or technological measures that
        legally restrict others from doing anything the license permits.

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

        // Busca y devuelve el recurso solicitado de la tabla dada
        NGN_TextureData* GetTexture(std::string repo_name, std::string resource_name);
        NGN_TiledBgData* GetTiledbg(std::string repo_name, std::string resource_name);
        NGN_SpriteData* GetSprite(std::string repo_name, std::string resource_name);
        NGN_CollisionMapData* GetCmap(std::string repo_name, std::string resource_name);
        NGN_AudioClipData* GetSfx(std::string repo_name, std::string resource_name);
        std::vector<std::string> GetTxt(std::string repo_name, std::string resource_name);
        NGN_TextFont* GetTypeface(std::string repo_name, std::string resource_name);


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
