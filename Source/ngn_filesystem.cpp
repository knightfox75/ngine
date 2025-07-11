/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0-wip_0x03 ***
    Funciones del sistema de archivos

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2025 by Cesar Rincon "NightFox"

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal	in the Software without restriction,
	including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software,
	and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be
	included in all	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER	LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************************/



/*** Includes ***/
// C++
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
//#include <algorithm>

// Libreria
#include "ngn.h"



/*** Constructor ***/
NGN_FileSystem::NGN_FileSystem() {

    // Datos del archivo empaquetado
    package_file = "";
    package_key = "";
    current_package_file = "";

    // Prepara la FAT
    fat.clear();

    // Clave
    key_values.clear();

}



/*** Destructor ***/
NGN_FileSystem::~NGN_FileSystem() {

    // Elimina la FAT
    fat.clear();

    // Elimina la clave
    key_values.clear();

}



/*** Metodo para cargar un archivo desde un archivo empaquetado a un buffer temporal en RAM ***/
int32_t NGN_FileSystem::LoadFileFromPakage(std::string filepath, std::vector<uint8_t> &buffer) {

    // Prepara el buffer temporal para cargar el archivo
    buffer.clear();

    // Busca el archivo en la FAT
    /*
    int32_t _id = -1;
    for (uint32_t i = 0; i < fat.size(); i ++) {
        // Si el archivo coincide...
        if (filepath == fat[i].file_name) {
            _id = (int32_t)i;
            break;
        }
    }
    */

    // Busca el archivo en la FAT (modo indexado)
    int32_t _id = -1;
    int32_t r = 0;
    uint32_t p_begin = 0;
    uint32_t p_end = 0;
    uint32_t position = 0;

    // Hay al menos un elemento valido en la FAT?
    if (fat.size() > 0) {
        // Parametros iniciales de la busqueda
        p_begin = 0;
        p_end = (fat.size() - 1);
        position = p_begin + (uint32_t)((p_end - p_begin) / 2);
        // Busqueda indexada del archivo
        do {
            // Analisis del archivo
            r = filepath.compare(fat[position].file_name);
            if (r == 0) {
                // Archivo encontrado
                _id = (int32_t)position;
                break;
            } else {
                // Nueva posicion, dependiendo de si el nombre esta por detras o por delante en el orden de la FAT
                if (r > 0) {
                    p_begin = position;
                } else {
                    p_end = position;
                }
                position = p_begin + (uint32_t)((p_end - p_begin) / 2);
            }
        } while ((p_end - p_begin) > 1);
        // Paso adicional (ultimas 2 opciones)
        if (_id < 0) {
            if (filepath.compare(fat[p_begin].file_name) == 0) {
                _id = (int32_t)p_begin;
            } else if (filepath.compare(fat[p_end].file_name) == 0) {
                _id = (int32_t)p_end;
            }
        }
    }

    // Si el nodo no se ha encontrado, error
    if (_id < 0) {
        ngn->log->Message("[NGN_FileSystem error] <" + filepath + "> not found.");
        return -1;
    }

    // Si el nodo es valido, guardalo
    uint32_t node_id = (uint32_t)_id;

    // Prepara el checksum
    std::vector<uint8_t> checksum;
    checksum.clear();

    // Abre el fichero de origen
    std::ifstream file;                                                 // Stream de acceso al archivo
    const char* filename = package_file.c_str();                        // General el nombre de archivo
    file.open(filename, std::ifstream::in | std::ifstream::binary);     // Abre el archivo en modo lectura
    if (!file.is_open()) {
        ngn->log->Message("[NGN_FileSystem error] Can't open <" + package_file + "> package for read.");
        return -1;   // Si el archivo no puede abrirse, sal
    }

    // Coloca el cabezal de lectura en el offset correcto
    file.seekg(fat[node_id].file_offset);
    // Prepara el buffer
    buffer.resize(fat[node_id].file_size);
    // Copia los datos del archivo de origen al de buffer
    file.read((char*)&buffer[0], fat[node_id].file_size);
    // Cierra el archivo
    file.close();

    // Si los datos tienen clave de encriptacion, desencriptalos
    if (package_key.length() > 0) DecryptData(buffer);

    // Calcula el checksum del archivo extraido y comparalo con el alamcenado en el nodo
    checksum = Checksum(buffer);                            // Calcula el checksum del archivo
    for (uint8_t n = 0; n < CHK_SIZE; n ++) {               // Verifica que el checksum sea correcto
        if (checksum[n] != fat[node_id].checksum[n]) {      // Si hay error de checksum
            ngn->log->Message("[NGN_FileSystem warning] <" + filepath + "> checksum failed.");
            checksum.clear();
            return -1;
        }
    }

    // Devuelve el numero de bytes leidos
    return fat[node_id].file_size;

}



/*** Establece el uso de un archivo empaquetado ***/
bool NGN_FileSystem::SetPackage(std::string pkg_file, std::string key) {

    // Verifica si el package ha sido ya seleccionado con exito anteriormente
    if ((pkg_file != "") && (pkg_file == current_package_file)) return true;

    // Guarda los datos de acceso al archivo
    package_file = pkg_file;
    package_key = key;

    // Borra la FAT actual
    fat.clear();
    // Borra la clave de encriptado
    key_values.clear();

    // Lee el encabezado del archivo
    switch (ReadPackageHeader()) {
        // Error abriendo el archivo
        case -1:
            ngn->log->Message("[NGN_FileSystem error] Can't open <" + package_file + "> package file for read.");
            return false;
            break;
        // Error de compatibilidad
        case -2:
            ngn->log->Message("[NGN_FileSystem error] <" + package_file + "> package file not compatible.");
            return false;
            break;
        // Error de version
        case -3:
            ngn->log->Message("[NGN_FileSystem error] <" + package_file + "> package file version mismatch.");
            return false;
            break;
    }

    // Extrae la informacion de la FAT y conviertela en nodos
    switch (CreateFatFromPackage()) {
        // Error abriendo el archivo
        case -1:
            ngn->log->Message("[NGN_FileSystem error] Can't open <" + package_file + "> package file for read.");
            return false;
            break;
        // Error de compatibilidad
        case 0:
            ngn->log->Message("[NGN_FileSystem warning] <" + package_file + "> is empty.");
            return false;
            break;
    }

    // Si exite la clave de encriptado, preparala
    if (package_key.length() > 0) KeyToValues();

    // Guarda los valores del package
    current_package_file = package_file;

    // Package establecido con exito
    return true;

}



/*** Lee el encabezado del archivo empaquetado ***/
int32_t NGN_FileSystem::ReadPackageHeader() {

    // Resultado de la ejecucion
    int32_t r = 0;

    // Borra el contenido de la estructura
    memset(&file_header, 0, sizeof(file_header));

    // Intenta abrir el archivo en modo binario
    const char* path = package_file.c_str();
    std::ifstream file;
    file.open(path, std::ifstream::in | std::ifstream::binary);

    // Si no se puede abrir el archivo...
    if (!file.is_open()) return -1;

    // Lee la cabecera del archivo
    file.read((char*)&file_header, sizeof(file_header));

    // Verifica que el archivo sea compatible
    if (std::string(file_header.magic) != MAGIC_STRING) r = -2;

    // Verifica que la version del programa coincida
    if (file_header.version != VERSION) r = -3;

    // Cierra el archivo
    file.close();

    // Resultado de la lectura
    return r;

}



/*** Crea una tabla de asignacion de archivos en base a los archivos del directorio de entrada (-ifile) ***/
int32_t NGN_FileSystem::CreateFatFromPackage() {

    // Prepara la FAT
    fat.clear();

    // Intenta abrir el archivo en modo binario
    const char* path = package_file.c_str();
    std::ifstream file;
    file.open(path, std::ifstream::in | std::ifstream::binary);

    // Si el archivo no puede abrirse
    if (!file.is_open()) return -1;

    // Crea un nodo temporal
    FatNode node;                       // Nodo
    // Datos de lectura del nodo
    uint32_t next_node_offset;          // Posicion del siguiente nodo
    uint32_t path_length;               // Numero de caracteres de la ruta
    uint32_t file_offset;               // Posicion del archivo
    uint32_t file_size;                 // Tamaño del archivo
    uint8_t checksum[CHK_SIZE];         // Checksum del archivo
    std::vector<uint8_t> file_name;     // Nombre del archivo (ruta incluida)

    // Coloca el cabezal de lectura en el offset donde empieza la fat
    file.seekg(file_header.fat_offset);

    // Lee nodos mientras en offset al siguiente nodo NO sea 0
    do {
        // Lee los datos
        file.read((char*)&next_node_offset, sizeof(uint32_t));
        file.read((char*)&path_length, sizeof(uint32_t));
        file.read((char*)&file_offset, sizeof(uint32_t));
        file.read((char*)&file_size, sizeof(uint32_t));
        file.read((char*)&checksum[0], (sizeof(uint8_t) * CHK_SIZE));
        file_name.clear();
        file_name.resize(path_length);
        file.read((char*)&file_name[0], path_length);
        // Copialos al nodo temporal
        node.next_node_offset = next_node_offset;
        node.path_length = path_length;
        node.file_offset = file_offset;
        node.file_size = file_size;
        for (uint8_t n = 0; n < CHK_SIZE; n ++) node.checksum[n] = checksum[n];
        std::string str(file_name.begin(), file_name.end());
        node.file_name = str;
        // Añade el nodo temporal a la FAT
        fat.push_back(node);
    } while (node.next_node_offset > 0);

    // Cierra el archivo
    file.close();

    // Devuelve el numero de archivo introducidos en la FAT
    return fat.size();

}



/*** Desencriptado de los datos contenidos en un vector ***/
int32_t NGN_FileSystem::DecryptData(std::vector<uint8_t> &data) {

    // Bucle de encriptado
    uint8_t byte = 0;
    uint32_t m = key_values.size();
    uint32_t k = m;
    uint8_t v = 0;
    for (uint32_t id = 0; id < data.size(); id ++) {
        byte = data[id];                        // Guarda el byte
        v = key_values[(id % m)];               // Obten un valor de la clave
        k += v;                                 // Calcula la K segun ese valor
        byte ^= ((k % 0x100) & 0xFF);           // XOR con el K
        RotateRight(byte, v);                   // Rota los bits a la derecha
        byte ^= v;                              // XOR con el valor
        data[id] = byte;                        // Devuelve el byte desencriptado
    }

    // Proceso correcto
    return 0;

}



/*** Convierte la clave a un vector de valores numericos ***/
void NGN_FileSystem::KeyToValues() {

    // Prepara el vector de destino
    key_values.clear();
    key_values.resize(package_key.length());

    // Convierte los caracteres a su valor ASCII
    for (uint32_t i = 0; i < package_key.length(); i ++) key_values[i] = package_key[i];

}



/*** Rota un valor n veces a la izquierda ***/
uint8_t NGN_FileSystem::RotateLeft(uint8_t value, uint32_t positions) {

    // Calcula el valor real de rotaciones
    uint8_t p = (positions % 8);

    // Guarda el ultimo bit
    uint8_t bit = 0;

    // Byte a rotar
    uint8_t byte = value;

    // Bucle de rotacion
    for (uint8_t i = 0; i < p; i ++) {
        // Guarda el BIT 7
        bit = (byte & 0x80);
        // Rota una posicion a la izquierda
        byte = (byte << 1);
        // Añade el BIT7 guardado en la posicion 0
        byte |= ((bit >> 7) & 0x01);
    }

    // Devuelve el resultado de la conversion
    return byte;

}



/*** Rota un valor n veces a la derecha ***/
uint8_t NGN_FileSystem::RotateRight(uint8_t value, uint32_t positions) {

    // Calcula el valor real de rotaciones
    uint8_t p = (positions % 8);

    // Guarda el primer BIT
    uint8_t bit = 0;

    // Byte a rotar
    uint8_t byte = value;

    // Bucle de rotacion
    for (uint8_t i = 0; i < p; i ++) {
        // Guarda el BIT 0
        bit = (byte & 0x01);
        // Rota una posicion a la derecha
        byte = (byte >> 1);
        // Añade el BIT0 guardado en la posicion 7
        byte |= ((bit << 7) & 0x80);
    }

    // Devuelve el resultado de la conversion
    return byte;

}



/*** Genera un checksum de 32 bytes para cada archivo ***/
std::vector<uint8_t> NGN_FileSystem::Checksum(std::vector<uint8_t> &data) {

    // Prepara el vector para los calculos
    std::vector<uint8_t> checksum;
    checksum.resize(CHK_SIZE, 0);

    // Calcula el checksum
    uint32_t offset = 0;
    uint32_t target = 0;
    uint32_t byte = 0;
    do {
        // Calcula el punto final del paquete de datos
        target += CHK_SIZE;
        if (target > data.size()) target = data.size();
        // Checksum para este paquete de datos
        byte = 0;
        for (uint32_t i = offset; i < target; i ++) {
            checksum[byte] ^= data[i];
            byte ++;
        }
        offset = target;
    } while (offset < data.size());

    // Devuelve el resultado
    return checksum;

}
