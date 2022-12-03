/******************************************************************************

    N'gine Lib for C++
    *** Version 1.11.0-wip0x02 ***
    Gestion del Renderer de SDL

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2022 by Cesar Rincon "NightFox"
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
#include <fstream>


// SDL
#include <SDL.h>

// LodePNG
#include "lodepng/lodepng.h"

// Libreria
#include "ngn.h"



/*** Contructor ***/
NGN_Graphics::NGN_Graphics() {

    // Inicializa las variables de la clase
    window = NULL;
    renderer = NULL;
    #if !defined (DISABLE_BACKBUFFER)
        backbuffer = NULL;
    #endif
    force_redaw = true;

    // Inicializa el control de frame rate
    last_frame_count = SDL_GetPerformanceCounter();     // Duracion del ultimo frame

    // Inicializa el control del contador FPS
    fps_frames = 0;
    fps = 0;
    fps_last_time = SDL_GetPerformanceCounter();

    // Inicia los viewports
    SetupViewports();

    // Genera el primer runtime frame
    runtime_frame = 0;
    GenerateRuntimeFrameId();

    // Control de escala
    output_scale.x = 1.0f;
    output_scale.y = 1.0f;
    output_viewport.x = 0.0f;
    output_viewport.y = 0.0f;

    // Grabacion de la pantalla a PNG
    png_pixels.clear();
    png_buffer.clear();
    screenshot_filename = "";
    screenshot_overlay = NULL;
    screenshot_overlay_alpha = 0xFF;
    take_screenshot = false;

}



/*** Destructor ***/
NGN_Graphics::~NGN_Graphics() {

    // Elimina los vectores de memoria
    for (uint8_t i = 0; i < viewport_list.capacity(); i ++) {
        if (viewport_list[i].surface != NULL) SDL_DestroyTexture(viewport_list[i].surface);
    }
    viewport_list.clear();

    png_pixels.clear();
    png_buffer.clear();


    // Elimina los contenedores graficos
    #if !defined (DISABLE_BACKBUFFER)
        if (backbuffer != NULL) SDL_DestroyTexture(backbuffer);
    #endif
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
                          int8_t scr_mode,                  // Modo de pantalla
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
        ngn->log->Message("[NGN_Graphics error] Native resolution must be greather than 0.");
        return false;
    }

    // Guarda la resolucion del escritorio
    SDL_DisplayMode display;
    if (SDL_GetDesktopDisplayMode(0, &display) < 0) {
        ngn->log->Message("[NGN_Graphics error] Can't get current desktop resolution.");
        return false;
    }
    desktop_w = display.w;
    desktop_h = display.h;
    desktop_refresh_rate = display.refresh_rate;

    // Crea la ventana para el renderer, con las opciones por defecto
    window = SDL_CreateWindow((const char*)window_caption.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, native_w, native_h, SDL_WINDOW_SHOWN);
    // Verifica si ha ocurrido un error en la creacion de la ventana
    if (window == NULL) {
        ngn->log->Message("[NGN_Graphics error] SDL is unable to create the main Window.");
        return false;
    }
    window_flags = SDL_GetWindowFlags(window);

    // Si la ventana se ha creado, intenta crear la superficie de renderizado, con el dibujado sincronizado al frame
    renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE));
    // Si no se puede crear, destruye la venta e informa del error
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        window = NULL;
        ngn->log->Message("[NGN_Graphics error] SDL is unable to create the rendering surface.");
        return false;
    }

    // Selecciona el color por defecto del renderer
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    // Filtrado bilinear por defecto?
    #if !defined (DISABLE_BACKBUFFER)
        // Poe defecto, no crees ningun elemento con filtrado
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        // Y guarda si debe filtrarse la escena final
        filtering = bilinear_filter;
        _filtering = !filtering;
    #else
        // Aplica el filtrado por defecto
        if (bilinear_filter) {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        } else {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        }
        // Y guarda si debe filtrarse la escena final
        filtering = bilinear_filter;
        _filtering = filtering;
    #endif

    // Calcula la relacion de aspecto
    aspect_native = ((float)native_w / (float)native_h);
    aspect_desktop = ((float)desktop_w / (float)desktop_h);

    // Pantalla completa o ventana?
    screen_mode = scr_mode;
    _screen_mode = 127;
    ChangeScreenMode();

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

    // Prepara el backbuffer
    #if !defined (DISABLE_BACKBUFFER)
        SetBackbuffer();
    #endif

    // Sal con normalidad
    return true;

}



/*** Cambia el modo de pantalla ***/
void NGN_Graphics::SetMode(int8_t mode) {

    switch (mode) {
        case NGN_SCR_WINDOW:
            screen_mode = NGN_SCR_WINDOW;
            break;
        case NGN_SCR_WINDOW_X2:
            screen_mode = NGN_SCR_WINDOW_X2;
            break;
        case NGN_SCR_WINDOW_X3:
            screen_mode = NGN_SCR_WINDOW_X3;
            break;
        case NGN_SCR_WINDOW_X4:
            screen_mode = NGN_SCR_WINDOW_X4;
            break;
        case NGN_SCR_WINDOW_FULL:
            screen_mode = NGN_SCR_WINDOW_FULL;
            break;
        case NGN_SCR_FULLSCREEN:
            screen_mode = NGN_SCR_FULLSCREEN;
            break;
    }

}



/*** Activa o desactiva el filtrado bilinear de la escena ***/
void NGN_Graphics::SetFiltering(bool enabled) {
    filtering = enabled;
}



/*** Activa o desactiva el sincronismo vertical ***/
void NGN_Graphics::SetVerticalSync(bool enabled) {
    vsync = enabled;
}



/*** Estable el destino del render al seleccionado por defecto ***/
void NGN_Graphics::RenderToSelected() {

    // Cambia el destino del renderer a la pantalla
    if (ngn->render->GetRenderToTextureState()) {
        ngn->render->SetRenderToTextureState();
    } else if (current_viewport >= 0) {
        SDL_SetRenderTarget(renderer, viewport_list[current_viewport].surface);
    } else {
        #if !defined (DISABLE_BACKBUFFER)
            SDL_SetRenderTarget(renderer, backbuffer);
        #else
            SDL_SetRenderTarget(renderer, NULL);
        #endif
    }
    // Restaura el color y alpha del renderer
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

}



/*** Actualiza la superficie del renderer ***/
void NGN_Graphics::Update() {

    // Presenta el backbuffer en la pantalla
    RenderBackbuffer();

    // Si es necesario, realiza la captura de pantalla
    SaveCurrentFrameToPng();

    // Espera al siguiente frame (60fps soft limit, definido en NGN_FPS_LIMIT)
    SyncFrame();

    // Contador de frames por segundo
    CountFramesPerSecond();

    // Contador de FPS en la consola [*** Debug ***]
    if (ngn->system->fps_counter) FpsCounter();

    // Borra los viewports de existir estos
    ClearViewports();

    // Borra el contenido para el siguiente frame
    ClearBackbuffer();

    // Actualiza los flags del renderer
    UpdateRendererFlags();

    // Genera el Runtime Frame ID para el siguiente frame
    GenerateRuntimeFrameId();

}



/*** Abre un viewport en la ID dada ***/
void NGN_Graphics::OpenViewport(
    uint8_t id,             // ID del VIEWPORT
    int32_t pos_x,          // Posicion del viewport
    int32_t pos_y,
    uint32_t width,         // Ancho del viewport
    uint32_t height,        // Alto del viewport
    uint32_t h_res,         // Resolucion del render en el viewport
    uint32_t v_res,
    bool local_filter       // Filtrado local?
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

    // Este viewport ha de disponer de filtrado local?
    if (local_filter) {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    } else {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    }
    v._local_filter = v.local_filter = local_filter;

    // Crea la textura
    v.surface = SDL_CreateTexture(
                             renderer,                      // Renderer
                             SDL_PIXELFORMAT_BGRA8888,      // Formato del pixel
                             SDL_TEXTUREACCESS_TARGET,      // Textura como destino del renderer
                             v.render_w,                    // Ancho de la textura
                             v.render_h                     // Alto de la textura
                             );

    // Restaura el filtrado
    #if !defined (DISABLE_BACKBUFFER)
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    #else
        if (filtering) {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        } else {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        }
    #endif

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
    v._local_filter = v.local_filter = false;

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

    if (id >= VIEWPORT_NUMBER) return;
    if (!viewport_list[id].available) return;

    viewport_list[id].x = x;
    viewport_list[id].y = y;

}
/*** Posiciona un viewport (Sobrecarga 1) ***/
void NGN_Graphics::ViewportPosition(uint8_t id, Vector2I32 position) {

    ViewportPosition(id, position.x, position.y);

}



/*** Cambia el estado del filtrado local del viewport ***/
void NGN_Graphics::ViewportLocalFilter(uint8_t id, bool status) {

    if (id >= VIEWPORT_NUMBER) return;
    viewport_list[id].local_filter = status;

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



/*** Crea una imagen PNG con el contenido de la pantalla ***/
void NGN_Graphics::ScreenShot(std::string path,  NGN_TextureData* overlay, uint8_t alpha) {

    // Guarda el nombre de archivo
    screenshot_filename = path;
    // Guarda el overlay
    screenshot_overlay = overlay;
    screenshot_overlay_alpha = alpha;
    // Indica que debe de realizarse el screenshoot
    take_screenshot = true;

}



/*** Clona un sprite con los parametros actuales ***/
NGN_Sprite* NGN_Graphics::CloneSprite(NGN_Sprite* sprite) {

    // Si el sprite no es valido...
    if (!sprite) return NULL;

    // Sprite base para recibir el clon
    NGN_Sprite* _spr = new NGN_Sprite(
        sprite->data,
        sprite->position.x,
        sprite->position.y,
        sprite->width,
        sprite->height,
        sprite->box.width,
        sprite->box.height,
        sprite->box.offset.x,
        sprite->box.offset.y
    );

    // Copia los parametros
    _spr->position = sprite->position;
    _spr->screen = sprite->screen;
    _spr->width = sprite->width;
    _spr->height = sprite->height;
    _spr->center = sprite->center;
    _spr->box = sprite->box;
    _spr->box_enabled = sprite->box_enabled;
    _spr->visible = sprite->visible;
    _spr->alpha = sprite->alpha;
    _spr->on_screen = sprite->on_screen;
    _spr->rotation = sprite->rotation;
    _spr->flip_h = sprite->flip_h;
    _spr->flip_v = sprite->flip_v;
    _spr->colliders = sprite->colliders;
    _spr->frame = sprite->frame;
    _spr->animation = sprite->animation;
    _spr->current_animation = sprite->current_animation;
    _spr->animation_pause = sprite->animation_pause;
    _spr->camera_layer = sprite->camera_layer;
    _spr->runtime_frame = sprite->runtime_frame;

    // Devuelve el clon creado
    return _spr;

}



/*** Devuelve la resolucion actual del escritorio ***/
Size2I32 NGN_Graphics::GetDesktopResolution() {

    Size2I32 res = {desktop_w, desktop_h};
    return res;

}



/*** Devuelve el numero de FPS actual ***/
uint32_t NGN_Graphics::GetFps() {

    return fps;

}



/*** Escala y adapta las coordenadas al area visible ***/
Vector2 NGN_Graphics::ScaleAndFitCoordinates(Vector2 coord) {

    // Resultado
    Vector2 r;

    // Escala las coordenadas
    r.x = (coord.x / output_scale.x);
    r.y = (coord.y / output_scale.y);

    // Ajusta el aspect ratio
    r.x -= output_viewport.x;
    r.y -= output_viewport.y;

    // Devuelve el resultado
    return r;

}



/*** Sincronizacion de velocidad del framerate ***/
void NGN_Graphics::SyncFrame() {

    // Calcula el tiempo actual
    uint64_t current_frame_count = SDL_GetPerformanceCounter();

    // Calcula el diferencial
    float time_elapsed = ((float)(current_frame_count - last_frame_count) / (float)SDL_GetPerformanceFrequency());

    // Si es necesario, esperate los ticks necesarios
    if (time_elapsed < frame_time) SDL_Delay((uint32_t)((frame_time - time_elapsed) * 1000));

    // Guarda la marca de tiempo de este frame
    last_frame_count = SDL_GetPerformanceCounter();

}



/*** Cambia de modo ventana a pantalla completa y vice-versa ***/
void NGN_Graphics::ChangeScreenMode() {

    // Full Screen?
    if (_screen_mode != screen_mode) {

        // Area del viewport
        SDL_Rect viewport;
        viewport.w = native_w;
        viewport.h = native_h;
        // Datos de posicion y escalado
        float w, h;
        float scale_x, scale_y;

        // MODO "FULL WINDOW"
        if (screen_mode == NGN_SCR_WINDOW_FULL) {

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

        } else if (screen_mode == NGN_SCR_FULLSCREEN) {     // MODO "FULL SCREEN"


            // Analiza el modo actual de pantalla y prepara el FULL SCREEN para la resolucion del escritorio
            SDL_DisplayMode display_mode;
            SDL_GetCurrentDisplayMode(0, &display_mode);
            display_mode.w = desktop_w;
            display_mode.h = desktop_h;
            SDL_SetWindowDisplayMode(window, &display_mode);
            float aspect_display = ((float)display_mode.w / (float)display_mode.h);

            // Modo del renderer
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

            // Segun la relacion de aspecto, calcula el viewport y el clip area
            if (aspect_native == aspect_display) {  // Mismo formato
                // Escala
                scale_x = ((float)display_mode.w / (float)native_w);
                scale_y = ((float)display_mode.h / (float)native_h);
                // Area de dibujado
                viewport.x = 0;
                viewport.y = 0;
            } else if (aspect_native > aspect_display) {    // Wide en pantalla letter box
                // Escala
                h = (((float)native_h * (float)display_mode.w) / (float)native_w);
                scale_x = ((float)display_mode.w / (float)native_w);
                scale_y = (h / (float)native_h);
                // Area de dibujado
                viewport.x = 0;
                viewport.y = (int32_t)((((float)display_mode.h - h) / scale_y) / 2.0f);
            } else {    // Letter box en pantalla wide
                // Escala
                w = (((float)native_w * (float)display_mode.h) / (float)native_h);
                scale_x = (w / (float)native_w);
                scale_y = ((float)display_mode.h / (float)native_h);
                // Area de dibujado
                viewport.x = (int32_t)((((float)display_mode.w - w) / scale_x) / 2.0f);
                viewport.y = 0;
            }

        } else {        // MODO "WINDOW"

            // Modo del renderer
            SDL_SetWindowFullscreen(window, 0);

            // Ajusta el tamaño de la ventana
            int8_t win_size = NGN_SCR_WINDOW;
            switch (screen_mode) {
                case NGN_SCR_WINDOW:
                    win_size = 1;
                    break;
                case NGN_SCR_WINDOW_X2:
                    win_size = 2;
                    break;
                case NGN_SCR_WINDOW_X3:
                    win_size = 3;
                    break;
                case NGN_SCR_WINDOW_X4:
                    win_size = 4;
                    break;
            }

            // Cambia el tamaño de la ventana
            int32_t win_width = native_w * win_size;
            int32_t win_height = native_h * win_size;
            SDL_SetWindowSize(window, win_width, win_height);


            // Posiciona la ventana
            SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

            // Area de dibujado
            viewport.x = 0;
            viewport.y = 0;

            // Escala
            scale_x = win_size;
            scale_y = win_size;

        }

        // Ajusta el escalado
        SDL_RenderSetScale(renderer, scale_x, scale_y);
        output_scale.x = scale_x;
        output_scale.y = scale_y;
        output_viewport.x = (float)viewport.x;
        output_viewport.y = (float)viewport.y;

        // Ajusta el viewport del renderer a la resolucion nativa
        SDL_RenderSetViewport(renderer, &viewport);

        // Actualiza los flags
        _screen_mode = screen_mode;
        force_redaw |= true;

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



/*** Cambia el modo de filtrado bilineal del renderer ***/
void NGN_Graphics::SetRenderScaleQuality() {

    #if !defined (DISABLE_BACKBUFFER)
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    #else
        if (_filtering != filtering) {
            if (filtering) {
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
            } else {
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
            }
        }
    #endif
    _filtering = filtering;

}



/*** Gestion de los parametros del render ***/
void NGN_Graphics::UpdateRendererFlags() {

    // VSYNC
    SetVsync();

    // Focus
    //GetWindowFocus();

    // Filtering
    SetRenderScaleQuality();


}



/*** Muestra los frames por segundo [DEBUG] ***/
void NGN_Graphics::FpsCounter() {

    std::cout << "Render Speed: " << fps << " fps.     \x0d";

}



/*** Cuenta el numero de frames por segundo ***/
void NGN_Graphics::CountFramesPerSecond() {

    // Marca actual
    uint64_t current_time = SDL_GetPerformanceCounter();

    // Tiempo transcurrido
    float time_elapsed = ((float)(current_time - fps_last_time) / (float)SDL_GetPerformanceFrequency());

    // Contador de fps
    if (time_elapsed >= 1.0f) {
        fps = fps_frames;
        fps_frames = 0;
        fps_last_time = SDL_GetPerformanceCounter();
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
    v._local_filter = v.local_filter = false;

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
            // Si hay un cambio de modo en el filtrado...
            if (viewport_list[i].local_filter != viewport_list[i]._local_filter) {
                // Destruye la textura actual
                if (viewport_list[i].surface != NULL) SDL_DestroyTexture(viewport_list[i].surface);
                // Selecciona el modo de filtrado correspondiente
                if (viewport_list[i].local_filter) {
                    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
                } else {
                    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
                }
                // Crea la textura
                viewport_list[i].surface = SDL_CreateTexture(
                     renderer,                      // Renderer
                     SDL_PIXELFORMAT_BGRA8888,      // Formato del pixel
                     SDL_TEXTUREACCESS_TARGET,      // Textura como destino del renderer
                     viewport_list[i].render_w,     // Ancho de la textura
                     viewport_list[i].render_h      // Alto de la textura
                );
                // Restaura el filtrado
                if (filtering) {
                    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
                } else {
                    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
                }
            }
            // Informa al renderer que la textura "backbuffer" del viewport es su destino
            SDL_SetRenderTarget(renderer, viewport_list[i].surface);
            // Borra el contenido de la textura actual
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_RenderFillRect(renderer, NULL);
            // Actualiza el estado del filtrado local
            viewport_list[i]._local_filter = viewport_list[i].local_filter;
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



/*** Crea o actualiza el backbuffer ***/
#if !defined (DISABLE_BACKBUFFER)
    void NGN_Graphics::SetBackbuffer() {

        // Si existe un backbuffer, eliminalo
        if (backbuffer != NULL) SDL_DestroyTexture(backbuffer);

        // Este backbuffer ha de disponer de filtrado?
        if (filtering) {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        } else {
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        }

        // Crea el backbuffer de este fondo
        backbuffer = SDL_CreateTexture(
                                 renderer,                      // Renderer
                                 SDL_PIXELFORMAT_BGRA8888,      // Formato del pixel
                                 SDL_TEXTUREACCESS_TARGET,      // Textura como destino del renderer
                                 native_w,                      // Ancho de la textura
                                 native_h                       // Alto de la textura
                                 );

        SDL_SetRenderTarget(renderer, backbuffer);

        // Borra el contenido de la textura actual
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(backbuffer, 0xFF);
        SDL_RenderFillRect(renderer, NULL);

    }
#endif


/*** Renderiza el backbuffer en la pantalla ***/
void NGN_Graphics::RenderBackbuffer() {

    #if !defined (DISABLE_BACKBUFFER)

        // Selecciona el renderer principal
        SDL_SetRenderTarget(renderer, NULL);

        // Datos para renderizar el backbuffer
        double _rotation = 0.0f;
        SDL_RendererFlip _flip = SDL_FLIP_NONE;
        // Centro de la rotacion
        SDL_Point* _center = new SDL_Point();
        _center->x = (native_w / 2);
        _center->y = (native_h / 2);

        // Define el area de render
        SDL_Rect _src = {
            0,              // Posicion X
            0,              // Posicion Y
            native_w,       // Ancho
            native_h        // Alto
        };
        SDL_Rect _dst = {
            0,              // Posicion X
            0,              // Posicion Y
            native_w,       // Ancho
            native_h        // Alto
        };

        // Renderiza la textura
        SDL_RenderCopyEx(renderer, backbuffer, &_src, &_dst, _rotation, _center, _flip);

        // Paso de limpieza
        delete _center;

    #endif

    // Actualiza el cambio de modo grafico
    ChangeScreenMode();

    // Presenta el resultado final en la pantalla
    SDL_RenderPresent(renderer);

}



/*** Borra el backbuffer ***/
void NGN_Graphics::ClearBackbuffer() {

    // Borra el contenido del renderer
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderClear(renderer);

    #if !defined (DISABLE_BACKBUFFER)

        // Informa al renderer que la textura "backbuffer" es su destino
        SDL_SetRenderTarget(renderer, backbuffer);

        // Borra el backbuffer o vuelvelo a crear si ha cambiado el modo de filtrado
        if (filtering != _filtering) {
            SetBackbuffer();
        } else {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
            SDL_SetTextureBlendMode(backbuffer, SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(backbuffer, 0xFF);
            SDL_RenderFillRect(renderer, NULL);
        }

    #endif

}



/*** Realiza un screenshoot si se solicita ***/
void NGN_Graphics::SaveCurrentFrameToPng() {

    // Si no esta marcado el flag
    if (!take_screenshot) return;

    // Si la ruta esta vacia
    if (screenshot_filename == "") return;

    #if !defined (DISABLE_BACKBUFFER)

        // Selecciona el renderer principal
        SDL_SetRenderTarget(renderer, NULL);

        // Borra el contenido actual
        SDL_RenderClear(renderer);

    #endif

    // Datos para renderizar el frame actual (una vez presentado en pantalla)
    double _rotation = 0.0f;
    SDL_RendererFlip _flip = SDL_FLIP_NONE;
    // Centro de la rotacion
    SDL_Point* _center = new SDL_Point();
    _center->x = (native_w / 2);
    _center->y = (native_h / 2);

    // Define el area de render
    SDL_Rect _src = {
        0,              // Posicion X
        0,              // Posicion Y
        native_w,       // Ancho
        native_h        // Alto
    };
    SDL_Rect _dst = {
        0,              // Posicion X
        0,              // Posicion Y
        native_w,       // Ancho
        native_h        // Alto
    };

    #if !defined (DISABLE_BACKBUFFER)

        // Renderiza el contenido del frame
        SDL_RenderCopyEx(renderer, backbuffer, &_src, &_dst, _rotation, _center, _flip);

    #endif

    // Overlay en la captura?
    if (screenshot_overlay != NULL) {
        // Tamaño de origen (textura)
        _src.w = screenshot_overlay->width;
        _src.h = screenshot_overlay->height;
        SDL_SetTextureBlendMode(screenshot_overlay->gfx, SDL_BLENDMODE_BLEND);
        SDL_SetTextureAlphaMod(screenshot_overlay->gfx, screenshot_overlay_alpha);
        SDL_RenderCopyEx(renderer, screenshot_overlay->gfx, &_src, &_dst, _rotation, _center, _flip);
    }

    // Paso de limpieza
    delete _center;

    // Borra los buffers
    png_buffer.clear();
    png_pixels.clear();

    // Consulta el tamaño del renderer
    int32_t _width;
    int32_t _height;
    SDL_GetRendererOutputSize(renderer, &_width, &_height);

    // Crea un surface para copiar los pixeles del renderer
    SDL_Surface* png_surface = SDL_CreateRGBSurface(
        0,                      // Flag
        _width,                 // Ancho
        _height,                // Alto
        32,                     // Profuncidad de color 32bpp [RGBA8888]
        0xFF000000,             // Mascara R
        0x00FF0000,             // Mascara G
        0x0000FF00,             // Mascara B
        0x000000FF              // Mascara A
    );

    // Copia los pixeles del renderer al surface
    if (SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA8888, png_surface->pixels, png_surface->pitch) != 0) {
        ngn->log->Message("[NGN_Graphics error] PNG Screenshot: Error creating surface.");
        SDL_FreeSurface(png_surface);
        take_screenshot = false;
        screenshot_filename = "";
        return;
    }

    // Prepara el backbuffer de destino
    uint32_t pnum = (_width * _height);
    png_buffer.resize((pnum << 2), 0);
    uint32_t* p = (uint32_t*)png_surface->pixels;

    // Copia los datos
    uint32_t n = 0;
    for (uint32_t idx = 0; idx < pnum; idx ++) {
        // RED
        png_buffer[n] = ((p[idx] >> 24) & 0x000000FF);
        n ++;
        // GREEN
        png_buffer[n] = ((p[idx] >> 16) & 0x000000FF);
        n ++;
        // BLUE
        png_buffer[n] = ((p[idx] >> 8) & 0x000000FF);
        n ++;
        // ALPHA
        png_buffer[n] = (p[idx] & 0x000000FF);
        n ++;
    }

    // Libera el surface temporal
    SDL_FreeSurface(png_surface);

    // Intenta codificar el buffer de la textura a PNG
    uint32_t _error = lodepng::encode(png_pixels, png_buffer, _width, _height);

    // Si se ha codificado correctamente, graba el archivo
    if (_error == 0) {
        // Intenta abrir el archivo en modo escritura
        const char* _filepath = screenshot_filename.c_str();
        std::ofstream file;
        file.open(_filepath, std::ofstream::out | std::ofstream::binary);
        // Si no puedes abrir el archivo, intenta crear antes la ruta
        if (!file.is_open()) {
            // Intenta generar la estructura de directorios, en caso de no existir
            ngn->disk->MakePath(screenshot_filename);
            // Y vuelve a intentar abrir el archivo para su escritura
            file.open(_filepath, std::ofstream::out | std::ofstream::binary);
            // Si continua sin poder abrirse, sal informando del error
            if (!file.is_open()) {
                ngn->log->Message("[NGN_Graphics error] PNG Screenshot: Error, can't create <" + screenshot_filename + "> file.");
            }
        }
        // Cierra el archivo de estar abierto
        if (file.is_open()) file.close();
        // Grabacion del archivo
        _error = lodepng::save_file(png_pixels, screenshot_filename);
        // Si hay algun error en la grabacion del PNG
        if (_error != 0) {
            ngn->log->Message("[NGN_Graphics error] PNG Screenshot: Error saving <" + screenshot_filename + ">.");
        }
    } else {    // Fallo en la codificacion del PNG
        ngn->log->Message("[NGN_Graphics error] PNG Screenshot: Error encoding the PNG data.");
    }

    // Libera los buffers
    png_buffer.clear();
    png_pixels.clear();

    // Resetea los parametros del screenshoot
    take_screenshot = false;
    screenshot_filename = "";
    RenderToSelected();

}

