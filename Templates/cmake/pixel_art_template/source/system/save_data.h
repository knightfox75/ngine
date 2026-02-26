/******************************************************************************

    Pixel Art Project Template: Gestion de los datos de guardado

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



#ifndef SAVE_DATA_H_INCLUDED
#define SAVE_DATA_H_INCLUDED



/*** Includes ***/
// Includes de C++
#include <string>
// Includes de la libreria
#include <ngn.h>
// Includes del proyecto
#include "shared_data.h"



/*** Declaracion de la clase ***/
class SaveData {

    public:

        // Devuelve la instancia
        static SaveData* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();

        // Carga los datos
        bool Load();

        // Salva los datos
        bool Save();


    private:

        // Constructor de la clase
        SaveData();

        // Destructor de la clase
        ~SaveData();

        // Puntero de memoria a la instancia
        static SaveData* instance;

        // Ruta al archivo de guardado
        std::string savefile_path;


        // Acceso a los datos compartidos
        SharedData* shared_data;

        // Copia de los datos de opciones
        struct GameDataStruct {
            uint32_t checksum;                              // Checksum
            SharedData::GameOptionsStruct game_options;     // Opciones del juego
            SharedData::GameRecordsStruct game_records;     // Puntuaciones y logros del juego
        };
        GameDataStruct game_data;

        // Crea los datos de guardado iniciales
        void Default();

        // Codifica/decodifica los datos (XOR simple)
        void XorData(std::vector<uint8_t> &data, uint32_t key, bool rev);


};



#endif // SAVE_DATA_H_INCLUDED
