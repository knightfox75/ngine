/******************************************************************************

    Ejemplo de uso de los mapas de colisiones y hitbox de sprites
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
#include <vector>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    bg_data = nullptr;
    lightmap_data = nullptr;
    cmap_data = nullptr;
    player_data = nullptr;
    cherry_data = nullptr;

    // Inicializa los punteros a objetos
    bg = nullptr;
    lightmap = nullptr;
    player = nullptr;
    cherry.clear();

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg; bg = nullptr;
    delete lightmap; lightmap = nullptr;
    delete player; player = nullptr;
    for (uint32_t i = 0; i < cherry.size(); i ++) {
        delete cherry[i]; cherry[i] = nullptr;
    }
    cherry.clear();

    // Borra los datos
    delete bg_data; bg_data = nullptr;
    delete lightmap_data; lightmap_data = nullptr;
    delete cmap_data; cmap_data = nullptr;
    delete player_data; player_data = nullptr;
    delete cherry_data; cherry_data = nullptr;

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

    // Carga los archivos
    if (!Load()) return false;

    // Crea el escenario
    CreateStage();

    // Crea al player
    player = new NGN_Sprite(player_data, 40, 40);

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



/*** Carga los archivos ***/
bool Demo::Load() {

    // Datos de los fondos
    bg_data = ngn->load->TiledBg("data/maze.tbg");
    if (bg_data == nullptr) return false;
    lightmap_data = ngn->load->TiledBg("data/maze_shadows.tbg");
    if (lightmap_data == nullptr) return false;

    // Mapa de collisiones
    cmap_data = ngn->load->CollisionMap("data/maze.map");
    if (cmap_data == nullptr) return false;

    // Sprites
    player_data = ngn->load->Sprite("data/block.spr");
    if (player_data == nullptr) return false;
    cherry_data = ngn->load->Sprite("data/cherry.spr");
    if (cherry_data == nullptr) return false;

    // Carga correcto
    return true;

}



/*** Crea el escenario ***/
void Demo::CreateStage() {

    // Crea el fondo
    bg = new NGN_TiledBg(bg_data);
    // Crea el mapa de iluminacion
    lightmap = new NGN_TiledBg(lightmap_data);
    // Añade las cerezas
    AddCherries();

}



/*** Genera las cerezas segun el mapa de colisiones ***/
void Demo::AddCherries() {

    Vector2I32 pos;     // Calculo de las posiciones
    Size2 map_size = ngn->collisions->GetMapSize(cmap_data);     // Obten el tamaño del mapa de colisiones

    for (int32_t y = 0; y < map_size.height; y += cmap_data->header.tile_size) {      // Steps del tamaño del tile del mapa de colisiones
        pos.y = y + (cmap_data->header.tile_size / 2);
        for (int32_t x = 0; x < map_size.width; x += cmap_data->header.tile_size) {
            // Punto para colocar el objeto
            pos.x = x + (cmap_data->header.tile_size / 2);
            // Si hay el color del mapa coincide, coloca un nuevo sprite
            if (ngn->collisions->GetPixel(cmap_data, pos.x, pos.y) == COLOR_MAGENTA) {
                cherry.push_back(new NGN_Sprite(cherry_data, pos.x, pos.y));
            }
        }
    }
}



/*** Update del programa ***/
void Demo::Update() {

    // Mueve al player
    Move();
    // Colisiones con las cerezas
    EatCherry();
    // Render de los elementos
    Render();

}



/*** Mueve al player ***/
void Demo::Move() {

    // Si se pulsa arriba
    if (ngn->input->key_ARROW_UP->held) {
        player->Translate(0.0f, -PLAYER_SPEED);
        CheckUp(player, cmap_data);
    }

    // Si se pulsa abajo
    if (ngn->input->key_ARROW_DOWN->held) {
        player->Translate(0.0f, PLAYER_SPEED);
        CheckDown(player, cmap_data);
    }

    // Si se pulsa izquierda
    if (ngn->input->key_ARROW_LEFT->held) {
        player->Translate(-PLAYER_SPEED, 0.0f);
        CheckLeft(player, cmap_data);
    }

    // Si se pulsa derecha
    if (ngn->input->key_ARROW_RIGHT->held) {
        player->Translate(PLAYER_SPEED, 0.0f);
        CheckRight(player, cmap_data);
    }

    /*
    std::cout <<
    ngn->collisions->GetPixel(cmap_data, (int32_t)player->position.x, (int32_t)player->position.y)
    << std::endl;
    */

}



/*** Colisiones con el decorado (arriba) ***/
bool Demo::CheckUp(NGN_Sprite* sprite, NGN_CollisionMapData* cmap) {

    // Variables
    bool r = false;
    bool c = false;
    int32_t x, y;

    // Si hay colision...
    for (int32_t i = 0; i < (int32_t)sprite->width; i ++) {
        do {
            // Por defecto, no hay colision
            r = false;
            // Calcula el punto de test
            x = ((int32_t)sprite->position.x + (i - ((int32_t)sprite->width / 2)));
            y = ((int32_t)sprite->position.y - ((int32_t)sprite->height / 2));
            // Si hay colision...
            if (ngn->collisions->GetPixel(cmap, x, y) == COLOR_GREEN) {
                // Desplaza un pixel abajo el sprite
                sprite->position.y ++;
                // Autoajuste
                sprite->position.y = (int32_t)sprite->position.y;
                // Repetimos el proceso
                r = true;
                // E indica que se ha producido una colision
                c = true;
            }
        } while (r);
        // En caso de colision, sal del check
        if (c) break;
    }

    // Devuelve el resultado
    return c;

}



/*** Colisiones con el decorado (abajo) ***/
bool Demo::CheckDown(NGN_Sprite* sprite, NGN_CollisionMapData* cmap) {

    // Variables
    bool r = false;
    bool c = false;
    int32_t x, y;

    // Si hay colision...
    for (int32_t i = 0; i < (int32_t)sprite->width; i ++) {
        do {
            // Por defecto, no hay colision
            r = false;
            // Calcula el punto de test
            x = ((int32_t)sprite->position.x + (i - ((int32_t)sprite->width / 2)));
            y = ((int32_t)sprite->position.y + ((int32_t)sprite->height / 2) - 1);
            // Si hay colision...
            if (ngn->collisions->GetPixel(cmap, x, y) == COLOR_GREEN) {
                // Desplaza un pixel arriba el sprite
                sprite->position.y --;
                // Autoajuste
                sprite->position.y = (int32_t)sprite->position.y;
                // Repetimos el proceso
                r = true;
                // E indica que se ha producido una colision
                c = true;
            }
        } while (r);
        // En caso de colision, sal del check
        if (c) break;
    }

    // Devuelve el resultado
    return c;

}



/*** Colisiones con el decorado (izquierda) ***/
bool Demo::CheckLeft(NGN_Sprite* sprite, NGN_CollisionMapData* cmap) {

    // Variables
    bool r = false;
    bool c = false;
    int32_t x, y;

    // Si hay colision...
    for (int32_t i = 0; i < (int32_t)sprite->height; i ++) {
        do {
            // Por defecto, no hay colision
            r = false;
            // Calcula el punto de test
            x = ((int32_t)sprite->position.x - ((int32_t)sprite->width / 2));
            y = ((int32_t)sprite->position.y + (i - ((int32_t)sprite->height / 2)));
            // Si hay colision...
            if (ngn->collisions->GetPixel(cmap, x, y) == COLOR_GREEN) {
                // Desplaza un pixel a la derecha el sprite
                sprite->position.x ++;
                // Autoajuste
                sprite->position.x = (int32_t)sprite->position.x;
                // Repetimos el proceso
                r = true;
                // E indica que se ha producido una colision
                c = true;
            }
        } while (r);
        // En caso de colision, sal del check
        if (c) break;
    }

    // Devuelve el resultado
    return c;

}



/*** Colisiones con el decorado (derecha) ***/
bool Demo::CheckRight(NGN_Sprite* sprite, NGN_CollisionMapData* cmap) {

    // Variables
    bool r = false;
    bool c = false;
    int32_t x, y;

    // Si hay colision...
    for (int32_t i = 0; i < (int32_t)sprite->height; i ++) {
        do {
            // Por defecto, no hay colision
            r = false;
            // Calcula el punto de test
            x = ((int32_t)sprite->position.x + ((int32_t)sprite->width / 2) - 1);
            y = ((int32_t)sprite->position.y + (i - ((int32_t)sprite->height / 2)));
            // Si hay colision...
            if (ngn->collisions->GetPixel(cmap, x, y) == COLOR_GREEN) {
                // Desplaza un pixel a la izquierda el sprite
                sprite->position.x --;
                // Autoajuste
                sprite->position.x = (int32_t)sprite->position.x;
                // Repetimos el proceso
                r = true;
                // E indica que se ha producido una colision
                c = true;
            }
        } while (r);
        // En caso de colision, sal del check
        if (c) break;
    }

    // Devuelve el resultado
    return c;

}



/*** Comete las cerezas ***/
bool Demo::EatCherry() {

    // Variables
    bool r = false;
    bool c = false;

    // Colision del player con las cerezas
    do {
        // Por defecto, no repitas
        r = false;
        // Si hay cerezas en cola
        if (cherry.size() > 0) {
            for (uint32_t i = 0; i < cherry.size(); i ++) {
                if (ngn->collisions->HitBox(player, cherry[i])) {
                    // Si hay colision, borra el elemento de la lista
                    delete cherry[i]; cherry[i] = nullptr;
                    cherry.erase(cherry.begin() + i);
                    r = true;
                    c = true;
                    break;
                }
            }
        }
    } while (r);

    // Devuelve el resultado
    return c;

}



/*** Renderiza todos los elementos de la escena ***/
void Demo::Render() {

    // Renderiza el fondo de tiles
    ngn->render->TiledBg(bg);

    // Renderiza las cerezas
    for (uint32_t i = 0; i < cherry.size(); i ++) ngn->render->Sprite(cherry[i]);

    // Renderiza el player
    ngn->render->Sprite(player);

    // Renderiza el lightmap
    ngn->render->TiledBg(lightmap);

}
