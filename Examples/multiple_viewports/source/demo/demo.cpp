/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Camara virtual 2D

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2019 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 0.6.2-a o superior

    Requiere GCC 7.3.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.8) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG
    (c) 2005 - 2016 by Lode Vandevenne
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

/*** Defines ***/
static const std::string WINDOW_TITLE = "N'GINE Multiple viewports.";
static const uint32_t SCR_WIDTH = 1280;
static const uint32_t SCR_HEIGHT = 720;

static const float MOVE_SPEED = 5.0f;
static const float BIRD_MIN_SPEED = 1.0f;
static const float BIRD_MAX_SPEED = 3.0f;



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros a los datos
    bg0_data = NULL;
    bg1_data = NULL;
    bg2_data = NULL;
    aim_data = NULL;
    bird_data = NULL;

    // Inicializa los punteros a los objetos
    bg0 = NULL;
    bg1 = NULL;
    bg2 = NULL;
    aim = NULL;
    bird.clear();

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg0; bg0 = NULL;
    delete bg1; bg1 = NULL;
    delete bg2; bg2 = NULL;
    delete aim; aim = NULL;
    for (uint32_t i = 0; i < bird.size(); i ++) {
        delete bird[i].sprite;
        bird[i].sprite = NULL;
    }

    // Borra los datos
    delete bg0_data; bg0_data = NULL;
    delete bg1_data; bg1_data = NULL;
    delete bg2_data; bg2_data = NULL;
    delete aim_data; aim_data = NULL;
    delete bird_data; bird_data = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Selecciona el modo grafico
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT)) return false;

    // Esconde el cursor del raton
    ngn->graphics->ShowMouse(false);

    // Debug?
    ngn->system->fps_counter = false;

    // Fuerza la actualizacion de la pantalla
    ngn->graphics->Update();

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Caga de archivos
    if (!Load()) return false;

    // Crea la escena
    CreateStage();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
void Demo::Run() {

    // Control del loop
    bool loop = true;

    // Loop principal
    while (loop) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();             // Actualiza los eventos

        // Actualizacion del programa
        Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();

        // Control del bucle principal
        loop &= !ngn->system->quit;             // Si se pulsa la [X] de la ventana
        loop &= !ngn->input->key_ESC->down;     // O se pulsa la tecla [ESC] sal del bucle de ejecucion

    }

}



/*** Carga los archivos necesarios ***/
bool Demo::Load() {

    // Carga los datos de los fondos
    bg0_data = ngn->load->TiledBg("data/bg0.tbg");
    if (bg0_data == NULL) return false;
    bg1_data = ngn->load->TiledBg("data/bg1.tbg");
    if (bg1_data == NULL) return false;
    bg2_data = ngn->load->TiledBg("data/bg2.tbg");
    if (bg2_data == NULL) return false;

    // Carga los sprites
    bird_data = ngn->load->Sprite("data/bird_small.spr");
    if (bird_data == NULL) return false;
    aim_data = ngn->load->Sprite("data/aim.spr");
    if (aim_data == NULL) return false;

    // Carga correcta
    return true;

}



/*** Crea la escena ***/
void Demo::CreateStage() {

    // Crea los fondos tiles
    bg0 = new NGN_TiledBg(bg0_data);
    bg1 = new NGN_TiledBg(bg1_data);
    bg2 = new NGN_TiledBg(bg2_data);

    // Crea la mirilla
    aim = new NGN_Sprite(aim_data, (bg2->width / 2.0f), (bg2->height * 0.75f));

    // Configura el numero de capas de la camara virtual 2d
    ngn->camera->CreateLayers(3);
    // Parametros de la camara
    ngn->camera->Setup(bg2->width, bg2->height, aim);

    // Añade los fondos a la camara para su gestion
    ngn->camera->PushBackground(0, bg0);
    ngn->camera->PushBackground(1, bg1);
    ngn->camera->PushBackground(2, bg2);

    // Genera los pajaros de la capa 0 y registralos en la camara(fondo)
    CreateBirds(0, 20, 0.33);
    CreateBirds(1, 15, 0.66f);
    CreateBirds(2, 10, 1.0f);

    // Añade el sprite de la mirilla a la camara para su gestion
    ngn->camera->PushSprite(2, aim);

    // Define los Viewports
    ngn->graphics->OpenViewport(0, 0, 0, 1280, 360);
    ngn->graphics->OpenViewport(1, 640, 360, 640, 360);
    ngn->graphics->OpenViewport(2, 0, 360, 640, 360, 1280, 720);
    ngn->graphics->OpenViewport(3, 480, 240, 320, 240, 160, 120);

}



/*** Crea los pajaros en la capa solicitada ***/
void Demo::CreateBirds(uint32_t ly, uint32_t num, float scale) {

    // Objeto temporal para crear pajaros
    bird_type b;
    b.sprite = NULL;

    // Calculos de la posicion
    int32_t x, y;

    // Calculos de la velocidad
    int32_t _min = (int32_t)(BIRD_MIN_SPEED * 100.0f);
    int32_t _max = (int32_t)(BIRD_MAX_SPEED * 100.0f);


    for (uint32_t i = 0; i < num; i ++) {
        // Crea un nuevo sprite
        b.sprite = new NGN_Sprite(bird_data);
        // Calcula el nuevo tamaño
        b.sprite->width *= scale;
        b.sprite->height *= scale;
        // Calcula los limites del desplazamiento
        b.left = -b.sprite->width;
        b.right = (ngn->camera->layer[ly].sprite_layer.width + b.sprite->width);
        // Calcula la posicion
        x = (rand() % ngn->camera->layer[ly].sprite_layer.width);
        y = ((rand() % (int32_t)(ngn->camera->layer[ly].sprite_layer.height * 0.80f)) + (ngn->camera->layer[ly].sprite_layer.height * 0.05f));
        b.sprite->Position(x, y);
        // Calcula la velocidad
        b.speed = ((float)((rand() % (_max - _min)) + _min) / 100.0f) * scale;
        if ((rand() % 100 > 50)) {
            b.speed = -b.speed;
            b.sprite->flip_h = true;
        }
        // Define la animacion
        b.sprite->AddAnimation("fly", 0, 7, 0, 5);
        b.sprite->SetAnimation("fly");
        b.sprite->frame = (rand() % 8);
        // Registra el sprite en la camara
        ngn->camera->PushSprite(ly, b.sprite);
        // Y metelo en la cola de gestion de pajaros
        bird.push_back(b);
    }

    // Limpia el puntero temporal
    b.sprite = NULL;

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Mueve a los pajaros
    MoveBirds();

    // Mueve la mirilla
    MoveAim();

    // Gestor de los viewports
    ViewportManager();

    // Render de los viewports
    ngn->render->Viewports();

}



/*** Mueve a los pajaros ***/
void Demo::MoveBirds() {

    for (uint32_t i = 0; i < bird.size(); i ++) {
        bird[i].sprite->Translate(bird[i].speed, 0.0f);
        if (bird[i].sprite->position.x < bird[i].left) {
            bird[i].sprite->position.x = bird[i].left;
            bird[i].speed = -bird[i].speed;
            bird[i].sprite->flip_h = false;
        }
        if (bird[i].sprite->position.x > bird[i].right) {
            bird[i].sprite->position.x = bird[i].right;
            bird[i].speed = -bird[i].speed;
            bird[i].sprite->flip_h = true;
        }
    }

}



/*** Mueve el punto de mira ***/
void Demo::MoveAim() {

    // Vector de movimiento
    Vector2 speed;
    speed.x = speed.y = 0.0f;

    // Limites
    float top = (aim->height / 2.0f);
    float left = (aim->width / 2.0f);
    float bottom = (bg2->height - top);
    float right = (bg2->width - left);

    // Control por teclado
    if (ngn->input->key_ARROW_UP->held) speed.y = -MOVE_SPEED;
    if (ngn->input->key_ARROW_DOWN->held) speed.y = MOVE_SPEED;
    if (ngn->input->key_ARROW_LEFT->held) speed.x = -MOVE_SPEED;
    if (ngn->input->key_ARROW_RIGHT->held) speed.x = MOVE_SPEED;

    // Mueve la mirilla
    aim->Translate(speed.x, speed.y);

    // Limites de la mirilla
    if (aim->position.x < left) aim->position.x = left;
    if (aim->position.x > right) aim->position.x = right;
    if (aim->position.y < top) aim->position.y = top;
    if (aim->position.y > bottom) aim->position.y = bottom;

}



/*** Gestor de viewports ***/
void Demo::ViewportManager() {

    // Posicion del Viewport central con el mouse
    Vector2I32 position;
    position.x = ngn->input->mouse.x - (ngn->graphics->viewport_list[3].w / 2);
    position.y = ngn->input->mouse.y - (ngn->graphics->viewport_list[3].h / 2);
    ngn->graphics->ViewportPosition(3, position);

    // Actualiza la camara 2D en cada viewport
    ngn->graphics->SelectViewport(0);
    ngn->camera->Update();
    ngn->graphics->SelectViewport(1);
    ngn->camera->Update();
    ngn->graphics->SelectViewport(2);
    ngn->camera->Update();
    ngn->graphics->SelectViewport(3);
    ngn->camera->Update();

}
