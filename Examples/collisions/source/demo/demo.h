/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Colisiones

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

/*** Defines ***/
#define COLOR_MAGENTA 0xFF00FFFF    // RGBA
#define COLOR_GREEN 0x00FF00FF      // RGBA



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
        NGN_TiledBgData* bg_data;           // Datos del fondo
        NGN_TiledBg* bg;                    // Fondo de tiles
        NGN_TiledBgData* lightmap_data;     // Datos del lightmap
        NGN_TiledBg* lightmap;              // Lightmap

        // Mapa de colisiones
        NGN_CollisionMapData* cmap_data;    // Datos del mapa de colisiones

        // Sprites
        NGN_SpriteData* player_data;        // Datos del player
        NGN_Sprite* player;                 // Sprite del player
        NGN_SpriteData* cherry_data;        // Datos de las cerezas
        std::vector<NGN_Sprite*> cherry;    // Vector con todos los sprites de las cerezas


        // Carga los arhivos necesarios
        bool Load();

        // Crea la escena
        void CreateStage();

        // Añade cerezas
        void AddCherries();

        // Mueve al player
        void Move();

        // Colisiones
        bool CheckUp(NGN_Sprite* sprite, NGN_CollisionMapData* cmap);
        bool CheckDown(NGN_Sprite* sprite, NGN_CollisionMapData* cmap);
        bool CheckLeft(NGN_Sprite* sprite, NGN_CollisionMapData* cmap);
        bool CheckRight(NGN_Sprite* sprite, NGN_CollisionMapData* cmap);

        // Colisiones con las cerezas
        bool EatCherry();

        // Renderiza los elementos de la escena
        void Render();

};


#endif // DEMO_H_INCLUDED
