/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Texto

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
#include <iomanip>
#include <sstream>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "demo.h"

/*** Defines ***/
#define WINDOW_TITLE "N'GINE Text example."
#define SCR_WIDTH   1280
#define SCR_HEIGHT  720



/*** Constructor de la clase ***/
Demo::Demo() {

    // Inicializa los punteros de datos
    bg_data = NULL;
    textbox_data = NULL;
    fnt_banner = NULL;
    fnt_box_aa = NULL;
    fnt_box_px = NULL;
    fnt_cursor = NULL;

    // Inicializa los punteros de objetos
    bg = NULL;
    textbox = NULL;
    banner = NULL;
    cursor = NULL;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Borra los objetos
    delete bg; bg = NULL;
    delete textbox; textbox = NULL;
    delete banner; banner = NULL;
    delete cursor; cursor = NULL;

    // Borra los datos
    delete bg_data; bg_data = NULL;
    delete textbox_data; textbox_data = NULL;
    delete fnt_banner; fnt_banner = NULL;
    delete fnt_box_aa; fnt_box_aa = NULL;
    delete fnt_box_px; fnt_box_px = NULL;
    delete fnt_cursor; fnt_cursor = NULL;

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

    // Carga de archivos
    if (!Load()) return false;

    // Crea la escena
    CreateStage();

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



/*** Carga los archivos necesarios ***/
bool Demo::Load() {

    // Imagen de fondo
    bg_data = ngn->load->Texture("data/bg_sample.png");
    if (bg_data == NULL) return false;

    // Imagen para la caja de texto
    textbox_data = ngn->load->Texture("data/text_box.png");
    if (textbox_data == NULL) return false;

    // Tipografias
    fnt_banner = ngn->load->TrueTypeFont("data/quadrangle.ttf", 48, true, 0xFFFFFF, 3, 0x808080);
    if (fnt_banner == NULL) return false;
    fnt_box_aa = ngn->load->TrueTypeFont("data/nk57.ttf", 24, true);
    if (fnt_box_aa == NULL) return false;
    fnt_box_px = ngn->load->TrueTypeFont("data/nk57.ttf", 24, false);
    if (fnt_box_px == NULL) return false;
    fnt_cursor = ngn->load->TrueTypeFont("data/nk57.ttf", 16, true, 0xFFFFFF, 2, 0x000000);
    if (fnt_cursor == NULL) return false;

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
    banner = new NGN_TextLayer(fnt_banner, NULL, 16, 16, (ngn->graphics->native_w - 32), 64);

    // Selecciona los colores y borra la capa de texto
    banner->InkColor(0xFFFF00);
    banner->Cls();
    banner->Print("This is a font with outline.");

}



/*** Crea el cursor animado ***/
void Demo::CreateCursor() {

    // Crea la capa de texto
    cursor = new NGN_TextLayer(fnt_cursor, NULL, 0.0f, 0.0f, 120, 32);

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
    std::stringstream text;
    text <<
    std::fixed << std::setfill('0') << std::setw(4) << std::setprecision(0) << ngn->input->mouse.x <<
    ":" <<
    std::fixed << std::setfill('0') << std::setw(4) << std::setprecision(0) << ngn->input->mouse.y <<
    std::endl;

    // Actualiza el contenido
    cursor->Cls();
    cursor->Print(text.str());

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
