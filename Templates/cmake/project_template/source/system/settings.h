/******************************************************************************

    Project Template: Configuracion

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

#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED




/*** Parametros de la ventana ***/
#if defined (MODE_DEBUG)
    const std::string WINDOW_TITLE = "N'gine Project Template [Debug]";     // Titulo de la ventana
#else
    const std::string WINDOW_TITLE = "N'gine Project Template";             // Titulo de la ventana
#endif
const uint32_t SCR_WIDTH = 1280;                                            // Resolucion
const uint32_t SCR_HEIGHT = 720;
const bool SHOW_MOUSE = false;          // Estado del puntero del raton
#if defined (MODE_DEBUG)
    const bool FPS_COUNTER = false;     // Contador de frames por segundo (solo en modo debug)
#else
    const bool FPS_COUNTER = false;
#endif



/*** Version del programa ***/
const int32_t VERSION_MAJOR = 0;                // Version mayor
const int32_t VERSION_MINOR = 1;                // Version menor
const int32_t VERSION_PATCH = 0;                // Version parche
const std::string VERSION_METADATA = "alpha";   // Version metadatos




/*** Nombre del archivo de guardado ***/
#if defined (MODE_RELEASE)
    #if defined (OS_WINDOWS)
        const std::string SAVEFILE_PATH = "/Documents/" + WINDOW_TITLE + "/Save/";
        const std::string LOGFILE_PATH = "/Documents/" + WINDOW_TITLE + "/Logs/";
    #elif defined (OS_LINUX)
        const std::string SAVEFILE_PATH = "/.config/" + WINDOW_TITLE + "/Save/";
        const std::string LOGFILE_PATH = "/.config/" + WINDOW_TITLE + "/Logs/";
    #else
        const std::string SAVEFILE_PATH = "";
        const std::string LOGFILE_PATH = "";
    #endif
#else
    const std::string SAVEFILE_PATH = "";
    const std::string LOGFILE_PATH = "";
#endif
const std::string SAVEFILE_NAME = "save.dat";
const std::string LOGFILE_NAME = "logging.txt";
const uint32_t SAVE_KEY = 0x11223344;



/*** Repositorios de recursos ***/
const std::string RS_LOCAL = "local";
const std::string RS_SHARED = "shared";
const std::string RS_PERSISTENT = "persistent";




#endif // SETTINGS_H_INCLUDED
