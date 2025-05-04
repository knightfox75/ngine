/******************************************************************************

    N'gine Lib for C++
    *** Version 1.19.0-wip_0x07 ***
    FLAGS de configuracion de la libreria
    La libreria debe compilarse para que estos FLAGS sean efectivos

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere GCC 13.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.11) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.24.0) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.6.2) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/


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



#ifndef CONFIG_FLAGS_H_INCLUDED
#define CONFIG_FLAGS_H_INCLUDED



/******************************************************************************

	Flags de seleccion del sistema operativo de destino
	Usar solo 1 a la vez y si no se esta usando el proyecto de code::blocks

	Flags to select the target operating system
	Use only 1 at a time and if you are not using the code::blocks project

******************************************************************************/

//#define OS_WINDOWS
//#define OS_LINUX


/******************************************************************************

	Flags de seleccion del tipo de binario (debug o release)
	Usar solo 1 a la vez y si no se esta usando el proyecto de code::blocks

	Flags to select the type of the binary (debug or release)
	Use only 1 at a time and if you are not using the code::blocks project

******************************************************************************/

// #define MODE_DEBUG
// #define MODE_RELEASE


/******************************************************************************

	Flags de configuracion avanzados de N'gine
	N'gine advanced configuration flags

******************************************************************************/

/*
	Deshabilita las funciones avanzadas de sonido de SFML
	Usar solo si se usa una version inferior a la 2.5.1.
	Disables the advanced features of SFML audio library.
	Use only if you're using a version under 2.5.1
*/
// #define DISABLE_SFML_AUDIO_ADVANCED_FEATURES

/*
	Deshabilita la funcion de backbuffer de N'gine.
	Esto aumenta el rendimiento en mas de un 50% en GPU's de perfil bajo,
	pero se pierde la opcion de filtrado bilineal en objetos y capas
	por separado.
	Disables the backbuffer feature of N'gine.
	That speeds up to 50% in lowend GPU's, but also disables the option
	of enable bilinear filtering in objects and layers separately.
*/
// #define DISABLE_BACKBUFFER



#endif // CONFIG_FLAGS_H_INCLUDED
