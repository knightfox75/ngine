/******************************************************************************

    N'gine Lib for C++
    *** Version 1.11.0-stable ***
    FLAGS de configuracion de la libreria
    La libreria debe compilarse para que estos FLAGS sean efectivos

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2023 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

    Requiere GCC 11.3.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.0.22) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.0.15) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.1) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20220717)
    (c) 2005 - 2022 by Lode Vandevenne
    http://lodev.org/lodepng/


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

//#define MODE_DEBUG
//#define MODE_RELEASE


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
//#define DISABLE_SFML_AUDIO_ADVANCED_FEATURES

/*
	Deshabilita la funcion de backbuffer de N'gine.
	Esto aumenta el rendimiento en mas de un 50% en GPU's de perfil bajo,
	pero se pierde la opcion de filtrado bilineal en objetos y capas
	por separado.
	Disables the backbuffer feature of N'gine.
	That speeds up to 50% in lowend GPU's, but also disables the option
	of enable bilinear filtering in objects and layers separately.
*/
//#define DISABLE_BACKBUFFER



#endif // CONFIG_FLAGS_H_INCLUDED
