/******************************************************************************

    Project Template: Menu de opciones

    Proyecto iniciado el 13 de Noviembre del 2022
    (c) 2022-2023 by Cesar Rincon "NightFox"
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
#include <iostream>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "options.h"
#include "../system/defines.h"
#include "../system/settings.h"



/*** Constructor de la clase ***/
Options::Options() {

    // Singletons (Adquisicion de instancias compartidas)
    shared_data = SharedData::GetInstance();
    save_data = SaveData::GetInstance();
    soundtrack = Soundtrack::GetInstance();
    input = Input::GetInstance();

    // Objetos
    asset.background = nullptr;

    // Prepara las estructuras de datos: Sprites
    sprite.clear();
    sprite.resize(sprite_id.enum_length);
    for (uint32_t i = 0; i < sprite.size(); i ++) sprite[i] = nullptr;

    // Datos iniciales
    cursor.option = 0;
    cursor._option = 0;

    // Copia temporal de los datos de las opciones del juego
    game_options = last_game_options = shared_data->game_options;

    // Inicia la maquina de estados
    state = next_state = STATE.fade_in;

    // Ajusta las mascaras
    shared_data->asset.black_texture->alpha = 0xFF;
    shared_data->asset.black_texture->visible = true;

    // Prepara el cache
    cache.move_cursor = nullptr;
    cache.validate = nullptr;

}



/*** Destructor de la clase ***/
Options::~Options() {

    // Elimina el cache
    cache.move_cursor = nullptr;
    cache.validate = nullptr;

    // Objetos
    delete asset.background; asset.background = nullptr;

    // Elimina los Sprites
    for (uint32_t i = 0; i < sprite.size(); i ++) {
        delete sprite[i];
        sprite[i] = nullptr;
    }
    sprite.clear();

    // Borra los datos
    ngn->resources->Clear(RS_LOCAL);

    // Singletons (anular referencias compartidas)
    input = nullptr;
    soundtrack = nullptr;
    save_data = nullptr;
    shared_data = nullptr;

}



/*** Al iniciar el modulo ***/
bool Options::Start() {

    // Carga de archivos de recursos
    if (!Load()) return false;

    // Crea los elementos del juego
    Create();

    // Inicia la maquina de estados
    state = next_state = STATE.fade_in;

    // Procesos correctos
    return true;

}



/*** Ejecucion del modulo ***/
uint32_t Options::Run() {

    // Resultado
    uint32_t r = FLAG_ZERO;

    // Bucle de ejecucion
    while (!r) {

        // Gestor de eventos de SDL y N'gine
        ngn->system->EventUpdate();

        // Actualizacion de procesos (logica)
        r |= Update();
        // Se ha solicitado por parte del usuario salir del programa?
        r |= ApplicationQuit();

        // Render de los elementos graficos
        Render();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

        // Procesos al finalizar el frame
        LateUpdate();

    }

    // Devuelve el resultado
    if (r & FLAG_RUN_QUIT) {                // Si el FLAG de salir del programa esta activo
        return OPTIONS_FLAG.exit;
    } else if (r & FLAG_RUN_TITLE) {        // Si el FLAG de volver al titulo esta activo
        return OPTIONS_FLAG.title;
    } else {                                // Si el FLAG no se reconoce, sal del programa
        return OPTIONS_FLAG.exit;
    }

}



/*** Carga de los recursos ***/
bool Options::Load() {

    // Recursos compartidos del juego
    if (!ngn->resources->Load(RS_LOCAL, "data/options/options.txt")) return false;

    // Cache de recursos
    cache.move_cursor = ngn->resources->GetSfx(RS_LOCAL, "move");
    cache.validate = ngn->resources->GetSfx(RS_LOCAL, "ok");

    // Carga correcta
    return true;

}



/*** Crea los elementos del juego ***/
void Options::Create() {

    // Crea el fondo
    asset.background = new NGN_Texture(RS_LOCAL, "background", position.background.x, position.background.y);

    // Elementos interactivos del menu
    MenuCreateOptions();
    // Actualiza los elementos del menu
    MenuUpdate();

}



/*** Logica del juego ***/
uint32_t Options::Update() {

    // Resultado
    uint32_t r = FLAG_ZERO;

    // Metodos de entrada
    input->Update();

    // Maquina de estados
    switch (state) {

        // Fade IN del menu de opciones
        case STATE.fade_in:
            shared_data->asset.black_texture->alpha -= FADE_SPEED;
            if (shared_data->asset.black_texture->alpha < 0x00) {
                shared_data->asset.black_texture->alpha = 0x00;
                shared_data->asset.black_texture->visible = false;
                next_state = STATE.run;
            }
            break;

        // Ejecucion del menu de opciones
        case STATE.run:
            // Menu de opciones
            MenuSelectOption();         // Selecciona una opcion del menu
            MenuSetOption();            // Ajusta una opcion del menu
            MenuUpdate();               // Actualiza los elementos del menu
            MenuApplyOptions(true);     // Aplica los cambios de las opciones
            if (input->cancel.down) {
                DiscardAndRestore();    // Descarta los cambios
                ngn->sound->PlaySfx(cache.validate);
                shared_data->asset.black_texture->visible = true;
                next_state = STATE.fade_out;
            } else if (input->accept.down) {
                ValidateAndSave();      // Guarda los cambios
                ngn->sound->PlaySfx(cache.validate);
                shared_data->asset.black_texture->visible = true;
                next_state = STATE.fade_out;
            }
            break;

        // Fade OUT del menu de opciones
        case STATE.fade_out:
            shared_data->asset.black_texture->alpha += FADE_SPEED;
            if (shared_data->asset.black_texture->alpha > 0xFF) {
                shared_data->asset.black_texture->alpha = 0xFF;
                next_state = STATE.exit;
            }
            // Volumen de la musica
            if (soundtrack->GetFadeOut()) soundtrack->SetVolumeTo((((0xFF - shared_data->asset.black_texture->alpha) * 100) / 0xFF));
            break;

        // Devuelve el resultado de la ejecucion
        case STATE.exit:
            if (soundtrack->GetFadeOut()) soundtrack->Stop();
            r = FLAG_RUN_TITLE;
            break;

    }

    // Siguiente estado
    state = next_state;

    // Resultado
    return r;

}



/*** Render de los elementos graficos ***/
void Options::Render() {

    // Render del fondo
    ngn->render->Texture(asset.background);

    // Sprites
    for (uint32_t i = 0; i < sprite.size(); i ++) {
        if (!sprite[i]) continue;
        ngn->render->Sprite(sprite[i]);
    }

    // Render de las mascaras
    ngn->render->Texture(shared_data->asset.black_texture);

}



/*** Actualizacion de los procesos post-render ***/
void Options::LateUpdate() {

}



/*** Salida del usuario ***/
uint32_t Options::ApplicationQuit() {

    // Valor a devolver
    uint32_t r = FLAG_ZERO;

    // Si se pulsa la X de la ventana o ALT+F4
    if (ngn->system->quit) r |= FLAG_RUN_QUIT;

    // Devuelve el resultado
    return r;

}



/*** Crea los elementos interactivos del menu ***/
void Options::MenuCreateOptions() {

    // Variables
    Vector2I32 pos = {0, 0};
    uint32_t idx = 0;

    // Cache de los graficos de los sprites
    NGN_SpriteData* square_button = ngn->resources->GetSprite(RS_LOCAL, "square");
    NGN_SpriteData* yes_button = ngn->resources->GetSprite(RS_LOCAL, "yes");
    NGN_SpriteData* no_button = ngn->resources->GetSprite(RS_LOCAL, "no");

    // Botones MAIN VOLUME
    pos.x = item_position.main_volume.x;
    pos.y = item_position.main_volume.y;
    idx = sprite_id.main_volume;
    for (uint8_t i = 0; i < 10; i ++) {
        sprite[idx] = new NGN_Sprite(square_button, pos.x, pos.y);
        pos.x += item_position.main_volume.offset;
        idx ++;
    }

    // Botones MUSIC
    pos.x = item_position.music.x;
    pos.y = item_position.music.y;
    idx = sprite_id.music;
    for (uint8_t i = 0; i < 10; i ++) {
        sprite[idx] = new NGN_Sprite(square_button, pos.x, pos.y);
        pos.x += item_position.music.offset;
        idx ++;
    }

    // Botones EFFECTS
    pos.x = item_position.effects.x;
    pos.y = item_position.effects.y;
    idx = sprite_id.effects;
    for (uint8_t i = 0; i < 10; i ++) {
        sprite[idx] = new NGN_Sprite(square_button, pos.x, pos.y);
        pos.x += item_position.effects.offset;
        idx ++;
    }

    // Botones FULL SCREEN
    pos.x = item_position.full_screen.x;
    pos.y = item_position.full_screen.y;
    idx = sprite_id.full_screen;
    sprite[idx] = new NGN_Sprite(no_button, pos.x, pos.y);
    pos.x += item_position.full_screen.offset;
    idx ++;
    sprite[idx] = new NGN_Sprite(yes_button, pos.x, pos.y);

    // Botones FILTERING
    pos.x = item_position.filtering.x;
    pos.y = item_position.filtering.y;
    idx = sprite_id.filtering;
    sprite[idx] = new NGN_Sprite(no_button, pos.x, pos.y);
    pos.x += item_position.filtering.offset;
    idx ++;
    sprite[idx] = new NGN_Sprite(yes_button, pos.x, pos.y);

    // Botones V-SYNC
    pos.x = item_position.vsync.x;
    pos.y = item_position.vsync.y;
    idx = sprite_id.vsync;
    sprite[idx] = new NGN_Sprite(no_button, pos.x, pos.y);
    pos.x += item_position.vsync.offset;
    idx ++;
    sprite[idx] = new NGN_Sprite(yes_button, pos.x, pos.y);

    // Cursor del menu
    sprite[sprite_id.cursor] = new NGN_Sprite(RS_LOCAL, "cursor", cursor.position[cursor.option].x, cursor.position[cursor.option].y);

    // Elimina las referencias
    square_button = nullptr;
    yes_button = nullptr;
    no_button = nullptr;

}



/*** Selecciona una opcion del menu ***/
void Options::MenuSelectOption() {

    // Seleccion de opcion
    if (input->down.down) {
        cursor.option ++;
        if (cursor.option > cursor.last_option) cursor.option = cursor.last_option;
    }
    if (input->up.down) {
        cursor.option --;
        if (cursor.option < 0) cursor.option = 0;
    }

    if (cursor.option != cursor._option) ngn->sound->PlaySfx(cache.move_cursor);
    cursor._option = cursor.option;

}



/*** Ajusta una opcion del menu ***/
void Options::MenuSetOption() {

    // Seleccion de propiedades de la opcion
    switch (cursor.option) {
        case menu_option.main_volume:
            game_options.main_volume = MenuSelectValue(game_options.main_volume, 10);
            break;
        case menu_option.music:
            game_options.music_volume = MenuSelectValue(game_options.music_volume, 10);
            break;
        case menu_option.effects:
            game_options.effects_volume = MenuSelectValue(game_options.effects_volume, 10);
            break;
        case menu_option.full_screen:
            game_options.screen_mode = MenuSelectValue(game_options.screen_mode, 1);
            break;
        case menu_option.filtering:
            game_options.bilinear_filtering = MenuSelectValue(game_options.bilinear_filtering, 1);
            break;
        case menu_option.vsync:
            game_options.v_sync = MenuSelectValue(game_options.v_sync, 1);
            break;
    }

}



/*** Actualiza los graficos del menu ***/
void Options::MenuUpdate() {

    // Actualiza la posicion del cursor
    sprite[sprite_id.cursor]->Position(cursor.position[cursor.option].x, cursor.position[cursor.option].y);

    // Opcion "MAIN VOLUME"
    MenuBar(sprite_id.main_volume, game_options.main_volume, 10);
    // Opcion "MUSIC"
    MenuBar(sprite_id.music, game_options.music_volume, 10);
    // Opcion "EFFECTS"
    MenuBar(sprite_id.effects, game_options.effects_volume, 10);

    // Opcion "FULL SCREEN"
    MenuSwitch(sprite_id.full_screen, game_options.screen_mode, 2);
    // Opcion "FILTERING"
    MenuSwitch(sprite_id.filtering, game_options.bilinear_filtering, 2);
    // Opcion "V-SYNC"
    MenuSwitch(sprite_id.vsync, game_options.v_sync, 2);

}



/*** Actualiza los graficos de una opcion tipo "switch" ***/
void Options::MenuSwitch(uint32_t spr_id, uint8_t current_val, uint8_t max_val) {

    int32_t frame = 0;

    for (uint8_t i = 0; i < max_val; i ++) {
        if (current_val == i) {
            frame = 1;
        } else {
            frame = 0;
        }
        sprite[(spr_id + i)]->frame = frame;
    }

}



/*** Actualiza los graficos de una opcion tipo "bar" ***/
void Options::MenuBar(uint32_t spr_id, uint8_t current_val, uint8_t max_val) {

    int32_t frame = 0;

    for (uint8_t i = 0; i < max_val; i ++) {
        if (current_val == 0) {
            frame = 0;
        } else if ((current_val - 1) >= i) {
            frame = 1;
        } else {
            frame = 0;
        }
        sprite[(spr_id + i)]->frame = frame;
    }

}



/*** Seleccion del valor de una opcion del menu ***/
uint8_t Options::MenuSelectValue(uint8_t current, uint8_t max_val) {

    int8_t op = (int8_t)current;

    if (input->left.down) {
        op --;
        if (op < 0) op = 0;
    }
    if (input->right.down) {
        op ++;
        if (op > (int8_t)max_val) op = (int8_t)max_val;
    }

    return (uint8_t)op;

}




/*** Aplica si hay algun cambio en las opciones ***/
void Options::MenuApplyOptions(bool sfx_enabled) {

    // Efecto de sonido
    bool sfx = false;

    // Volumen general
    if (game_options.main_volume != last_game_options.main_volume) {
        ngn->sound->SetMixerLevel(MIXER_MASTER_CH, (game_options.main_volume * 10));
        sfx |= true;
    }

    // Volumen de la musica
    if (game_options.music_volume != last_game_options.music_volume) {
        ngn->sound->SetMixerLevel(MIXER_MUSIC_CH, (game_options.music_volume * 10));
        sfx |= true;
    }

    // Volumen de los efectos de sonido
    if (game_options.effects_volume != last_game_options.effects_volume) {
        ngn->sound->SetMixerLevel(MIXER_EFFECTS_CH, (game_options.effects_volume * 10));
        sfx |= true;
    }

    // Pantalla completa
    if (game_options.screen_mode != last_game_options.screen_mode) {
        if (game_options.screen_mode == 0) {
            ngn->graphics->SetMode(NGN_SCR_WINDOW);
        } else {
            ngn->graphics->SetMode(NGN_SCR_WINDOW_FULL);
        }
        sfx |= true;
    }

    // Filtrado bilinear
    if (game_options.bilinear_filtering != last_game_options.bilinear_filtering) {
        if (game_options.bilinear_filtering == 0) {
            ngn->graphics->SetFiltering(false);
        } else {
            ngn->graphics->SetFiltering(true);
        }
        sfx |= true;
    }

    // Sincronismo vertical
    if (game_options.v_sync != last_game_options.v_sync) {
        if (game_options.v_sync == 0) {
            ngn->graphics->SetVerticalSync(false);
        } else {
            ngn->graphics->SetVerticalSync(true);
        }
        sfx |= true;
    }

    // Guarda los valores actuales
    last_game_options = game_options;

    // Efecto de sonido
    if (sfx && sfx_enabled) ngn->sound->PlaySfx(cache.move_cursor);

}



/*** Valida y guarda los datos ***/
void Options::ValidateAndSave() {

    // Si no hay cambios, no hagas nada
    bool update = false;
    update |= (shared_data->game_options.main_volume != game_options.main_volume);
    update |= (shared_data->game_options.music_volume != game_options.music_volume);
    update |= (shared_data->game_options.effects_volume != game_options.effects_volume);
    update |= (shared_data->game_options.screen_mode != game_options.screen_mode);
    update |= (shared_data->game_options.bilinear_filtering != game_options.bilinear_filtering);
    update |= (shared_data->game_options.v_sync != game_options.v_sync);
    if (!update) return;

    // Copia los datos de opcion temporales a los definitivos
    shared_data->game_options = game_options;

    // Guardalos
    save_data->Save();

}



/*** Descarta y restaura los datos ***/
void Options::DiscardAndRestore() {

    // Restaura las opciones al valor que tenian al entrar en el menu
    game_options = shared_data->game_options;

    // Y aplicalas
    MenuUpdate();               // Actualiza los elementos del menu
    MenuApplyOptions(false);    // Aplica los cambios de las opciones

}

