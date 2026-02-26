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



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "settings.h"
#include "save_data.h"
#include "shared_data.h"
#include "defines.h"



/*** Puntero de la instancia a nullptr ***/
SaveData* SaveData::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
SaveData* SaveData::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new SaveData();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void SaveData::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Constructor de la clase ***/
SaveData::SaveData() {

    // Genera la ruta del archivo de guardado
    savefile_path = "";

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
        // En caso de estar en modoe DEBUG, indica la ruta como nula
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
        std::string savefile_folder = SAVEFILE_PATH;
        const char space = 0x20;            // Busca el caracter " "
        const char underscore = 0x5F;       // Y cambialo por el caracter "_"
        std::replace(savefile_folder.begin(), savefile_folder.end(), space, underscore);
        // Genera el path a la ruta del archivo de guardado
        savefile_path += user_folder;
        savefile_path += savefile_folder;
    }

    // Añade el nombre de archivo a la ruta generada
    savefile_path += SAVEFILE_NAME;

}



/*** Destructor de la clase ***/
SaveData::~SaveData() {

    // Cancela el vinculo a la instancia
    shared_data = nullptr;

}



/*** Procesos iniciales despues de crear la instancia ***/
void SaveData::BootUp() {

    // Acceso a los datos de guardado
    shared_data = SharedData::GetInstance();

}



/*** Carga los datos ***/
bool SaveData::Load() {

    // Verifica que el archivo existe, de no existir, carga los valores por defecto y crea uno
    if (ngn->disk->CheckFile(savefile_path) < 0) {
        ngn->log->Message("Save file not found. Creating a new one.");
        // Crea los datos por defecto del archivo
        Default();
        // Intenta crear el archivo, en caso de error informa y sal
        if (!Save()) return false;
    }

    // Si el archivo existe, intenta cargar los datos
    std::vector<uint8_t> buffer;
    buffer.clear();
    if (ngn->disk->ReadBinaryFile(savefile_path, buffer) < 0) {
        ngn->log->Message("Error reading save file. Default values will be loaded.");
        Default();
        shared_data->savefile_available = false;
        return false;
    }

    // Descifrado
    XorData(buffer, SAVE_KEY, true);

    // Con los datos cargados, copialos a la estructura principal
    memcpy((uint8_t*)&game_data, (uint8_t*)&buffer[0], buffer.size());

    // Calcula el checksum, si falla, genera un nuevo archivo de guardado
    uint32_t chk = 0;
    for (uint32_t i = 4; i < buffer.size(); i ++) chk += (buffer[i] == 0) ? (0xFF - i):buffer[i];
    if (game_data.checksum != chk) {
        ngn->log->Message("Save file checksum error. Creating a new one.");
        // Crea los datos por defecto del archivo
        Default();
        // Intenta crear el archivo, en caso de error informa y sal
        if (!Save()) return false;
    }

    // Y ahora copialos a sus respectivas estructuras
    shared_data->game_options = game_data.game_options;
    shared_data->game_records = game_data.game_records;

    // Carga correcta
    shared_data->savefile_available = true;
    return true;

}



/*** Salva los datos ***/
bool SaveData::Save() {

    // Buffer de datos
    std::vector<uint8_t> buffer;
    buffer.clear();

    // Copia los datos de las estructuras
    game_data.game_options = shared_data->game_options;
    game_data.game_records = shared_data->game_records;

    // Copia los datos al buffer
    buffer.resize(sizeof(game_data));
    memcpy((uint8_t*)&buffer[0], (uint8_t*)&game_data, buffer.size());

    // Calcula el checksum
    uint32_t chk = 0;
    for (uint32_t i = 4; i < buffer.size(); i ++) chk += (buffer[i] == 0) ? (0xFF - i):buffer[i];
    memcpy((uint8_t*)&buffer[0], (uint8_t*)&chk, 4);

    // Cifrado
    XorData(buffer, SAVE_KEY, false);

    // Guarda los datos de las opciones del juego
    if (ngn->disk->WriteBinaryFile(savefile_path, buffer) < 0) {
        shared_data->savefile_available = false;
        ngn->log->Message("Save file creation failed. Save data function will not be available.");
        return false;
    }

    // Salvado correcto
    shared_data->savefile_available = true;
    return true;

}



/*** Genera los datos por defecto ***/
void SaveData::Default() {

    // Opciones del juego
    game_data.game_options.main_volume = 10;
    game_data.game_options.music_volume = 10;
    game_data.game_options.effects_volume = 10;
    game_data.game_options.screen_mode = 1;
    game_data.game_options.bilinear_filtering = 0;
    game_data.game_options.v_sync = 1;
    shared_data->game_options = game_data.game_options;

    // Puntuaciones y records
    game_data.game_records.dummy_data1 = 0x00;
    game_data.game_records.dummy_data2 = 0xFF;
    game_data.game_records.dummy_data3 = 0xFF0000FF;
    game_data.game_records.dummy_data4 = 0x00FFFF00;
    shared_data->game_records = game_data.game_records;

}



/*** Codifica/decodifica los datos (XOR simple) ***/
void SaveData::XorData(std::vector<uint8_t> &data, uint32_t key, bool rev) {

    for (uint32_t i = 0; i < data.size(); i ++) {
        if (rev) {
            data[i] ^= (key >> 24) & 0xFF;
            data[i] ^= (key >> 16) & 0xFF;
            data[i] ^= (key >> 8) & 0xFF;
            data[i] ^= key & 0xFF;
        } else {
            data[i] ^= key & 0xFF;
            data[i] ^= (key >> 8) & 0xFF;
            data[i] ^= (key >> 16) & 0xFF;
            data[i] ^= (key >> 24) & 0xFF;
        }
    }

}
