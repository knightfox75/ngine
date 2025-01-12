/******************************************************************************

    Sistema de empaquetado de archivos para N'gine
	- Funciones criptograficas -

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
#include <iostream>
#include <string>
#include <vector>

// Includes del proyecto
#include "cryptography.h"
#include "defines.h"



/*** Constructor ***/
Cryptography::Cryptography() {

    // Prepara los vectores
    key_values.clear();

}



/*** Destructor ***/
Cryptography::~Cryptography() {

    // Borra los vectores
    key_values.clear();

}



/*** Encriptacion de los datos contenidos en un vector ***/
int32_t Cryptography::EncryptData(std::vector<uint8_t> &data, std::string key) {

    // Convierte la clave a una secuencia numerica
    KeyToValues(key);

    // Bucle de encriptado
    uint8_t byte = 0;
    uint32_t m = key_values.size();
    uint32_t k = m;
    uint8_t v = 0;
    for (uint32_t id = 0; id < data.size(); id ++) {
        byte = data[id];                        // Guarda el byte
        v = key_values[(id % m)];               // Obten un valor de la clave
        k += v;                                 // Calcula la K segun ese valor
        byte ^= v;                              // XOR con el valor
        RotateLeft(byte, v);                    // Rota los bits a la izquierda n veces segun el valor
        byte ^= ((k % 0x100) & 0xFF);           // XOR con K
        data[id] = byte;                        // Devuelve el byte encriptado
    }

    // Proceso correcto
    return 0;

}



/*** Desencriptado de los datos contenidos en un vector ***/
int32_t Cryptography::DecryptData(std::vector<uint8_t> &data, std::string key) {

    // Convierte la clave a una secuencia numerica
    KeyToValues(key);

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
void Cryptography::KeyToValues(std::string key) {

    // Prepara el vector de destino
    key_values.clear();
    key_values.resize(key.length(), 0);

    // Convierte los caracteres a su valor ASCII
    for (uint32_t i = 0; i < key.length(); i ++) key_values[i] = key[i];

}



/*** Rota un valor n veces a la izquierda ***/
uint8_t Cryptography::RotateLeft(uint8_t value, uint32_t positions) {

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
        // A�ade el BIT7 guardado en la posicion 0
        byte |= ((bit >> 7) & 0x01);
    }

    // Devuelve el resultado de la conversion
    return byte;

}



/*** Rota un valor n veces a la derecha ***/
uint8_t Cryptography::RotateRight(uint8_t value, uint32_t positions) {

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
        // A�ade el BIT0 guardado en la posicion 7
        byte |= ((bit << 7) & 0x80);
    }

    // Devuelve el resultado de la conversion
    return byte;

}
