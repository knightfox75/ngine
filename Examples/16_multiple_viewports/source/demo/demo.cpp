/******************************************************************************

    Ejemplo de uso de varios viewports simultaneos
    Archivo de Demo (Programa)

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
#include <ctime>
#include <iostream>
#include <string>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros a los datos
    bg0_data = NULL;
    bg1_data = NULL;
    bg2_data = NULL;
    aim_data = NULL;
    bird_data = NULL;

    // Inicializa los punteros a los objetos
    bg0 = NULL;
    bg1 = NULL;
    bg2 = NULL;
    aim = NULL;
    bird.clear();

    // GUI
    mono = NULL;
    text = NULL;

    // Screenshoot
    shutter = NULL;
    overlay = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg0; bg0 = NULL;
    delete bg1; bg1 = NULL;
    delete bg2; bg2 = NULL;
    delete aim; aim = NULL;
    for (uint32_t i = 0; i < bird.size(); i ++) {
        delete bird[i].sprite;
        bird[i].sprite = NULL;
    }

    // Borra los datos
    delete bg0_data; bg0_data = NULL;
    delete bg1_data; bg1_data = NULL;
    delete bg2_data; bg2_data = NULL;
    delete aim_data; aim_data = NULL;
    delete bird_data; bird_data = NULL;

    // GUI
    delete text; text = NULL;
    delete mono; mono = NULL;

    // Screenshoot
    delete shutter; shutter = NULL;
    delete overlay; overlay = NULL;

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

    // Caga de archivos
    if (!Load()) return false;

    // Crea la escena
    CreateStage();

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

        // Render de los elementos adicionales
        Render();

        // Actualiza el contenido de la pantalla
        ngn->graphics->Update();
        // Actualiza el sonido
        ngn->sound->Update();

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



/*** Carga los archivos necesarios ***/
bool Demo::Load() {

    // Carga los datos de los fondos
    bg0_data = ngn->load->TiledBg("data/bg0.tbg");
    if (!bg0_data) return false;
    bg1_data = ngn->load->TiledBg("data/bg1.tbg");
    if (!bg1_data) return false;
    bg2_data = ngn->load->TiledBg("data/bg2.tbg");
    if (!bg2_data) return false;

    // Carga los sprites
    bird_data = ngn->load->Sprite("data/bird_small.spr");
    if (!bird_data) return false;
    aim_data = ngn->load->Sprite("data/aim.spr");
    if (!aim_data) return false;

    // Carga la tipografia
    mono = ngn->load->TrueTypeFont("data/monofonto.ttf", 16, true, 0xFFFFFF, 1, 0x202020);
    if (!mono) return false;

    // Carga los efectos de sonido
    shutter = ngn->load->AudioClip("data/shutter.wav");
    if (!shutter) return false;

    // Carga el overlay del screenshoot
    overlay = ngn->load->Texture("data/logo_overlay.png");
    if (!overlay) return false;

    // Carga correcta
    return true;

}



/*** Crea la escena ***/
void Demo::CreateStage() {

    // Crea los fondos tiles
    bg0 = new NGN_TiledBg(bg0_data);
    bg1 = new NGN_TiledBg(bg1_data);
    bg2 = new NGN_TiledBg(bg2_data);

    // Crea la mirilla
    aim = new NGN_Sprite(aim_data, (bg2->width / 2.0f), (bg2->height * 0.75f));

    // Configura el numero de capas de la camara virtual 2d
    ngn->camera->CreateLayers(3);
    // Parametros de la camara
    ngn->camera->Setup(bg2->width, bg2->height, aim);

    // A�ade los fondos a la camara para su gestion
    ngn->camera->PushBackground(0, bg0);
    ngn->camera->PushBackground(1, bg1);
    ngn->camera->PushBackground(2, bg2);

    // Genera los pajaros de la capa 0 y registralos en la camara(fondo)
    CreateBirds(0, 20, 0.33);
    CreateBirds(1, 15, 0.66f);
    CreateBirds(2, 10, 1.0f);

    // A�ade el sprite de la mirilla a la camara para su gestion
    ngn->camera->PushSprite(2, aim);

    // Define los Viewports
    ngn->graphics->OpenViewport(0, 0, 0, 1280, 360);
    ngn->graphics->OpenViewport(1, 640, 360, 640, 360);
    ngn->graphics->OpenViewport(2, 0, 360, 640, 360, 1280, 720, true);      // Filtro local activo
    ngn->graphics->OpenViewport(3, 480, 240, 320, 240, 160, 120, false);    // Filtro local desactivado

    // Crea la capa de texto para la GUI
    text = new NGN_TextLayer(mono, NULL, 16, 16, 380, 192);
    text->Padding(16);
    text->CanvasColor(0x20202080);

}



/*** Crea los pajaros en la capa solicitada ***/
void Demo::CreateBirds(uint32_t ly, uint32_t num, float scale) {

    // Objeto temporal para crear pajaros
    bird_type b;
    b.sprite = NULL;

    // Calculos de la posicion
    int32_t x, y;

    // Calculos de la velocidad
    int32_t _min = (int32_t)(BIRD_MIN_SPEED * 100.0f);
    int32_t _max = (int32_t)(BIRD_MAX_SPEED * 100.0f);


    for (uint32_t i = 0; i < num; i ++) {
        // Crea un nuevo sprite
        b.sprite = new NGN_Sprite(bird_data);
        // Calcula el nuevo tama�o
        b.sprite->width *= scale;
        b.sprite->height *= scale;
        // Calcula los limites del desplazamiento
        b.left = -b.sprite->width;
        b.right = (ngn->camera->layer[ly].sprite_layer.width + b.sprite->width);
        // Calcula la posicion
        x = (rand() % ngn->camera->layer[ly].sprite_layer.width);
        y = ((rand() % (int32_t)(ngn->camera->layer[ly].sprite_layer.height * 0.80f)) + (ngn->camera->layer[ly].sprite_layer.height * 0.05f));
        b.sprite->Position(x, y);
        // Calcula la velocidad
        b.speed = ((float)((rand() % (_max - _min)) + _min) / 100.0f) * scale;
        if ((rand() % 100 > 50)) {
            b.speed = -b.speed;
            b.sprite->flip_h = true;
        }
        // Define la animacion
        b.sprite->AddAnimation("fly", 0, 7, 0, 5);
        b.sprite->SetAnimation("fly");
        b.sprite->frame = (rand() % 8);
        // Registra el sprite en la camara
        ngn->camera->PushSprite(ly, b.sprite);
        // Y metelo en la cola de gestion de pajaros
        bird.push_back(b);
    }

    // Limpia el puntero temporal
    b.sprite = NULL;

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Mueve a los pajaros
    MoveBirds();

    // Mueve la mirilla
    MoveAim();

    // Gestor de los viewports
    ViewportManager();

    // Gestion de los modos de pantalla y filtro
    ScreenConfig();

    // GUI
    Gui();

    // Captura de pantalla
    TakeScreenshot();

}



/*** Render de los elementos ***/
void Demo::Render() {

    // Render de los viewports
    ngn->render->Viewports();

    // GUI
    ngn->render->TextLayer(text);

}



/*** Mueve a los pajaros ***/
void Demo::MoveBirds() {

    for (uint32_t i = 0; i < bird.size(); i ++) {
        bird[i].sprite->Translate(bird[i].speed, 0.0f);
        if (bird[i].sprite->position.x < bird[i].left) {
            bird[i].sprite->position.x = bird[i].left;
            bird[i].speed = -bird[i].speed;
            bird[i].sprite->flip_h = false;
        }
        if (bird[i].sprite->position.x > bird[i].right) {
            bird[i].sprite->position.x = bird[i].right;
            bird[i].speed = -bird[i].speed;
            bird[i].sprite->flip_h = true;
        }
    }

}



/*** Mueve el punto de mira ***/
void Demo::MoveAim() {

    // Vector de movimiento
    Vector2 speed;
    speed.x = speed.y = 0.0f;

    // Limites
    float top = (aim->height / 2.0f);
    float left = (aim->width / 2.0f);
    float bottom = (bg2->height - top);
    float right = (bg2->width - left);

    // Control por teclado
    if (ngn->input->key_ARROW_UP->held) speed.y = -MOVE_SPEED;
    if (ngn->input->key_ARROW_DOWN->held) speed.y = MOVE_SPEED;
    if (ngn->input->key_ARROW_LEFT->held) speed.x = -MOVE_SPEED;
    if (ngn->input->key_ARROW_RIGHT->held) speed.x = MOVE_SPEED;

    // Mueve la mirilla
    aim->Translate(speed.x, speed.y);

    // Limites de la mirilla
    if (aim->position.x < left) aim->position.x = left;
    if (aim->position.x > right) aim->position.x = right;
    if (aim->position.y < top) aim->position.y = top;
    if (aim->position.y > bottom) aim->position.y = bottom;

}



/*** Gestor de viewports ***/
void Demo::ViewportManager() {

    // Posicion del Viewport central con el mouse
    Vector2I32 position;
    position.x = ngn->input->mouse.x - (ngn->graphics->viewport_list[3].w / 2);
    position.y = ngn->input->mouse.y - (ngn->graphics->viewport_list[3].h / 2);
    ngn->graphics->ViewportPosition(3, position);

    // Actualiza la camara 2D en cada viewport
    ngn->graphics->SelectViewport(0);
    ngn->camera->Update();
    ngn->graphics->SelectViewport(1);
    ngn->camera->Update();
    ngn->graphics->SelectViewport(2);
    ngn->camera->Update();
    ngn->graphics->SelectViewport(3);
    ngn->camera->Update();

}



/*** Configuracion de la pantalla y filtros ***/
void Demo::ScreenConfig() {

    // Modo de pantalla con F10
    if (ngn->input->key_F10->down) {
        if (ngn->graphics->screen_mode != NGN_SCR_FULLSCREEN) {
            ngn->graphics->SetMode(NGN_SCR_FULLSCREEN);
        } else {
            ngn->graphics->SetMode(NGN_SCR_WINDOW);
        }
    }

    // Filtrado general con F9
    if (ngn->input->key_F9->down) ngn->graphics->SetFiltering(!ngn->graphics->filtering);

    // Filtrado local del viewport 0 con F5 (viewport superior)
    if (ngn->input->key_F5->down) ngn->graphics->ViewportLocalFilter(0, !ngn->graphics->viewport_list[0].local_filter);
    // Filtrado local del viewport 1 con F6 (viewport inferior derecho)
    if (ngn->input->key_F6->down) ngn->graphics->ViewportLocalFilter(1, !ngn->graphics->viewport_list[1].local_filter);
    // Filtrado local del viewport 2 con F7 (viewport inferior izquierdo) (zoom out)
    if (ngn->input->key_F7->down) ngn->graphics->ViewportLocalFilter(2, !ngn->graphics->viewport_list[2].local_filter);
    // Filtrado local del viewport 3 con F8 (viewport movil) (zoom in)
    if (ngn->input->key_F8->down) ngn->graphics->ViewportLocalFilter(3, !ngn->graphics->viewport_list[3].local_filter);

}



/*** GUI ***/
void Demo::Gui() {

    // Borra la capa
    text->Cls();

    // Prepara el texto
    std::string t = "";
    t +=  "F5 - TOP VIEWPORT FILTER: ";
    t += (ngn->graphics->viewport_list[0].local_filter) ? "ON":"OFF";
    t += "\n";
    t +=  "F6 - RIGHT VIEWPORT FILTER: ";
    t += (ngn->graphics->viewport_list[1].local_filter) ? "ON":"OFF";
    t += "\n";
    t +=  "F7 - LEFT VIEWPORT FILTER: ";
    t += (ngn->graphics->viewport_list[2].local_filter) ? "ON":"OFF";
    t += "\n";
    t +=  "F8 - FLOATING VIEWPORT FILTER: ";
    t += (ngn->graphics->viewport_list[3].local_filter) ? "ON":"OFF";
    t += "\n";
    t +=  "F9 - MAIN FILTER: ";
    t += (ngn->graphics->filtering) ? "ON":"OFF";
    t += "\n";
    t +=  "F10 - FULL SCREEN: ";
    t += (ngn->graphics->screen_mode == NGN_SCR_FULLSCREEN) ? "ON":"OFF";
    t += "\n";
    t +=  "F12 - TAKE SCREENSHOT";
    t += "\n";

    // Imprime el texto
    text->Print(t);

}



/*** Realiza una captura de pantalla ***/
void Demo::TakeScreenshot() {

    if (ngn->input->key_F12->down) {
        // Lee la marca de tiempo
        current_time = time(NULL);
        // Conviertela al formato TM
        datetime = localtime(&current_time);
        // Crea el nombre de archivo
        std::string filename = "screenshots/scr_";
        filename += ngn->toolbox->Int2String((datetime->tm_year + 1900), 4, "0");
        filename += ngn->toolbox->Int2String((datetime->tm_mon + 1), 2, "0");
        filename += ngn->toolbox->Int2String(datetime->tm_mday, 2, "0");
        filename += "_";
        filename += ngn->toolbox->Int2String(datetime->tm_hour, 2, "0");
        filename += ngn->toolbox->Int2String(datetime->tm_min, 2, "0");
        filename += ngn->toolbox->Int2String(datetime->tm_sec, 2, "0");
        filename += ".png";
        ngn->graphics->ScreenShot(filename, overlay, 0XD0);
        // Efecto de sonido
        ngn->sound->PlaySfx(shutter);
    }

}


