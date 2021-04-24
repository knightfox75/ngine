/******************************************************************************

    N'gine Lib for C++
    *** Version 1.5.0-wip3 ***
    Funciones para la manipulacion de imagenes en RAW

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
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



#ifndef NGN_IMAGE_H_INCLUDED
#define NGN_IMAGE_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <string>
#include <vector>
// Includes de la libreria
#include "ngn_defines.h"





/*** Declaracion de la clase ***/
class NGN_Image {

    public:

        // Constructor
        NGN_Image();

        // Destructor
        ~NGN_Image();

        // Convierte una imagen RAW a datos de textura
        NGN_TextureData* ConvertRawToTextureData(NGN_RawImage* raw);

        // Aplica una mascara simple de recorte a una imagen RAW
        bool CutOutMask(NGN_RawImage* source, NGN_RawImage* mask, NGN_RawImage* destination);

        // Aplica una mascara simple de vaciado a una imagen RAW
        bool HollowMask(NGN_RawImage* source, NGN_RawImage* mask, NGN_RawImage* destination);

        // Aplica una mascara avanzada a una imagen RAW
        bool AdvancedMask(
            NGN_RawImage* source,                   // Imagen de origen
            NGN_RawImage* mask,                     // Imagen de la mascara
            NGN_RawImage* destination,              // Imagen de destino
            Vector2I32 offset = {0, 0},             // Offset de la mascara
            uint8_t mode = NGN_MASKMODE_CUTOUT      // Modo de la mascara
        );

        // Guarda el contenido actual del renderer a un surface
        bool RendererToSurface(NGN_RendererSurface* destination);

        // Convierte un surface a una imagen RAW
        bool SurfaceToRaw(NGN_RendererSurface* source, NGN_RawImage* destination, NGN_RawImage* mask = NULL);


    private:


};



#endif // NGN_IMAGE_H_INCLUDED
