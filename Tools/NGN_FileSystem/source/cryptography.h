/******************************************************************************

    Sistema de empaquetado de archivos para N'gine
	- Funciones criptograficas -

    Proyecto iniciado el 14 de Febrero del 2021
    (c) 2021 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	Sistema de empaquetado de archivos para N'gine is under MIT License

	Copyright (c) 2016-2023 by Cesar Rincon "NightFox"

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



#ifndef CRYPTOGRAPHY_H_INCLUDED
#define CRYPTOGRAPHY_H_INCLUDED



/*** Includes ***/
// C++
#include <vector>
#include <string>
// Proyecto
#include "defines.h"



/*** Declaracion de la clase ***/
class Cryptography {

    public:

        // Constructor
        Cryptography();

        // Destructor
        ~Cryptography();

        /*** Propiedades ***/



        /*** Metodos ***/

        // Encriptacion de los datos contenidos en un vector
        int32_t EncryptData(std::vector<uint8_t> &data, std::string key);

        // Desencriptado de los datos contenidos en un vector
        int32_t DecryptData(std::vector<uint8_t> &data, std::string key);


    private:

        /*** Propiedades ***/

        std::vector<uint8_t> key_values;


        /*** Metodos ***/

        // Convierte la clave a un vector de valores numericos
        void KeyToValues(std::string key);

        // Rota un valor n veces a la izquierda
        uint8_t RotateLeft(uint8_t value, uint32_t positions);

        // Rota un valor n veces a la derecha
        uint8_t RotateRight(uint8_t value, uint32_t positions);


};



#endif // CRYPTOGRAPHY_H_INCLUDED
