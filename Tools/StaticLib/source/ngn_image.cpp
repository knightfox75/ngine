/******************************************************************************

    N'gine Lib for C++
    *** Version 1.11.0-stable ***
    Funciones para la manipulacion de imagenes en RAW

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



/*** Includes ***/
// C++
#include <cstdio>
#include <iostream>


// Libreria
#include "ngn.h"



/*** Puntero de la instancia a NULL ***/
NGN_Image* NGN_Image::instance = NULL;



/*** Metodo para crear/obtener la instancia ***/
NGN_Image* NGN_Image::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Image();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Image::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = NULL;
    }

}



/*** Constructor ***/
NGN_Image::NGN_Image() {

}



/*** Destructor ***/
NGN_Image::~NGN_Image() {

}


/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Image::BootUp() {
}



/*** Convierte una imagen RAW a datos de textura ***/
NGN_TextureData* NGN_Image::ConvertRawToTextureData(NGN_RawImage* raw) {

    // Textura nula
    if (!raw) return NULL;

    // Crea un surface temporal para convertir la imagen
    SDL_Surface* surface = NULL;

    // Crea la superficie en base a los pixeles cargados
    surface = SDL_CreateRGBSurfaceFrom(
        (uint8_t*)&raw->pixels[0],      // Datos
        raw->width,                     // Ancho
        raw->height,                    // Alto
        32,                             // Profuncidad de color 32bpp [RGBA8888]
        (raw->width << 2),              // Longitud de una fila de pixeles en bytes
        0x000000ff,                     // Mascara R
        0x0000ff00,                     // Mascara G
        0x00ff0000,                     // Mascara B
        0xff000000                      // Mascara A
    );

    // Verifica si el surface se ha creado correctamente
    if (!surface) {
         ngn->log->Message("[NGN_Image error] Unable to convert RAW image to a surface.");
         return NULL;
    }

    // Crea unos datos de textura temporales para convertir la imagen
    NGN_TextureData* texture_data = new NGN_TextureData();

    // Convierte la imagen RAW a textura
    texture_data->gfx = SDL_CreateTextureFromSurface(ngn->graphics->renderer, surface);

    // Verifica que la conversion ha sido correcta
    if (!texture_data->gfx) {
         ngn->log->Message("[NGN_Image error] Unable to convert RAW image to a texture.");
         SDL_FreeSurface(surface);
         return NULL;
    }

    // Guarda los datos de la textura
    texture_data->width = surface->w;
    texture_data->height = surface->h;

    // Destruye el surface temporal antes de salir
    SDL_FreeSurface(surface);

    // Devuelve la textura cargada
    return texture_data;

}



/*** Aplica una mascara simple de recorte a una imagen RAW ***/
bool NGN_Image::CutOutMask(NGN_RawImage* source, NGN_RawImage* mask, NGN_RawImage* destination) {

    // Proteccion
    if (!source || !mask || !destination) return false;

    // El tamaño debe de coincidir entre la imagen y la mascara
    if ((source->width != mask->width) || (source->height != mask->height)) return false;

    // Prepara el buffer de destino
    if (destination->pixels.size() != source->pixels.size()) {
        destination->pixels.clear();
        destination->pixels.resize(source->pixels.size());
        destination->width = source->width;
        destination->height = source->height;
    }

    // Prepara el buffer de destino
    uint32_t length = destination->pixels.size();
    uint32_t offset = 0;

    // Aplica la mascara
    while (offset < length) {
        destination->pixels[offset] = source->pixels[offset];       // R
        offset ++;
        destination->pixels[offset] = source->pixels[offset];       // G
        offset ++;
        destination->pixels[offset] = source->pixels[offset];       // B
        offset ++;
        destination->pixels[offset] = source->pixels[offset] & mask->pixels[offset];    // A
        offset ++;
    }

    // Proceso correcto
    return true;

}



/*** Aplica una mascara simple de vaciado a una imagen RAW ***/
bool NGN_Image::HollowMask(NGN_RawImage* source, NGN_RawImage* mask, NGN_RawImage* destination) {

    // Proteccion
    if (!source || !mask || !destination) return false;

    // El tamaño debe de coincidir entre la imagen y la mascara
    if ((source->width != mask->width) || (source->height != mask->height)) return false;

    // Prepara el buffer de destino
    if (destination->pixels.size() != source->pixels.size()) {
        destination->pixels.clear();
        destination->pixels.resize(source->pixels.size());
        destination->width = source->width;
        destination->height = source->height;
    }

    // Prepara el buffer de destino
    uint32_t length = destination->pixels.size();
    uint32_t offset = 0;

    // Aplica la mascara
    while (offset < length) {
        destination->pixels[offset] = source->pixels[offset];       // R
        offset ++;
        destination->pixels[offset] = source->pixels[offset];       // G
        offset ++;
        destination->pixels[offset] = source->pixels[offset];       // B
        offset ++;
        destination->pixels[offset] = source->pixels[offset] & ~mask->pixels[offset];       // A
        offset ++;
    }

    // Proceso correcto
    return true;

}



/*** Aplica una mascara avanzada a una imagen RAW ***/
bool NGN_Image::AdvancedMask(
    NGN_RawImage* source,               // Imagen de origen
    NGN_RawImage* mask,                 // Imagen de la mascara
    NGN_RawImage* destination,          // Imagen de destino
    Vector2I32 offset,                  // Offset de la mascara
    uint8_t mode                        // Modo de la mascara
) {

    // Proteccion
    if (!source || !mask || !destination) return false;

    // Prepara el buffer de destino
    if (destination->pixels.size() != source->pixels.size()) {
        destination->pixels.clear();
        destination->pixels.resize(source->pixels.size());
        destination->width = source->width;
        destination->height = source->height;
    }

    // Parametros de la copia
    int32_t w = destination->width;
    int32_t h = destination->height;
    int32_t pos = 0;
    int32_t mx, my, mpos;
    int32_t mw = mask->width;
    int32_t mh = mask->height;

    // Segun el tipo de mascara a aplicar
    switch (mode) {

        case NGN_MASKMODE_CUTOUT:
            // Copia los pixeles del origen al destino, aplicando la mascara
            for (int32_t y = 0; y < h; y ++) {
                // Calcula el offset de la mascara
                my = y - offset.y;
                for (int32_t x = 0; x < w; x ++) {
                    // Calcula el offset de la mascara
                    mx = x - offset.x;
                    // Copia los pixeles con la informacion de color
                    destination->pixels[pos] = source->pixels[pos];     // R
                    pos ++;
                    destination->pixels[pos] = source->pixels[pos];     // G
                    pos ++;
                    destination->pixels[pos] = source->pixels[pos];     // B
                    pos ++;
                    // Aplica la mascara si esta esta dentro de los limites de la imagen de origen
                    if ((mx < 0) || (my < 0) || (mx >= w) || (my >= h) || (mx >= mw) || (my >= mh)) {
                        // Esta fuera de los limites, canal alpha a 0
                        destination->pixels[pos] = 0x00;    // A
                    } else {
                        // Esta dentro de los limites, aplica el alpha
                        mpos = (((my * mw) + mx) << 2) + 3;
                        destination->pixels[pos] = source->pixels[pos] & mask->pixels[mpos];    // A
                    }
                    pos ++;
                }
            }
            break;


        case NGN_MASKMODE_HOLLOW:
            // Copia los pixeles del origen al destino, aplicando la mascara
            for (int32_t y = 0; y < h; y ++) {
                // Calcula el offset de la mascara
                my = y - offset.y;
                for (int32_t x = 0; x < w; x ++) {
                    // Calcula el offset de la mascara
                    mx = x - offset.x;
                    // Copia los pixeles con la informacion de color
                    destination->pixels[pos] = source->pixels[pos];     // R
                    pos ++;
                    destination->pixels[pos] = source->pixels[pos];     // G
                    pos ++;
                    destination->pixels[pos] = source->pixels[pos];     // B
                    pos ++;
                    // Aplica la mascara si esta esta dentro de los limites de la imagen de origen
                    if ((mx < 0) || (my < 0) || (mx >= w) || (my >= h) || (mx >= mw) || (my >= mh)) {
                        // Esta fuera de los limites, aplica el alpha original
                        destination->pixels[pos] = source->pixels[pos];     // A
                    } else {
                        // Esta dentro de los limites, aplica el alpha
                        mpos = (((my * mw) + mx) << 2) + 3;
                        destination->pixels[pos] = source->pixels[pos] & ~mask->pixels[mpos];   // A
                    }
                    pos ++;
                }
            }
            break;

    }

    // Proceso correcto
    return true;

}



/*** Guarda el contenido actual del renderer a un surface ***/
bool NGN_Image::RendererToSurface(NGN_RendererSurface* destination) {

    // Consulta el tamaño del renderer
    int32_t _width = 0;
    int32_t _height = 0;
    SDL_GetRendererOutputSize(ngn->graphics->renderer, &_width, &_height);

    // Si el surface no existe o no coincide en tamaño
    if (!destination) {

        return false;

    } else if ((destination->width != _width) || (destination->height != _height) || !destination->surface) {

        // Elimina el surface previo
        SDL_FreeSurface(destination->surface);
        // Crea el nuevo surface
        destination->surface = SDL_CreateRGBSurface(
            0,              // Flag
            _width,         // Ancho
            _height,        // Alto
            32,             // Profuncidad de color 32bpp [RGBA8888]
            0xFF000000,     // Mascara R
            0x00FF0000,     // Mascara G
            0x0000FF00,     // Mascara B
            0x000000FF      // Mascara A
        );
        // Registra el tamaño
        destination->width = _width;
        destination->height = _height;

    }


    // Copia los pixeles del renderer al surface
    if (SDL_RenderReadPixels(ngn->graphics->renderer, NULL, SDL_PIXELFORMAT_RGBA8888, destination->surface->pixels, destination->surface->pitch) != 0) {
        ngn->log->Message("[NGN_Image error] RendererToSurface: Error creating surface.");
        SDL_FreeSurface(destination->surface);
        return false;
    }

    // Captura del renderer completada
    return true;

}



/*** Convierte un surface a una imagen RAW ***/
bool NGN_Image::SurfaceToRaw(NGN_RendererSurface* source, NGN_RawImage* destination, NGN_RawImage* mask) {

    // Proteccion
    if (!source || !destination) return false;

    // Prepara el buffer de destino
    uint32_t buffer_size = ((source->width * source->height) << 2);
    if (destination->pixels.size() != buffer_size) {
        destination->pixels.clear();
        destination->pixels.resize(buffer_size);
    }
    destination->width = source->width;
    destination->height = source->height;
    uint32_t length = (source->width * source->height);

    // Aplicacion de la mascara?
    bool apply_mask = false;
    if (mask) {
        if ((mask->width == destination->width) && (mask->height == destination->height)) apply_mask = true;
    }

    // Acceso a los pixeles del surface
    uint32_t* p = (uint32_t*)source->surface->pixels;

    // Copia los datos al buffer del RAW
    uint32_t n = 0;
    if (apply_mask) {
        // Aplica la mascara durante la conversion
        for (uint32_t idx = 0; idx < length; idx ++) {
            // RED
             destination->pixels[n] = ((p[idx] >> 24) & 0x000000FF);
            n ++;
            // GREEN
             destination->pixels[n] = ((p[idx] >> 16) & 0x000000FF);
            n ++;
            // BLUE
             destination->pixels[n] = ((p[idx] >> 8) & 0x000000FF);
            n ++;
            // ALPHA
             destination->pixels[n] = (p[idx] & 0x000000FF) & mask->pixels[n];
            n ++;
        }
    } else {
        // No apliques la mascara
        for (uint32_t idx = 0; idx < length; idx ++) {
            // RED
             destination->pixels[n] = ((p[idx] >> 24) & 0x000000FF);
            n ++;
            // GREEN
             destination->pixels[n] = ((p[idx] >> 16) & 0x000000FF);
            n ++;
            // BLUE
             destination->pixels[n] = ((p[idx] >> 8) & 0x000000FF);
            n ++;
            // ALPHA
             destination->pixels[n] = (p[idx] & 0x000000FF);
            n ++;
        }
    }

    // Conversion correcta
    return true;

}
