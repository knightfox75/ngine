/******************************************************************************

    Ejemplo del uso de las capas de texto
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
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    bg_data = nullptr;
    textbox_data = nullptr;
    fnt_banner = nullptr;
    fnt_box_aa = nullptr;
    fnt_box_px = nullptr;
    fnt_cursor = nullptr;

    // Inicializa los punteros de objetos
    bg = nullptr;
    textbox = nullptr;
    banner = nullptr;
    cursor = nullptr;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg; bg = nullptr;
    delete textbox; textbox = nullptr;
    delete banner; banner = nullptr;
    delete cursor; cursor = nullptr;

    // Borra los datos
    delete bg_data; bg_data = nullptr;
    delete textbox_data; textbox_data = nullptr;
    delete fnt_banner; fnt_banner = nullptr;
    delete fnt_box_aa; fnt_box_aa = nullptr;
    delete fnt_box_px; fnt_box_px = nullptr;
    delete fnt_cursor; fnt_cursor = nullptr;

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



/*** Carga los archivos necesarios ***/
bool Demo::Load() {

    // Imagen de fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (bg_data == nullptr) return false;

    // Imagen para la caja de texto
    textbox_data = ngn->load->Texture("data/text_box.png");
    if (textbox_data == nullptr) return false;

    // Tipografias
    fnt_banner = ngn->load->TrueTypeFont("data/quadrangle.ttf", 48, true, 0xFFFFFF, 3, 0x808080);
    if (fnt_banner == nullptr) return false;
    fnt_box_aa = ngn->load->TrueTypeFont("data/nk57.ttf", 24, true);
    if (fnt_box_aa == nullptr) return false;
    fnt_box_px = ngn->load->TrueTypeFont("data/nk57.ttf", 24, false);
    if (fnt_box_px == nullptr) return false;
    fnt_cursor = ngn->load->TrueTypeFont("data/nk57.ttf", 16, true, 0xFFFFFF, 2, 0x000000);
    if (fnt_cursor == nullptr) return false;

    // Carga correcta
    return true;

}



/*** Crea la escena ***/
void Demo::CreateStage() {

    // Textura de fondo
    bg = new NGN_Texture(bg_data, 0, 0);

    // Crea la caja de texto
    CreateTextBox();

    // Crea el banner de texto
    CreateBanner();

    // Crea el cursor animado
    CreateCursor();

}



/*** Crea la caja de texto ***/
void Demo::CreateTextBox() {

    // Crea la caja con el tamaño de la textura de fondo
    textbox = new NGN_TextLayer(fnt_box_aa, textbox_data);

    // Posicionala en el centro
    float x = ((ngn->graphics->native_w / 2) - (textbox->width / 2.0f));
    float y = ((ngn->graphics->native_h / 2) - (textbox->height / 2.0f));
    textbox->Position(x, y);

    // Ajusta el padding de la caja
    textbox->Padding(24);
    // Borra el contenido de la caja para aplicar el padding y el fondo
    textbox->Cls();

    // Textos
    textbox->Print("This is an antialiased text with the default parameters.\n");

    textbox->Font(fnt_box_px);
    textbox->Print("This is the same font without anti-alias.\n\n");

    textbox->Font(fnt_box_aa);
    textbox->InkColor(0xFF0000);
    textbox->Print("Now the color of the text is red, ");

    textbox->InkColor(255, 200, 0);
    textbox->Print("but we can use any color we want\n");

    textbox->InkColor(0, 255, 0);
    textbox->Print("like in this little example.\n");

}



/*** Crea el banner de texto ***/
void Demo::CreateBanner() {

    // Crea una capa para el banner superior de texto
    banner = new NGN_TextLayer(fnt_banner, nullptr, 16, 16, (ngn->graphics->native_w - 32), 64);

    // Selecciona los colores y borra la capa de texto
    banner->InkColor(0xFFFF00);
    banner->Cls();
    banner->Print("This is a font with outline.");

}



/*** Crea el cursor animado ***/
void Demo::CreateCursor() {

    // Crea la capa de texto
    cursor = new NGN_TextLayer(fnt_cursor, nullptr, 0.0f, 0.0f, 120, 32);

    // Parametros de la capa
    cursor->Padding(4);
    cursor->InkColor(0xFFFFFF);
    cursor->CanvasColor(255, 200, 100, 128);
    cursor->Cls();

}



/*** Actualizacion del programa ***/
void Demo::Update() {

    // Actualiza los elementos de la escena
    UpdateCursor();
    // Render de la escena
    Render();

}



/*** Actualiza el cursor ***/
void Demo::UpdateCursor() {

    // Calcula la posicion de la capa de texto segun el puntero del raton
    float x = (ngn->input->mouse.x - (cursor->width / 2));
    float y = (ngn->input->mouse.y - (cursor->height / 2));

    // Prepara el contenido de la capa de texto
    std::string text = ngn->toolbox->Int2String(ngn->input->mouse.x, 4, "0");
    text += ":";
    text += ngn->toolbox->Int2String(ngn->input->mouse.y, 4, "0");

    // Actualiza el contenido
    cursor->Cls();
    cursor->Print(text);

    // Posiciona la caja de texto
    cursor->Position(x, y);

}



/*** Renderiza la escena ***/
void Demo::Render() {

    // Renderiza el fondo
    ngn->render->Texture(bg);
    // Renderiza la caja de texto
    ngn->render->TextLayer(textbox);
    // Renderiza el banner de texto
    ngn->render->TextLayer(banner);
    // Renderiza el cursor
    ngn->render->TextLayer(cursor);

}
