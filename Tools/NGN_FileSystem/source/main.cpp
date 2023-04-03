/******************************************************************************

    Sistema de empaquetado de archivos para N'gine

    Proyecto iniciado el 14 de Febrero del 2021
    (c) 2021 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

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



/*** Includes ***/

// C++
#include <cstdio>
#include <cstdlib>
#include <iostream>

// CollisionMap class
#include "file_system.h"
#include "defines.h"



/*** Main ***/
int32_t main(int32_t argc, char* args[]) {

    // Crea el objeto principal del programa
    FileSystem* fs = new FileSystem(argc, args);

    // Si no se crea con exito, sal del programa
    if (!fs) return ERR_CODE_BOOTFAILURE;

    // Transfiere el control del objeto principal
    int32_t r = fs->Run();

    // Elimina el objeto principal
    delete fs;

    // Devuelve el resultado de ejecucion del programa
    return r;

}
