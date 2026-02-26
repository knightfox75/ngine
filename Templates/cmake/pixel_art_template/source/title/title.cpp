/******************************************************************************

    Pixel Art Project Template: Titulo

    Proyecto iniciado el xx de xx del xx
    (c) xxxx by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.20.0+10th-anniversary o superior
	(c) 2016 - 2026 by Cesar Rincon "NightFox"
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
#include <cmath>
// Includes de la libreria NGN
#include <ngn.h>
// Includes del proyecto
#include "title.h"
#include "../system/defines.h"



/*** Constructor de la clase ***/
Title::Title() {

    // Singletons
    shared_data = SharedData::GetInstance();
    soundtrack = Soundtrack::GetInstance();
    input = Input::GetInstance();

    // Objetos
    asset.title = nullptr;
    asset.cursor = nullptr;

    // Configuracion inicial de las mascaras
    shared_data->asset.black_texture->visible = true;
    shared_data->asset.black_texture->alpha = 0xFF;

}



/*** Destructor de la clase ***/
Title::~Title() {

    // Objetos
    delete asset.title; asset.title = nullptr;
    delete asset.cursor; asset.cursor = nullptr;

    // Borra los datos
    ngn->resources->Clear(RS_LOCAL);

    // Singletons
    input = nullptr;
    soundtrack = nullptr;
    shared_data = nullptr;

}



/*** Al iniciarse el objeto ***/
bool Title::Start() {

    // Carga de archivos
    if (!Load()) return false;
    // Crea la escena
    Create();

    // Inicia la maquina de estados
    state = next_state = STATE.fade_in;

    // Valores iniciales
    menu.option = 0;

    // Reproduce la musica de fondo
    soundtrack->SetVolumeTo(100);
    soundtrack->Play();

    // Inicializacion correcta
    return true;

}



/*** Ejecucion del objeto ***/
uint32_t Title::Run() {

    // Resultado
    uint32_t r = FLAG_ZERO;

    // Bucle de ejecucion
    while (!r) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();            // Actualiza los eventos

        // Logica del programa
        r |= Update();
        // Se ha solicitado por parte del usuario salir del programa?
        r |= ApplicationQuit();

        // Render de los graficos
        Render();
        // Actualiza la BGM
        soundtrack->Update();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

    }

    // Devuelve el resultado
    if (r & FLAG_RUN_QUIT) {
        return TITLE_FLAG.exit;
    } else {
        return r;
    }

}



/*** Carga los archivos de datos ***/
bool Title::Load() {

    // Carga los recursos de la lista "title.txt" en la tabla local
    if (!ngn->resources->Load(RS_LOCAL, "data/title/title.txt")) return false;

    // Cache de archivos mas utilizados
    cache.menu_move = ngn->resources->GetSfx(RS_LOCAL, "move");
    cache.menu_accept = ngn->resources->GetSfx(RS_LOCAL, "ok");

    // Musica
    if (!soundtrack->Load(soundtrack->TRACK_ID.title)) return false;

    // Carga correcta
    return true;

}



/*** Crea la escena ***/
void Title::Create() {

    // Texturas
    asset.title = new NGN_Texture(RS_LOCAL, "title", position.title.x, position.title.y);

    // Sprites
    asset.cursor = new NGN_Sprite(RS_LOCAL, "cursor", position.cursor.x, position.cursor.y);

    // Ajusta las mascaras
    shared_data->asset.black_texture->alpha = 0xFF;
    shared_data->asset.black_texture->visible = true;

}



/*** Logica del programa ***/
uint32_t Title::Update() {

    // Resultado
    uint32_t r = FLAG_ZERO;

    // Metodos de entrada
    input->Update();

    // Maquina de estados
    switch (state) {

        // Fade IN del titulo
        case STATE.fade_in:
            shared_data->asset.black_texture->alpha -= FADE_SPEED;
            if (shared_data->asset.black_texture->alpha < 0x00) {
                shared_data->asset.black_texture->alpha = 0x00;
                shared_data->asset.black_texture->visible = false;
                next_state = STATE.run;
            }
            break;

        // Ejecucion
        case STATE.run:
            if (Menu()) {
                switch (menu.option) {
                    case TITLE_FLAG.new_game:
                    case TITLE_FLAG.continue_game:
                    case TITLE_FLAG.exit:
                        soundtrack->SetFadeOut(true);
                        break;
                    case TITLE_FLAG.options:
                    default:
                        soundtrack->SetFadeOut(false);
                        break;
                }
                shared_data->asset.black_texture->visible = true;
                next_state = STATE.fade_out;
            }
            break;

        // Fade OUT del titulo
        case STATE.fade_out:
            shared_data->asset.black_texture->alpha += FADE_SPEED;
            if (shared_data->asset.black_texture->alpha > 0xFF) {
                shared_data->asset.black_texture->alpha = 0xFF;
                next_state = STATE.exit;
            }
            // Volumen de la musica
            if (soundtrack->GetFadeOut()) soundtrack->SetVolumeTo((((0xFF - shared_data->asset.black_texture->alpha) * 100) / 0xFF));
            break;

        // Acepta el resultado del menu
        case STATE.exit:
            if (soundtrack->GetFadeOut()) soundtrack->Stop();
            r = (FLAG_RUN_EXIT | menu.option);
            break;

    }

    // Siguiente estado
    state = next_state;

    // Resultado
    return r;

}



/*** Render de los graficos ***/
void Title::Render() {

    // Texturas
    ngn->render->Texture(asset.title);

    // Menu
    ngn->render->Sprite(asset.cursor);

    // Render de las mascaras
    ngn->render->Texture(shared_data->asset.black_texture);

}



/*** Control del menu ***/
bool Title::Menu() {

    // Resultado
    bool r = false;

    // Si se pulsa arriba
    if (input->up.down) {
        menu.option --;
        if (menu.option < menu.first_option) {
            menu.option = menu.first_option;
        } else {
            ngn->sound->PlaySfx(cache.menu_move);
        }
    }
    // Si se pulsa abajo
    if (input->down.down) {
        menu.option ++;
        if (menu.option > menu.last_option) {
            menu.option = menu.last_option;
        } else {
            ngn->sound->PlaySfx(cache.menu_move);
        }
    }

    // Acepta la opcion
    if (input->cancel.down) {
        if (menu.option == menu.last_option) {
            ngn->sound->PlaySfx(cache.menu_accept);
            menu.option = TITLE_FLAG.exit;
            r = true;
        } else {
            menu.option = menu.last_option;
            ngn->sound->PlaySfx(cache.menu_move);
        }
    } else if (input->accept.down) {
        ngn->sound->PlaySfx(cache.menu_accept);
        r = true;
    }

    // Coloca el cursor
    asset.cursor->Position(position.cursor.x, (position.cursor.y + (menu.cursor_offset * menu.option)));

    // Resultado
    return r;

}



/*** Salida del usuario ***/
uint32_t Title::ApplicationQuit() {

    // Valor a devolver
    uint32_t r = FLAG_ZERO;

    // Si se pulsa la X de la ventana o ALT+F4
    if (ngn->system->quit) r |= FLAG_RUN_QUIT;

    // Devuelve el resultado
    return r;

}



/*** Solicita saltarte la escena ***/
bool Title::Skip() {

    if (
        input->accept.down
        ||
        input->cancel.down
    ) {
        // Se ha solicitado
        return true;
    }

    // No se ha solicitado
    return false;

}
