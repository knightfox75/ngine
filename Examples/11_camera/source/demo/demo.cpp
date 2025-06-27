/******************************************************************************

    Ejemplo del uso de la camara 2D
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.19.0-stable o superior
	(c) 2016 - 2025 by Cesar Rincon "NightFox"
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

    // Inicializa los punteros a los datos
    bg0_data = nullptr;
    bg1_data = nullptr;
    bg2_data = nullptr;
    aim_data = nullptr;
    bird_data = nullptr;

    // Inicializa los punteros a los objetos
    bg0 = nullptr;
    bg1 = nullptr;
    bg2 = nullptr;
    aim = nullptr;
    bird.clear();

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg0; bg0 = nullptr;
    delete bg1; bg1 = nullptr;
    delete bg2; bg2 = nullptr;
    delete aim; aim = nullptr;
    for (uint32_t i = 0; i < bird.size(); i ++) {
        delete bird[i].sprite;
        bird[i].sprite = nullptr;
    }

    // Borra los datos
    delete bg0_data; bg0_data = nullptr;
    delete bg1_data; bg1_data = nullptr;
    delete bg2_data; bg2_data = nullptr;
    delete aim_data; aim_data = nullptr;
    delete bird_data; bird_data = nullptr;

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

    // Caga de archivos
    if (!Load()) return false;

    // Crea la escena
    CreateStage();

    // Procesos correctos
    return true;

}



/*** Ejecucion del programa ***/
int8_t Demo::Run() {

    // Control del loop
    int8_t loop = -1;

    while (loop < 0) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos

        // Actualizacion del programa
        Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();

        // Control del bucle principal
        if (ngn->system->quit) {    // Si se pulsa la [X] de la ventana
            loop = 0;
        } else if (ngn->input->key_ESC->down || ngn->input->controller[0].button[10].down) {    // Si se pulsa la tecla [ESC] O se pulsa el boton XBOX
            loop = 1;
        }

    }

    // Devuelve el resultado
    return loop;

}



/*** Carga los archivos necesarios ***/
bool Demo::Load() {

    // Carga los datos de los fondos
    bg0_data = ngn->load->TiledBg("data/bg0.tbg");
    if (bg0_data == nullptr) return false;
    bg1_data = ngn->load->TiledBg("data/bg1.tbg");
    if (bg1_data == nullptr) return false;
    bg2_data = ngn->load->TiledBg("data/bg2.tbg");
    if (bg2_data == nullptr) return false;

    // Carga los sprites
    bird_data = ngn->load->Sprite("data/bird_small.spr");
    if (bird_data == nullptr) return false;
    aim_data = ngn->load->Sprite("data/aim.spr");
    if (aim_data == nullptr) return false;

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

}



/*** Crea los pajaros en la capa solicitada ***/
void Demo::CreateBirds(uint32_t ly, uint32_t num, float scale) {

    // Objeto temporal para crear pajaros
    bird_type b;
    b.sprite = nullptr;

    // Calculos de la posicion
    int32_t x, y;
    Size2I32 layer_size = ngn->camera->GetLayerSize(ly);

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
        b.right = (layer_size.width + b.sprite->width);
        // Calcula la posicion
        x = (rand() % layer_size.width);
        y = ((rand() % (int32_t)(layer_size.height * 0.80f)) + (layer_size.height * 0.05f));
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
    b.sprite = nullptr;

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Mueve a los pajaros
    MoveBirds();
    // Mueve la mirilla
    MoveAim();

    // Actualiza la camara 2D
    ngn->camera->Update();

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
