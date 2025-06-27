/******************************************************************************

    Ejemplo del uso de multiples colisionadores en un Sprite
    Archivo de Demo (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 1.19.0-stable o superior

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.5) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.22.0) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (3.0.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20190615)
    (c) 2005 - 2019 by Lode Vandevenne
    http://lodev.org/lodepng/


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
    bg_img = nullptr;
    bg = nullptr;
    aim_img = nullptr;
    aim = nullptr;
    colliders_img = nullptr;
    colliders = nullptr;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Elimina los datos
    delete bg; bg = nullptr;
    delete bg_img; bg_img = nullptr;
    delete aim; aim = nullptr;
    delete colliders; colliders = nullptr;
    delete aim_img; aim_img = nullptr;
    delete colliders_img; colliders_img = nullptr;

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
