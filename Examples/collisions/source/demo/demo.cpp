/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Colisiones

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 0.6.0-a o superior

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
#define WINDOW_TITLE "N'GINE Collisions example."
#define SCR_WIDTH   1280
#define SCR_HEIGHT  720

#define PLAYER_SPEED 4.0f
#define TILE_SIZE 16
#define HALF_TILE (TILE_SIZE / 2)



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    bg_data = NULL;
    lightmap_data = NULL;
    cmap_data = NULL;
    player_data = NULL;
    cherry_data = NULL;

    // Inicializa los punteros a objetos
    bg = NULL;
    lightmap = NULL;
    player = NULL;
    cherry.clear();

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg; bg = NULL;
    delete lightmap; lightmap = NULL;
    delete player; player = NULL;
    for (uint32_t i = 0; i < cherry.size(); i ++) {
        delete cherry[i]; cherry[i] = NULL;
    }
    cherry.clear();

    // Borra los datos
    delete bg_data; bg_data = NULL;
    delete lightmap_data; lightmap_data = NULL;
    delete cmap_data; cmap_data = NULL;
    delete player_data; player_data = NULL;
    delete cherry_data; cherry_data = NULL;

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
void Demo::Run() {

    // Control del loop
    bool loop = true;

    // Loop principal
    while (loop) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos

        // Actualizacion del programa
        Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();

        // Control del bucle principal
        loop &= !ngn->system->quit;                                 // Si se pulsa la [X] de la ventana
        loop &= !ngn->input->key_ESC->down;                         // O se pulsa la tecla [ESC] sal del bucle de ejecucion

    }

}



/*** Carga los archivos ***/
bool Demo::Load() {

    // Datos de los fondos
    bg_data = ngn->load->TiledBg("data/maze.tbg");
    if (bg_data == NULL) return false;
    lightmap_data = ngn->load->TiledBg("data/maze_shadows.tbg");
    if (lightmap_data == NULL) return false;

    // Mapa de collisiones
    cmap_data = ngn->load->CollisionMap("data/maze.map");
    if (cmap_data == NULL) return false;

    // Sprites
    player_data = ngn->load->Sprite("data/block.spr");
    if (player_data == NULL) return false;
    cherry_data = ngn->load->Sprite("data/cherry.spr");
    if (cherry_data == NULL) return false;

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
                    delete cherry[i]; cherry[i] = NULL;
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
