/******************************************************************************

    N'gine Lib for C++
    *** Version 0.11.0-a ***
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



#ifndef NGN_GRAPHICS_H_INCLUDED
#define NGN_GRAPHICS_H_INCLUDED


/*** Includes ***/
// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <string>
// N'gine
#include "ngn_defines.h"



/*** Definiciones ***/
static const uint8_t VIEWPORT_NUMBER = 8;       // Numero de viewports configurables



/*** Definicion de la clase ***/
class NGN_Graphics {

   // Public
    public:

        // Contructor
        NGN_Graphics();

        // Destructor
        ~NGN_Graphics();

        // Inicializa la ventana principal y la superficie de renderizado
        bool Init(
                  std::string window_name,              // Nombre en la ventana
                  uint32_t native_width,                // Resolucion Nativa del juego
                  uint32_t native_height,
                  int8_t full_scr = NGN_SCR_WINDOW,     // Pantalla completa?
                  bool bilinear_filter = false,         // Filtro bilinear activado?
                  bool sync = true                      // VSYNC activo?
                  );

        SDL_Window* window;             // Puntero a la ventana
        SDL_Renderer* renderer;         // Puntero al renderer

        int8_t full_screen;             // Pantalla completa?
        bool vsync;                     // VSYNC Activo?

        int32_t native_w;               // Resolucion nativa del juego
        int32_t native_h;
        std::string window_caption;     // Guarda el titulo de la ventana

        Size2I32 render_resolution;     // Resolucion de salida del render

        bool force_redaw;               // Indicar que se debe forzar el redibujado

        // Estable el destino del render al seleccionado por defecto
        void RenderToSelected();

        // Actualiza el renderer a 60fps
        void Update();

        // Multi-viewport
        struct Viewport_struct {
            int32_t x;
            int32_t y;
            int32_t w;
            int32_t h;
            int32_t render_w;
            int32_t render_h;
            bool available;
            SDL_Texture* surface;
        };
        std::vector<Viewport_struct> viewport_list;
        int8_t current_viewport;                    // Viewport actual (-1 sin viewport)

        // Abre un viewport
        void OpenViewport(
            uint8_t id,                             // ID del VIEWPORT
            int32_t pos_x,                          // Posicion del viewport
            int32_t pos_y,
            uint32_t width,                         // Ancho del viewport
            uint32_t height,                        // Alto del viewport
            uint32_t h_res = NGN_DEFAULT_VALUE,     // Resolucion del render en el viewport
            uint32_t v_res = NGN_DEFAULT_VALUE
        );
        // Cierra un viewport
        void CloseViewport(uint8_t id);
        // Selecciona el viewport
        void SelectViewport(uint8_t id);
        // Posiciona un viewport
        void ViewportPosition(uint8_t id, int32_t x, int32_t y);    // Sobrecarga 1
        void ViewportPosition(uint8_t id, Vector2I32 position);     // Sobrecarga 2
        // Viewport por defecto
        void DefaultViewport();


        // Ajusta el clip del viewport
        void SetViewportClip(int32_t x, int32_t y, int32_t w, int32_t h);
        SDL_Rect cliparea;

        // Ajusta la visibilidad del cursor del raton
        void ShowMouse(bool visible);

        // ID del frame en tiempo de ejecucion
        uint32_t runtime_frame;


        /*** Clonado de objetos ***/
        // Clona un sprite con los parametros actuales
        NGN_Sprite* CloneSprite(NGN_Sprite* sprite);



    // Private
    private:

        // Resolucion de pantalla completa
        int32_t desktop_w;
        int32_t desktop_h;
        int32_t desktop_refresh_rate;

        // Control del formato de pantalla
        float aspect_native, aspect_desktop;

        // Control del framerate
        void SyncFrame(int32_t fps);
        int32_t time_gap;
        int32_t time_elapsed;
        int32_t time_last_frame;

        // Control de la pantalla (verificacion)
        int8_t _full_screen;        // Pantalla completa?
        bool _vsync;                // VSYNC Activo?

        // Cambio de pantalla completa a ventana
        void SetFullScreen();
        // Cambio del VSYNC
        void SetVsync();
        // Gestion de los parametros del render
        void UpdateRendererFlags();

        // Contador de FPS (Debug)
        uint32_t fps_frames;
        uint32_t fps_timer;
        void FpsCounter();

        // Inicializa los viewports
        void SetupViewports();

        // Limpeza del renderer de los viewports
        void ClearViewports();

        // Genera un ID del frame unico en Runtime
        void GenerateRuntimeFrameId();

};



#endif // NGN_GRAPHICS_H_INCLUDED
