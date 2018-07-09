/******************************************************************************

    N'gine Lib for C++
    *** Version 0.6.1-alpha ***
    Funciones de carga de archivos

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
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



/*** Includes ***/
// C++
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

// SDL
#include <SDL.h>
#include <SDL_ttf.h>

// LodePNG
#include "lodepng.h"

// Libreria
#include "ngn.h"



/*** Contructor ***/
NGN_Load::NGN_Load() {
}



/*** Destructor ***/
NGN_Load::~NGN_Load() {
}



/*** Carga una textura ***/
NGN_TextureData* NGN_Load::Texture(std::string filepath) {

    const char* _filepath = filepath.c_str();

    // Crea una textura temporal que se usara como buffer
    NGN_TextureData* texture = new NGN_TextureData();
    // Crea un surface temporal para guardar la imagen cargada
    SDL_Surface* surface = NULL;

    // Crea un buffer temporal para la descompresion del PNG
    std::vector<uint8_t> png;
    std::vector<uint8_t> pixels;

    // Tamaño
    uint32_t w, h;

    // Carga el archivo PNG
    uint32_t error = lodepng::load_file(png, _filepath);

    // Si se ha cargado correctamente, decodifica la imagen
    if (error == 0) {
        error = lodepng::decode(pixels, w, h, png);
    } else {
        std::cout << "Error loading " << filepath << "." << std::endl;
        png.clear();
        return NULL;
    }

    // Crea la superficie en base a los pixeles cargados
    surface = SDL_CreateRGBSurfaceFrom(
                                        (char*)&pixels[0],     // Datos
                                        w,                      // Ancho
                                        h,                      // Alto
                                        32,                     // Profuncidad de color 32bpp [RGBA8888]
                                        (w << 2),               // Longitud de una fila de pixeles en bytes
                                        0x000000ff,             // Mascara R
                                        0x0000ff00,             // Mascara G
                                        0x00ff0000,             // Mascara B
                                        0xff000000              // Mascara A
                                        );

    // Elimina los buffers temporales
    png.clear();
    pixels.clear();

    // Verifica si el archivo se ha creado correctamente
    if (surface == NULL) {
         std::cout << "Unable to convert [" << filepath << "] to a surface." << std::endl;
         return NULL;
    }

    // Convierte la imagen cargada a textura
    texture->gfx = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface);
    // Verifica que la conversion ha sido correcta
    if (texture == NULL) {
         std::cout << "Unable to convert [" << filepath << "] to a texture." << std::endl;
         SDL_FreeSurface(surface);
         return NULL;
    }

    // Guarda los datos de la textura
    texture->width = surface->w;
    texture->height = surface->h;

    // Destruye el surface temporal antes de salir
    SDL_FreeSurface(surface);

    // Devuelve la textura cargada
    return texture;

}



/*** Carga un fondo tileado ***/
NGN_TiledBgData* NGN_Load::TiledBg(std::string filepath) {

    const char* _filepath = filepath.c_str();

    // Crea un objeto temporal para procesar el fondo cargado
    NGN_TiledBgData* bg = new NGN_TiledBgData();

    // Crea un buffer temporal para la descompresion del Tileset
    std::vector<uint8_t> tiles;
    std::vector<uint8_t> pixels;

    // Crea una superficie temporal para la generacion del tileset
    SDL_Surface* surface = NULL;

    // Intenta cargar el archivo
    std::ifstream file;
    file.open(_filepath, std::ifstream::in | std::ifstream::binary);
    if (file.is_open()) {
        // Carga la cabecera del archivo
        file.read((char*)&bg->header, sizeof(bg->header));
        // Tileset
        tiles.resize(bg->header.tileset_length);
        file.read((char*)&tiles[0], bg->header.tileset_length);
        // Mapa
        bg->tmap.resize(bg->header.map_length);
        file.read((char*)&bg->tmap[0], bg->header.map_length);
        file.close();
    } else {
        std::cout << "Error opening " << filepath << "  for read." << std::endl;
        delete bg;
        return NULL;
    }

    // Verifica que el archivo es compatible
    if (strcmp(bg->header.magic, MAGIC_STRING_TBG) != 0) {
        std::cout << "File " << filepath << "  is in unknow format." << std::endl;
        delete bg;
        return NULL;
    }


    // Decodifica los datos del tileset en PNG a Pixeles
    uint32_t w, h;
    if (lodepng::decode(pixels, w, h, tiles) > 0) {
        std::cout << "Error decoding " << filepath << "  tileset." << std::endl;
        tiles.clear();
        delete bg;
        return NULL;
    }

    // Crea la superficie en base a los pixeles cargados
    surface = SDL_CreateRGBSurfaceFrom(
                                        (char*)&pixels[0],      // Datos
                                        w,                      // Ancho
                                        h,                      // Alto
                                        32,                     // Profuncidad de color 32bpp [RGBA8888]
                                        (w << 2),               // Longitud de una fila de pixeles en bytes
                                        0x000000ff,             // Mascara R
                                        0x0000ff00,             // Mascara G
                                        0x00ff0000,             // Mascara B
                                        0xff000000              // Mascara A
                                        );


    // Borra los buffers temporales
    tiles.clear();
    pixels.clear();

    if (surface == NULL) {
         std::cout << "Unable to convert [" << filepath << "] to a surface." << std::endl;
         return NULL;
    }

    // Si se ha creado la superficie con exito, conviertela a textura
    bg->tiles = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface);
    // Verifica que la conversion ha sido correcta
    if (bg->tiles == NULL) {
         std::cout << "Unable to load [" << filepath << "] as a tileset." << std::endl;
         SDL_FreeSurface(surface);
         return NULL;
    }

    // Destruye el surface temporal antes de salir
    SDL_FreeSurface(surface);

    // Fondo cargado
    return bg;

}




/*** Carga un sprite ***/
NGN_SpriteData* NGN_Load::Sprite(std::string filepath) {

    const char* _filepath = filepath.c_str();

    // Crea un objeto temporal para procesar el sprite
    NGN_SpriteData* spr = new NGN_SpriteData();

    // Crea un buffer temporal para la descompresion del Tileset
    std::vector<uint8_t> raw;
    std::vector<uint8_t> pixels;

    // Crea una superficie temporal para la generacion del tileset
    SDL_Surface* surface = NULL;

    // Intenta cargar el archivo
    std::ifstream file;
    file.open(_filepath, std::ifstream::in | std::ifstream::binary);
    if (file.is_open()) {
        // Carga la cabecera del archivo
        file.read((char*)&spr->header, sizeof(spr->header));
        // Calcula el tamaño del buffer
        uint32_t buffer_size = ((spr->header.sheet_width * spr->header.sheet_height) << 2);      // * 4
        // Frames del sprite
        raw.resize(buffer_size);
        file.read((char*)&raw[0], buffer_size);
        file.close();
    } else {
        std::cout << "Error opening " << filepath << "  for read." << std::endl;
        delete spr;
        return NULL;
    }

    // Verifica que el archivo es compatible
    if (strcmp(spr->header.magic, MAGIC_STRING_SPR) != 0) {
        std::cout << "File " << filepath << "  is in unknow format." << std::endl;
        delete spr;
        return NULL;
    }


    // Decodifica los datos del sprite en PNG a pixeles.
    uint32_t w, h;
    if (lodepng::decode(pixels, w, h, raw) > 0) {
        std::cout << "Error decoding " << filepath << "  tileset." << std::endl;
        raw.clear();
        delete spr;
        return NULL;
    }
    // Borra el buffer temporal
    raw.clear();

    // Redimensiona el vector
    spr->gfx.resize(spr->header.total_frames);
    for (uint32_t i = 0; i < spr->gfx.size(); i ++) spr->gfx[i] = NULL;
    uint32_t frame_data_size = (spr->header.frame_width * spr->header.frame_height);

    // Crea una textura por cada fotograma
    for (uint32_t i = 0; i < spr->header.total_frames; i ++) {

        // Crea la superficie en base a los pixeles cargados
        surface = SDL_CreateRGBSurfaceFrom(
                                            (char*)&pixels[((frame_data_size * i) << 2)],   // Datos [frame * datos del frame * 4bpp]
                                            spr->header.frame_width,                        // Ancho
                                            spr->header.frame_height,                       // Alto
                                            32,                     // Profuncidad de color 32bpp [RGBA8888]
                                            (spr->header.frame_width << 2),                 // Longitud de una fila de pixeles en bytes
                                            0x000000ff,             // Mascara R
                                            0x0000ff00,             // Mascara G
                                            0x00ff0000,             // Mascara B
                                            0xff000000              // Mascara A
                                            );


        if (surface == NULL) {
             std::cout << "Unable to convert [" << filepath << "] to a surface." << std::endl;
             pixels.clear();
             return NULL;
        }

        // Si se ha creado la superficie con exito, conviertela a textura
        spr->gfx[i] = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface);

        // Verifica que la conversion ha sido correcta
        if (spr->gfx[i] == NULL) {
             std::cout << "Unable to load [" << filepath << "] as a sprite." << std::endl;
             SDL_FreeSurface(surface);
             pixels.clear();
             return NULL;
        }

        // Borra el surface creado
        SDL_FreeSurface(surface);

    }

    // Borra el buffer de pixeles
    pixels.clear();

    // Fondo cargado
    return spr;

}



/*** Carga un mapa de colisiones ***/
NGN_CollisionMapData* NGN_Load::CollisionMap(std::string filepath) {

    const char* _filepath = filepath.c_str();

    // Crea un objeto temporal para procesar el archivo
    NGN_CollisionMapData* collision = new NGN_CollisionMapData();

    // Intenta cargar el archivo
    std::ifstream file;
    file.open(_filepath, std::ifstream::in | std::ifstream::binary);
    if (file.is_open()) {
        // Carga la cabecera del archivo
        file.read((char*)&collision->header, sizeof(collision->header));
        // Carga la paleta
        collision->palette.resize(collision->header.pal_length, 0);
        file.read((char*)&collision->palette[0], (collision->header.pal_length * sizeof(int32_t)));
        // Carga los tiles
        collision->tiles.resize(collision->header.tileset_length, 0);
        file.read((char*)&collision->tiles[0], collision->header.tileset_length);
        // Carga el mapa
        collision->tmap.resize(collision->header.map_length, 0);
        file.read((char*)&collision->tmap[0], (collision->header.map_length * sizeof(int32_t)));
        // Cierra el archivo
        file.close();
    } else {
        std::cout << "Error opening " << filepath << "  for read." << std::endl;
        delete collision;
        return NULL;
    }

    // Verifica que el archivo es compatible
    if (strcmp(collision->header.magic, MAGIC_STRING_CMAP) != 0) {
        std::cout << "File " << filepath << "  is in unknow format." << std::endl;
        delete collision;
        return NULL;
    }

    // Calcula los datos adicionales del mapa de colisiones
    collision->tiles_row_width = (collision->header.width / collision->header.tile_size);
    if ((collision->header.width % collision->header.tile_size) != 0) collision->tiles_row_width ++;
    collision->tile_bytes = (collision->header.tile_size * collision->header.tile_size);

    /*
    uint32_t bb = 0;
    for (uint32_t z = 0; z < collision->tmap.capacity(); z ++) {
        bb = collision->tmap[z];
        std::cout << z << ":" << bb << std::endl;
    }
    std::cout << "Debug: Collision Map Loading" << std::endl;
    std::cout << "Map size: " << collision->header.width << "x" << collision->header.height << std::endl;
    std::cout << "Size of the tile: " << collision->header.tile_size << std::endl;
    std::cout << "Palette length: " << collision->palette.capacity() << std::endl;
    std::cout << "Tileset length: " << (collision->tiles.capacity() / collision->tile_bytes) << std::endl;
    std::cout << "Map length: " << collision->tmap.capacity() << std::endl;
    std::cout << "Row width: " << collision->tiles_row_width << std::endl;
    std::cout << "Tile space: " << collision->tile_bytes << std::endl;
    */

    // Devuelve el mapa cargado
    return collision;

}



/*** Carga un archivo de audio ***/
NGN_AudioClipData* NGN_Load::AudioClip(std::string filepath) {

    const char* _filepath = filepath.c_str();

    // Crea un archivo temporal con el audio
    NGN_AudioClipData* clip = new NGN_AudioClipData();

    // Carga el archivo
    if (clip->data.loadFromFile(_filepath)) {
        return clip;
    } else {
        std::cout << "Error opening " << filepath << "  for read." << std::endl;
        return NULL;
    }

}



/*** Carga y convierte una fuente TFF al formato de la libreria ***/
NGN_TextFont* NGN_Load::TrueTypeFont(
    std::string filepath,                   // Archivo a cargar
    uint32_t height,                        // Altura de la fuente (en pixeles)
    bool antialias,                         // Antialias?
    uint32_t font_color,                    // Color base
    uint32_t outline,                       // Borde? (en pixeles)
    uint32_t outline_color                  // Color del borde
) {

    const char* _filepath = filepath.c_str();

    // Intenta iniciar el subsitema de TTF para SDL
    if (TTF_Init() < 0) {
        std::cout << "SDL TTF initialization failed." << std::endl;
        return NULL;
    }

    // Crea un objeto temporal para almacenar la fuente resultante
    NGN_TextFont* font = new NGN_TextFont();

    // Intenta cargar la tipografia
    TTF_Font* ttf = TTF_OpenFont(_filepath, height);
    if (ttf == NULL) {
        std::cout << "Error opening " << filepath << "  for read." << std::endl;
        return NULL;
    }

    // Ajuste del outline
    if (outline > 0) {
        outline = (outline >= (height / 5)) ? (height / 5) : outline;
        outline = (outline < 1) ? 1 : outline;
    }

    // Guarda los parametros de la fuente
    /*
    TTF_SetFontOutline(ttf, outline);
    font->height = TTF_FontHeight(ttf);
    TTF_SetFontOutline(ttf, 0);
    font->line_spacing = (font->height + 1);
    */

    // Crea una superficie temporal para la generacion de la tipografia
    SDL_Surface* surface = NULL;
    SDL_Surface* surface_outline = NULL;

    // Crea el color por defecto del texto (blanco por defecto)
    SDL_Color _font_color;
    _font_color.a = 0xFF;
    _font_color.r = (font_color & 0xFF0000) >> 16;
    _font_color.g = (font_color & 0x00FF00) >> 8;
    _font_color.b = (font_color & 0x0000FF);

    // Crea el color por defecto del contorno (negro por defecto)
    SDL_Color _outline_color;
    _outline_color.a = 0xFF;
    _outline_color.r = (outline_color & 0xFF0000) >> 16;
    _outline_color.g = (outline_color & 0x00FF00) >> 8;
    _outline_color.b = (outline_color & 0x0000FF);

    // Marcos de dibujado de las superficies
    SDL_Rect rect_in = {0, 0, 0, 0};
    SDL_Rect rect_out = {0, 0, 0, 0};

    // Conversion de int a char
    unsigned char t[2];
    t[1] = '\0';

    // Renderiza todos los caracteres de la tabla ASCII >= 32 (<32 reservados)
    for (uint32_t i = 0x20; i <= 0xFF; i ++) {
        // Vacia el contenido previo del surface
        SDL_FreeSurface(surface);
        if (outline > 0) SDL_FreeSurface(surface_outline);
        // Renderiza el caracter en el surface
        t[0] = i;
        if (antialias) {
            surface = TTF_RenderText_Blended(ttf, (const char*)t, _font_color);
            if (outline > 0) {
                TTF_SetFontOutline(ttf, outline);
                surface_outline = TTF_RenderText_Blended(ttf, (const char*)t, _outline_color);
                TTF_SetFontOutline(ttf, 0);
            }
        } else {
            surface = TTF_RenderText_Solid(ttf, (const char*)t, _font_color);
            if (outline > 0) {
                TTF_SetFontOutline(ttf, outline);
                surface_outline = TTF_RenderText_Solid(ttf, (const char*)t, _outline_color);
                TTF_SetFontOutline(ttf, 0);
            }
        }
        // Si hay outline, haz la mezcla
        if (outline > 0) {
            // Mezcla las dos surfaces
            rect_in.x = outline;
            rect_in.y = outline;
            rect_in.w = surface->w;
            rect_in.h = surface->h;
            rect_out.x = 0;
            rect_out.y = 0;
            rect_out.w = surface_outline->w;
            rect_out.h = surface_outline->h;
            if (antialias) {
                SDL_SetSurfaceBlendMode(surface_outline, SDL_BLENDMODE_BLEND);
            } else {
                SDL_SetSurfaceBlendMode(surface_outline, SDL_BLENDMODE_NONE);
            }
            SDL_BlitSurface(surface, &rect_out, surface_outline, &rect_in);
            // Pasalo a la textura
            TTF_SetFontOutline(ttf, outline);
            if (
                (surface_outline != NULL)
                &&
                (surface != NULL)
                &&
                (TTF_SizeText(ttf, (const char*)t, &font->character[i]->width, &font->character[i]->height) == 0)
            ) {
                font->character[i]->gfx = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface_outline);
            }
            TTF_SetFontOutline(ttf, 0);
        } else {
            // Pasalo a la textura
            if (
                (surface != NULL)
                &&
                (TTF_SizeText(ttf, (const char*)t, &font->character[i]->width, &font->character[i]->height) == 0)
            ) {
                font->character[i]->gfx = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface);
            }
        }

        // Calcula la altura maxima del texto
        if ((uint32_t)font->character[i]->height > font->height) {
            font->height = (uint32_t)font->character[i]->height;
            font->line_spacing = (font->height + 1);
        }

    }

    // Paso de limpieza
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface_outline);
    TTF_CloseFont(ttf); ttf = NULL;

    // Cierra el subsistema de TTF
    TTF_Quit();

    // Devuelve la fuente cargada y convertida
    return font;

}
