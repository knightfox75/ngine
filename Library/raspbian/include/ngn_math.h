/******************************************************************************

    N'gine Lib for C++
    *** Version 1.15.0-stable ***
    Funciones matematicas

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2023 by Cesar Rincon "NightFox"
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
        bool operator==(Vector2);
        bool operator!=(Vector2);
        void operator+=(Vector2);
        void operator-=(Vector2);
        void operator*=(float);
        void operator/=(float);


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
        bool operator==(Vector2I32);
        bool operator!=(Vector2I32);
        void operator+=(Vector2I32);
        void operator-=(Vector2I32);
        void operator*=(int32_t);
        void operator/=(int32_t);


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
        bool operator==(Vector2I64);
        bool operator!=(Vector2I64);
        void operator+=(Vector2I64);
        void operator-=(Vector2I64);
        void operator*=(int64_t);
        void operator/=(int64_t);


    private:

};



/*** Clase NGN_Math (funciones matematicas) ***/
class NGN_Math {

    // Segmento publico
    public:

        // Devuelve la instancia
        static NGN_Math* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


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

        // Contructor de la clase
        NGN_Math();

        // Destructor de la clase
        ~NGN_Math();

        // Puntero de memoria a la instancia
        static NGN_Math* instance;


};



#endif // NGN_MATH_H_INCLUDED
