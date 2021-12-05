/******************************************************************************

    N'gine Lib for C++
    *** Version 1.7.0-beta ***
    Funciones de acceso al disco

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


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



/*** Constructor ***/
NGN_Disk::NGN_Disk() {



}



/*** Destructor ***/
NGN_Disk::~NGN_Disk() {



}



/*** Lee un archivo en formato binario desde el disco y almacenalo en un buffer en RAM ***/
int32_t NGN_Disk::ReadBinaryFile(std::string filepath, std::vector<uint8_t> &buffer) {

    // Convierte la ruta de archivo a constante
    const char* _filepath = filepath.c_str();

    // Prepara el buffer temporal para cargar el archivo
    buffer.clear();
    int32_t length = -1;

    // Intenta abrir el archivo en modo binario
    std::ifstream file;
    file.open(_filepath, std::ifstream::in | std::ifstream::binary);

    // Si has abierto el archivo con exito...
    if (file.is_open()) {
        // Calcula el tamaño del archivo
        file.seekg(0, file.end);        // Avanza hasta el final del archivo
        length = file.tellg();          // Consulta el numero de bytes recorridos
        file.seekg(0, file.beg);        // Rebobina el archivo
        // Dimensiona el buffer
        buffer.resize(length);
        // Lee el archivo y coloca los datos en el buffer
        file.read((char*)&buffer[0], length);
        // Cierra el archivo
        file.close();
    } else {
        ngn->log->Message("[NGN_Disk error] <" + filepath + "> not found.");
    }

    // Devuelve el numero de bytes leidos (-1 == no se ha podido abrir el archivo)
    return length;

}



/*** Escribe un archivo en formato binario al disco desde un buffer en RAM ***/
int32_t NGN_Disk::WriteBinaryFile(std::string filepath, std::vector<uint8_t> &buffer) {

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
std::string NGN_Disk::ReadTextFile(std::string filepath) {

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
bool NGN_Disk::ReadTextFile(std::string filepath, std::vector<std::string> &lines) {

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
int32_t NGN_Disk::WriteTextFile(std::string filepath, std::string text, bool append) {

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
int32_t NGN_Disk::CheckFile(std::string path) {

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
int32_t NGN_Disk::MakePath(std::string path) {

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
