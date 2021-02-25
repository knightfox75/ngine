/******************************************************************************

    N'gine Lib for C++
    Sistema de archivos
	- Sistema de mensages -

    Proyecto iniciado el 14 de Febrero del 2021
    (cc) 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

******************************************************************************/



#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED



/*** Includes ***/
// C++
#include <vector>
#include <string>
// Proyecto
#include "defines.h"



/*** Declaracion de la clase ***/
class Message {

    public:

        // Constructor
        Message(std::string pname);

        // Destructor
        ~Message();

        /*** Propiedades ***/


        /*** Metodos ***/
        void AppHeader();                       // Texto de cabecera de la aplicacion
        void QuestionMarkForHelp();             // -? para mostrar la ayuda
        void UseError();                        // Error del uso
        void UserManual();                      // Manual del usuario


    private:

        /*** Propiedades ***/
        std::string program_name;           // Nombre del archivo ejecutable



        /*** Metodos ***/

};



#endif // MESSAGE_H_INCLUDED
