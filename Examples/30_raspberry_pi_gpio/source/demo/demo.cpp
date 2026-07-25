/******************************************************************************

    N'gine Lib for C++
    Ejemplo de acceso al GPIO de Raspberry PI. Nota: Requiere hardware real
    (Programa)

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
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
#include "demo.h"



/*** Constructor de la clase ***/
Demo::Demo() {

    // Prepara los punteros de datos
    bg_data = nullptr;
    bg = nullptr;
    led_spr_data = nullptr;
    for (uint8_t k = 0; k < GPIO_LEDS; k ++) led_spr[k] = nullptr;

    // Variables de control
    timer = 0;
    cnt = 0;

}



/*** Destructor de la clase ***/
Demo::~Demo() {

    // Elimina los punteros de datos
    delete bg; bg = nullptr;
    delete bg_data; bg_data = nullptr;
    for (uint8_t k = 0; k < GPIO_LEDS; k ++) {
        delete led_spr[k];
        led_spr[k] = nullptr;
    }
    delete led_spr_data;
    led_spr_data = nullptr;

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

    // Inicia el GPIO
    GpioInit();

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

    // Fondo
    bg_data = ngn->load->Texture("data/rpi_gpio_bg.png");
    if (!bg_data) return false;

    // Led
    led_spr_data = ngn->load->Sprite("data/rpi_gpio_led_32x32.spr");
    if (!led_spr_data) return false;

    // Archivos cargados con exito
    return true;

}



/*** Crea los elementos del programa ***/
void Demo::Create() {

    // Fondo
    bg = new NGN_Texture(bg_data, 0, 0);

    // Leds
    const int32_t START_X = 1096;
    const int32_t START_Y = 287;
    const int32_t X_GAP = 48;
    const int32_t Y_GAP = 145;
    for (uint8_t k = GPIO_MIN; k <= GPIO_MAX; k ++) {
        int32_t p = GPIO_LED_PIN[k];
        int32_t x = (START_X - (((p - 1) / 2) * X_GAP));
        int32_t y = ((p % 2) == 0) ? (START_Y + Y_GAP) : START_Y;
        led_spr[k] = new NGN_Sprite(led_spr_data, x, y);
        led_spr[k]->frame = 0;
    }

    // Valores iniciales
    timer = DELAY;
    cnt = GPIO_MIN;

    // Led inicial
    ngn->gpio->DigitalWrite(GPIO_ORDER[cnt], true);

}



/*** Logica del programa ***/
void Demo::Update() {

    // Actualiza el temporizador de espera
    timer --;
    if (timer > 0) return;
    timer = DELAY;

    // Apaga el puerto del led actual
    ngn->gpio->DigitalWrite(GPIO_ORDER[cnt], false);

    // Pasa al siguiente led
    cnt ++;
    if (cnt > GPIO_MAX) cnt = GPIO_MIN;

    // Enciende el puerto del siguiente led
    ngn->gpio->DigitalWrite(GPIO_ORDER[cnt], true);

    // Actualiza la interfaz en pantalla segun el estado de los puertos
    for (uint8_t k = GPIO_MIN; k <= GPIO_MAX; k ++) {
        if (k == GPIO_ORDER[cnt]) {
            led_spr[k]->frame = 1;
        } else {
            led_spr[k]->frame = 0;
        }
    }
    
    // Solo en modo debug - Muestra el ID de puerto activo actualmente
    #if defined (MODE_DEBUG)
        std::cout << "GPIO: " << (int32_t)GPIO_ORDER[cnt] << std::endl;
    #endif

}



/*** Render de los elementos graficos ***/
void Demo::Render() {

    // Bg
    ngn->render->Texture(bg);

    // Leds
    for (uint8_t k = GPIO_MIN; k <= GPIO_MAX; k ++) ngn->render->Sprite(led_spr[k]);

}



/*** Inicia el GPIO ***/
void Demo::GpioInit() {

    // Solo en modo debug - Muestra si la interfaz GPIO esta disponible
    #if defined (MODE_DEBUG)
        std::string available = ngn->gpio->IsAvailable() ? "Yes":"No";
        std::cout << "GPIO available: " << available << std::endl;
    #endif

    // Configura los puertos a usar en modo "salida" (OUTPUT) y por defecto, dejalos "apagados" (LOW)
    for (uint8_t k = GPIO_MIN; k <= GPIO_MAX; k ++) {
        ngn->gpio->PortMode(k, NGN_GPIO_MODE.output);
        ngn->gpio->DigitalWrite(k, false);
    }

}
