/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso de repositorios y archivos empaquetados

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.20.0+10th-anniversary o superior
	(c) 2016 - 2026 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.5) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.22.0) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (3.0.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20230410)
    (c) 2005 - 2023 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <string>
#include <vector>
// Includes de la libreria
#include <ngn.h>



/*** Parametros de la ventana ***/
static const std::string WINDOW_TITLE = "Repositories and packages usage example.";     // Titulo de la ventana
static const uint32_t SCR_WIDTH = 1280;                                                 // Resolucion
static const uint32_t SCR_HEIGHT = 720;
static const int8_t SCR_MODE_WINDOWS = NGN_SCR_WINDOW;              // Modo de pantalla en Windows
static const int8_t SCR_MODE_LINUX = NGN_SCR_WINDOW;                // Modo de pantalla en Linux
static const bool SHOW_MOUSE = false;                               // Estado del puntero del raton
static const bool BILINEAR_FILTER = false;                          // Filtrado bi-linear
static const bool VSYNC = true;                                     // Sincronismo vertical
static const bool FPS_COUNTER = true;                               // Contador de frames por segundo (solo en modo debug)



/*** Declaracion de la clase ***/
class Demo {

    public:

        // Constructor
        Demo();

        // Destructor
        ~Demo();

        // Procesos iniciales
        bool Awake();

        // Al iniciar el programa
        bool Start();

        // Nucleo de ejecucion
        uint32_t Run();


    private:

        // FLAGS de estado
        const uint32_t FLAG_ZERO = 0;
        const uint32_t FLAG_EXIT = 1;

        // Carga de archivos necesarios
        bool Load();

        // Crea los elementos
        void Create();

        // Logica del programa
        void Update();

        // Render de los elementos graficos
        void Render();

        // Fondo
        struct {
            NGN_Texture* texture;                       // Textura del fondo
            const Vector2 position = {0.0f, 0.0f};      // Posicion de la textura
        } background;

        // Isla
        struct {
            NGN_Texture* texture;                       // Textura de la isla
            const Vector2 position = {48.0f, 500.0f};   // Posicion de la textura
        } island;

        // Nubes
        struct {
            NGN_TiledBg* tilemap;                       // Tilemap de las nubes
            const Vector2 position = {0.0f, -136.0f};   // Posicion del tilemap (son coordenadas a mostrar, no de posicion en pantalla)
            const float speed = 0.25f;                  // Velocidad
            const float loop = 1088.0f;                 // Punto de loop
        } clouds;

        // Bote
        struct {
            NGN_Sprite* sprite;                         // Sprite del bote
            const Vector2 position = {200.0f, 560.0f};  // Posicion inicial del sprite
            const float speed = 0.5f;                   // Velocidad
        } boat;

        // Pajaro
        static const uint8_t BIRDS = 5;                 // Numero de pajaros de la bandada
        struct {
            NGN_Sprite* sprite[BIRDS];                  // Array con los sprites
            const uint32_t top = 284;                   // Limite superior de creacion
            const uint32_t bottom = 484;                // Limite inferior de creacion
            const float speed = 0.25f;                  // Velocidad de los pajaros
        } bird;

        // Caja de texto
        struct {
            NGN_TextLayer* layer;
            const Vector2 position = {64.0f, 64.0f};    // Posicion de la caja de texto
            const uint32_t padding = 32;                // Margen interior
            const uint32_t lines = 3;                   // Numero de lineas a imprimir
            std::vector<std::string> textlines;         // Lineas con el texto a mostrar
        } textbox;

        // Sonido de fondo
        struct {
            NGN_MusicClip* seaside;                     // Stream de sonido (folley de fondo)
            NGN_AudioClipData* horn;                    // Sirena del barco (cachear el recurso)
            int32_t timer;                              // Temporizador para la sirena
            const int32_t horn_ticks = 600;             // Tiempo para que suene la sirena
        } sound;

};


#endif // DEMO_H_INCLUDED
