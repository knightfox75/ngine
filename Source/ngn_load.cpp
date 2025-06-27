/******************************************************************************

    N'gine Lib for C++
    *** Version 1.19.0-stable ***
    Funciones de carga de archivos

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2025 by Cesar Rincon "NightFox"

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



/*** Definiciones para la depuracion ***/
// #define NGN_LOAD_DEBUG



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
#include "lodepng/lodepng.h"

// Libreria
#include "ngn.h"



/*** Puntero de la instancia a nullptr ***/
NGN_Load* NGN_Load::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
NGN_Load* NGN_Load::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Load();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Load::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Contructor ***/
NGN_Load::NGN_Load() {

    // Objeto para el acceso al file system
    file_system = new NGN_FileSystem();

    // Por defecto no uses un archivo empaquetado (usa el disco)
    use_package = false;

}



/*** Destructor ***/
NGN_Load::~NGN_Load() {

    // Objeto para el acceso al file system
    delete file_system;
    file_system = nullptr;

}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Load::BootUp() {
}



/*** Carga una textura ***/
NGN_TextureData* NGN_Load::Texture(std::string filepath) {

    // Crea los buffers temporales para la descompresion del PNG
    std::vector<uint8_t> buffer;
    std::vector<uint8_t> pixels;

    // Tamaño de la imagen
    uint32_t w, h;

    // Intenta abrir el archivo
    if (LoadFile(filepath, buffer) > 0) {
        // Si se ha abierto correctamente, intenta decodificarlo
        if (lodepng::decode(pixels, w, h, buffer) > 0) {
            // Si no puedes decodificarlo, error
            ngn->log->Message("[NGN_Load error] Error decoding <" + filepath + "> texture.");
            buffer.clear();
            pixels.clear();
            return nullptr;
        }
    } else {
        ngn->log->Message("[NGN_Load error] Error loading <" + filepath + ">.");
        buffer.clear();
        return nullptr;
    }

    // Crea un surface temporal para guardar la imagen cargada
    SDL_Surface* surface = nullptr;

    // Crea la superficie en base a los pixeles cargados
    surface = SDL_CreateRGBSurfaceFrom(
                                        (uint8_t*)&pixels[0],   // Datos
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
    buffer.clear();
    pixels.clear();

    // Verifica si el archivo se ha creado correctamente
    if (!surface) {
         ngn->log->Message("[NGN_Load error] Unable to convert <" + filepath + "> to a surface.");
         return nullptr;
    }


    // Crea una textura temporal que se usara como buffer
    NGN_TextureData* texture = new NGN_TextureData();

    // Convierte la imagen cargada a textura
    texture->gfx = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface);

    // Verifica que la conversion ha sido correcta
    if (!texture->gfx) {
         ngn->log->Message("[NGN_Load error] Unable to convert <" + filepath + "> to a texture.");
         SDL_FreeSurface(surface);
         return nullptr;
    }

    // Guarda los datos de la textura
    texture->width = surface->w;
    texture->height = surface->h;

    // Destruye el surface temporal antes de salir
    SDL_FreeSurface(surface);

    // Devuelve la textura cargada
    return texture;

}



/*** Carga un fondo de tiles ***/
NGN_TiledBgData* NGN_Load::TiledBg(std::string filepath) {

    // Crea un objeto temporal para procesar el fondo cargado
    NGN_TiledBgData* bg = new NGN_TiledBgData();

    // Crea un buffer temporal para la carga del archivo
    std::vector<uint8_t> buffer;
    uint32_t file_pos = 0;

    // Crea un buffer temporal para la descompresion del Tileset
    std::vector<uint8_t> tiles;

    // Intenta cargar el archivo
    if (LoadFile(filepath, buffer) > 0) {

        // Cabecera del archivo
        memcpy((uint8_t*)&bg->header, (uint8_t*)&buffer[0], sizeof(bg->header));
        file_pos += sizeof(bg->header);

        // Tileset
        tiles.resize(bg->header.tileset_length);
        memcpy((uint8_t*)&tiles[0], (uint8_t*)&buffer[file_pos], bg->header.tileset_length);
        file_pos += bg->header.tileset_length;

        // Mapa
        bg->tmap.resize(bg->header.map_length);
        memcpy((uint8_t*)&bg->tmap[0], (uint8_t*)&buffer[file_pos], bg->header.map_length);

    } else {

        ngn->log->Message("[NGN_Load error] Error opening <" + filepath + "> for read.");
        buffer.clear();
        delete bg;
        return nullptr;

    }

    // Elimina los datos del buffer temporal de archivo
    buffer.clear();


    // Verifica que el archivo es compatible
    if (std::string(bg->header.magic) != MAGIC_STRING_TBG) {
        ngn->log->Message("[NGN_Load error] File <" + filepath + "> is in unknow format.");
        delete bg;
        return nullptr;
    }

    // Decodifica los datos del tileset en PNG a Pixeles
    std::vector<uint8_t> pixels;
    uint32_t w, h;
    if (lodepng::decode(pixels, w, h, tiles) > 0) {
        ngn->log->Message("[NGN_Load error] Error decoding <" + filepath + "> tileset.");
        tiles.clear();
        pixels.clear();
        delete bg;
        return nullptr;
    }
    // Borra los buffers temporales
    tiles.clear();

    // Genera una surface con los datos del atlas
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
            (uint8_t*)&pixels[0],       // Datos
            w,                          // Ancho del atlas
            h,                          // Alto del atlas
            32,                         // Profundidad de color 32bpp [RGBA8888]
            (w << 2),                   // Longitud de una fila de pixeles en bytes
            0x000000ff,                 // Mascara R
            0x0000ff00,                 // Mascara G
            0x00ff0000,                 // Mascara B
            0xff000000                  // Mascara A
    );
    // Verifica si se ha generado el surface
    if (surface == nullptr) {
         ngn->log->Message("[NGN_Load error] Unable to convert <" + filepath + "> tileset to a surface.");
         pixels.clear();
         delete bg;
         return nullptr;
    }

    // Crea la textura única del atlas a partir del surface
    bg->tiles_atlas = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface);
    // Libera la surface, ya no es necesaria
    SDL_FreeSurface(surface);

    // Verifica si se ha creado con éxito
    if (bg->tiles_atlas == nullptr) {
         ngn->log->Message("[NGN_Load error] Unable to create <" + filepath + "> tileset texture.");
         pixels.clear();
         delete bg;
         return nullptr;
    }

    // Elimina los pixeles no necesarios
    pixels.clear();

    // Devuelve el fondo cargado
    return bg;

}



/*** Carga un sprite ***/
NGN_SpriteData* NGN_Load::Sprite(std::string filepath) {

    // Crea un objeto temporal para procesar el sprite
    NGN_SpriteData* spr = new NGN_SpriteData();

    // Crea un buffer temporal para la descompresion de los pixeles
    std::vector<uint8_t> pixels;

    // Carga los datos del sprite
    if (!LoadSpriteData(filepath, pixels, spr)) {
        delete spr;
        pixels.clear();
        return nullptr;
    }

    // Redimensiona el vector
    spr->gfx.resize(spr->header.total_frames);
    for (uint32_t i = 0; i < spr->gfx.size(); i ++) spr->gfx[i] = nullptr;
    uint32_t frame_data_size = (spr->header.frame_width * spr->header.frame_height);

    // Crea una superficie temporal para la generacion del spritesheet
    SDL_Surface* surface = nullptr;

    // Crea una textura por cada fotograma
    for (uint32_t i = 0; i < spr->header.total_frames; i ++) {

        // Crea la superficie en base a los pixeles cargados
        surface = SDL_CreateRGBSurfaceFrom(
                                            (uint8_t*)&pixels[((frame_data_size * i) << 2)],    // Datos [frame * datos del frame * 4bpp]
                                            spr->header.frame_width,                            // Ancho
                                            spr->header.frame_height,                           // Alto
                                            32,                                                 // Profuncidad de color 32bpp [RGBA8888]
                                            (spr->header.frame_width << 2),                     // Longitud de una fila de pixeles en bytes
                                            0x000000ff,             // Mascara R
                                            0x0000ff00,             // Mascara G
                                            0x00ff0000,             // Mascara B
                                            0xff000000              // Mascara A
                                            );


        if (surface == nullptr) {
            ngn->log->Message("[NGN_Load error] Unable to convert <" + filepath + "> to a surface.");
            pixels.clear();
            return nullptr;
        }

        // Si se ha creado la superficie con exito, conviertela a textura
        spr->gfx[i] = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface);

        // Verifica que la conversion ha sido correcta
        if (spr->gfx[i] == nullptr) {
            ngn->log->Message("[NGN_Load error] Unable to load <" + filepath + "> as a sprite.");
            SDL_FreeSurface(surface);
            pixels.clear();
            return nullptr;
        }

        // Borra el surface creado
        SDL_FreeSurface(surface);

    }

    // Borra el buffer de pixeles
    pixels.clear();

    // Devuelve el sprite cargado
    return spr;

}



/*** Carga un mapa de colisiones ***/
NGN_CollisionMapData* NGN_Load::CollisionMap(std::string filepath) {

    // Crea un objeto temporal para procesar el archivo
    NGN_CollisionMapData* collision = new NGN_CollisionMapData();

    // Crea un buffer temporal para la carga del archivo
    std::vector<uint8_t> buffer;
    uint32_t file_pos = 0;

    // Intenta cargar el archivo
    if (LoadFile(filepath, buffer) > 0) {

        // Cabecera del archivo
        memcpy((uint8_t*)&collision->header, (uint8_t*)&buffer[0], sizeof(collision->header));
        file_pos += sizeof(collision->header);

        // Paleta
        collision->palette.resize(collision->header.pal_length, 0);
        memcpy((uint8_t*)&collision->palette[0], (uint8_t*)&buffer[file_pos], (collision->header.pal_length * sizeof(int32_t)));
        file_pos += (collision->header.pal_length * sizeof(int32_t));

        // Tileset
        collision->tiles.resize(collision->header.tileset_length, 0);
        memcpy((uint8_t*)&collision->tiles[0], (uint8_t*)&buffer[file_pos], collision->header.tileset_length);
        file_pos += collision->header.tileset_length;

        // Mapa
        collision->tmap.resize(collision->header.map_length, 0);
        memcpy((uint8_t*)&collision->tmap[0], (uint8_t*)&buffer[file_pos], (collision->header.map_length * sizeof(int32_t)));

    } else {

        // Error leyendo el archivo solicitado
        ngn->log->Message("[NGN_Load error] Error opening <" + filepath + "> for read.");
        buffer.clear();
        delete collision;
        return nullptr;

    }

    // Elimina los datos del buffer temporal de archivo
    buffer.clear();


    // Verifica que el archivo es compatible
    if (std::string(collision->header.magic) != MAGIC_STRING_CMAP) {
        ngn->log->Message("[NGN_Load error] File <" + filepath + "> is in unknow format.");
        delete collision;
        return nullptr;
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

    // Crea un buffer temporal para la carga del archivo
    std::vector<uint8_t> buffer;

    // Intenta cargar el archivo
    int32_t file_length = LoadFile(filepath, buffer);
    if (file_length <= 0) {
        // Error leyendo el archivo solicitado
        ngn->log->Message("[NGN_Load error] Error opening <" + filepath + "> for read.");
        buffer.clear();
        return nullptr;
    }

    // Crea un archivo temporal con el audio
    NGN_AudioClipData* clip = new NGN_AudioClipData();

    // Transfiere el archivo desde el buffer de RAM al subsistema de sonido
    if (clip->data.loadFromMemory((uint8_t*)&buffer[0], file_length)) {
        // Elimina los datos del buffer del archivo temporal
        buffer.clear();
        // Devuelve el clip
        return clip;
    } else {
        // Error en la carga desde RAM
        ngn->log->Message("[NGN_Load error] Error opening <" + filepath + "> from memory.");
        // Elimina los datos del buffer del archivo temporal
        buffer.clear();
        // Error en la carga
        return nullptr;
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

    // Crea un buffer temporal para la carga del archivo
    std::vector<uint8_t> buffer;

    // Intenta cargar el archivo
    int32_t file_length = LoadFile(filepath, buffer);
    if (file_length <= 0) {
        // Error leyendo el archivo solicitado
        ngn->log->Message("[NGN_Load error] Error opening <" + filepath + "> for read.");
        buffer.clear();
        return nullptr;
    }

    // Intenta iniciar el subsitema de TTF para SDL
    if (TTF_Init() < 0) {
        ngn->log->Message("[NGN_Load error] SDL TTF initialization failed.");
        buffer.clear();
        return nullptr;
    }

    // Crea un objeto temporal para almacenar la fuente resultante
    NGN_TextFont* font = new NGN_TextFont();

    // Intenta cargar la tipografia
    TTF_Font* ttf = TTF_OpenFontRW(SDL_RWFromMem((uint8_t*)&buffer[0], file_length), 0, height);
    if (ttf == nullptr) {
        ngn->log->Message("[NGN_Load error] Error opening <" + filepath + "> from memory.");
        buffer.clear();
        delete font;
        return nullptr;
    }

    // Elimina los datos del buffer del archivo temporal
    buffer.clear();

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
    SDL_Surface* surface = nullptr;
    SDL_Surface* surface_outline = nullptr;

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

        // Proteccion contra caracteres ilegales
        if (!surface || ((outline > 0) && !surface_outline)) {
            #if defined (MODE_DEBUG) && defined (NGN_LOAD_DEBUG)
                ngn->log->Message(
                    "Error creating the ["
                    +
                    ngn->toolbox->Int2String(i, 3, "0")
                    +
                    "] character from the ["
                    +
                    filepath
                    +
                    "] file."
                );
            #endif
            // Saltate la creacion de este caracter
            continue;
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
                (surface_outline)
                &&
                (surface)
                &&
                (TTF_SizeText(ttf, (const char*)t, &font->character[i]->width, &font->character[i]->height) == 0)
            ) {
                font->character[i]->gfx = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface_outline);
            }
            TTF_SetFontOutline(ttf, 0);
        } else {
            // Pasalo a la textura
            if (
                (surface)
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
    TTF_CloseFont(ttf); ttf = nullptr;

    // Cierra el subsistema de TTF
    TTF_Quit();

    // Devuelve la fuente cargada y convertida
    return font;

}



/*** Carga una imagen en formato PNG como RAW ***/
NGN_RawImage* NGN_Load::PngAsRaw(std::string filepath) {

    // Crea los buffers temporales para la descompresion del PNG
    std::vector<uint8_t> buffer;

    // Prepara el destino de los pixeles en RAW
    NGN_RawImage* raw = new NGN_RawImage();

    // Intenta abrir el archivo
    if (LoadFile(filepath, buffer) > 0) {
        // Si se ha abierto correctamente, intenta decodificarlo
        if (lodepng::decode(raw->pixels, raw->width, raw->height, buffer) > 0) {
            // Si no puedes decodificarlo, error
            ngn->log->Message("[NGN_Load error] Error decoding <" + filepath + "> image.");
            buffer.clear();
            delete raw;
            return nullptr;
        }
    } else {
        ngn->log->Message("[NGN_Load error] Error loading <" + filepath + ">.");
        buffer.clear();
        delete raw;
        return nullptr;
    }

    // Elimina el buffer temporal
    buffer.clear();

    // Devuelve el PNG en RAW
    return raw;

}



/*** Carga un fotograma de un sprite como RAW ***/
NGN_RawImage* NGN_Load::SpriteAsRaw(std::string filepath, uint32_t frame) {

    // Crea un objeto temporal para procesar el sprite
    NGN_SpriteData* spr = new NGN_SpriteData();

    // Crea un buffer temporal para la descompresion de los pixeles
    std::vector<uint8_t> pixels;

    // Carga los datos del sprite
    if (!LoadSpriteData(filepath, pixels, spr)) {
        delete spr;
        pixels.clear();
        return nullptr;
    }

    // Proteccion de frame
    if (frame >= spr->header.total_frames) frame = (spr->header.total_frames - 1);

    // Crea el objeto temporal de la imagen
    NGN_RawImage* raw = new NGN_RawImage();

    // Valores iniciales
    raw->width = spr->header.frame_width;
    raw->height = spr->header.frame_height;

    // Calcula el tamaño de cada frame en memoria
    uint32_t frame_size = ((raw->width * raw->height) << 2);        // WIDTH * HEIGHT * 4
    // Calcula el offset con el origen de los datos
    uint32_t offset = (frame_size * frame);

    // Dimensiona el vector de destino
    raw->pixels.resize(frame_size);
    // Copia los datos del buffer de pixeles al vector de destino
    memcpy((uint8_t*)&raw->pixels[0], (uint8_t*)&pixels[offset], frame_size);

    // Vacia el buffer de pixeles temporal
    pixels.clear();
    // Elimina el sprite data temporal
    delete spr;

    // Devuelve los datos en RAW
    return raw;

}



/*** Carga los fotogramas de un sprite como un vector de RAWs ***/
bool NGN_Load::SpriteAsRawVector(
    std::string filepath,                       // Archivo a cargar
    std::vector<NGN_RawImage*> &raw_frames,     // Vector de destino con los frames
    uint32_t first_frame,                       // Frame inicial (0 por defecto)
    uint32_t last_frame                         // Frame final (ultimo por defecto)
) {

    // Crea un objeto temporal para procesar el sprite
    NGN_SpriteData* spr = new NGN_SpriteData();

    // Crea un buffer temporal para la descompresion de los pixeles
    std::vector<uint8_t> pixels;

    // Carga los datos del sprite
    if (!LoadSpriteData(filepath, pixels, spr)) {
        delete spr;
        pixels.clear();
        return false;
    }

    // Rango de frames
    uint32_t _last_frame = (last_frame >= spr->header.total_frames) ? (spr->header.total_frames - 1):last_frame;
    uint32_t _first_frame = (first_frame > _last_frame) ? _last_frame:first_frame;

    // Valores iniciales
    uint32_t width = spr->header.frame_width;
    uint32_t height = spr->header.frame_height;
    uint32_t total_frames = ((_last_frame - _first_frame) + 1);

    // Calcula el tamaño de cada frame en memoria
    uint32_t frame_size = ((width * height) << 2);        // WIDTH * HEIGHT * 4
    // Calcula el offset con el origen de los datos
    uint32_t offset = 0;

    // Elimina el contenido previo del vector si es necesario
    for (uint32_t i = 0; i < raw_frames.size(); i ++) {
        delete raw_frames[i];
    }
    raw_frames.clear();

    // Prepara el vector para almacenar todos los fotogramas
    raw_frames.resize(total_frames);

    // Copia los datos de cada frame al vector
    uint32_t cnt = 0;
    for (uint32_t i = _first_frame; i <= _last_frame; i ++) {
        // Genera el espacio para el frame
        raw_frames[cnt] = new NGN_RawImage();
        raw_frames[cnt]->pixels.resize(frame_size);
        raw_frames[cnt]->width = width;
        raw_frames[cnt]->height = height;
        // Copia los datos del buffer de pixeles al vector de destino
        memcpy((uint8_t*)&raw_frames[cnt]->pixels[0], (uint8_t*)&pixels[offset], frame_size);
        // Siguiente offset
        offset += frame_size;
        // Siguiente frame
        cnt ++;
    }

    // Vacia el buffer de pixeles temporal
    pixels.clear();
    // Elimina el sprite data temporal
    delete spr;

    // Carga correcta
    return true;

}



/*** Metodo para cargar un archivo de texto: Primera sobrecarga, archivo a string ***/
std::string NGN_Load::TextFile(std::string filepath) {

    // Prepara los datos
    std::vector<uint8_t> buffer;
    buffer.clear();
    std::string text = "";          // Destino del texto

    // Intenta cargar el archivo
    if (ngn->load->LoadFile(filepath, buffer) < 0) return text;

    // Si no hay contenido, sal
    if (buffer.size() == 0) {
        buffer.clear();
        return text;
    }

    // Procesa el archivo
    for (uint32_t i = 0; i < buffer.size(); i ++) {
        if (
            (
                (buffer[i] >= 0x20)     // Dentro del rango de caracteres validos
                &&
                (buffer[i] != 0x7F)     // No es DEL
                &&
                (buffer[i] != 0xFF)     // No es BLANK
            )
            ||
            (buffer[i] == 0x0A)         // Salto de linea
            ) {
            // Si el caracter esta dentro del rango, almacenalo
            text += buffer[i];
        }
    }

    // Elimina el buffer de datos
    buffer.clear();

    // Carga correcta
    return text;

}

/*** Metodo para cargar un archivo de texto: Segunda sobrecarga, archivo a vector de lineas ***/
bool NGN_Load::TextFile(std::string filepath, std::vector<std::string> &text_lines) {

    // Prepara el vector de destino
    text_lines.clear();
    std::string character = "";     // Caracter leido
    uint8_t ascii = 0;              // Codigo ASCII del caracter leido
    std::string line = "";          // Linea actual

    // Intenta cargar el archivo
    std::string text = TextFile(filepath);

    // Si no hay texto, sal
    if (text.size() == 0) return false;

    // Procesa el archivo
    for (uint32_t i = 0; i < text.size(); i ++) {
        character = text.at(i);
        ascii = (uint8_t)character[0];
        // Si el caracter es final de linea
        if (ascii == 0x0A) {
            // Almacena la linea actual
            text_lines.push_back(line);
            // Nueva linea
            line = "";
        } else if ((ascii >= 0x20) && (ascii != 0x7F) && (ascii != 0xFF)) {
            // Si el caracter esta dentro del rango, almacenalo
            line += character;
        }
    }
    // Verifica si hay datos pendientes (ultima linea sin \n)
    if (line.size() > 0) text_lines.push_back(line);

    // Carga correcta
    return true;

}



/*** Metodo para cargar un archivo desde el origen predeterminado. El archivo se desencriptara en el caso de estarlo ***/
int32_t NGN_Load::LoadFile(std::string filepath, std::vector<uint8_t> &data) {

    if (use_package) {
        // Lee el archivo desde un paquete
        return file_system->LoadFileFromPakage(filepath, data);
    } else {
        // Lee el archivo desde el disco
        return ngn->disk->ReadBinaryFile(filepath, data);
    }

}



/*** Establece el disco como el origen de datos ***/
void NGN_Load::SetDisk() {

    use_package = false;

}



/*** Establece un archivo empaquetado como el origen de datos ***/
bool NGN_Load::SetPackage(std::string pkg_file, std::string key) {

    if (file_system->SetPackage(pkg_file, key)) {
        use_package = true;
        return true;
    } else {
        use_package = false;
        return false;
    }

}



/*** Devuelve si se esta usando actualmente un archivo de empaquetado de datos ***/
bool NGN_Load::PackageEnabled() {

    return use_package;

}




/*** Carga los datos de un sprite ***/
bool NGN_Load::LoadSpriteData(std::string filepath, std::vector<uint8_t> &img_pixels, NGN_SpriteData* spr) {

    // Crea un buffer temporal para la carga del archivo
    std::vector<uint8_t> buffer;
    uint32_t file_pos = 0;

    // Crea un buffer temporal para la descompresion de los pixeles
    std::vector<uint8_t> raw;

    // Intenta cargar el archivo
    if (LoadFile(filepath, buffer) > 0) {

        // Cabecera del archivo
        memcpy((uint8_t*)&spr->header, (uint8_t*)&buffer[0], sizeof(spr->header));
        file_pos += sizeof(spr->header);

        // Calcula el tamaño del buffer
        uint32_t buffer_size = (buffer.size() - sizeof(spr->header));
        raw.resize(buffer_size);

        // Lee los datos del spritesheet
        memcpy((uint8_t*)&raw[0], (uint8_t*)&buffer[file_pos], buffer_size);

    } else {

        // Error leyendo el archivo solicitado
        ngn->log->Message("[NGN_Load error] Error opening <" + filepath + "> for read.");
        buffer.clear();
        return false;

    }

    // Elimina los datos del buffer temporal de archivo
    buffer.clear();


    // Verifica que el archivo es compatible
    if (std::string(spr->header.magic) != MAGIC_STRING_SPR) {
        ngn->log->Message("[NGN_Load error] File <" + filepath + "> is in unknow format.");
        return false;
    }


    // Decodifica los datos del sprite en PNG a pixeles.
    uint32_t w, h;
    if (lodepng::decode(img_pixels, w, h, raw) > 0) {
        ngn->log->Message("[NGN_Load error] Error decoding <" + filepath + "> sprite.");
        raw.clear();
        return false;
    }
    // Borra el buffer temporal
    raw.clear();


    // Carga correcta
    return true;

}
