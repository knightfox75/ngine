/******************************************************************************

    N'gine Lib for C++
    *** Version 1.22.0-wip_0x04 ***
    Funciones de acceso al disco

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016 - 2026 by Cesar Rincon "NightFox"

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
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
// C
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// Libreria
#include "ngn.h"



/*** Puntero de la instancia a nullptr ***/
NGN_Disk* NGN_Disk::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
NGN_Disk* NGN_Disk::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Disk();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Disk::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Constructor ***/
NGN_Disk::NGN_Disk() {



}



/*** Destructor ***/
NGN_Disk::~NGN_Disk() {



}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Disk::BootUp() {
}



/*** Lee un archivo en formato binario (o un fragmento de este) desde el disco y almacenalo en un buffer en RAM ***/
int32_t NGN_Disk::ReadBinaryFile(const std::string& filepath, std::vector<uint8_t>& buffer, uint32_t offset, uint32_t length) {

    // Prepara el buffer temporal limpiandolo desde el inicio
    buffer.clear();

    // Intenta abrir el archivo en modo binario
    std::ifstream file;
    file.open(filepath.c_str(), std::ifstream::in | std::ifstream::binary);

    // Si no se pudo abrir, registramos el error y salimos
    if (!file.is_open()) {
        ngn->log->Message("[NGN_Disk error] <" + filepath + "> not found.");
        return -1;
    }

    // Calcula el tamaño real del archivo interrogando al sistema de archivos
    file.seekg(0, std::ios::end);               // Avanza hasta el final del archivo
    int64_t file_size = (int64_t)file.tellg();  // Consulta el numero total de bytes (usamos 64 bits temporalmente por seguridad)
    
    // Si el archivo esta vacio fisicamente (0 bytes), cerramos de forma segura
    if (file_size <= 0) {
        file.close();
        return 0;
    }

    // Normaliza el offset: Si no se especifico, asume 0 (inicio del archivo)
    uint32_t actual_offset = (offset == NGN_DEFAULT_VALUE) ? 0 : offset;

    // Proteccion: Si el offset de inicio solicitado esta fuera de los limites del archivo
    if (actual_offset >= (uint32_t)file_size) {
        file.close();
        return -1;
    }

    // Calcula el tamaño maximo que se puede leer desde el offset hasta el final
    uint32_t max_length = (uint32_t)file_size - actual_offset;
    
    // Normaliza el tamaño de bloque a leer:
    // Si no se especifico length, lee hasta el final. Si se especifico, previene desbordamientos.
    uint32_t chunk_size = max_length;
    if (length != NGN_DEFAULT_VALUE) {
        chunk_size = (length > max_length) ? max_length : length;
    }

    // Salida temprana para prevenir comportamiento indefinido si el bloque final resulto ser de 0 bytes
    if (chunk_size == 0) {
        file.close();
        return 0;
    }

    // Dimensiona el buffer para que encaje exactamente con lo que vamos a leer
    buffer.resize(chunk_size);

    // Coloca el cabezal de lectura en el offset solicitado
    file.seekg((std::streamoff)actual_offset, std::ios::beg);
    
    // Lee el bloque del archivo y lo coloca directamente en la memoria cruda del vector
    file.read((char*)buffer.data(), chunk_size);
    
    // Extrae cuantos bytes consiguio leer realmente la controladora de disco
    int32_t bytes_read = (int32_t)file.gcount();
    
    // Cierra el archivo liberando el handle del sistema
    file.close();

    // Si por algun error del SO o desconexion del disco se leyeron menos bytes, recorta el buffer
    if (bytes_read >= 0 && (uint32_t)bytes_read < chunk_size) {
        buffer.resize(bytes_read);
    }

    // Devuelve el numero de bytes leidos (-1 indicaria error antes de leer)
    return bytes_read;
    
}



/*** Escribe un archivo en formato binario al disco desde un buffer en RAM ***/
int32_t NGN_Disk::WriteBinaryFile(const std::string& filepath, std::vector<uint8_t> &buffer) {

    // Convierte la ruta de archivo a constante
    const char* _filepath = filepath.c_str();

    // Intenta abrir el archivo en modo escritura
    std::ofstream file;
    file.open(_filepath, std::ofstream::out | std::ofstream::binary);

    // Si no puedes abrir el archivo, intenta crear antes la ruta
    if (!file.is_open()) {
        // Intenta generar la estructura de directorios, en caso de no existir
        MakePath(filepath);
        // Y vuelve a intentar abrir el archivo para su escritura
        file.open(_filepath, std::ofstream::out | std::ofstream::binary);
        // Si continua sin poder abrirse, sal informando del error
        if (!file.is_open()) {
            ngn->log->Message("[NGN_Disk error] Can't open <" + filepath + "> for writing.");
            return -1;
        }
    }

    // Guarda el tamaño del buffer a escribir
    int32_t length = buffer.size();

    // Escribe los datos del buffer en el archivo
    file.write((char*)&buffer[0], length);

    // Cierra el archivo
    file.close();

    // Verifica que el archivo existe y tiene el tamaño esperado
    if (CheckFile(filepath) != length) {
        ngn->log->Message("[NGN_Disk error] Error writing <" + filepath + ">, size mismatch.");
        return -1;
    }

    // Devuelve el numero de bytes escritos (-1 == no se ha podido crear o escribir el archivo)
    return length;

}



/*** Lee un archivo en formato de texto desde el disco y almacenalo en un string (Primera sobrecarga) ***/
std::string NGN_Disk::ReadTextFile(const std::string& filepath) {

    // Convierte la ruta de archivo a constante
    const char* _filepath = filepath.c_str();

    // Destino de los datos
    std::string text = "";

    // Intenta abrir el archivo en modo texto
    std::fstream file;
    file.open(_filepath, std::fstream::in);

    // Si no puedes abrir el archivo, informa del error y sal
    if (!file.is_open()) {
        ngn->log->Message("[NGN_Disk error] <" + filepath + "> not found.");
        return text;
    }

    // Lee los datos del archivo
    std::string line = "";
    while (std::getline(file, line)) {
        text += line;
        text += '\n';
    }

    // Cierra el archivo
    file.close();

    // Y devuelve los datos
    return text;

}



/*** Lee un archivo en formato de texto desde el disco y almacena las lineas en un vector de strings (Segunda sobrecarga) ***/
bool NGN_Disk::ReadTextFile(const std::string& filepath, std::vector<std::string> &lines) {

    // Convierte la ruta de archivo a constante
    const char* _filepath = filepath.c_str();

    // Intenta abrir el archivo en modo texto
    std::fstream file;
    file.open(_filepath, std::fstream::in);

    // Si no puedes abrir el archivo, informa del error y sal
    if (!file.is_open()) {
        ngn->log->Message("[NGN_Disk error] <" + filepath + "> not found.");
        return false;
    }

    // Lee los datos del archivo
    std::string line = "";
    lines.clear();
    while (std::getline(file, line)) lines.push_back(line);

    // Cierra el archivo
    file.close();

    // Y devuelve los datos
    return true;

}



// Escribe un archivo en formato de texto al disco desde un string
int32_t NGN_Disk::WriteTextFile(const std::string& filepath, std::string text, bool append) {

    // Convierte la ruta de archivo a constante
    const char* _filepath = filepath.c_str();

    // Intenta abrir el archivo en modo texto
    std::fstream file;
    if (append) {
        file.open(_filepath, std::fstream::out | std::fstream::app);
    } else {
        file.open(_filepath, std::fstream::out | std::fstream::trunc);
    }

    // Si no puedes abrir el archivo, intenta crear antes la ruta
    if (!file.is_open()) {
        // Intenta generar la estructura de directorios, en caso de no existir
        MakePath(filepath);
        // Y vuelve a intentar abrir el archivo para su escritura
        if (append) {
            file.open(_filepath, std::fstream::out | std::fstream::app);
        } else {
            file.open(_filepath, std::fstream::out | std::fstream::trunc);
        }
        // Si continua sin poder abrirse, sal informando del error
        if (!file.is_open()) {
            ngn->log->Message("[NGN_Disk error] Can't open <" + filepath + "> for writing.");
            return -1;
        }
    }

    // Escribe los datos
    file << text;

    // Cierra el archivo
    file.close();

    // Y devuelve el numero de caracteres escritos (saltos de linea incluidos)
    return text.size();

}



/*** Verifica que el archivo existe y devuelve su tamaño ***/
int32_t NGN_Disk::CheckFile(const std::string& path) {

    // Convierte la ruta de archivo a constante
    const char* _path = path.c_str();

    // Tamaño del archivo
    int32_t length = -1;

    // Intenta abrir el archivo en modo binario
    std::ifstream file;
    file.open(_path, std::ifstream::in | std::ifstream::binary);

    // Segun si has podido o no abrirlo...
    if (file.is_open()) {
        file.seekg(0, file.end);            // Avanza hasta el final del archivo
        length = file.tellg();              // Consulta el numero de bytes recorridos
        file.close();                       // Cierra el archivo
    }

    // Devuelve el tamaño del archivo [-1 no existe]
    return length;

}



/*** Crea un directorio a partir de un path, si este no existe ***/
int32_t NGN_Disk::MakePath(const std::string& path) {

    // Caracter de referencia
    std::string getchar = "";

    // Genera una lista de directorios
    std::vector<std::string> directory;
    directory.clear();
    for (uint32_t i = 0; i < path.length(); i ++) {
        getchar = path.at(i);
        if (getchar == "/") {
            directory.push_back(path.substr(0, i));
        }
    }

    // Genera un path sin el nombre de archivo
    for (uint32_t i = 0; i < directory.size(); i ++) {
        if (directory[i] == ".") continue;
        //if (i == (directory.size() - 1)) std::cout << "Attempting to create [" << directory[i] << "]" << std::endl;
        const char* p = directory[i].c_str();
        #if defined (OS_WINDOWS)
            mkdir(p);
        #elif defined (OS_LINUX)
            mkdir(p, 0777);
        #else
            mkdir(p);
        #endif
    }

    // Devuelve el resultado
    return directory.size();

}
