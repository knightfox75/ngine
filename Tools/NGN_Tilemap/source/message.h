/******************************************************************************

    Conversor de PNG a Fondo de Tiles (.tbg) para N'gine
	- Sistema de mensages -

    Proyecto iniciado el 11 de Febrero del 2016
    (c) 2016-2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/


	Conversor de PNG a Fondo de Tiles is under MIT License

	Copyright (c) 2016 - 2024 by Cesar Rincon "NightFox"

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
