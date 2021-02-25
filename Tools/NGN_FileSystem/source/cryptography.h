/******************************************************************************

    N'gine Lib for C++
    Sistema de archivos
	Funciones criptograficas

    Proyecto iniciado el 14 de Febrero del 2021
    (cc) 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

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
