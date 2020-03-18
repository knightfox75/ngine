/******************************************************************************

    Ejemplo del uso de Sprites
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 0.11.0-a o superior

    Requiere GCC 7.3.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.10) - 64-bits
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
#include <ctime>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    bg_data = NULL;
    sprite_data = NULL;

    // Inicializa las listas de sprites
    sprite.clear();
    sprite.resize(8);   // <--- Numero de frames de la animacion, para mostrarlos todos
    for (uint32_t i = 0; i < sprite.capacity(); i ++) sprite[i] = NULL;

    bird.clear();
    bird.resize(NUMBER_OF_SPRITES);
    for (uint32_t i = 0; i < bird.capacity(); i ++) bird[i].sprite = NULL;

    // Inicializa los punteros de objetos
    bg = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los sprites
    for (uint32_t i = 0; i < sprite.capacity(); i ++) {
        delete sprite[i];
        sprite[i] = NULL;
    }
    for (uint32_t i = 0; i < bird.capacity(); i ++) {
        delete bird[i].sprite;
        bird[i].sprite = NULL;
    }
    bird.clear();

    // Borra los objetos
    delete bg; bg = NULL;

    // Borra los datos
    delete bg_data; bg_data = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Selecciona el modo grafico
    #if defined (OS_WINDOWS)
        if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, SCR_MODE_WINDOWS, BILINEAR_FILTER, VSYNC)) return false;
    #elif defined (OS_LINUX)
        if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    #endif

    // Esconde el cursor del raton
    ngn->graphics->ShowMouse(SHOW_MOUSE);

    // Contador de FPS
    ngn->system->fps_counter = FPS_COUNTER;

    // Fuerza la actualizacion de la pantalla
    ngn->graphics->Update();
    #if defined (OS_LINUX)
        ngn->graphics->full_screen = SCR_MODE_LINUX;
    #endif

    // Muestra la version de la libreria en la consola
    #if defined (MODE_DEBUG)
        std::cout << ngn->system->GetVersion() << std::endl;
    #endif

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Cambia la semilla del random
    srand(time(NULL));

    // Carga los archivos
    if (!Load()) return false;

    // Crea la escena
    CreateStage();

    // Crea los sprites
    CreateSprites();

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

        // Actualiza el programa
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



/*** Carga todos los archivos necesarios ***/
bool Demo::Load() {

    // Carga la imagen para el fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (bg_data == NULL) return false;

    // Carga el grafico del sprite
    sprite_data = ngn->load->Sprite("data/blue_bird.spr");
    if (sprite_data == NULL) return false;

    // Carga correcta
    return true;

}



/*** Crea el escenario ***/
void Demo::CreateStage() {

    // Crea la textura de fondo
    bg = new NGN_Texture(bg_data, 0, 0);


}



/*** Crea los sprites ***/
void Demo::CreateSprites() {

    // Crea los sprites estaticos de la parte superior
    for (uint32_t i = 0; i < sprite.capacity(); i ++) {
        sprite[i] = new NGN_Sprite(sprite_data, (192 + (i * 128)), 128);
        sprite[i]->AddAnimation("fly", 0, 7, 0, 5);
        sprite[i]->SetAnimation("fly");
        sprite[i]->frame = i % 8;     // Empieza en un frame aleatorio
    }

    // Crea los sprites que se moveran
    float scale = 0.0f;
    float w = ((float)ngn->graphics->native_w / 2.0f);
    float h = ((float)ngn->graphics->native_h / 2.0f);
    float x = 0.0f;
    float y = 0.0f;
    for (uint32_t i = 0; i < bird.capacity(); i ++) {
        // Calcula la escala
        scale = (((((float)i + 1.0f) * 0.95f)/ (float)NUMBER_OF_SPRITES) + 0.05f);
        // Define la animacion
        bird[i].sprite = new NGN_Sprite(sprite_data, (192 + (i * 128)), 640);
        bird[i].sprite->AddAnimation("fly", 0, 7, 0, 5);
        bird[i].sprite->SetAnimation("fly");
        bird[i].sprite->frame = i % 8;
        // Aplica la escala
        bird[i].sprite->Scale(scale);
        // Calcula los limites
        bird[i].left = w - (w * scale);
        bird[i].right = w + (w * scale);
        // Calcula las posiciones
        x = ((float)(rand() % ngn->graphics->native_w)) * scale;
        y = ((float)(rand() % ngn->graphics->native_h / 2) + (ngn->graphics->native_h / 4)) * scale;
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



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Mueve los sprites
    MoveSprites();
    // Renderiza la escena
    Render();

}



/*** Mueve a todos los sprites ***/
void Demo::MoveSprites() {

    // Sprites estaticos en la parte superior
    for (uint32_t i = 0; i < sprite.capacity(); i ++) {
        sprite[i]->PlayAnimation();
    }

    // Sprites en movimiento
    for (uint32_t i = 0; i < bird.capacity(); i ++) {
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



/*** Renderiza la escena ***/
void Demo::Render() {

    // Renderiza el fondo
    ngn->render->Texture(bg);

    // Renderiza todos los sprites
    for (uint32_t i = 0 ; i < bird.capacity(); i ++) ngn->render->Sprite(bird[i].sprite);
    for (uint32_t i = 0; i < sprite.capacity(); i ++) ngn->render->Sprite(sprite[i]);

}
