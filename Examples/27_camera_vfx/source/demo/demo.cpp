/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso los efectos visuales de la camara

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.19.0-stable o superior
	(c) 2016 - 2025 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
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

    // Prepara los vectores de datos
    bg_data.clear();
    bg_data.assign(BG_LAYERS, nullptr);
    bg_layer.clear();
    bg_layer.assign(BG_LAYERS, nullptr);
    bird_sprite.clear();
    std::vector<NGN_Sprite*> empty_list;
    empty_list.clear();
    bird_sprite.assign(BG_LAYERS, empty_list);

    // Datos adicionales
    shake_intensity = 0.0f;
    effect_amount = 0.0f;
    COLOR_DELTA.r = (DAY_COLOR.r - NIGHT_COLOR.r);
    COLOR_DELTA.g = (DAY_COLOR.g - NIGHT_COLOR.g);
    COLOR_DELTA.b = (DAY_COLOR.b - NIGHT_COLOR.b);
    COLOR_DELTA.a = (DAY_COLOR.a - NIGHT_COLOR.a);

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Elimina los datos de la camara
    ngn->camera->Reset();

    // Elimina los sprites
    for (uint8_t l = 0; l < bird_sprite.size(); l ++) {
        for (uint32_t s = 0; s < bird_sprite[l].size(); s ++) {
            delete bird_sprite[l][s];
        }
    }

    // Elimina las capas del fondo
    for (uint8_t k = 0; k < bg_layer.size(); k ++) delete bg_layer[k];
    bg_layer.clear();

    // Elimina las imagenes de los fondos
    for (uint8_t k = 0; k < bg_data.size(); k ++) delete bg_data[k];
    bg_data.clear();

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
uint32_t Demo::Run() {

    // Control del loop
    uint32_t loop = FLAG_ZERO;

    // Mientras loop valga cero, repite
    while (!loop) {

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
        ) loop |= FLAG_EXIT;

    }

    // Devuelve el resultado
    return loop;

}



/*** Carga de los archivos necesarios ***/
bool Demo::Load() {

    // Carga las imagenes
    bg_data[0] = ngn->load->Texture("data/jungle_bg0.png");
    bg_data[1] = ngn->load->Texture("data/jungle_bg1.png");
    bg_data[2] = ngn->load->Texture("data/jungle_bg2.png");
    bg_data[3] = ngn->load->Texture("data/jungle_bg3.png");
    for (uint8_t k = 0; k < bg_layer.size(); k ++) if (!bg_data[k]) return false;

    // Carga el sprite del pajaro
    blue_bird_data = ngn->load->Sprite("data/blue_bird_24x24.spr");
    if (!blue_bird_data) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Crea las capas del fondo
    for (uint8_t k = 0; k < bg_layer.size(); k ++) bg_layer[k] = new NGN_Texture(bg_data[k]);

    // Crea la camara
    ngn->camera->Reset();
    ngn->camera->CreateLayers(BG_LAYERS);
    ngn->camera->Setup(WORLD_WIDTH, WORLD_HEIGHT);

    // Registra las capas del fondo en la camara
    uint32_t width = 0;
    for (uint8_t k = 0; k < bg_layer.size(); k ++) {
        width = std::floor((float)WORLD_WIDTH * PARALLAX_FACTOR[k]);
        ngn->camera->PushVirtualBg(k, bg_layer[k], width, WORLD_HEIGHT, LAYER_LOOP, 0);
    }

    // Posicion inicial de la camara
    camera_position = {(float)(WORLD_WIDTH / 2), (float)(WORLD_HEIGHT / 2)};
    ngn->camera->LookAt(camera_position.x, camera_position.y);


    /// Crea a los pajaros

    // Sprite de referencia
    NGN_Sprite* ref_sprite = new NGN_Sprite(blue_bird_data);
    ref_sprite->AddAnimation("fly", 0, 7, 0, 5);
    ref_sprite->SetAnimation("fly");

    // Datos de entorno
    float screen_div = ((float)SCR_HEIGHT / 5.0f);
    float centre = (screen_div * 2.0f);
    float top = 0.0f, bottom = 0.0f;

    // Sprite temporal
    NGN_Sprite* spr = nullptr;

    // Variables de apoyo
    float x = 0.0f, y = 0.0f;
    float scale = 1.0f;

    // Crea a los pajaros
    for (uint8_t layer = 0; layer < BG_LAYERS; layer ++) {
        // Parametros de entorno segun la capa
        top = (centre - (screen_div * PARALLAX_FACTOR[layer]));
        bottom = (centre + (screen_div * PARALLAX_FACTOR[layer]));
        scale = ((float)layer + 1.0f);
        for (uint32_t bird = 0; bird < BIRDS_PER_LAYER[layer]; bird ++) {
            // Crea un clon del sprite
            spr = ngn->graphics->CloneSprite(ref_sprite);
            // Frame aleatorio
            spr->frame = ngn->math->RandomInt(0, 7);
            // Asignale una posicion aleatoria
            x = ngn->math->RandomFloat(WORLD_LEFT, WORLD_RIGHT);
            y = ngn->math->RandomFloat(top, bottom);
            spr->Position(x, y);
            // Aplicale la escala
            spr->Scale(scale);
            // Registralo en la camara
            ngn->camera->PushSprite(layer, spr);
            // Almacenalo en el vector
            bird_sprite[layer].push_back(spr);
        }
    }

    // Libera los datos inecesarios
    spr = nullptr;
    delete ref_sprite; ref_sprite = nullptr;

}



/*** Logica del programa ***/
void Demo::Update() {

    // Mueve a los pajaros
    for (uint8_t l = 0; l < bird_sprite.size(); l ++) {
        for (uint32_t s = 0; s < bird_sprite[l].size(); s ++) {
            // Aplica el vector de translacion
            bird_sprite[l][s]->Translate(BIRD_SPEED[l], 0.0f);
            // Limites (efecto loop)
            if (bird_sprite[l][s]->position.x > (WORLD_WIDTH + bird_sprite[l][s]->width)) {
                bird_sprite[l][s]->position.x = -bird_sprite[l][s]->width;
            }
        }
    }

    // Control de movimiento
    Vector2 direction = Vector2::Zero();
    if (ngn->input->key_ARROW_LEFT->held) direction.x -= 1.0f;
    if (ngn->input->key_ARROW_RIGHT->held) direction.x += 1.0f;
    camera_position += (direction * CAM_SPEED);
    if (camera_position.x < WORLD_LEFT) camera_position.x = WORLD_LEFT;
    if (camera_position.x > WORLD_RIGHT) camera_position.x = WORLD_RIGHT;
    ngn->camera->LookAt(camera_position.x, camera_position.y);

    // Aplica el efecto "shake"
    if (ngn->input->key_SPACE->held) {
        if (shake_intensity < SHAKE_MIN_INTENSITY) {
            shake_intensity = SHAKE_MIN_INTENSITY;
        } else {
            shake_intensity *= SHAKE_LERP;
            if (shake_intensity > SHAKE_MAX_INTENSITY) shake_intensity = SHAKE_MAX_INTENSITY;
        }
    } else {
        if (shake_intensity < SHAKE_MIN_INTENSITY) {
            shake_intensity = 0.0f;
        } else {
            shake_intensity /= SHAKE_LERP;
        }
    }
    if (shake_intensity > 0.0f) {
        ngn->camera->Shake(shake_intensity, SHAKE_FREQ, true);
    } else {
        ngn->camera->Shake(0.0f, 0.0f, true);
    }

    // Aplica el efecto de "tinte"
    if (ngn->input->key_ARROW_DOWN->held) {
        effect_amount += FADE_SPEED;
        if (effect_amount > 100.0f) effect_amount = 100.0f;
    }
    if (ngn->input->key_ARROW_UP->held) {
        effect_amount -= FADE_SPEED;
        if (effect_amount < 0.0f) effect_amount = 0.0f;
    }
    for (uint8_t k = 0; k < BG_LAYERS; k ++) {
        float deep = (1.0f - ((float)k * 0.10f));    // En la operacion "(float)k * n", el valor de "n" no puede ser mayor de "1 / BG_LAYERS"
        float color_mod = ((effect_amount / 100.0f) * deep);
        uint8_t r = DAY_COLOR.r - std::floor((float)(COLOR_DELTA.r) * color_mod);
        uint8_t g = DAY_COLOR.g - std::floor((float)(COLOR_DELTA.g) * color_mod);
        uint8_t b = DAY_COLOR.b - std::floor((float)(COLOR_DELTA.b) * color_mod);
        ngn->camera->SetLayerTintColor(k, r, g, b);
    }

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Elementos de la camara
    ngn->camera->Update();

}
