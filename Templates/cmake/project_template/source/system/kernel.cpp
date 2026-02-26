/******************************************************************************

    Project Template: Nucleo de ejecucion del juego

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
#include <algorithm>
// Includes de la libreria NGN
#include <ngn.h>
// Includes del proyecto
#include "kernel.h"
#include "settings.h"
#include "defines.h"
#include "save_data.h"
#include "../welcome/welcome.h"
#include "../title/title.h"
#include "../game/game.h"
#include "../options/options.h"



/*** Constructor de la clase ***/
Kernel::Kernel() {

    // Crea las instancias de los singletons principales
    shared_data = SharedData::GetInstance();
    save_data = SaveData::GetInstance();
    soundtrack = Soundtrack::GetInstance();
    input = Input::GetInstance();

    // Inicia los singletons creados
    shared_data->BootUp();
    save_data->BootUp();
    soundtrack->BootUp();
    input->BootUp();

}



/*** Destructor de la clase ***/
Kernel::~Kernel() {

    // Elimina las instancias de los singletons
    Input::RemoveInstance(); input = nullptr;
    Soundtrack::RemoveInstance(); soundtrack = nullptr;
    SaveData::RemoveInstance(); save_data = nullptr;
    SharedData::RemoveInstance(); shared_data = nullptr;

    // Elimina los repositorios de datos
    ngn->resources->RemoveRepository(RS_LOCAL);
    ngn->resources->RemoveRepository(RS_SHARED);
    ngn->resources->RemoveRepository(RS_PERSISTENT);

}



/*** Inicializa N'GINE ***/
bool Kernel::Awake() {

    // Inicializa la libreria
    if (!ngn->Init()) {
        std::cout << "Critical error, can't initialize n'gine." << std::endl;
        return false;
    }

    // Habilita el archivo de registro
    EnableLogging();

    // Lee el archivo de guardado (crea uno nuevo si no existe con los datos por defecto)
    save_data->Load();

    // Opciones de configuracion iniciales
    int8_t screen_mode = NGN_SCR_WINDOW;
    bool bilinear_filter = false;
    bool vsync = false;
    #if defined (MODE_DEBUG)
        screen_mode = NGN_SCR_WINDOW;
        bilinear_filter = false;
        vsync = true;
    #else
        screen_mode = (shared_data->game_options.screen_mode == 0) ? NGN_SCR_WINDOW:NGN_SCR_WINDOW_FULL;
        bilinear_filter = (shared_data->game_options.bilinear_filtering == 0) ? false:true;
        vsync = (shared_data->game_options.v_sync == 0) ? false:true;
    #endif

    // Crea la ventana con la resolucion nativa
    if (!ngn->graphics->Init(WINDOW_TITLE, SCR_WIDTH, SCR_HEIGHT, screen_mode, bilinear_filter, vsync)) return false;
    ngn->graphics->Update();

    // Visibilidad del cursor del raton
    ngn->graphics->ShowMouse(SHOW_MOUSE);

    // Contador de FPS activo?
    ngn->system->fps_counter = FPS_COUNTER;

    // Selecciona el modo grafico (ventana/full screen)
    ngn->graphics->SetMode(screen_mode);
    ngn->graphics->Update();

    // Crea los repositorios de recursos
    ngn->resources->AddRepository(RS_LOCAL);
    ngn->resources->AddRepository(RS_SHARED);
    ngn->resources->AddRepository(RS_PERSISTENT);

    // Usa el archivo empaquetado de datos si es la version release
    #if defined (MODE_RELEASE)
        if(!ngn->load->SetPackage("data.pkg", "0123456789ABCDEF")) return false;
    #endif

	// Muestra la version de la libreria en la consola
	#if defined (MODE_DEBUG)
		ngn->log->Message("Versions check:\n" + ngn->system->GetVersion());
		ngn->log->Message("N'gine started successfully!");
		ngn->log->Message("Running " + WINDOW_TITLE + " " + shared_data->GetVersion());
	#endif

    // Inicializacion completada con exito
    return true;

}



/*** Al iniciar el programa ***/
bool Kernel::Start() {

    // Carga los recursos persistentes
    if (!shared_data->Load()) return false;

    // Crea los recursos persistentes
    shared_data->Create();

    // Valores por defecto del mixer
    ngn->sound->SetMixerLevel(MIXER_MASTER_CH, (shared_data->game_options.main_volume * 10));
    ngn->sound->SetMixerLevel(MIXER_MUSIC_CH, (shared_data->game_options.music_volume * 10));
    ngn->sound->SetMixerLevel(MIXER_EFFECTS_CH, (shared_data->game_options.effects_volume * 10));

    // Maquina de estados
    #if defined (MODE_DEBUG)
        state = next_state = STATE.title;
    #else
        state = next_state = STATE.welcome;
    #endif

    // Procesos correctos
    return true;

}



/*** Ejecuta el juego ***/
void Kernel::Run() {

    // Control del loop
    bool loop = true;

    // Bucle de ejecucion
    while (loop) {

        // Nucleo de la logica del juego
        switch (state) {

            // Muestra las pantallas de bienvenida
            case STATE.welcome:
                StWelcome();
                break;

            // Titulo del juego
            case STATE.title:
                StTitle();
                break;

            // Nucleo del juego
            case STATE.game:
                StGame();
                break;

            // Menu de opciones del juego
            case STATE.options:
                StOptions();
                break;

            // Termina de ejecucion del juego
            case STATE.exit:
                loop = false;
                break;

            // Error trap
            default:
                loop = false;
                break;

        }

        // Actualiza el estado
        state = next_state;

    }

    // Elimina todos los datos restantes cargados por el programa
    CleanUp();

}



/*** Habilita el archivo de registro ***/
void Kernel::EnableLogging() {

    // Genera la ruta del archivo de guardado
    std::string logfile_path = "";

    // Segun el sistema operativo...
    #if defined (MODE_RELEASE)
        #if defined (OS_WINDOWS)
            // Guarda la ruta a la carpeta del usuario
            const char *user_path = std::getenv("USERPROFILE");
        #elif defined (OS_LINUX)
            // Guarda la ruta a la carpeta del usuario
            const char *user_path = std::getenv("HOME");
        #else
            // No se reconoce el sistema operativo
            const char *user_path = nullptr;
        #endif
    #else
        // En caso de estar en modo DEBUG, indica la ruta como nula
        const char *user_path = nullptr;
    #endif

    // Si hay una carpeta de sistema valida...
    if (user_path) {
        // Convierte el user_path a string
        std::string user_folder(user_path);
        // Remplaza las contrabarras por barras
        const char backslash = 0x5C;        // Busca el caracter "\"
        const char slash = 0x2F;            // Y cambialo por el caracter "/"
        std::replace(user_folder.begin(), user_folder.end(), backslash, slash);
        // Remplaza los espacios por guiones bajos en el nombre de la carpeta
        std::string logfile_folder = LOGFILE_PATH;
        const char space = 0x20;            // Busca el caracter " "
        const char underscore = 0x5F;       // Y cambialo por el caracter "_"
        std::replace(logfile_folder.begin(), logfile_folder.end(), space, underscore);
        // Genera el path a la ruta del archivo de guardado
        logfile_path += user_folder;
        logfile_path += logfile_folder;
    }

    // A�ade el nombre de archivo a la ruta generada
    logfile_path += LOGFILE_NAME;

    // Habilita el archivo de registro
    ngn->log->OpenLogFile(logfile_path);

}



/*** Elimina todos los datos restantes cargados por el programa ***/
void Kernel::CleanUp() {

    ngn->resources->Clear(RS_LOCAL);
    ngn->resources->Clear(RS_SHARED);
    ngn->resources->Clear(RS_PERSISTENT);

}



/*** Estado "Welcome" ***/
void Kernel::StWelcome() {

    // Crea el objeto "welcome"
    Welcome* welcome = new Welcome();

    // Inicia el objeto
    if (welcome->Start()) {
        // Ejecuta y decide
        switch (welcome->Run()) {
            case WELCOME_FLAG.exit:     // Sal del programa
                next_state = STATE.exit;
                break;
            case WELCOME_FLAG.title:    // Ve al titulo
                next_state = STATE.title;
                break;
            default:                    // En caso de estado no definido, sal
                next_state = STATE.exit;
                break;
        }
    } else {
        // En caso de error, sal
        next_state = STATE.exit;
    }

    // Borra el objeto "welcome"
    delete welcome; welcome = nullptr;

}



/*** Estado "Title" ***/
void Kernel::StTitle() {

    // Crea el objeto "title"
    Title* title = new Title();

    // Resultado de la ejecucion
    uint32_t r = 0;

    // Inicia el objeto
    if (title->Start()) {
        // Ejecuta y decide
        r = title->Run();
        switch ((r & BITMASK_DATA_LOW)) {
            case TITLE_FLAG.game_start:             // Opcion "NEW GAME" [0]
                next_state = STATE.game;
                break;
            case TITLE_FLAG.options:                // Opcion "OPTIONS" [1]
                next_state = STATE.options;
                break;
            case TITLE_FLAG.exit:                   // Opcion "EXIT" [2]
                next_state = STATE.exit;
                break;
            default:    // En caso de estado no definido, sal
                next_state = STATE.exit;
                break;
        }
    } else {
        // En caso de error, sal
        next_state = STATE.exit;
    }

    // Borra el objeto "welcome"
    delete title; title = nullptr;

}




/*** Estado "Game" ***/
void Kernel::StGame() {

    // Crea el objeto "game"
    Game* game = new Game();

    // Resultado de la ejecucion
    uint32_t r = 0;

    // Inicia el objeto
    if (game->Start()) {
        // Ejecuta y decide
        r = game->Run();
        switch ((r & BITMASK_DATA_LOW)) {
            case GAME_FLAG.exit:        // Sal del programa
                next_state = STATE.exit;
                break;
            case GAME_FLAG.title:       // Ve al titulo
                next_state = STATE.title;
                break;
            default:                    // En caso de estado no definido, sal
                next_state = STATE.exit;
                break;
        }
    } else {
        // En caso de error, sal
        next_state = STATE.exit;
    }

    // Borra el objeto "game"
    delete game; game = nullptr;

}



/*** Estado "Options" ***/
void Kernel::StOptions() {

    // Crea el objeto "options"
    Options* options = new Options();

    // Resultado de la ejecucion
    uint32_t r = 0;

    // Inicia el objeto
    if (options->Start()) {
        // Ejecuta y decide
        r = options->Run();
        switch ((r & BITMASK_DATA_LOW)) {
            case OPTIONS_FLAG.exit:         // Sal del programa
                next_state = STATE.exit;
                break;
            case OPTIONS_FLAG.title:        // Ve al titulo
                next_state = STATE.title;
                break;
            default:                        // En caso de estado no definido, sal
                next_state = STATE.exit;
                break;
        }
    } else {
        // En caso de error, sal
        next_state = STATE.exit;
    }

    // Borra el objeto "options"
    delete options; options = nullptr;

}
