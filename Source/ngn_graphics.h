/******************************************************************************

    N'gine Lib for C++
    *** Version 1.15.0-stable ***
    Gestion del Renderer de SDL

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2023 by Cesar Rincon "NightFox"

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



#ifndef NGN_GRAPHICS_H_INCLUDED
#define NGN_GRAPHICS_H_INCLUDED


/*** Includes ***/
// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <string>
// N'gine
#include "ngn_defines.h"
#include "ngn_sprite.h"



/*** Definiciones ***/
static const uint8_t VIEWPORT_NUMBER = 8;       // Numero de viewports configurables



/*** Definicion de la clase ***/
class NGN_Graphics {

    // Public
    public:

        // Devuelve la instancia
        static NGN_Graphics* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


        // Inicializa la ventana principal y la superficie de renderizado
        bool Init(
                  std::string window_name,              // Nombre en la ventana
                  uint32_t native_width,                // Resolucion Nativa del juego
                  uint32_t native_height,
                  int8_t scr_mode = NGN_SCR_WINDOW,     // Modo de pantalla
                  bool bilinear_filter = false,         // Filtro bilinear activado?
                  bool sync = true                      // VSYNC activo?
                  );

        void SetMode(int8_t mode);              // Cambia el modo de pantalla
        void SetFiltering(bool enabled);        // Filtrado bilinear de la escena?
        void SetVerticalSync(bool enabled);     // Sincronismo vertical?

        SDL_Window* window;             // Puntero a la ventana
        int32_t window_flags;           // Control de los falgs de la ventana
        SDL_Renderer* renderer;         // Puntero al renderer
        #if !defined (DISABLE_BACKBUFFER)
            SDL_Texture* backbuffer;    // Backbufer para el renderizado
        #endif

        int8_t screen_mode;             // Modo de pantalla actual
        bool vsync;                     // VSYNC Activo?
        bool filtering;                 // Filtrado bilinear activo?

        int32_t native_w;               // Resolucion nativa del juego
        int32_t native_h;
        std::string window_caption;     // Guarda el titulo de la ventana

        Size2I32 render_resolution;     // Resolucion de salida del render

        bool force_redaw;               // Indicar que se debe forzar el redibujado

        // Estable el destino del render al seleccionado por defecto
        void RenderToSelected();

        // Color del backdrop del viewport principal
        void SetBackdropColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        // Actualiza el renderer a 60fps
        void Update();

        // Multi-viewport
        struct Viewport_struct {
            int32_t x;                              // Posicion X
            int32_t y;                              // Posicion Y
            int32_t w;                              // Ancho del viewport
            int32_t h;                              // Altura del viewport
            int32_t render_w;                       // Ancho del render
            int32_t render_h;                       // Altura del render
            SDL_Rect clip_area;                     // Area del clip del renderer
            bool local_filter;                      // Filtrado local?
            bool _local_filter;                     // Estado actual del filtrado
            bool available;                         // Viewport disponible?
            SDL_Texture* surface;                   // Textura para usar de backbuffer para el viewport
            Rgba backdrop_color;                    // Color por defecto del backdrop
        };
        std::vector<Viewport_struct> viewport_list;
        int8_t current_viewport;                    // Viewport actual (-1 sin viewport)

        // Abre un viewport
        void OpenViewport(
            uint8_t id,                                 // ID del VIEWPORT
            int32_t pos_x,                              // Posicion del viewport
            int32_t pos_y,
            uint32_t width,                             // Ancho del viewport
            uint32_t height,                            // Alto del viewport
            uint32_t h_res = NGN_DEFAULT_VALUE,         // Resolucion del render en el viewport
            uint32_t v_res = NGN_DEFAULT_VALUE,
            bool local_filter = false                   // Filtrado local?
        );
        // Cierra un viewport
        void CloseViewport(uint8_t id);
        // Selecciona el viewport
        void SelectViewport(uint8_t id);
        // Posiciona un viewport
        void ViewportPosition(uint8_t id, int32_t x, int32_t y);            // Sobrecarga 1
        void ViewportPosition(uint8_t id, Vector2I32 position);             // Sobrecarga 2
        // Cambia el estado local del filtro de un viewport
        void ViewportLocalFilter(uint8_t id, bool status);
        // Cambia el color del backdrop de un viewport
        void ViewportBackdropColor(uint8_t id, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        // Viewport por defecto
        void DefaultViewport();


        // Cambia el tamaño del clip del viewport [1ra sobrecarga, viewport principal]
        void SetViewportClip(int32_t x, int32_t y, int32_t w, int32_t h);
        SDL_Rect clip_area;
        // Cambia el tamaño del clip del viewport [2da sobrecarga, viewports multiples]
        void SetViewportClip(uint8_t id, int32_t x, int32_t y, int32_t w, int32_t h);


        // Ajusta la visibilidad del cursor del raton
        void ShowMouse(bool visible);

        // Crea una captura del frame actual en formato PNG
        void ScreenShot(std::string path, NGN_TextureData* overlay = NULL, uint8_t alpha = 0xFF);

        // ID del frame en tiempo de ejecucion
        uint32_t runtime_frame;


        /*** Clonado de objetos ***/
        // Clona un sprite con los parametros actuales
        NGN_Sprite* CloneSprite(NGN_Sprite* sprite);

        /*** Resolucion del escritorio ***/
        Size2I32 GetDesktopResolution();                    // Devuelve la resolucion actual del escritorio

        /*** Devuelve el numero de FPS actual ***/
        uint32_t GetFps();                                  // Devuelve el n� de fotogramas renderizado en el ultimo segundo

        /*** Metodos de uso interno ***/
        Vector2 ScaleAndFitCoordinates(Vector2 coord);      // Escala las coordenadas al area visible



    // Private
    private:

        // Contructor
        NGN_Graphics();

        // Destructor
        ~NGN_Graphics();

        // Puntero de memoria a la instancia
        static NGN_Graphics* instance;


        // Resolucion de pantalla completa
        int32_t desktop_w;
        int32_t desktop_h;
        int32_t desktop_refresh_rate;

        // Control del formato de pantalla
        float aspect_native, aspect_desktop;
        Vector2 output_scale, output_viewport;

        // Control del framerate (definido el framerate maximo en NGN_FPS_LIMIT)
        void SyncFrame();
        uint64_t last_frame_count;
        const float frame_time = (1.0f / (float)NGN_FPS_LIMIT);

        // Control de la pantalla (verificacion)
        int8_t _screen_mode;        // Modo actual de pantalla?
        bool _vsync;                // VSYNC Activo?
        bool _filtering;            // Filtrado bilinear activo?

        // Cambio del modo de pantalla
        void ChangeScreenMode();
        // Cambio del VSYNC
        void SetVsync();
        // Cambio del modo de filtrado
        void SetRenderScaleQuality();
        // Gestion de los parametros del render
        void UpdateRendererFlags();

        // Contador de FPS (Debug)
        uint64_t fps_last_time;         // Marca de tiempo
        uint32_t fps_frames;            // Contador de frames renderizados desde el ultimo conteo
        uint32_t fps;                   // Guarda el numero actual de FPS del ultimo segundo
        void FpsCounter();
        void CountFramesPerSecond();

        // Inicializa los viewports
        void SetupViewports();

        // Limpeza del renderer de los viewports
        void ClearViewports();

        // Genera un ID del frame unico en Runtime
        void GenerateRuntimeFrameId();

        // Crea/actualiza el backbuffer del render
        #if !defined (DISABLE_BACKBUFFER)
            void SetBackbuffer();
        #endif
        // Manda el backbuffer a la pantalla
        void RenderBackbuffer();
        // Limpia el backbuffer
        void ClearBackbuffer();

        // Color del backdrop
        Rgba backdrop_color;

        // Graba el frame actual en formato PNG
        std::vector<uint8_t> png_pixels;        // Pixeles para formar el PNG
        std::vector<uint8_t> png_buffer;        // Datos del backbuffer para formar el PNG
        std::string screenshot_filename;        // Ruta y nombre del archivo para el screen shoot
        bool take_screenshot;                   // Debes de realizar un screenshoot?
        NGN_TextureData* screenshot_overlay;    // Overlay del screenshoot
        uint8_t screenshot_overlay_alpha;       // Alpha del overlay
        void SaveCurrentFrameToPng();           // Realiza una captura de pantalla al final del frame si se solicita

};



#endif // NGN_GRAPHICS_H_INCLUDED
