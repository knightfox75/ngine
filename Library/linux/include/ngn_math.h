/******************************************************************************

    N'gine Lib for C++
    *** Version 1.3.0-beta ***
    Funciones matematicas

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
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



#ifndef NGN_MATH_H_INCLUDED
#define NGN_MATH_H_INCLUDED



/*** Includes C++ ***/
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11



/******************************************************************************

    Aritmetica de vectores

******************************************************************************/



/*** Clase Vector2 (float) ***/
class Vector2 {

    public:

        // Propiedades
        float x;
        float y;

        // Calcula la magnitud del vector
        float Magnitude();
        // Normaliza este vector, actualizando el contenido actual
        void Normalize();

        // Operaciones
        Vector2 operator+(Vector2);
        Vector2 operator-(Vector2);
        Vector2 operator*(float);
        Vector2 operator/(float);


    private:

};



/*** Clase Vector2 (INT32) ***/
class Vector2I32 {

    public:

        // Propiedades
        int32_t x;
        int32_t y;

        // Calcula la magnitud del vector
        int32_t Magnitude();

        // Operaciones
        Vector2I32 operator+(Vector2I32);
        Vector2I32 operator-(Vector2I32);
        Vector2I32 operator*(int32_t);
        Vector2I32 operator/(int32_t);


    private:

};



/*** Clase Vector2 (INT64) ***/
class Vector2I64 {

    public:

        // Propiedades
        int64_t x;
        int64_t y;

        // Calcula la magnitud del vector
        int64_t Magnitude();

        // Operaciones
        Vector2I64 operator+(Vector2I64);
        Vector2I64 operator-(Vector2I64);
        Vector2I64 operator*(int64_t);
        Vector2I64 operator/(int64_t);


    private:

};



/*** Clase NGN_Math (funciones matematicas) ***/
class NGN_Math {

    // Segmento publico
    public:

        // Contructor de la clase
        NGN_Math();

        // Destructor de la clase
        ~NGN_Math();


        // Distancia entre 2 puntos (4 parametros, punto flotante, 1a sobrecarga)
        float GetDistance(float x1, float y1, float x2, float y2);
        // Distancia entre 2 puntos (2 parametros, punto flotante, 2a sobrecarga)
        float GetDistance(Vector2 a, Vector2 b);
        // Distancia entre 2 puntos (4 parametros, entero, 3a sobrecarga)
        uint32_t GetDistance(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
        // Distancia entre 2 puntos (2 parametros, entero, 4a sobrecarga)
        uint32_t GetDistance(Vector2I32 a, Vector2I32 b);


        // Angulo formado por 2 puntos (4 parametros, punto flotante, 1a sobrecarga)
        float GetAngle(float x1, float y1, float x2, float y2);
        // Angulo formado por 2 puntos (2 parametros, punto flotante, 2a sobrecarga)
        float GetAngle(Vector2 a, Vector2 b);

        // Distancia Manhattan entre 2 puntos
        float GetManhattan(float x1, float y1, float x2, float y2);
        // Distancia entre 2 puntos (2 parametros, punto flotante, 2a sobrecarga)
        float GetManhattan(Vector2 a, Vector2 b);
        // Distancia entre 2 puntos (4 parametros, entero, 3a sobrecarga)
        uint32_t GetManhattan(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
        // Distancia entre 2 puntos (2 parametros, entero, 4a sobrecarga)
        uint32_t GetManhattan(Vector2I32 a, Vector2I32 b);



    // Segmento privado
    private:



};



#endif // NGN_MATH_H_INCLUDED
