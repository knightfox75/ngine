/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso de mascaras (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2022 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.9.0-stable o superior

    Requiere GCC 11.3.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.0.22) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.0.15) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.1) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20200306)
    (c) 2005 - 2020 by Lode Vandevenne
    http://lodev.org/lodepng/


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
// c++
#include <cstdio>
#include <iostream>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Fondo
    bg_data = NULL;
    bg = NULL;

    // Imagen en RAW
    ball_raw = NULL;
    ball_data = NULL;
    ball = NULL;

    // Sprite en RAW
    bird_data = NULL;
    bird = NULL;
    bird_frames.clear();
    bird_fr = 0;
    bird_timer = 0;

    // Mascara
    mask_result_data = NULL;
    mask_result = NULL;
    mask_raw = NULL;
    mask_mode = NGN_MASKMODE_HOLLOW;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Fondo
    delete bg; bg = NULL;
    delete bg_data; bg_data = NULL;

    // Imagen en RAW
    delete ball; ball = NULL;
    delete ball_data; ball_data = NULL;
    delete ball_raw; ball_raw = NULL;

    // Sprite en RAW
    delete bird; bird = NULL;
    delete bird_data; bird_data = NULL;
    for (uint32_t i = 0; i < bird_frames.size(); i ++) delete bird_frames[i];
    bird_frames.clear();

    // Mascara
    delete mask_raw; mask_raw = NULL;
    delete mask_result; mask_result = NULL;
    delete mask_result_data; mask_result_data = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Habilita el archivo de registro
    ngn->log->OpenLogFile("debug_log.txt");

    // Crea la ventana con la resolucion nativa
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    ngn->graphics->Update();

    // Visibilidad del cursor del raton
    ngn->graphics->ShowMouse(SHOW_MOUSE);

    // Contador de FPS activo?
    ngn->system->fps_counter = FPS_COUNTER;

    // Selecciona el modo grafico (ventana/full screen)
    #if defined (OS_WINDOWS)
        ngn->graphics->SetMode(SCR_MODE_WINDOWS);
    #elif defined (OS_LINUX)
        ngn->graphics->SetMode(SCR_MODE_LINUX);
    #endif
    ngn->graphics->Update();

	// Muestra la version de la libreria en la consola
	#if defined (MODE_DEBUG)
		ngn->log->Message(ngn->system->GetVersion() + " started successfully!");
	#endif

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Carga de archivos
    if (!Load()) return false;

    // Crea los elementos del programa
    Create();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
uint8_t Demo::Run() {

    // Control del loop
    uint8_t loop = FLAG_ZERO;

    while (loop == FLAG_ZERO) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();

        // Logica del programa
        Update();
        // Render de los elementos graficos
        Render();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

        // Salida del programa
        if (
            ngn->system->quit                                           // Si se pulsa la [X] de la ventana o ALT+F4
            ||
            ngn->input->key_ESC->down                                   // la tecla ESC
            ||
            ngn->input->controller[0].button[XBOX_BUTTON_XBOX].down     // o el boton [X] del gamepad de XBOX
        ) loop = FLAG_EXIT;

    }

    // Devuelve el resultado
    return loop;

}



/*** Carga de los archivos necesarios ***/
bool Demo::Load() {

    // Carga la textura de fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (!bg_data) return false;

    // Carga la imagen en RAW
    ball_raw = ngn->load->PngAsRaw("data/basketball.png");
    if (!ball_raw) return false;

    // Carga todos los frames del sprite en RAW
    if (!ngn->load->SpriteAsRawVector("data/blue_bird.spr", bird_frames)) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Crea el fondo
    bg = new NGN_Texture(bg_data, 0, 0);

    // Bola de baloncesto
    ball_data = ngn->image->ConvertRawToTextureData(ball_raw);
    ball = new NGN_Texture(ball_data, 412, 300);

    // Crea la imagen RAW para usar como destino de la mascara resultante
    mask_raw = new NGN_RawImage();


}



/*** Logica del programa ***/
void Demo::Update() {

    // Genera la textura con la referencia visual de la mascara
    delete bird_data; bird_data = NULL;
    bird_data = ngn->image->ConvertRawToTextureData(bird_frames[bird_fr]);
    delete bird; bird = NULL;
    bird = new NGN_Texture(bird_data);

    // Posicion
    int32_t px = ngn->input->mouse.x - (bird->width / 2);
    int32_t py = ngn->input->mouse.y - (bird->height / 2);
    bird->Position(px, py);

    // Cambia el modo de la mascara con la tecla espacio
    if (ngn->input->key_SPACE->down) mask_mode = (mask_mode == NGN_MASKMODE_HOLLOW) ? NGN_MASKMODE_CUTOUT:NGN_MASKMODE_HOLLOW;

    // Genera la imagen con la mascara superpuesta
    Vector2I32 offset = {(px - (int32_t)ball->position.x), (py - (int32_t)ball->position.y)};
    ngn->image->AdvancedMask(ball_raw, bird_frames[bird_fr], mask_raw, offset, mask_mode);

    // Crea los datos de textura con el resultado
    delete mask_result_data; mask_result_data = NULL;
    mask_result_data = ngn->image->ConvertRawToTextureData(mask_raw);
    // Crea la textura con la mascara aplicada
    delete mask_result; mask_result = NULL;
    mask_result = new NGN_Texture(mask_result_data, 740, 300);

    // Animacion
    bird_timer ++;
    if (bird_timer >= anim_delay) {
        bird_timer = 0;
        bird_fr ++;
        if (bird_fr >= bird_frames.size()) bird_fr = 0;
    }

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Fondo
    ngn->render->Texture(bg);

    // Texturas
    ngn->render->Texture(ball);
    ngn->render->Texture(mask_result);
    ngn->render->Texture(bird);

}
