/******************************************************************************

    N'gine Lib for C++
    *** Version 0.4.5-alpha ***
    Gestion del Renderer de SDL

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
#include <string>
#include <cmath>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/*** Contructor ***/
NGN_Graphics::NGN_Graphics() {

    // Inicializa las variables de la clase
    window = NULL;
    renderer = NULL;
    force_redaw = true;

    // Inicializa el control de frame rate
    time_gap = 0;                           // Duracion de 1 frame
    time_elapsed = 0;                       // Duracion del frame actual
    time_last_frame = SDL_GetTicks();       // Duracion del ultimo frame

    // Inicializa el control del contador FPS
    fps_frames = 0;
    fps_timer = SDL_GetTicks() + 1000;

}



/*** Destructor ***/
NGN_Graphics::~NGN_Graphics() {

    // Elimina los contenedores graficos
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

}



/*** Inicializa el engine grafico ***/
bool NGN_Graphics::Init(
                          std::string window_name,          // Nombre en la ventana
                          uint32_t native_width,            // Resolucion Nativa del juego
                          uint32_t native_height,
                          bool full_scr,                    // Pantalla completa?
                          bool sync,                        // VSYNC activo?
                          int32_t window_width,             // Resolucion de la ventana
                          int32_t window_height
                         ) {

    // Guarda el titulo de la ventana
    window_caption = window_name;

    // Guarda la resolucion nativa e informa a la camara de esta resolucion
    if (native_width > 0 && native_height > 0) {
        ngn->camera->world.width = native_w = native_width;
        ngn->camera->world.height = native_h = native_height;
    } else {
        std::cout << "Native resolution must be greather than 0." << std::endl;
        return false;
    }

    // Guarda el tamaño de la ventana
    if ((window_width != DEFAULT_VALUE) && (window_height != DEFAULT_VALUE)) {
        screen_w = window_width;
        screen_h = window_height;
    } else {
        screen_w = native_w;
        screen_h = native_h;
    }

    // Guarda la resolucion del escritorio
    SDL_DisplayMode display;
    if (SDL_GetDesktopDisplayMode(0, &display) < 0) {
        std::cout << "Can't get current desktop resolution." << std::endl;
        return false;
    }
    desktop_w = display.w;
    desktop_h = display.h;
    desktop_refresh_rate = display.refresh_rate;

    // Calcula el factor de escala
    scale_x = ((float)screen_w / (float)native_w);
    scale_y = ((float)screen_h / (float)native_h);

    // Crea la ventana para el renderer, con las opciones por defecto
    window = SDL_CreateWindow((const char*)window_caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_w, screen_h, SDL_WINDOW_SHOWN);
    // Verifica si ha ocurrido un error en la creacion de la ventana
    if (window == NULL) {
        std::cout << "SDL unable to create the main Window." << std::endl;
        return false;
    }

    // Si la ventana se ha creado, intenta crear la superficie de renderizado, con el dibujado sincronizado al frame
    renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    // Si no se puede crear, destruye la venta e informa del error
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
        std::cout << "SDL unable to create the rendering surface." << std::endl;
        return false;
    }
    // Ajusta el viewport del renderer a la resolucion nativa
    SDL_Rect viewport = {
                0,          // Posicion X
                0,          // Posicion Y
                screen_w,   // Ancho
                screen_h    // Alto
                };
    SDL_RenderSetViewport(renderer, &viewport);
    // Ahora limita el area de dibujado a esa resolucion
    SDL_RenderSetClipRect(renderer, &viewport);

    // Selecciona el color por defecto del renderer
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    // Ahora intenta desactivar el filtrado bilinear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
        std::cout << "SDL unable to modify bilinear filtering settings." << std::endl;
    }

    // Pantalla completa o ventana?
    _full_screen = full_screen = full_scr;
    if (full_screen) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    } else {
        SDL_SetWindowFullscreen(window, 0);
    }

    // Intenta modificar el VSYNC
    _vsync = vsync = sync;
    if (vsync && (desktop_refresh_rate >= 60)) {
        if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
            std::cout << "SDL unable to activate VSYNC." << std::endl;
        }
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0")) {
            std::cout << "SDL unable to deactivate VSYNC." << std::endl;
        }
    }

    // Ajusta el escalado
    SDL_RenderSetScale(renderer, scale_x, scale_y);

    // Deshabilita el salvapantallas
    SDL_DisableScreenSaver();

    // Sal con normalidad
    return true;

}



/*** Actualiza la superficie del renderer ***/
void NGN_Graphics::Update(void) {

    // Actualiza los flags del renderer
    UpdateRendererFlags();

    // Actualiza la escena
    SDL_RenderPresent(renderer);

    // Espera al siguiente frame
    SyncFrame(60);

    // Contador de FPS [ *** Debug ***]
    if (ngn->system->fps_counter) FpsCounter();

    // Borra el contenido para el siguiente frame
    SDL_RenderClear(renderer);

}



/*** Cambia el tamaño del clip del viewport ***/
void NGN_Graphics::SetViewportClip(int32_t x, int32_t y, int32_t w, int32_t h) {

    SDL_Rect viewport = {
                x,          // Posicion X
                y,          // Posicion Y
                w,          // Ancho
                h           // Alto
                };

    if (viewport.x < 0) viewport.x = 0;
    if (viewport.y < 0) viewport.y = 0;

    if ((viewport.x + viewport.w) > native_w) viewport.w = (native_w - viewport.x);
    if (viewport.w < 0) return;

    if ((viewport.y + viewport.h) > native_h) viewport.h = (native_h - viewport.y);
    if (viewport.h < 0) return;

    // Ahora limita el area de dibujado a esa resolucion
    SDL_RenderSetClipRect(renderer, &viewport);

}



/*** Sincronizacion de velocidad del framerate ***/
void NGN_Graphics::SyncFrame(int32_t fps) {

    // Calcula el time gap entre frames
    time_gap = floor(1000 / fps);

    // Control del framerate
    time_elapsed = (SDL_GetTicks() - time_last_frame);      // Lapso de tiempo desde el frame anterior

    // Si es necesario, esperate los ticks necesarios
    if (time_elapsed < time_gap) SDL_Delay((time_gap - time_elapsed));

    // Guarda la marca de tiempo de este frame
    time_last_frame = SDL_GetTicks();

}



/*** Gestion de los parametros del render ***/
void NGN_Graphics::UpdateRendererFlags() {

    // Full Screen?
    if (_full_screen != full_screen) {
        if (full_screen) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        } else {
            SDL_SetWindowFullscreen(window, 0);
        }
        _full_screen = full_screen;
        force_redaw = true;
    } else {
        force_redaw = false;
    }

    // VSYNC
    if (_vsync != vsync) {
        if (vsync && (desktop_refresh_rate >= 60)) {
            SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
        } else {
            SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
        }
        _vsync = vsync;
    }


}



/*** Contador de Frames por segundo ***/
void NGN_Graphics::FpsCounter() {

    // Contador
    if (SDL_GetTicks() > fps_timer) {
        std::cout << "Render Speed: " << fps_frames << " fps.     \x0d";
        fps_timer = SDL_GetTicks() + 1000;
        fps_frames = 0;
    } else {
        fps_frames ++;
    }

}
