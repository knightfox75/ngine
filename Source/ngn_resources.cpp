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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
// Includes de la libreria
#include "ngn.h"



/*** Puntero de la instancia a NULL ***/
NGN_Resources* NGN_Resources::instance = NULL;



/*** Metodo para crear/obtener la instancia ***/
NGN_Resources* NGN_Resources::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Resources();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Resources::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = NULL;
    }

}



/*** Constructor de la clase ***/
NGN_Resources::NGN_Resources() {

    // Prepara las estructuras de datos
    repository.clear();

}



/*** Destructor de la clase ***/
NGN_Resources::~NGN_Resources() {

    // Elimina los repositorios existentes y su contenido
    for (uint8_t i = 0; i < repository.size(); i ++) {
        Clear(repository[i].name);
    }
    repository.clear();

}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Resources::BootUp() {

}



/*** Crea un nuevo repositorio, si este ya existe, ignora la orden ***/
void NGN_Resources::AddRepository(std::string repo_name) {

    // Verifica si el repositorio ya existe, si es asi, sal
    for (uint8_t i = 0; i < repository.size(); i ++) {
        if (repo_name.compare(repository[i].name) == 0) return;
    }

    // Verifica si no se ha alcanzado el numero maximo de repositorios
    if (repository.size() >= (uint32_t)MAX_REPOSITORIES) {
        std::string txt = "[NGN_Resources error] Maximum number of repositories (";
        txt += ngn->toolbox->Int2String(MAX_REPOSITORIES, 3, "0");
        txt += ") reached.";
        ngn->log->Message(txt);
        return;
    }

    // Crea un repositorio temporal para su configuracion
    ResourcesT rs;
    rs.texture.clear();
    rs.tiledbg.clear();
    rs.sprite.clear();
    rs.cmap.clear();
    rs.sfx.clear();
    rs.txt.clear();
    rs.typeface.clear();
    rs.name = repo_name;

    // Añade el repositorio
    repository.push_back(rs);

}



/*** Elimina un repositorio, si este no existe, ignora la orden ***/
void NGN_Resources::RemoveRepository(std::string repo_name) {

    #if (defined MODE_DEBUG && defined RESOURCES_DEBUG)
        std::cout << "Current repositories: " << repository.size() << std::endl;
    #endif

    // Busca el ID del repositorio
    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return;

    // Borra el contenido del repositorio
    Clear(repo_name);

    // Elimina el repositorio de la lista
    repository.erase((repository.begin() + repo_id));

    #if (defined MODE_DEBUG && defined RESOURCES_DEBUG)
        std::cout << repo_name << " repository deleted." << std::endl;
        std::cout << "Actual repositories: " << repository.size() << std::endl;
    #endif

}



/*** Elimina el contenido de un repositorio ***/
void NGN_Resources::Clear(std::string repo_name) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return;

    // Texturas
    for (uint32_t i = 0; i < repository[repo_id].texture.size(); i ++) {
        #if (defined MODE_DEBUG && defined RESOURCES_DEBUG)
            std::cout << "Deleting Texture in " << repository[repo_id].name << ": " << i << " > " << repository[repo_id].texture[i].id << std::endl;
        #endif
        delete repository[repo_id].texture[i].data;
        repository[repo_id].texture[i].data = NULL;
    }
    repository[repo_id].texture.clear();

    // Fondos de tiles
    for (uint32_t i = 0; i < repository[repo_id].tiledbg.size(); i ++) {
        #if (defined MODE_DEBUG && defined RESOURCES_DEBUG)
            std::cout << "Deleting Tiled Background in " << repository[repo_id].name << ": " << i << " > " << repository[repo_id].tiledbg[i].id << std::endl;
        #endif
        delete repository[repo_id].tiledbg[i].data;
        repository[repo_id].tiledbg[i].data = NULL;
    }
    repository[repo_id].tiledbg.clear();

    // Sprites
    for (uint32_t i = 0; i < repository[repo_id].sprite.size(); i ++) {
        #if (defined MODE_DEBUG && defined RESOURCES_DEBUG)
            std::cout << "Deleting Sprite in " << repository[repo_id].name << ": " << i << " > " << repository[repo_id].sprite[i].id << std::endl;
        #endif
        delete repository[repo_id].sprite[i].data;
        repository[repo_id].sprite[i].data = NULL;
    }
    repository[repo_id].sprite.clear();

    // Mapas de colisiones
    for (uint32_t i = 0; i < repository[repo_id].cmap.size(); i ++) {
        #if (defined MODE_DEBUG && defined RESOURCES_DEBUG)
            std::cout << "Deleting Collisions Map in " << repository[repo_id].name << ": " << i << " > " << repository[repo_id].cmap[i].id << std::endl;
        #endif
        delete repository[repo_id].cmap[i].data;
        repository[repo_id].cmap[i].data = NULL;
    }
    repository[repo_id].cmap.clear();

    // SFX
    for (uint32_t i = 0; i < repository[repo_id].sfx.size(); i ++) {
        #if (defined MODE_DEBUG && defined RESOURCES_DEBUG)
            std::cout << "Deleting Sound Effect in " << repository[repo_id].name << ": " << i << " > " << repository[repo_id].sfx[i].id << std::endl;
        #endif
        delete repository[repo_id].sfx[i].data;
        repository[repo_id].sfx[i].data = NULL;
    }
    repository[repo_id].sfx.clear();

    // Archivos de texto
    #if (defined MODE_DEBUG && defined RESOURCES_DEBUG)
        for (uint32_t i = 0; i < repository[repo_id].txt.size(); i ++) {
            std::cout << "Deleting Text File in " << repository[repo_id].name << ": " << i << " > " << repository[repo_id].txt[i].id << std::endl;
        }
    #endif
    repository[repo_id].txt.clear();

    // Tipografias
    for (uint32_t i = 0; i < repository[repo_id].typeface.size(); i ++) {
        #if (defined MODE_DEBUG && defined RESOURCES_DEBUG)
            std::cout << "Deleting Type Face in " << repository[repo_id].name << ": " << i << " > " << repository[repo_id].typeface[i].id << std::endl;
        #endif
        delete repository[repo_id].typeface[i].data;
        repository[repo_id].typeface[i].data = NULL;
    }
    repository[repo_id].typeface.clear();

}



/*** Carga una lista de recursos en una estructura de datos ***/
bool NGN_Resources::Load(std::string repo_name, std::string filelist) {

    // Busca el ID del repositorio, sal si no se encuentra
    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return false;

    // Elimina el contenido actual del repositorio
    Clear(repo_name);

    // Carga el archivo de texto (en lineas)
    std::vector<std::string> text_lines;
    text_lines.clear();
    if (!ngn->load->TextFile(filelist, text_lines)) return false;

    // Conviertelo a FAT
    std::vector<FatT> fat;
    if (!ParseFileTable(text_lines, fat)) {
        ngn->log->Message("[NGN_Resources error] Error parsing " + filelist + ".");
        return false;
    }

    // Elimina el buffer del archivo de texto
    text_lines.clear();

    // Prepara los objetos temporales
    TextureT texture;
    TiledbgT tiledbg;
    SpriteT sprite;
    CmapT cmap;
    SfxT sfx;
    TxtT txt;
    TypefaceT typeface;

    // Carga los archivos
    for (uint32_t i = 0; i < fat.size(); i ++) {

        if (fat[i].filetype == FILETYPE_ID.texture) {

            texture.data = ngn->load->Texture(fat[i].filename);
            if (!texture.data) return false;
            texture.id = fat[i].id;
            repository[repo_id].texture.push_back(texture);

        } else if (fat[i].filetype == FILETYPE_ID.tiledbg) {

            tiledbg.data = ngn->load->TiledBg(fat[i].filename);
            if (!tiledbg.data) return false;
            tiledbg.id = fat[i].id;
            repository[repo_id].tiledbg.push_back(tiledbg);

        } else if (fat[i].filetype == FILETYPE_ID.sprite) {

            sprite.data = ngn->load->Sprite(fat[i].filename);
            if (!sprite.data) return false;
            sprite.id = fat[i].id;
            repository[repo_id].sprite.push_back(sprite);

        } else if (fat[i].filetype == FILETYPE_ID.cmap) {

            cmap.data = ngn->load->CollisionMap(fat[i].filename);
            if (!cmap.data) return false;
            cmap.id = fat[i].id;
            repository[repo_id].cmap.push_back(cmap);

        } else if (fat[i].filetype == FILETYPE_ID.sfx) {

            sfx.data = ngn->load->AudioClip(fat[i].filename);
            if (!sfx.data) return false;
            sfx.id = fat[i].id;
            repository[repo_id].sfx.push_back(sfx);

        } else if (fat[i].filetype == FILETYPE_ID.txt) {

            if (!ngn->load->TextFile(fat[i].filename, txt.data)) return false;
            txt.id = fat[i].id;
            repository[repo_id].txt.push_back(txt);

        } else if (fat[i].filetype == FILETYPE_ID.typeface) {

            uint32_t height = 0;
            bool antialias = false;
            uint32_t base_color = 0xFFFFFF;
            uint32_t outline = 0;
            uint32_t outline_color = 0x000000;

            if (fat[i].typeface.height != "") height = std::stoi(fat[i].typeface.height, nullptr, 0);
            if (fat[i].typeface.antialias != "") antialias = (std::stoi(fat[i].typeface.antialias, nullptr, 0) != 0) ? true:false;
            if (fat[i].typeface.base_color != "") base_color = std::stoi(fat[i].typeface.base_color, nullptr, 0);
            if (fat[i].typeface.outline != "") outline = std::stoi(fat[i].typeface.outline, nullptr, 0);
            if (fat[i].typeface.outline_color != "") outline_color = std::stoi(fat[i].typeface.outline_color, nullptr, 0);

            typeface.data = ngn->load->TrueTypeFont(
                fat[i].filename,
                height,
                antialias,
                base_color,
                outline,
                outline_color
            );
            if (!typeface.data) return false;
            typeface.id = fat[i].id;
            repository[repo_id].typeface.push_back(typeface);

        } else {

            ngn->log->Message("[NGN_Resources error] Unknow " + fat[i].filetype + " file type.");

        }

    }

    // Elimina la FAT
    fat.clear();

    // Carga correcta
    return true;

}



/*** Prepara la tabla de asignacion de archivos ***/
bool NGN_Resources::ParseFileTable(std::vector<std::string> &text, std::vector<FatT> &fat) {

    // Prepara la lista
    fat.clear();

    // Si no hay datos que procesar, sal
    if (text.size() == 0) return false;

    // Nodo de FAT
    FatT node;

    // Control
    uint32_t p_count = 0;
    uint32_t offset = 0;
    std::string line = "";
    std::string chr = "";
    uint8_t ascii = 0;

    // Lee las lineas del archivo
    for (uint32_t i = 0; i < text.size(); i ++) {

        // Si la linea esta vacia, saltatela
        if (text[i].size() == 0) continue;

        // Si el primer caracter de la linea, es el caracter de comentario o no es valido, saltala
        chr = text[i].at(0);
        ascii = (uint8_t)chr[0];
        if (
            (chr == COMMENT_CHAR)       // Comentario
            ||
            (ascii < 0x20)              // Antes del ESPACIO
            ||
            (ascii == 0x7F)             // DEL
            ||
            (ascii == 0xFF)             // BLANK
        ) continue;

        // Inicia el nodo
        node.filetype = "";
        node.id = "";
        node.filename = "";
        node.typeface.height = "";
        node.typeface.antialias = "";
        node.typeface.base_color = "";
        node.typeface.outline = "";
        node.typeface.outline_color = "";
        p_count = 0;
        offset = 0;
        line = text[i];

        // Lee la informacion de la linea y pasala al nodo temporal
        while (offset < line.size()) {
            chr = line.at(offset);
            if (chr == " ") {
                offset ++;
                continue;
            } else if (chr == CSV_CHAR) {
                offset ++;
                p_count ++;
                continue;
            }
            switch (p_count) {
                case 0:
                    node.filetype += chr;
                    break;
                case 1:
                    node.id += chr;
                    break;
                case 2:
                    node.filename += chr;
                    break;
                case 3:
                    node.typeface.height += chr;
                    break;
                case 4:
                    node.typeface.antialias += chr;
                    break;
                case 5:
                    node.typeface.base_color += chr;
                    break;
                case 6:
                    node.typeface.outline += chr;
                    break;
                case 7:
                    node.typeface.outline_color += chr;
                    break;
                default:
                    return false;
                    break;
            }
            offset ++;
        }

        // Si no se han completado el nodo, error
        if (
            ((node.filetype != FILETYPE_ID.typeface) && (p_count > 2))
            ||
            ((node.filetype == FILETYPE_ID.typeface) && (p_count > 7))
            ||
            (p_count < 2)
        ) return false;

        // Si todo ha ido bien, registra el nodo
        fat.push_back(node);

    }

    // Conversion correcta
    return true;

}



/*** Busca una textura ***/
NGN_TextureData* NGN_Resources::GetTexture(std::string repo_name, std::string resource_name, bool err) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return NULL;

    for (uint32_t i = 0; i < repository[repo_id].texture.size(); i ++) {
        if (resource_name.compare(repository[repo_id].texture[i].id) == 0) return repository[repo_id].texture[i].data;
    }

    if (err) ngn->log->Message("[NGN_Resources error] Texture with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return NULL;

}



/*** Busca un fondo de tiles ***/
NGN_TiledBgData* NGN_Resources::GetTiledbg(std::string repo_name, std::string resource_name, bool err) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return NULL;

    for (uint32_t i = 0; i < repository[repo_id].tiledbg.size(); i ++) {
        if (resource_name.compare(repository[repo_id].tiledbg[i].id) == 0) return repository[repo_id].tiledbg[i].data;
    }

    if (err) ngn->log->Message("[NGN_Resources error] Tiled background with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return NULL;

}



/*** Busca un sprite ***/
NGN_SpriteData* NGN_Resources::GetSprite(std::string repo_name, std::string resource_name, bool err) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return NULL;

    for (uint32_t i = 0; i < repository[repo_id].sprite.size(); i ++) {
        if (resource_name.compare(repository[repo_id].sprite[i].id) == 0) return repository[repo_id].sprite[i].data;
    }

    if (err) ngn->log->Message("[NGN_Resources error] Sprite with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return NULL;

}



/*** Busca un mapa de colisiones ***/
NGN_CollisionMapData* NGN_Resources::GetCmap(std::string repo_name, std::string resource_name, bool err) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return NULL;

    for (uint32_t i = 0; i < repository[repo_id].cmap.size(); i ++) {
        if (resource_name.compare(repository[repo_id].cmap[i].id) == 0) return repository[repo_id].cmap[i].data;
    }

    if (err) ngn->log->Message("[NGN_Resources error] Collision map with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return NULL;

}



/*** Busca un efecto de sonido ***/
NGN_AudioClipData* NGN_Resources::GetSfx(std::string repo_name, std::string resource_name, bool err) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return NULL;

    for (uint32_t i = 0; i < repository[repo_id].sfx.size(); i ++) {
        if (resource_name.compare(repository[repo_id].sfx[i].id) == 0) return repository[repo_id].sfx[i].data;
    }

    if (err) ngn->log->Message("[NGN_Resources error] SFX with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return NULL;

}



/*** Busca un archivo de texto ***/
std::vector<std::string> NGN_Resources::GetTxt(std::string repo_name, std::string resource_name, bool err) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) {
        std::vector<std::string> txt;
        txt.clear();
        return txt;
    }

    for (uint32_t i = 0; i < repository[repo_id].txt.size(); i ++) {
        if (resource_name.compare(repository[repo_id].txt[i].id) == 0) return repository[repo_id].txt[i].data;
    }

    if (err) ngn->log->Message("[NGN_Resources error] Text file with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    std::vector<std::string> txt;
    txt.clear();
    return txt;

}



/*** Busca una tipografia ***/
NGN_TextFont* NGN_Resources::GetTypeface(std::string repo_name, std::string resource_name, bool err) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return NULL;

    for (uint32_t i = 0; i < repository[repo_id].typeface.size(); i ++) {
        if (resource_name.compare(repository[repo_id].typeface[i].id) == 0) return repository[repo_id].typeface[i].data;
    }

    if (err) ngn->log->Message("[NGN_Resources error] Typeface with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return NULL;

}



/*** Elimina una textura ***/
bool NGN_Resources::RemoveTexture(std::string repo_name, std::string resource_name) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return false;

    for (uint32_t i = 0; i < repository[repo_id].texture.size(); i ++) {
        if (resource_name.compare(repository[repo_id].texture[i].id) != 0) continue;
        delete repository[repo_id].texture[i].data;
        repository[repo_id].texture.erase(repository[repo_id].texture.begin() + i);
        return true;
    }

    ngn->log->Message("[NGN_Resources error] Texture with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return false;

}



/*** Elimina un fondo de tiles ***/
bool NGN_Resources::RemoveTiledbg(std::string repo_name, std::string resource_name) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return false;

    for (uint32_t i = 0; i < repository[repo_id].tiledbg.size(); i ++) {
        if (resource_name.compare(repository[repo_id].tiledbg[i].id) != 0) continue;
        delete repository[repo_id].tiledbg[i].data;
        repository[repo_id].tiledbg.erase(repository[repo_id].tiledbg.begin() + i);
        return true;
    }

    ngn->log->Message("[NGN_Resources error] Tiled background with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return false;

}


/*** Elimina un sprite ***/
bool NGN_Resources::RemoveSprite(std::string repo_name, std::string resource_name) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return false;

    for (uint32_t i = 0; i < repository[repo_id].sprite.size(); i ++) {
        if (resource_name.compare(repository[repo_id].sprite[i].id) != 0) continue;
        delete repository[repo_id].sprite[i].data;
        repository[repo_id].sprite.erase(repository[repo_id].sprite.begin() + i);
        return true;
    }

    ngn->log->Message("[NGN_Resources error] Sprite with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return false;

}



/*** Elimina un mapa de colisiones ***/
bool NGN_Resources::RemoveCmap(std::string repo_name, std::string resource_name) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return false;

    for (uint32_t i = 0; i < repository[repo_id].cmap.size(); i ++) {
        if (resource_name.compare(repository[repo_id].cmap[i].id) != 0) continue;
        delete repository[repo_id].cmap[i].data;
        repository[repo_id].cmap.erase(repository[repo_id].cmap.begin() + i);
        return true;
    }

    ngn->log->Message("[NGN_Resources error] Collision map with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return false;

}



/*** Elimina un efecto de sonido ***/
bool NGN_Resources::RemoveSfx(std::string repo_name, std::string resource_name) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return false;

    for (uint32_t i = 0; i < repository[repo_id].sfx.size(); i ++) {
        if (resource_name.compare(repository[repo_id].sfx[i].id) != 0) continue;
        delete repository[repo_id].sfx[i].data;
        repository[repo_id].sfx.erase(repository[repo_id].sfx.begin() + i);
        return true;
    }

    ngn->log->Message("[NGN_Resources error] SFX with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return false;

}



/*** Elimina un archivo de texto ***/
bool NGN_Resources::RemoveTxt(std::string repo_name, std::string resource_name) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return false;

    for (uint32_t i = 0; i < repository[repo_id].txt.size(); i ++) {
        if (resource_name.compare(repository[repo_id].txt[i].id) != 0) continue;
        repository[repo_id].txt[i].data.clear();
        repository[repo_id].txt.erase(repository[repo_id].txt.begin() + i);
        return true;
    }

    ngn->log->Message("[NGN_Resources error] Text file with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return false;

}



/*** Elimina una tipografia ***/
bool NGN_Resources::RemoveTypeface(std::string repo_name, std::string resource_name) {

    int8_t repo_id = GetRepositoryId(repo_name);
    if (repo_id < 0) return false;

    for (uint32_t i = 0; i < repository[repo_id].typeface.size(); i ++) {
        if (resource_name.compare(repository[repo_id].typeface[i].id) != 0) continue;
        delete repository[repo_id].typeface[i].data;
        repository[repo_id].typeface.erase(repository[repo_id].typeface.begin() + i);
        return true;
    }

    ngn->log->Message("[NGN_Resources error] Typeface with " + resource_name + " ID in " + repository[repo_id].name + " repository not found.");
    return false;

}



/*** Devuelve el ID del repositorio solicitado ***/
int8_t NGN_Resources::GetRepositoryId(std::string repo_name) {

    for (uint8_t i = 0; i < repository.size(); i ++) {
        if (repo_name.compare(repository[i].name) == 0) return i;
    }

    ngn->log->Message("[NGN_Resources error] Repository " + repo_name + " not found.");
    return -1;

}

