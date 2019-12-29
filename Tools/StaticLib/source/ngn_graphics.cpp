/******************************************************************************

    N'gine Lib for C++
    *** Version 0.10.0-a ***
    Gestion del Renderer de SDL

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
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

    // Inicia los viewports
    SetupViewports();

    // Genera el primer runtime frame
    runtime_frame = 0;
    GenerateRuntimeFrameId();

}



/*** Destructor ***/
NGN_Graphics::~NGN_Graphics() {

    // Elimina los vectores de memoria
    for (uint8_t i = 0; i < viewport_list.capacity(); i ++) {
        if (viewport_list[i].surface != NULL) SDL_DestroyTexture(viewport_list[i].surface);
    }
    viewport_list.clear();

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
                          int8_t full_scr,                  // Pantalla completa?
                          bool bilinear_filter,             // Filtrado bilinear?
                          bool sync                         // VSYNC activo?
                         ) {

    // Guarda el titulo de la ventana
    window_caption = window_name;

    // Guarda la resolucion nativa e informa a la camara de esta resolucion
    if (native_width > 0 && native_height > 0) {
        ngn->camera->world.width = render_resolution.width = native_w = native_width;
        ngn->camera->world.height = render_resolution.height = native_h = native_height;
    } else {
        std::cout << "Native resolution must be greather than 0." << std::endl;
        return false;
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

    // Crea la ventana para el renderer, con las opciones por defecto
    window = SDL_CreateWindow((const char*)window_caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, native_w, native_h, SDL_WINDOW_SHOWN);
    // Verifica si ha ocurrido un error en la creacion de la ventana
    if (window == NULL) {
        std::cout << "SDL unable to create the main Window." << std::endl;
        return false;
    }

    // Si la ventana se ha creado, intenta crear la superficie de renderizado, con el dibujado sincronizado al frame
    renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE));
    // Si no se puede crear, destruye la venta e informa del error
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
        std::cout << "SDL unable to create the rendering surface." << std::endl;
        return false;
    }

    // Selecciona el color por defecto del renderer
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    // Filtro bi-linear habilitado?
    if (bilinear_filter) {
        // Ahora intenta activar el filtrado bilinear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            std::cout << "SDL unable to enable bilinear filtering setting." << std::endl;
        }
    } else {
        // Ahora intenta desctivar el filtrado bilinear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
            std::cout << "SDL unable to disable bilinear filtering setting." << std::endl;
        }
    }

    // Calcula la relacion de aspecto
    aspect_native = ((float)native_w / (float)native_h);
    aspect_desktop = ((float)desktop_w / (float)desktop_h);

    // Pantalla completa o ventana?
    full_screen = full_scr;
    _full_screen = -1;
    SetFullScreen();

    // Intenta modificar el VSYNC
    vsync = sync;
    _vsync = !sync;
    SetVsync();

    // Parametros por defecto del clip
    cliparea = {
        0,
        0,
        native_w,
        native_h
    };

    // Deshabilita el salvapantallas
    SDL_DisableScreenSaver();

    // Sal con normalidad
    return true;

}



/*** Estable el destino del render al seleccionado por defecto ***/
void NGN_Graphics::RenderToSelected() {

    // Cambia el destino del renderer a la pantalla
    if (ngn->render->GetRenderToTextureState()) {
        ngn->render->SetRenderToTextureState();
    } else if (current_viewport >= 0) {
        SDL_SetRenderTarget(renderer, viewport_list[current_viewport].surface);
    } else {
        SDL_SetRenderTarget(renderer, NULL);
    }
    // Restaura el color y alpha del renderer
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

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

    // Borra los viewports de existir estos
    ClearViewports();

    // Borra el contenido para el siguiente frame
    SDL_RenderClear(renderer);

    // Genera el Runtime Frame ID para el siguiente frame
    GenerateRuntimeFrameId();

}



/*** Abre un viewport en la ID dada ***/
void NGN_Graphics::OpenViewport(
    uint8_t id,         // ID del VIEWPORT
    int32_t pos_x,      // Posicion del viewport
    int32_t pos_y,
    uint32_t width,     // Ancho del viewport
    uint32_t height,    // Alto del viewport
    uint32_t h_res,     // Resolucion del render en el viewport
    uint32_t v_res
) {

    // Check de ID
    if (id >= VIEWPORT_NUMBER) return;

    // Configura los parametros del viewport
    Viewport_struct v;
    v.x = pos_x;
    v.y = pos_y;
    v.w = width;
    v.h = height;
    if ((h_res != NGN_DEFAULT_VALUE) && (v_res != NGN_DEFAULT_VALUE)) {
        v.render_w = h_res;
        v.render_h = v_res;
    } else {
        v.render_w = width;
        v.render_h = height;
    }

    // Check de datos
    if ((v.w > native_w) || (v.h > native_h) || (v.render_w > native_w) || (v.render_h > native_h)) return;

    if (viewport_list[id].available) {
        if (viewport_list[id].surface != NULL) SDL_DestroyTexture(viewport_list[id].surface);
    }

    // Crea la textura
    v.surface = SDL_CreateTexture(
                             renderer,                      // Renderer
                             SDL_PIXELFORMAT_BGRA8888,      // Formato del pixel
                             SDL_TEXTUREACCESS_TARGET,      // Textura como destino del renderer
                             v.render_w,                    // Ancho de la textura
                             v.render_h                     // Alto de la textura
                             );

    // Viewport disponible
    v.available = true;

    // Registra el viewport
    viewport_list[id] = v;

}



/*** Cierra el viewport seleccionado ***/
void NGN_Graphics::CloseViewport(uint8_t id) {

    // Check de ID
    if (id >= VIEWPORT_NUMBER) return;

    // Elimina la textura del surface
    if (viewport_list[id].available) {
        if (viewport_list[id].surface != NULL) SDL_DestroyTexture(viewport_list[id].surface);
    }

    // Datos del viewport por defecto
    Viewport_struct v;
    v.available = false;
    v.x = 0;
    v.y = 0;
    v.w = 0;
    v.h = 0;
    v.render_w = 0;
    v.render_h = 0;
    v.surface = NULL;

    // Actualiza los datos del viewport cerrado
    viewport_list[id] = v;

}



/*** Selecciona el viewport ***/
void NGN_Graphics::SelectViewport(uint8_t id) {

    if (id > VIEWPORT_NUMBER) return;
    if (!viewport_list[id].available) return;

    // Registra y actualiza el viewport
    current_viewport = id;
    SDL_SetRenderTarget(renderer, viewport_list[id].surface);

    // Registra el tamaño del area de render
    render_resolution.width = viewport_list[id].render_w;
    render_resolution.height = viewport_list[id].render_h;

}



/*** Posiciona un viewport (Sobrecarga 1) ***/
void NGN_Graphics::ViewportPosition(uint8_t id, int32_t x, int32_t y) {

    if (id > VIEWPORT_NUMBER) return;
    if (!viewport_list[id].available) return;

    viewport_list[id].x = x;
    viewport_list[id].y = y;

}
/*** Posiciona un viewport (Sobrecarga 1) ***/
void NGN_Graphics::ViewportPosition(uint8_t id, Vector2I32 position) {

    ViewportPosition(id, position.x, position.y);

}



/*** Selecciona el VIEWPORT por defecto ***/
void NGN_Graphics::DefaultViewport() {

    // Ningun viewport seleccionado
    current_viewport = -1;

    // Registra el tamaño del area de render
    render_resolution.width = native_w;
    render_resolution.height = native_h;

    // Devuelve el render al seleccionado
    RenderToSelected();

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

    // Guarda el area
    cliparea = viewport;

    // Ahora limita el area de dibujado a esa resolucion
    SDL_RenderSetClipRect(renderer, &viewport);

}



/*** Cambia el estado de visibilidad del cursor del raton ***/
void NGN_Graphics::ShowMouse(bool visible) {

    if (visible) {
        SDL_ShowCursor(SDL_ENABLE);
    } else {
        SDL_ShowCursor(SDL_DISABLE);
    }

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



/*** Cambia de modo ventana a pantalla completa y vice-versa ***/
void NGN_Graphics::SetFullScreen() {

    // Full Screen?
    if (_full_screen != full_screen) {

        // Area del viewport
        SDL_Rect viewport;
        viewport.w = native_w;
        viewport.h = native_h;
        // Datos de posicion y escalado
        float w, h;
        float scale_x, scale_y;

        // MODO "FULL WINDOW"
        if (full_screen == NGN_SCR_WINDOW_FULL) {
            // Modo del renderer
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            // Segun la relacion de aspecto, calcula el viewport y el clip area
            if (aspect_native == aspect_desktop) {  // Mismo formato
                // Escala
                scale_x = ((float)desktop_w / (float)native_w);
                scale_y = ((float)desktop_h / (float)native_h);
                // Area de dibujado
                viewport.x = 0;
                viewport.y = 0;
            } else if (aspect_native > aspect_desktop) {    // Wide en pantalla letter box
                // Escala
                h = (((float)native_h * (float)desktop_w) / (float)native_w);
                scale_x = ((float)desktop_w / (float)native_w);
                scale_y = (h / (float)native_h);
                // Area de dibujado
                viewport.x = 0;
                viewport.y = (int32_t)((((float)desktop_h - h) / scale_y) / 2.0f);
            } else {    // Letter box en pantalla wide
                // Escala
                w = (((float)native_w * (float)desktop_h) / (float)native_h);
                scale_x = (w / (float)native_w);
                scale_y = ((float)desktop_h / (float)native_h);
                // Area de dibujado
                viewport.x = (int32_t)((((float)desktop_w - w) / scale_x) / 2.0f);
                viewport.y = 0;
            }
        } else if (full_screen == NGN_SCR_FULLSCREEN) {     // MODO "FULL SCREEN"
            // Modo del renderer
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            // Area de dibujado
            viewport.x = 0;
            viewport.y = 0;
            // Escala
            scale_x = 1.0f;
            scale_y = 1.0f;
        } else {        // MODO "WINDOW"
            // Modo del renderer
            SDL_SetWindowFullscreen(window, 0);
            // Posiciona la ventana
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            // Area de dibujado
            viewport.x = 0;
            viewport.y = 0;
            // Escala
            scale_x = 1.0f;
            scale_y = 1.0f;
        }

        // Ajusta el escalado
        SDL_RenderSetScale(renderer, scale_x, scale_y);

        // Ajusta el viewport del renderer a la resolucion nativa
        SDL_RenderSetViewport(renderer, &viewport);

        // Actualiza los flags
        _full_screen = full_screen;
        force_redaw = true;

    } else {

        force_redaw = false;

    }

}



/*** Cambia el modo del VSYNC ***/
void NGN_Graphics::SetVsync() {

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



/*** Gestion de los parametros del render ***/
void NGN_Graphics::UpdateRendererFlags() {

    // Pantalla completa
    SetFullScreen();
    // VSYNC
    SetVsync();

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



/*** Parametros por defecto de los viewports ***/
void NGN_Graphics::SetupViewports() {

    Viewport_struct v;

    v.available = false;
    v.x = 0;
    v.y = 0;
    v.w = 0;
    v.h = 0;
    v.render_w = 0;
    v.render_h = 0;
    v.surface = NULL;

    viewport_list.clear();
    viewport_list.reserve(VIEWPORT_NUMBER);

    for (uint8_t i = 0; i < viewport_list.capacity(); i ++) {
        viewport_list[i] = v;
    }

    current_viewport = -1;

}



/*** Limpia todas las texturas de los viewports ***/
void NGN_Graphics::ClearViewports() {

    for (uint8_t i = 0; i < viewport_list.capacity(); i ++) {
        if (viewport_list[i].available) {
            // Informa al renderer que la textura "backbuffer" es su destino
            SDL_SetRenderTarget(renderer, viewport_list[i].surface);
            // Borra el contenido de la textura actual
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderFillRect(renderer, NULL);
        }
    }

    // Devuelve el render al seleccionado
    RenderToSelected();

}




/*** Genera la ID del frame en Runtime ***/
void NGN_Graphics::GenerateRuntimeFrameId() {

    uint32_t id = 0;

    do {
        id = (uint32_t)(rand() % 0x7FFFFFFF);
    } while (id == runtime_frame);

    runtime_frame = id;

}
