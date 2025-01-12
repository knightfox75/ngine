/******************************************************************************

    N'gine Lib for C++
    Ejemplo de la captura del surface (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.18.0-stable o superior
	(c) 2016 - 2025 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 13.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.11) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.24.0) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.6.2) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/

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

    // Target del renderer
    target_texture = NULL;

    // CrystalBall
    for (uint8_t i = 0; i < layers; i ++) {
        tbg_data[i] = NULL;
        tbg[i] = NULL;
    }

    // Captura del renderer
    renderer_surface = NULL;
    renderer_target = NULL;

    // Mascara a aplicar
    mask_texture = NULL;

    // Efecto del interior de la bola de cristal
    glass_data = NULL;
    glass = NULL;

    // Bola de cristal
    crystal_ball_data = NULL;
    crystal_ball = NULL;

    // Sprites del fondo
    bird_data = NULL;
    for (uint32_t i = 0; i < NUMBER_OF_BIRDS; i ++) bird[i].sprite = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Fondo
    delete bg; bg = NULL;
    delete bg_data; bg_data = NULL;

    // Target del renderer
    delete target_texture; target_texture = NULL;

    // CrystalBall
    for (uint8_t i = 0; i < layers; i ++) {
        delete tbg[i];
        tbg[i] = NULL;
        delete tbg_data[i];
        tbg_data[i] = NULL;
    }

    // Captura del renderer
    delete renderer_surface; renderer_surface = NULL;
    delete renderer_target; renderer_target = NULL;

    // Mascara a aplicar
    delete mask_texture; mask_texture = NULL;

    // Efecto del interior de la bola de cristal
    delete glass; glass = NULL;
    delete glass_data; glass_data = NULL;

    // Bola de cristal
    delete crystal_ball; crystal_ball = NULL;
    delete crystal_ball_data; crystal_ball_data = NULL;

    // Sprites del fondo
    for (uint32_t i = 0; i < NUMBER_OF_BIRDS; i ++) {
        delete bird[i].sprite;
        bird[i].sprite = NULL;
    }
    delete bird_data; bird_data = NULL;



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

    // Usa el archivo empaquetado de datos si es la version release
    #if defined (MODE_RELEASE)
        if(!ngn->load->SetPackage("data.pkg", "0123456789ABCDEF")) return false;
    #endif

	// Muestra la version de la libreria en la consola
	#if defined (MODE_DEBUG)
		ngn->log->Message("Versions check:\n" + ngn->system->GetVersion());
		ngn->log->Message("N'gine started successfully!");
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

    // Capas del parallax
    tbg_data[0] = ngn->load->TiledBg("data/bg_sky.tbg");
    if (!tbg_data[0]) return false;
    tbg_data[1] = ngn->load->TiledBg("data/bg_water.tbg");
    if (!tbg_data[1]) return false;
    tbg_data[2] = ngn->load->TiledBg("data/bg_main.tbg");
    if (!tbg_data[2]) return false;

    // Mascara en RAW
    mask_texture = ngn->load->PngAsRaw("data/circle_384.png");
    if (!mask_texture) return false;

    // Bola de cristal
    crystal_ball_data = ngn->load->Texture("data/crystal_ball.png");
    if (!crystal_ball_data) return false;

    // Carga el grafico del sprite
    bird_data = ngn->load->Sprite("data/blue_bird.spr");
    if (!bird_data) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Crea el fondo
    bg = new NGN_Texture(bg_data, 0, 0);

    // Crea las capas del parallax
    for (uint8_t i = 0; i < layers; i ++) tbg[i] = new NGN_TiledBg(tbg_data[i], 0, 0);

    // Textura de destino del renderer
    target_texture = new NGN_Texture(CRYSTAL_BALL_SIZE, CRYSTAL_BALL_SIZE, 0, 0);

    // Crea el surface para la captura del renderer
    renderer_surface = new NGN_RendererSurface();

    // Crea las imagenes en RAW
    renderer_target = new NGN_RawImage();

    // Crea la bola de cristal
    crystal_ball = new NGN_Texture(crystal_ball_data, 0, 0);

    // Crea los sprites
    CreateBirds();

}



/*** Logica del programa ***/
void Demo::Update() {

    MoveBirds();
    CrystalBall();

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Fondo
    ngn->render->Texture(bg);

    // Sprites de los pajaros
    for (uint32_t i = 0 ; i < NUMBER_OF_BIRDS; i ++) ngn->render->Sprite(bird[i].sprite);

    // Bola de cristal
    ngn->render->Texture(glass);
    ngn->render->Texture(crystal_ball);

}



/*** Bola de cristal ***/
void Demo::CrystalBall() {

    // Posicion de la textura de destino
    float px = (ngn->input->mouse.x - (target_texture->width / 2.0f));
    float py = (ngn->input->mouse.y - (target_texture->height / 2.0f));
    target_texture->Position(px, py);

    // Posicion vertical
    for (uint8_t i = 0; i < layers; i ++) {
        tbg[i]->position.y = target_texture->position.y;
    }

    // Mueve la capa principal
    uint8_t main_layer = (layers - 1);
    tbg[main_layer]->position.x = (ngn->input->mouse.x * 2);

    // Calcula la posicion del resto de capas
    float main_size = (tbg[main_layer]->width - (float)SCR_WIDTH);
    float layer_size = 0.0f;
    for (uint8_t i = 0; i < (layers - 1); i ++) {
        layer_size = (tbg[i]->width - (float)SCR_WIDTH);
        tbg[i]->position.x = ((layer_size * tbg[main_layer]->position.x) / main_size);
    }

    // Selecciona la taxtura como destino del renderer
    ngn->render->RenderToTexture(target_texture);

    // Render de las capas del parallax
    for (uint8_t i = 0; i < layers; i ++) ngn->render->TiledBg(tbg[i]);

    // Captura el contenido del renderer
    ngn->image->RendererToSurface(renderer_surface);

    // Devuelve el renderer a la pantalla
    ngn->render->RenderToScreen();

    // Convierte el surface capturado en RAW, aplicandole la mascara
    ngn->image->SurfaceToRaw(renderer_surface, renderer_target, mask_texture);

    // Crea el interior de la bola de cristal a partir del resultado
    delete glass; glass = NULL;
    delete glass_data; glass_data = NULL;
    glass_data = ngn->image->ConvertRawToTextureData(renderer_target);
    glass = new NGN_Texture(glass_data, px, py);

    // Posiciona la textura del cristal de la bola en la misma posicion que el contenido
    crystal_ball->Position(px, py);

}



/*** Crea los sprites ***/
void Demo::CreateBirds() {

    // Crea los sprites que se moveran
    float scale = 0.0f;
    float w = ((float)ngn->graphics->native_w / 2.0f);
    float h = ((float)ngn->graphics->native_h / 2.0f);
    float x = 0.0f;
    float y = 0.0f;
    for (uint32_t i = 0; i < NUMBER_OF_BIRDS; i ++) {
        // Calcula la escala
        scale = (((((float)i + 1.0f) * 0.95f)/ (float)NUMBER_OF_BIRDS) + 0.05f);
        // Define la animacion
        bird[i].sprite = new NGN_Sprite(bird_data, (192 + (i * 128)), 640);
        bird[i].sprite->AddAnimation("fly", 0, 7, 0, 5);
        bird[i].sprite->SetAnimation("fly");
        bird[i].sprite->frame = (i % 8);
        // Aplica la escala
        bird[i].sprite->Scale(scale);
        // Calcula los limites
        bird[i].left = (w - (w * scale));
        bird[i].right = (w + (w * scale));
        // Calcula las posiciones
        x = (((float)(rand() % ngn->graphics->native_w)) * scale);
        y = (((float)(rand() % ngn->graphics->native_h / 2) + (ngn->graphics->native_h / 4)) * scale);
        bird[i].sprite->position.x = (x + (w - (w * scale)));
        bird[i].sprite->position.y = (y + (h - (h * scale)));
        // Velocidades
        bird[i].speed = 5.0f * scale;
        if ((rand() % 100 > 50)) {
            bird[i].speed = -bird[i].speed;
            // Dale la vuelta al sprite
            bird[i].sprite->flip_h = true;
        }
    }

}



/*** Mueve a todos los sprites ***/
void Demo::MoveBirds() {

    // Sprites en movimiento
    for (uint32_t i = 0; i < NUMBER_OF_BIRDS; i ++) {
        // Animacion
        bird[i].sprite->PlayAnimation();
        // Movimiento
        bird[i].sprite->Translate(bird[i].speed, 0.0f);
        // Limites a derecha e izquierda
        if (bird[i].sprite->position.x > bird[i].right) {
            bird[i].sprite->position.x = bird[i].right;
            bird[i].speed = -bird[i].speed;
            bird[i].sprite->flip_h = true;
        }
        if (bird[i].sprite->position.x < bird[i].left) {
            bird[i].sprite->position.x = bird[i].left;
            bird[i].speed = -bird[i].speed;
            bird[i].sprite->flip_h = false;
        }
    }

}
