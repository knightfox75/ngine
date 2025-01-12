/******************************************************************************

    N'gine Lib for C++
    Ejemplo del uso de repositorios y archivos empaquetados

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

    // Punteros a objetos
    background.texture = NULL;
    island.texture = NULL;
    clouds.tilemap = NULL;
    boat.sprite = NULL;
    for (uint8_t k = 0; k < BIRDS; k ++) bird.sprite[k] = NULL;
    textbox.layer = NULL;
    sound.seaside = NULL;
    sound.horn = NULL;

    // Datos iniciales
    textbox.textlines.clear();
    sound.timer = 0;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Deten el sonido
    ngn->sound->CloseMusic(sound.seaside);
    sound.seaside = NULL;

    // Anula las referencias
    sound.horn = NULL;

    // Elimina los datos adicionales
    textbox.textlines.clear();

    // Elimina los objetos creados
    delete background.texture; background.texture = NULL;
    delete island.texture; island.texture = NULL;
    delete clouds.tilemap; clouds.tilemap = NULL;
    delete boat.sprite; boat.sprite = NULL;
    for (uint8_t k = 0; k < BIRDS; k ++) {
        delete bird.sprite[k];
        bird.sprite[k] = NULL;
    }
    delete textbox.layer; textbox.layer = NULL;

    // Elimina el repositorio y los datos que contiene
    ngn->resources->RemoveRepository("demo");

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

    // Define el repositorio a usar
    ngn->resources->AddRepository("demo");

    // Usa el archivo empaquetado de datos si es la version release
    #if defined (MODE_RELEASE)
        if(!ngn->load->SetPackage("assets.pkg", "set_your_key_here")) return false;
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

    // Carga los recursos al repositorio indicado
    if (!ngn->resources->Load("demo", "assets/resources.txt")) return false;

    // Prepara el stream de sonido
    sound.seaside = ngn->sound->OpenMusic("assets/sounds/seaside.ogg", false);
    if (!sound.seaside) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Crea el fondo
    background.texture = new NGN_Texture("demo", "background", background.position.x, background.position.y);

    // Crea la isla
    island.texture = new NGN_Texture("demo", "land", island.position.x, island.position.y);

    // Crea las nubes
    clouds.tilemap = new NGN_TiledBg("demo", "clouds", clouds.position.x, clouds.position.y);

    // Crea el bote
    boat.sprite = new NGN_Sprite("demo", "boat", boat.position.x, boat.position.y);

    // Cache de los datos de sprite para los pajaros
    NGN_SpriteData* bird_data = ngn->resources->GetSprite("demo", "bird");
    // Crea los sprites de los pajaros
    for (uint8_t k = 0; k < BIRDS; k ++) {
        bird.sprite[k] = new NGN_Sprite(bird_data);
        bird.sprite[k]->position.x = (float)(std::rand() % SCR_WIDTH);
        bird.sprite[k]->position.y = (float)((std::rand() % (bird.bottom - bird.top)) + bird.top);
        bird.sprite[k]->AddAnimation("fly", 0, 7, 0, 10);
        bird.sprite[k]->SetAnimation("fly");
        bird.sprite[k]->frame = (std::rand() % 8);
    }
    bird_data = NULL;

    // Crea la caja de texto
    textbox.layer = new NGN_TextLayer("demo", "chary_64", "textbg", textbox.position.x, textbox.position.y);
    textbox.layer->Padding(textbox.padding);

    // Asigna el dialogo a mostrar
    textbox.textlines = ngn->resources->GetTxt("demo", "dialog");

    // Imprime el texto en la caja
    textbox.layer->Cls();
    for (uint32_t k = 0; k < textbox.lines; k ++) {
        textbox.layer->Print(textbox.textlines[k]);
        if (k < (textbox.lines - 1)) textbox.layer->Print("\n");
    }

    // Cache de los efectos de sonidos
    sound.horn = ngn->resources->GetSfx("demo", "horn");

    // Reproduce el stream de sonido
    ngn->sound->PlayMusic(sound.seaside);

}



/*** Logica del programa ***/
void Demo::Update() {

    // Scroll de las nubes
    clouds.tilemap->Translate(clouds.speed, 0.0f);
    if (clouds.tilemap->position.x > clouds.loop) clouds.tilemap->position.x -= clouds.loop;

    // Movimiento del bote
    boat.sprite->Translate(boat.speed, 0.0f);
    if (boat.sprite->position.x > ((float)SCR_WIDTH + boat.sprite->width)) boat.sprite->position.x = -boat.sprite->width;

    // Movimiento de los pajaros
    for (uint8_t k = 0; k < BIRDS; k ++) {
        bird.sprite[k]->PlayAnimation();
        bird.sprite[k]->Translate(bird.speed, 0.0f);
        if (bird.sprite[k]->position.x > ((float)SCR_WIDTH + bird.sprite[k]->width)) {
            bird.sprite[k]->position.x = -bird.sprite[k]->width;
            bird.sprite[k]->position.y = (float)((std::rand() % (bird.bottom - bird.top)) + bird.top);
        }
    }

    // Sirena del barco
    sound.timer ++;
    if (sound.timer >= sound.horn_ticks) {
        sound.timer = 0;
        ngn->sound->PlaySfx(sound.horn);
    }

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Fondo
    ngn->render->Texture(background.texture);
    // Nubes
    ngn->render->TiledBg(clouds.tilemap);
    // Pajaros
    for (uint8_t k = 0; k < BIRDS; k ++) ngn->render->Sprite(bird.sprite[k]);
    // Bote
    ngn->render->Sprite(boat.sprite);
    // Isla
    ngn->render->Texture(island.texture);
    // Caja de texto
    ngn->render->TextLayer(textbox.layer);

}
