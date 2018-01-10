/******************************************************************************

    N'gine Lib for C++
    *** Version 0.4.2-alpha ***
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
                          const char* window_name,          // Nombre en la ventana
                          uint32_t native_width,            // Resolucion Nativa del juego
                          uint32_t native_height,
                          int32_t screen_width,             // Resolucion de la ventana
                          int32_t screen_height,
                          bool full_scr,                    // Pantalla completa?
                          bool sync                         // VSYNC activo?
                         ) {

    // Guarda la resolucion nativa e informa a la camara de esta resolucion
    if (native_width > 0 && native_height > 0) {
        ngn->camera->world.width = native_w = native_width;
        ngn->camera->world.height = native_h = native_height;
    } else {
        std::cout << "Native resolution must be greather than 0." << std::endl;
        return false;
    }

    // Guarda el tamaño de la ventana
    if ((screen_width != DEFAULT_VALUE) && (screen_height != DEFAULT_VALUE)) {
        screen_w = screen_width;
        screen_h = screen_height;
    } else {
        screen_w = native_w;
        screen_h = native_h;
    }

    // Calcula el factor de escala
    scale_x = ((float)screen_w / (float)native_w);
    scale_y = ((float)screen_h / (float)native_h);

    // Crea la ventana para el renderer, con las opciones por defecto
    window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_w, screen_h, SDL_WINDOW_SHOWN);

    // Verifica si ha ocurrido un error en la creacion de la ventana
    if (window == NULL) {
        std::cout << "SDL unable to create the main Window." << std::endl;
        return false;
    }

    // Si la ventana se ha creado, intenta crear la superficie de renderizado, con el dibujado sincronizado al frame
    renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    //renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED));
    // Si no se puede crear, destruye la venta e informa del error
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
        std::cout << "SDL unable to create the rendering surface." << std::endl;
        return false;
    }

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
    if (vsync) {
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
        if (vsync) {
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
