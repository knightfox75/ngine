/******************************************************************************

    Ejemplo del uso de multiples colisionadores en un Sprite
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.0.0-stable o superior

    Requiere GCC 7.3.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.12) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.0.15) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.1) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20190615)
    (c) 2005 - 2019 by Lode Vandevenne
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

    // Inicia los punteros
    bg_img = NULL;
    bg = NULL;
    aim_img = NULL;
    aim = NULL;
    colliders_img = NULL;
    colliders = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Elimina los datos
    delete bg; bg = NULL;
    delete bg_img; bg_img = NULL;
    delete aim; aim = NULL;
    delete colliders; colliders = NULL;
    delete aim_img; aim_img = NULL;
    delete colliders_img; colliders_img = NULL;

}



/*** Inicializa N'GINE ***/
bool Demo::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Crea la ventana con la resolucion nativa
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, NGN_SCR_WINDOW, BILINEAR_FILTER, VSYNC)) return false;
    ngn->graphics->Update();

    // visibilidad del cursor del raton
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
        std::cout << ngn->system->GetVersion() << std::endl;
    #endif

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Demo::Start() {

    // Carga de archivos
    if (!Load()) return false;

    // Crea la escena
    Create();

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

        // Logica del programa
        Logic();

        // Render de los elementos
        Render();

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



/*** Logica del programa ***/
void Demo::Logic() {

    // El sprite AIM sigue el puntero del raton
    aim->Position(ngn->input->mouse.x, ngn->input->mouse.y);

    // Movimiento del collider
    colliders->Translate(translation);
    if (colliders->position.x < limits.left) {
        translation.x = speed;
        colliders->position.x = limits.left;
    }
    if (colliders->position.x > limits.right) {
        translation.x = -speed;
        colliders->position.x = limits.right;
    }
    if (colliders->position.y < limits.top) {
        translation.y = speed;
        colliders->position.y = limits.top;
    }
    if (colliders->position.y > limits.bottom) {
        translation.y = -speed;
        colliders->position.y = limits.bottom;
    }

    // Cambia la opacidad del collider segun si hay o no colision
    if (ngn->collisions->HitBox(aim, colliders)) {
        colliders->alpha = 127;
    } else {
        colliders->alpha = 255;
    }

}



/*** Renderizado ***/
void Demo::Render() {

    // Fondo
    ngn->render->Texture(bg);

    // Sprites
    ngn->render->Sprite(colliders);
    ngn->render->Sprite(aim);

}



/*** Carga los archivos ***/
bool Demo::Load() {

    // Carga el fondo
    bg_img = ngn->load->Texture("data/bg_sample.png");
    if (!bg_img) return false;

    // Carga los sprites
    aim_img = ngn->load->Sprite("data/aim.spr");
    if (!aim_img) return false;
    colliders_img = ngn->load->Sprite("data/colliders_sample.spr");
    if (!colliders_img) return false;

    // Carga correcta
    return true;

}



/*** Crea la escena ***/
void Demo::Create() {

    // Fondo
    bg = new NGN_Texture(bg_img, 0, 0);

    // Crea los sprites
    aim = new NGN_Sprite(aim_img);
    colliders = new NGN_Sprite(colliders_img, (SCR_WIDTH / 2), (SCR_HEIGHT / 2));

    // Ajusta la caja pricipal
    aim->box.width = 4.0f;
    aim->box.height = 4.0f;
    colliders->box.width = 64.0f;
    colliders->box.height = 64.0f;

    // Crea los colliders adicionales
    colliders->AddCollider("bottom", -16.0f, 80.0f, 32.0f, 96.0f);
    colliders->AddCollider("right", 80.0f, 16.0f, 32.0f, 32.0f);
    colliders->AddCollider("top", 16.0f, -80.0f, 32.0f, 96.0f);
    colliders->AddCollider("left", -64.0f, -80.0f, 64.0f, 32.0f);

    // Parametros iniciales
    limits.top = colliders->height / 2.0f;
    limits.bottom = SCR_HEIGHT - limits.top;
    limits.left = colliders->width / 2.0f;
    limits.right = SCR_WIDTH - limits.left;
    translation.x = speed;
    translation.y = speed;

}
