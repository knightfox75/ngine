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



#ifndef NGN_GRAPHICS_H_INCLUDED
#define NGN_GRAPHICS_H_INCLUDED


/*** Includes ***/
// C++
#include <string>



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
                  std::string window_name,                  // Nombre en la ventana
                  uint32_t native_width,                    // Resolucion Nativa del juego
                  uint32_t native_height,
                  bool full_scr = false,                    // Pantalla completa?
                  bool sync = true,                         // VSYNC activo?
                  int32_t window_width = DEFAULT_VALUE,     // Resolucion de la ventana
                  int32_t window_height = DEFAULT_VALUE
                  );

        SDL_Window* window;             // Puntero a la ventana
        SDL_Renderer* renderer;         // Puntero al renderer

        bool full_screen;               // Pantalla completa?
        bool vsync;                     // VSYNC Activo?

        int32_t native_w;               // Resolucion nativa del juego
        int32_t native_h;
        std::string window_caption;     // Guarda el titulo de la ventana

        bool force_redaw;               // Indicar que se debe forzar el redibujado

        // Actualiza el renderer a 60fps
        void Update();

        // Ajusta el clip del viewport
        void SetViewportClip(int32_t x, int32_t y, int32_t w, int32_t h);



    // Private
    private:

        // Resolucion de la ventana
        int32_t screen_w;
        int32_t screen_h;

        // Resolucion de pantalla completa
        int32_t desktop_w;
        int32_t desktop_h;
        int32_t desktop_refresh_rate;

        // Control del framerate
        void SyncFrame(int32_t fps);
        int32_t time_gap;
        int32_t time_elapsed;
        int32_t time_last_frame;

        // Control de la pantalla (verificacion)
        bool _full_screen;          // Pantalla completa?
        bool _vsync;                // VSYNC Activo?

        // Gestion de los parametros del render
        void UpdateRendererFlags();

        // Contador de FPS (Debug)
        uint32_t fps_frames;
        uint32_t fps_timer;
        void FpsCounter();

        // Factor de escalado
        float scale_x;
        float scale_y;


};



#endif // NGN_GRAPHICS_H_INCLUDED
