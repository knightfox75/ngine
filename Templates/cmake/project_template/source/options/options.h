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



#ifndef OPTIONS_H_INCLUDED
#define OPTIONS_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>
// Includes del proyecto
#include "../system/settings.h"
#include "../system/shared_data.h"
#include "../system/save_data.h"
#include "../system/soundtrack.h"
#include "../system/input.h"



/*** Declaracion de la clase ***/
class Options {

    public:

        // Constructor
        Options();

        // Destructor
        ~Options();

        // Al iniciar el modulo
        bool Start();

        // Ejecuta el modulo
        uint32_t Run();


    private:

        // Punteros a los singletons (compartidos)
        SharedData* shared_data;
        SaveData* save_data;
        Soundtrack* soundtrack;
        Input* input;

        // Copia temporal de los datos de las opciones
        SharedData::GameOptionsStruct game_options;
        SharedData::GameOptionsStruct last_game_options;

        // Carga de recursos
        bool Load();

        // Creacion de los diferentes elementos
        void Create();

        // Actualizacion de los procesos
        uint32_t Update();

        // Render de los elementos graficos
        void Render();

        // Actualizacion de los procesos post-render
        void LateUpdate();

        // Salida del usuario
        uint32_t ApplicationQuit();

        // Objetos
        struct {
            NGN_Texture* background;                // Fondo
        } asset;

        // Posiciones
        const struct {
            Vector2 background = {0.0f, 0.0f};      // Fondo
        } position;

        // Cache de recursos
        struct {
            NGN_AudioClipData* move_cursor;         // Sonido al mover el cursor
            NGN_AudioClipData* validate;            // Sonido al validar las opciones (o salir del menu)
        } cache;

        // Sprites (bloques de opciones interactivas)
        struct {
            enum {
                cursor = 0,                                 // Cursor
                main_volume = (cursor + 1),                 // Opcion "Volumen general"
                music = (main_volume + 10),                 // Opcion "Volumen de la musica"
                effects = (music + 10),                     // Opcion "Volumen de los efectos"
                full_screen = (effects + 10),               // Opcion "Pantalla completa"
                filtering = (full_screen + 2),              // Opcion "Filtrado"
                vsync = (filtering + 2),                    // Opcion "Sincronismo vertical"
                enum_length = (vsync + 2)                   // Numero total de elementos
            };
        } sprite_id;
        std::vector<NGN_Sprite*> sprite;

        // Posicion de los elementos del menu
        struct ItemPosition {
            int32_t x;
            int32_t y;
            int32_t offset;
        };
        const struct {
            ItemPosition main_volume = {160, 256, 32};
            ItemPosition music = {160, 384, 32};
            ItemPosition effects = {160, 512, 32};
            ItemPosition full_screen = {848, 256, 96};
            ItemPosition filtering = {848, 384, 96};
            ItemPosition vsync = {848, 512, 96};
        } item_position;

        // Datos del cursor del menu
        struct {
            enum {
                main_volume,
                music,
                effects,
                full_screen,
                filtering,
                vsync
            };
        } menu_option;
        struct {
            const int8_t last_option = 5;
            const Vector2I32 position[6] = {
                {96, 256},     // main_volume
                {96, 384},     // music
                {96, 512},     // effects
                {768, 256},     // full_screen
                {768, 384},     // filtering
                {768, 512}      // vsync
            };
            int8_t option;
            int8_t _option;
        } cursor;

        // Crea los elementos interactivos del menu
        void MenuCreateOptions();

        void MenuSelectOption();        // Selecciona una opcion del menu
        void MenuSetOption();           // Ajusta una opcion del menu
        void MenuUpdate();              // Actualiza los graficos del menu
        // Metodos adicionales del menu
        void MenuSwitch(uint32_t spr_id, uint8_t current_val, uint8_t max_val);     // Actualiza los graficos de una opcion tipo "switch"
        void MenuBar(uint32_t spr_id, uint8_t current_val, uint8_t max_val);        // Actualiza los graficos de una opcion tipo "bar"
        uint8_t MenuSelectValue(uint8_t current, uint8_t max_val);                  // Selecciona un valor de la opcion del menu
        void MenuApplyOptions(bool sfx_enabled);                                    // Aplica los cambios (si hay alguno)

        // Gestion de los datos
        void ValidateAndSave();         // Valida y guarda los datos
        void DiscardAndRestore();       // Descarta y restaura los datos

        // Maquina de estados
        struct {
            enum {
                fade_in,            // Efecto FADE-IN
                fade_out,           // Efecto FADE-OUT
                run,                // Ejecuta el menu de opciones
                exit,               // Sal del menu de opciones
                none
            };
        } STATE;
        int32_t state, next_state;

};


#endif // OPTIONS_H_INCLUDED
