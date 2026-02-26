/******************************************************************************

    Pixel Art Project Template: Datos compartidos entre modulos

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



#ifndef SHARED_DATA_H_INCLUDED
#define SHARED_DATA_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <string>
#include <vector>
// Includes de la libreria
#include <ngn.h>
// Includes del programa




/*** Declaracion de la clase ***/
class SharedData {

    public:

        // Devuelve la instancia
        static SharedData* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();

        // Devuelve un string con la version actual del programa
        std::string GetVersion();


        // Configuracion del juego
        struct GameOptionsStruct {
            uint8_t main_volume;
            uint8_t music_volume;
            uint8_t effects_volume;
            uint8_t screen_mode;
            uint8_t bilinear_filtering;
            uint8_t v_sync;
        };
        GameOptionsStruct game_options;

        // Datos de guardado
        struct GameRecordsStruct {
            uint32_t dummy_data1;
            uint32_t dummy_data2;
            uint32_t dummy_data3;
            uint32_t dummy_data4;
        };
        GameRecordsStruct game_records;

        // Funcion de guardado disponible
        bool savefile_available;


        // Assets compartidos en todos el programa
        struct {
            NGN_Texture* black_texture;
        } asset;

        // Carga los archivos compartidos por todo el programa
        bool Load();

        // Crea los recursos compartidos por todo el programa
        void Create();


    private:

        // Constructor de la clase
        SharedData();

        // Destructor de la clase
        ~SharedData();

        // Puntero de memoria a la instancia
        static SharedData* instance;

        // Singletons

};



#endif // SHARED_DATA_H_INCLUDED
