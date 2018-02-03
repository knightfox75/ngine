/******************************************************************************

    N'gine Lib for C++
    *** Version 0.5.1-alpha ***
    Canvas - Capa de dibujo

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com


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



#ifndef NGN_CANVAS_H_INCLUDED
#define NGN_CANVAS_H_INCLUDED



/*** Includes ***/

// C++
#include <vector>
#include <string>

// Includes de la libreria
#include "ngn_defines.h"

// Definiciones
#define CIRCLE_PRECISION 2.0f



/******************************************************************************

    Clase NGN_Canvas

******************************************************************************/

/*** Declaracion de la clase del canvas ***/
class NGN_Canvas {

    // Segmento publico
    public:

        // Constructor de la clase
        NGN_Canvas(
            float position_x = 0.0f,            // Posicion X (0 por defecto)
            float position_y = 0.0f,            // Posicion Y (0 por defecto)
            int32_t _width = NGN_DEFAULT_VALUE,     // Ancho de la capa (Toda la pantalla por defecto)
            int32_t _height = NGN_DEFAULT_VALUE     // Alto de la capa (Toda la pantalla por defecto)
        );

        // Destructor de la clase
        ~NGN_Canvas();

        // Prototipos de datos
        struct RGBA {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };

        // Propiedades
        Vector2 position;                   // Posicion de la capa
        int32_t width;                      // Tamaño
        int32_t height;
        Vector2 center;                     // Centro de la capa para su rotacion

        bool visible;                       // Visibilidad
        int32_t alpha;                      // Nivel de alpha

        double rotation;                    // Rotacion de la capa

        bool flip_h;                        // Flip de la capa
        bool flip_v;

        Size2 scale;                        // Escala actual

        // Posiciona la capa
        void Position(float position_x, float position_y);
        void Position(Vector2 pos);

        // Mueve la capa
        void Translate(float speed_x, float speed_y);
        void Translate(Vector2 spd);

        // Cambia el tamaño de la capa
        void Size(float w, float h);

        //  Escala una capa
        void Scale(float w, float h);   // [Sobrecarga 1]       Escala los dos ejes por separado
        void Scale(float sc);           // [Sobrecarga 2]       Escala ambos ejes a la vez

        // Rota la capa
        void Rotate(double degrees);

        // Fija el centro de la capa (Posicion relativa desde el centro REAL)
        void SetCenter(float x, float y);


        SDL_Texture* backbuffer;            // Backbufer de la capa para su renderizado


        // Borra la capa
        void Cls(uint32_t color = 0x00000000);

        // Dibuja un punto
        void Point(uint32_t x, uint32_t y, uint32_t color);

        // Dibuja una matriz de puntos
        void Points(const std::vector<CanvasPoint> &points);

        // Dibuja una linea
        void Line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color);

        // Dibuja una matriz de lineas
        void Lines(const std::vector<CanvasLine> &lines);

        // Dibuja un cuadrado
        void Box(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color, bool paint = false);

        // Dibuja un circulo
        void Circle(uint32_t x, uint32_t y, uint32_t r, uint32_t color, uint32_t r2 = NGN_DEFAULT_VALUE, double in_angle = 0.0f, double out_angle = 0.0f);

        // Dibuja un circulo relleno
        void FilledCircle(uint32_t x, uint32_t y, uint32_t r, uint32_t color, uint32_t r2 = NGN_DEFAULT_VALUE);



    // Segmento privado
    private:

        std::vector<CanvasLine> segment;    // Buffer para almacenar los segmentos de una forma
        std::vector<CanvasPoint> vertex;    // Buffer para almacenar los vertices de una forma

        // Limpieza del surface
        void SurfaceCleanUp();

};




#endif // NGN_CANVAS_H_INCLUDED
