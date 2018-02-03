/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Movimiento de texturas

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere SDL2 (2.0.5 or higher)
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.4.1 or higher)
    http://www.sfml-dev.org/

    Requiere LodePNG
    (c) 2005 - 2016 by Lode Vandevenne
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



#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED



/*** Includes ***/
// Includes C++
#include <vector>
// Includes de la libreria
#include <ngn.h>



/*** Declaracion de la clase ***/
class Demo {

    public:

        // Constructor
        Demo();

        // Destructor
        ~Demo();

        // Awake
        bool Awake();

        // Start
        bool Start();

        // Update
        bool Update();


    private:

        // Fondo
        NGN_TextureData* bg_data;       // Imagen
        NGN_Texture* bg;                // Textura

        // Sprites
        NGN_TextureData* ball_data;     // Imagen
        struct ball_type{               // Estructura de datos de la bola
            Vector2 speed;
            int32_t top;
            int32_t right;
            int32_t bottom;
            int32_t left;
            NGN_Texture* gfx;
        };
        std::vector<ball_type> ball;    // Vector para almacenar todas las bolas

        // Carga los archivos necesarios
        bool Load();

        // Crea los objetos
        void CreateObjects();

        // Mueve a los objetos
        void Move();

        // Funcion para renderizar la escena
        void Render();


};


#endif // DEMO_H_INCLUDED
