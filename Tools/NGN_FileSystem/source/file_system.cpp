/******************************************************************************

    Sistema de empaquetado de archivos para N'gine
    - Sistema de archivos -

    Proyecto iniciado el 14 de Febrero del 2021
    (c) 2021 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	Sistema de empaquetado de archivos para N'gine is under MIT License

	Copyright (c) 2016 - 2025 by Cesar Rincon "NightFox"

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
#include <vector>
#include <algorithm>

// Includes del proyecto
#include "file_system.h"
#include "defines.h"
#include "message.h"
#include "cryptography.h"



/*** Constructor ***/
FileSystem::FileSystem(int32_t argc, char* args[]) {


    // Guarda el numero de argumentos
    argument_count = argc;

    // General al lista de argumentos
    argument_list.clear();
    argument_list.resize(argument_count);
    for (int32_t i = 0; i < argument_count; i ++) argument_list[i] = args[i];

    // Crea los objetos adicionales
    msg = new Message(argument_list[0]);
    fs = new FsManager();
    crypto = new Cryptography();

    // Flags de argumentos
    arg_help.state = false; arg_help.value = "";
    arg_in_dir.state = false; arg_in_dir.value = "";
    arg_out_dir.state = false; arg_out_dir.value = "";
    arg_in_file.state = false; arg_in_file.value = "";
    arg_out_file.state = false; arg_out_file.value = "";
    arg_key.state = false; arg_key.value = "";

    // Lista con la FAT
    fat.clear();

}



/*** Destructor ***/
FileSystem::~FileSystem() {

    // Elimina la lista de argumentos
    argument_list.clear();

    // Elimina la FAT
    fat.clear();

    // Elimina los objetos adicionales
    delete msg; msg = NULL;
    delete fs; fs = NULL;
    delete crypto; crypto = NULL;

}



/*** Programa principal ***/
int32_t FileSystem::Run() {

    // Resultado de la ejecucion
    int32_t r = ERR_CODE_OK;

    // Texto de cabecera
    msg->AppHeader();

    // Analiza la lista de argumentos
    int8_t arglist = CheckArguments();
    //DebugArgList();

    // Analisis de los posibles errores en la linea de comandos / peticion de ayuda
    if (arglist == 0) {
        // Si no se ha especificado ningun argumento
        msg->QuestionMarkForHelp();
    } else if (arglist < 0) {
        // Uso incorrecto
        msg->UseError();
        msg->UserManual();
        r = ERR_CODE_ARG_INVALID;
    } else if (arg_help.state) {
        // Manual en pantalla
        msg->UserManual();
    } else if (arg_in_dir.state && arg_out_file.state) {
        // Crea un archivo empaquetado
        if (CreateFatFromDir() > 0) {
            if (CreatePackage() < 0) r = ERR_CODE_OUTFILE_FAILS;
        } else {
            r = ERR_CODE_INDIR_EMPTY;
        }
    } else if (arg_in_file.state && arg_out_dir.state) {
        // Extrae los archivos de un empaquetado
        if (CreateFatFromPackage() > 0) {
            if (ExtractPackage() < 0) r = ERR_CODE_EXTRACT_FAILS;
        } else {
            r = ERR_CODE_INFILE_EMPTY;
        }
    }

    // Resultado de la ejecucion
    return r;

}



/*** Analiza la lista de argumentos proporcionados ***/
int32_t FileSystem::CheckArguments() {

    // Si no hay argumentos, muestra el texto de ayuda
    if (argument_count == 1) return 0;

    // Si el numero de argumentos es incorrecto
    if ((argument_count < ARG_MIN_NUM) || (argument_count > ARG_MAX_NUM)) return -1;

    // Flag de argumentos validos
    bool arg = false;
    bool valid = false;

    // Analiza la lista de argumentos
    for (uint32_t i = 1; i < argument_list.size(); i ++) {

        valid = false;

        if (argument_list[i] == ARG_HELP) {                         // Texto de ayuda

            if (arg_help.state) return -1;
            arg_help.state = true;
            valid = true;

        } else if (argument_list[i] == ARG_IN_DIR) {                // Directorio de origen

            if (arg_in_dir.state) return -1;
            if ((i + 1) >= argument_list.size()) return -1;
            arg_in_dir.state = true;
            i ++;
            arg_in_dir.value = argument_list[i];
            valid = true;

        } else if (argument_list[i] ==  ARG_OUT_DIR) {              // Directorio de destino

            if (arg_out_dir.state) return -1;
            if ((i + 1) >= argument_list.size()) return -1;
            arg_out_dir.state = true;
            i ++;
            arg_out_dir.value = argument_list[i];
            valid = true;

        } else if (argument_list[i] ==  ARG_IN_FILE) {             // Archivo de origen

            if (arg_in_file.state) return -1;
            if ((i + 1) >= argument_list.size()) return -1;
            arg_in_file.state = true;
            i ++;
            arg_in_file.value = argument_list[i];
            valid = true;

        } else if (argument_list[i] ==  ARG_OUT_FILE) {             // Archivo de destino

            if (arg_out_file.state) return -1;
            if ((i + 1) >= argument_list.size()) return -1;
            arg_out_file.state = true;
            i ++;
            arg_out_file.value = argument_list[i];
            valid = true;

        } else if (argument_list[i] ==  ARG_KEY) {                  // Clave de encriptado

            if (arg_key.state) return -1;
            if ((i + 1) >= argument_list.size()) return -1;
            arg_key.state = true;
            i ++;
            arg_key.value = argument_list[i];
            valid = true;

        }

        // Si el argumento no se reconoce...
        if (!valid) return -1;

        // Argumento valido?
        arg |= valid;

    }

    // Indica los argumentos se han leido correctamente
    return (arg) ? 1:-1;

}



/*** Crea una tabla de asignacion de archivos en base a los archivos del directorio de entrada (-idir) ***/
int32_t FileSystem::CreateFatFromDir() {

    // Si el path no es valido, sal
    if (arg_in_dir.value.length() < 1) {
        std::cout << "Source path must not be empty." << std::endl;
        return -1;
    }

    // Escanea el directorio especificado en -idir en busca de todos los archivos validos
    std::cout << "Searching for files in [" << arg_in_dir.value << "]." << std::endl;
    std::vector<std::string> file_list = fs->GetListOfFiles(arg_in_dir.value);

    // Si el directorio esta vacio o no se encuentra, sal
    if (file_list.size() == 0) {
        std::cout << "[" << arg_in_dir.value << "] directory is empty or not found." << std::endl;
        return -1;
    }

    // Informa del numero de archivos encontrados
    std::cout << "[" << arg_in_dir.value << "] contains " << file_list.size() << " files." << std::endl;

    // Ordena la lista de archivos alfabeticamente en orden ascendente (A-Z)
    struct {
        bool operator() (std::string a, std::string b) {
            return ((a.compare(b)) < 0);
        }
    } comp;
    std::cout << "Sorting file list entries in ascending order (A to Z)... ";
    std::sort(file_list.begin(), file_list.end(), comp);
    std::cout << "Ok." << std::endl;
    //for (uint32_t k = 0; k < fat.size(); k ++) std::cout << fat[k].file_name << std::endl;

    // Prepara la FAT
    fat.clear();

    // Crea el nodo temporal
    FatNode node;
    // Offset del primer nodo despues del encabezado del archivo
    node.next_node_offset = sizeof(file_header);
    // Offset de posicion del archivo (de momento, 0)
    node.file_offset = 0;
    // Estimacion de tamaño de la FAT y de los datos
    fat_size = 0;
    data_size = 0;
    // Checksum del archivo
    std::vector<uint8_t> checksum;
    checksum.clear();
    std::vector<uint8_t> buffer;
    buffer.clear();

    // Recore la lista de archivos, analizando el tamaño de cada uno de ellos y creando el nodo correspondiente en la FAT
    std::cout << "FAT creation started... ";
    for (uint32_t i = 0; i < file_list.size(); i ++) {

        // Prepara el nodo
        node.file_name = file_list[i];                          // Nombre del archivo
        node.path_length = node.file_name.length();             // Numero de caracteres en la ruta
        node.next_node_offset += NODE_SIZE + node.path_length;  // Offset al siguiente nodo

        /*
        std::cout << "****************************************" << std::endl;
        std::cout << "Position: " << i << std::endl;
        std::cout << "Node name: " << node.file_name << std::endl;
        std::cout << "Name length: " << node.path_length << std::endl;
        std::cout << "Node next pos: " << node.next_node_offset << std::endl;
        std::cout << "****************************************" << std::endl;
        */

        // Registra el tamaño del nodo
        fat_size += NODE_SIZE + node.path_length;

        // Intenta abrir el archivo para guardar su tamaño
        const char* path = node.file_name.c_str();

        // Intenta abrir el archivo en modo binario
        std::ifstream file;
        file.open(path, std::ifstream::in | std::ifstream::binary);

        // Segun si has podido o no abrirlo...
        if (file.is_open()) {
            // Calcula el tamaño del archivo
            file.seekg(0, file.end);            // Avanza hasta el final del archivo
            node.file_size = file.tellg();      // Consulta el numero de bytes recorridos
            file.seekg(0, file.beg);            // Rebobina el archivo
            // Lee el archivo para el checksum
            buffer.resize(node.file_size);
            file.read((char*)&buffer[0], node.file_size);
            checksum = Checksum(buffer);
            buffer.clear();
            file.close();                       // Cierra el archivo
            // Registra el tamaño de archivo
            data_size += node.file_size;
            // Registra el checksum del archivo
            for (uint8_t i = 0; i < CHK_SIZE; i ++) node.checksum[i] = checksum[i];
            checksum.clear();
        } else {
            // Error, borra la FAT e informa
            fat.clear();
            std::cout << std::endl;
            std::cout << "Error opening [" << node.file_name << "] for read." << std::endl << "FAT creation aborted." << std::endl;
            return -1;
        }

        // Verifica si es el ultimo nodo para marcarlo (offset a 0)
        if (i == file_list.size() - 1) node.next_node_offset = 0;

        // Añade el nodo creado a la FAT
        fat.push_back(node);

    }
    std::cout << "Ok." << std::endl;

    // Con la FAT creada y los datos analizados, calcula ahora la posicion que ocupara cada archivo en la seccion de datos
    uint32_t offset = sizeof(file_header) + fat_size;
    for (uint32_t i = 0; i < fat.size(); i ++) {
        fat[i].file_offset = offset;
        offset += fat[i].file_size;
    }

    // Fat creada
    std::cout << "FAT successfully created with " << fat.size() << " nodes." << std::endl;
    std::cout << "File header: " << sizeof(file_header) << " bytes." << std::endl;
    std::cout << "FAT length: " << fat_size << " bytes." << std::endl;
    std::cout << "DATA length: " << data_size << " bytes." << std::endl;
    std::cout << "Estimated file size: " << (sizeof(file_header) + fat_size + data_size) << " bytes." << std::endl;
    std::cout << std::endl;

    // Devuelve el numero de archivo introducidos en la FAT
    return fat.size();

}



/*** Crea un empaquetado de datos ***/
int32_t FileSystem::CreatePackage() {

    // Si el path no es valido, sal
    if (arg_out_file.value.length() < 1) {
        std::cout << "Destination path must not be empty." << std::endl;
        return -1;
    }

    // Genera la cabecera del archivo con los datos actuales
    GenerateFileHeader();

    // Intenta crear el archivo de destino
    if (WritePackageFile(arg_out_file.value) < 0) {
        // Error en la creacion
        return -1;
    } else {
        // Creacion correcta
        return 0;
    }

}



/*** Genera el encabezado del archivo empaquetado ***/
void FileSystem::GenerateFileHeader() {

    // Borra el contenido de la estructura
    memset((void*)&file_header, 0, sizeof(file_header));

    // Datos de la cabecera
    file_header.version = VERSION;
    strncpy(file_header.magic, MAGIC_STRING.c_str(), MAGIC_STRING.length());
    file_header.fat_offset = sizeof(file_header);
    file_header.fat_size = fat_size;
    file_header.data_offset = (file_header.fat_offset + file_header.fat_size);
    file_header.data_size = data_size;

}



/*** Graba el archivo del paquete con los datos ***/
int32_t FileSystem::WritePackageFile(std::string filepath) {

    // Convierte la ruta de archivo a constante
    const char* _filepath = filepath.c_str();

    // Intenta crear/abrir el archivo para escritura
    std::ofstream outfile;
    outfile.open(_filepath, std::ofstream::out | std::ofstream::binary);

    // Si no puedes abrir el archivo
    if (!outfile.is_open()) {
        std::cout << "Can't open [" << filepath << "] for writting." << std::endl;
        return -1;
    }

    // Inicio de la escritura del archivo
    std::cout << "Writing data in [" << filepath << "] file." << std::endl;

    // Escribe el encabezado del archivo
    std::cout << "File header... ";
    outfile.write((char*)&file_header, sizeof(file_header));
    std::cout << "Ok." << std::endl;

    // Escribe la tabla de asignacion de archivos (FAT)
    std::cout << "File assignation table (FAT)... ";
    uint32_t next_node_offset;          // Posicion del siguiente nodo
    uint32_t path_length;               // Numero de caracteres de la ruta
    uint32_t file_offset;               // Posicion del archivo
    uint32_t file_size;                 // Tama�o del archivo
    uint8_t checksum[CHK_SIZE];         // Checksum del archivo
    std::string file_name;              // Nombre del archivo (ruta incluida)
    // Lee el contenido de la tabla y escribelo en el archivo
    for (uint32_t i = 0; i < fat.size(); i ++) {
        next_node_offset = fat[i].next_node_offset;
        path_length = fat[i].path_length;
        file_offset = fat[i].file_offset;
        file_size = fat[i].file_size;
        for (uint8_t n = 0; n < CHK_SIZE; n ++) checksum[n] = fat[i].checksum[n];
        file_name = fat[i].file_name;
        outfile.write((char*)&next_node_offset, sizeof(uint32_t));
        outfile.write((char*)&path_length, sizeof(uint32_t));
        outfile.write((char*)&file_offset, sizeof(uint32_t));
        outfile.write((char*)&file_size, sizeof(uint32_t));
        outfile.write((char*)&checksum[0], (sizeof(uint8_t) * CHK_SIZE));
        outfile.write((char*)&file_name[0], path_length);
    }
    std::cout << "Ok." << std::endl;

    // Lee los archivos y copialos al empaquetado
    std::ifstream infile;               // Stream de acceso al archivo
    std::vector<uint8_t> buffer;        // Buffer del archivo
    buffer.clear();                     // Borra el contenido del buffer
    int32_t files_added = 0;
    for (uint32_t i = 0; i < fat.size(); i  ++) {

        // Intenta abrir el archivo
        const char* infile_name = fat[i].file_name.c_str();
        infile.open(infile_name, std::ifstream::in | std::ifstream::binary);

        // Añadiendo el archivo...
        std::cout << "Adding";
        if (arg_key.state) std::cout << " and encrypting";
        std::cout << " [" << fat[i].file_name << "]... ";

        // Si se abre con exito, realiza la copia
        if (infile.is_open()) {
            // Reserva el espacio segun el tama�o del archivo
            buffer.resize(fat[i].file_size);
            // Lee el archivo de origen
            infile.read((char*)&buffer[0], fat[i].file_size);
            // Si se ha solicitado, encripta el archivo
            if (arg_key.state) crypto->EncryptData(buffer, arg_key.value);
            // Escribe el archivo en el destino
            outfile.write((char*)&buffer[0], fat[i].file_size);
            // Borra el contenido del buffer
            buffer.clear();
        } else {
            std::cout << "error." << std::endl;
            outfile.close();
            return -1;
        }

        // Cierra el archivo
        infile.close();
        std::cout << "Ok." << std::endl;
        files_added ++;

    }

    // Cierra el archivo
    outfile.close();

    // Verificacion del tama�o del archivo creado
    int32_t length = -1;
    // Intenta abrir el archivo en modo binario
    infile.open(_filepath, std::ifstream::in | std::ifstream::binary);
    // Si has abierto el archivo con exito...
    if (infile.is_open()) {
        // Calcula el tama�o del archivo
        infile.seekg(0, infile.end);        // Avanza hasta el final del archivo
        length = infile.tellg();            // Consulta el numero de bytes recorridos
        infile.seekg(0, infile.beg);        // Rebobina el archivo
        infile.close();                     // Cierra el archivo
    } else {
        std::cout << "Can't verify [" << filepath << "] after creation." << std::endl;
        return -1;
    }

    // Resumen
    std::cout << std::endl;
    std::cout << "[" << filepath << "] package file created successfully." << std::endl;
    std::cout << "Package file size: " << length << " bytes." << std::endl;
    std::cout << "The package contains " << files_added << " files." << std::endl;
    std::cout << std::endl;

    // Debug
    //DebugShowFat();

    // Creacion correcta
    return 0;

}



/*** Crea una tabla de asignacion de archivos en base a los archivos del directorio de entrada (-ifile) ***/
int32_t FileSystem::CreateFatFromPackage() {

    // Si el path no es valido, sal
    if (arg_in_file.value.length() < 1) {
        std::cout << "You must specify a valid source file." << std::endl;
        return -1;
    }

    // Escanea el archivo especificado en -ifile en busca de todos los archivos empaquetados
    std::cout << "Searching for files in [" << arg_in_file.value << "]." << std::endl;

    int32_t r = ReadFileHeader();
    if (r < 0) {
        std::cout << "[" << arg_in_file.value << "] ";
        switch (r) {
            case -1:
                // Archivo no encontrado
                std::cout << "not found." << std::endl;
                break;
            case -2:
                // Archivo incompatible
                std::cout << "invalid file type." << std::endl;
                break;
        }
        // Sal e informa del error
        return -1;
    }

    // Prepara la FAT
    fat.clear();


    // Lee la FAT

    // Intenta abrir el archivo en modo binario
    const char* path = arg_in_file.value.c_str();
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
    uint32_t file_size;                 // Tama�o del archivo
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

    // Informa del contenido de la FAT
    std::cout << "[" << arg_in_file.value << "] package contains " << fat.size() << " files." << std::endl;
    std::cout << std::endl;

    // Paso de limpieza
    file_name.clear();

    // Devuelve el numero de archivo introducidos en la FAT
    return fat.size();

}



/*** Extrae los archivos de un empaquetado a un directorio ***/
int32_t FileSystem::ExtractPackage() {

    // Abre el fichero de origen
    std::ifstream infile;               // Stream de acceso al archivo
    const char* in_filename = arg_in_file.value.c_str();                        // General el nombre de archivo
    infile.open(in_filename, std::ifstream::in | std::ifstream::binary);        // Abre el archivo en modo lectura
    if (!infile.is_open()) {
        std::cout << "Can't open [" << in_filename << "] for read." << std::endl;
        return -1;   // Si el archivo no puede abrirse, sal
    }

    // Prapara el stream para los archivos de destino
    std::ofstream outfile;

    // Prepara el buffer
    std::vector<uint8_t> buffer;        // Buffer del archivo
    buffer.clear();

    // Prepara el checksum
    std::vector<uint8_t> checksum;
    checksum.clear();

    // Extrae los archivos del empaquetado en base a la informacion de la FAT
    int32_t file_count = 0;
    bool chk_err = false;
    for (uint32_t id = 0; id < fat.size(); id ++) {

        // Reset de los flags
        chk_err = false;

        // Genera el nombre de archivo de salida
        std::string o_filename = arg_out_dir.value + "/" + fat[id].file_name;
        const char* out_filename = o_filename.c_str();
        // Intenta abrir el archivo en modo escritura
        outfile.open(out_filename, std::ofstream::out | std::ofstream::binary);
        // Si no puedes abrir el archivo
        if (!outfile.is_open()) {
            // Si no puede crearse el archivo quizas es que no existe el directorio de destino, intenta crearlo.
            std::cout << "Can't open [" << out_filename << "] for writting." << std::endl;
            std::cout << "Maybe the path doesn't exist, attempting to create it." << std::endl;
            if (fs->MakePath(out_filename) > 0) {
                // Intena abrir el archivo nuevamente despues de la creacion del path
                outfile.open(out_filename, std::ofstream::out | std::ofstream::binary);
                // Si no se ha abierto, informa del error y cancela el proceso
                if (!outfile.is_open()) {
                    std::cout << "Can't open [" << out_filename << "] for writting." << std::endl;
                    // Cierra el archivo de entrada
                    infile.close();
                    return -1;
                }
            } else {
                std::cout << "Path creation failed." << std::endl;
                // Cierra el archivo de entrada
                infile.close();
                return -1;
            }
        }



        // Inicia la extraccion
        std::cout << "Extracting";
        if (arg_key.state) std::cout << " and decrypting";
        std::cout << " [" << out_filename << "]... ";
        // Coloca el cabezal de lectura en el offset correcto
        infile.seekg(fat[id].file_offset);
        // Prepara el buffer
        buffer.resize(fat[id].file_size);

        // Copia los datos del archivo de origen al de destino
        infile.read((char*)&buffer[0], fat[id].file_size);                  // Lee el archivo de origen
        if (arg_key.state) crypto->EncryptData(buffer, arg_key.value);      // Si se ha solicitado, descripta el archivo
        checksum = Checksum(buffer);                                        // Calcula el checksum del archivo
        for (uint8_t n = 0; n < CHK_SIZE; n ++) {                           // Verifica que el checksum sea correcto
            if (checksum[n] != fat[id].checksum[n]) chk_err = true;         // Marca el flag de error
        }
        outfile.write((char*)&buffer[0], fat[id].file_size);                // Escribe el archivo en el destino

        // Cierra el archivo de salida
        outfile.close();

        // Borra el buffer
        buffer.clear();

		// Borra el checksum
		checksum.clear();

        // Resultado
        std::cout << fat[id].file_size << " bytes OK." << std::endl;
        if (chk_err) std::cout << "Warning: [" << out_filename << "] checksum failed." << std::endl;

        // Actualiza el conteo
        file_count ++;

    }

    // Cierra el archivo de origen
    infile.close();

    std::cout << std::endl;
    std::cout << file_count << " files extracted successfully." << std::endl;
    std::cout << std::endl;

    // Devuelve el numero de archivos extraidos
    return file_count;

}



/*** Lee el encabezado del archivo ***/
int32_t FileSystem::ReadFileHeader() {

    // Borra el contenido de la estructura
    memset((void*)&file_header, 0, sizeof(file_header));

    // Intenta abrir el archivo en modo binario
    const char* path = arg_in_file.value.c_str();
    std::ifstream file;
    file.open(path, std::ifstream::in | std::ifstream::binary);

    // Si no se puede abrir el archivo...
    if (!file.is_open()) return -1;

    // Lee la cabecera del archivo
    file.read((char*)&file_header, sizeof(file_header));
    // Verifica que el archivo sea compatible
    if (std::string(file_header.magic) != MAGIC_STRING) return -2;

    // Cierra el archivo
    file.close();

    // Lectura correcta
    return 0;

}



/*** Genera un checksum de 32 bytes para cada archivo ***/
std::vector<uint8_t> FileSystem::Checksum(std::vector<uint8_t> &data) {

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

    // Revuelve el resultado
    return checksum;

}



/*** DEBUG: Imprime la lista de argumentos introducidos ***/
void FileSystem::DebugArgList() {

    std::cout << std::endl;
    std::cout << "Argument list" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Executable name: " << argument_list[0] << std::endl;
    std::cout << ARG_HELP << " " << arg_help.state << " " << arg_help.value << std::endl;
    std::cout << ARG_IN_DIR << " " << arg_in_dir.state << " " << arg_in_dir.value << std::endl;
    std::cout << ARG_OUT_DIR << " " << arg_out_dir.state << " " << arg_out_dir.value << std::endl;
    std::cout << ARG_IN_FILE << " " << arg_in_file.state << " " << arg_in_file.value << std::endl;
    std::cout << ARG_OUT_FILE << " " << arg_out_file.state << " " << arg_out_file.value << std::endl;
    std::cout << ARG_KEY << " " << arg_key.state << " " << arg_key.value << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::endl;

}



/*** DEBUG: Imprime el contenido actual de la FAT ***/
void FileSystem::DebugShowFat() {

    std::cout << std::endl;
    std::cout << "Current FAT contens" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Number of nodes: " << fat.size() << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    for (uint32_t id = 0; id < fat.size(); id ++) {
        std::cout << "Node: " << id << "     Next node offset: " << fat[id].next_node_offset << std::endl;
        std::cout << "File: " << fat[id].file_name << std::endl;
        std::cout << "Size: " << fat[id].file_size << "     Offset: " << fat[id].file_offset << std::endl;
        std::cout << "CHECKSUM: ";
        std::cout << std::hex;
        for (uint8_t n = 0; n < CHK_SIZE; n ++) {
            if ((n % 8) == 0) std::cout << std::endl;
            std::cout << (uint32_t)fat[id].checksum[n];
        }
        std::cout << std::dec;
        std::cout << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    }
    std::cout << std::endl;

}
