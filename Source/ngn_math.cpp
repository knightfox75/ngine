/******************************************************************************

    N'gine Lib for C++
    *** Version 1.14.0-stable ***
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



/*** Includes ***/
// C++
#include <cstdio>
#include <iostream>
#include <cmath>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/******************************************************************************

    Metodos de las clases Vector

******************************************************************************/

/*** Clase Vector2 (float) - Calcula la magnitud del vector ***/
float Vector2::Magnitude() {

    return std::sqrt(std::pow(x, 2.0f) + std::pow(y, 2.0f));

}

/*** Clase Vector2 (float) - Normaliza el vector ***/
void Vector2::Normalize() {

    float m = Magnitude();
    if (m > 0.0f) {
        x /= m;
        y /= m;
    } else {
        x = 0.0f;
        y = 0.0f;
    }

}

/*** Operadores de la clase Vector2 (float) ***/
Vector2 Vector2::operator+(Vector2 p) {

    Vector2 v = {x, y};
    v.x += p.x;
    v.y += p.y;
    return v;

}

Vector2 Vector2::operator-(Vector2 p) {

    Vector2 v = {x, y};
    v.x -= p.x;
    v.y -= p.y;
    return v;

}

Vector2 Vector2::operator*(float p) {

    Vector2 v = {x, y};
    v.x *= p;
    v.y *= p;
    return v;

}

Vector2 Vector2::operator/(float p) {

    Vector2 v = {x, y};
    v.x /= p;
    v.y /= p;
    return v;

}

bool Vector2::operator==(Vector2 p) {

    if ((x == p.x) && (y == p.y)) return true; else return false;

}

bool Vector2::operator!=(Vector2 p) {

    if ((x != p.x) || (y != p.y)) return true; else return false;

}

void Vector2::operator+=(Vector2 p) {

    x += p.x;
    y += p.y;

}

void Vector2::operator-=(Vector2 p) {

    x -= p.x;
    y -= p.y;

}

void Vector2::operator*=(float p) {

    x *= p;
    y *= p;

}

void Vector2::operator/=(float p) {

    x /= p;
    y /= p;

}



/*** Clase Vector2I32 (int32_t) - Calcula la magnitud del vector ***/
int32_t Vector2I32::Magnitude() {

    return std::round(std::sqrt(std::pow(x, 2) + std::pow(y, 2)));

}

/*** Operadores de la clase Vector2I32 (int32_t) ***/
Vector2I32 Vector2I32::operator+(Vector2I32 p) {

    Vector2I32 v = {x, y};
    v.x += p.x;
    v.y += p.y;
    return v;

}

Vector2I32 Vector2I32::operator-(Vector2I32 p) {

    Vector2I32 v = {x, y};
    v.x -= p.x;
    v.y -= p.y;
    return v;

}

Vector2I32 Vector2I32::operator*(int32_t p) {

    Vector2I32 v = {x, y};
    v.x *= p;
    v.y *= p;
    return v;

}

Vector2I32 Vector2I32::operator/(int32_t p) {

    Vector2I32 v = {x, y};
    v.x /= p;
    v.y /= p;
    return v;

}

bool Vector2I32::operator==(Vector2I32 p) {

    if ((x == p.x) && (y == p.y)) return true; else return false;

}

bool Vector2I32::operator!=(Vector2I32 p) {

    if ((x != p.x) || (y != p.y)) return true; else return false;

}

void Vector2I32::operator+=(Vector2I32 p) {

    x += p.x;
    y += p.y;

}

void Vector2I32::operator-=(Vector2I32 p) {

    x -= p.x;
    y -= p.y;

}

void Vector2I32::operator*=(int32_t p) {

    x *= p;
    y *= p;

}

void Vector2I32::operator/=(int32_t p) {

    x /= p;
    y /= p;

}



/*** Clase Vector2I64 (int64_t) - Calcula la magnitud del vector ***/
int64_t Vector2I64::Magnitude() {

    return std::round(std::sqrt(std::pow(x, 2) + std::pow(y, 2)));

}

/*** Operadores de la clase Vector2I64 (int64_t) ***/
Vector2I64 Vector2I64::operator+(Vector2I64 p) {

    Vector2I64 v = {x, y};
    v.x += p.x;
    v.y += p.y;
    return v;

}

Vector2I64 Vector2I64::operator-(Vector2I64 p) {

    Vector2I64 v = {x, y};
    v.x -= p.x;
    v.y -= p.y;
    return v;

}

Vector2I64 Vector2I64::operator*(int64_t p) {

    Vector2I64 v = {x, y};
    v.x *= p;
    v.y *= p;
    return v;

}

Vector2I64 Vector2I64::operator/(int64_t p) {

    Vector2I64 v = {x, y};
    v.x /= p;
    v.y /= p;
    return v;

}

bool Vector2I64::operator==(Vector2I64 p) {

    if ((x == p.x) && (y == p.y)) return true; else return false;

}

bool Vector2I64::operator!=(Vector2I64 p) {

    if ((x != p.x) || (y != p.y)) return true; else return false;

}

void Vector2I64::operator+=(Vector2I64 p) {

    x += p.x;
    y += p.y;

}

void Vector2I64::operator-=(Vector2I64 p) {

    x -= p.x;
    y -= p.y;

}

void Vector2I64::operator*=(int64_t p) {

    x *= p;
    y *= p;

}

void Vector2I64::operator/=(int64_t p) {

    x /= p;
    y /= p;

}





/******************************************************************************

    Metodos de la clase NGN_Math

******************************************************************************/

/*** Puntero de la instancia a NULL ***/
NGN_Math* NGN_Math::instance = NULL;



/*** Metodo para crear/obtener la instancia ***/
NGN_Math* NGN_Math::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Math();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Math::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = NULL;
    }

}



/*** Constructor ***/
NGN_Math::NGN_Math() {

}



/*** Destructor ***/
NGN_Math::~NGN_Math() {

}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Math::BootUp() {
}



/*** Distancia entre 2 puntos (4 parametros, punto flotante, 1ra sobrecarga) ***/
float NGN_Math::GetDistance(float x1, float y1, float x2, float y2) {

    return std::sqrt((std::pow((x1 - x2), 2) + std::pow((y1 - y2), 2)));

}

/*** Distancia entre 2 puntos (2 parametros, punto flotante, 2da sobrecarga) ***/
float NGN_Math::GetDistance(Vector2 a, Vector2 b) {

    return std::sqrt((std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2)));

}

/*** Distancia entre 2 puntos (4 parametros, entero, 3ra sobrecarga) ***/
uint32_t NGN_Math::GetDistance(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {

    return std::round(std::sqrt((std::pow((x1 - x2), 2) + std::pow((y1 - y2), 2))));

}

/*** Distancia entre 2 puntos (2 parametros, entero, 4a sobrecarga) ***/
uint32_t NGN_Math::GetDistance(Vector2I32 a, Vector2I32 b) {

    return std::round(std::sqrt((std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2))));

}



/*** Angulo formado por 2 puntos (4 parametros, punto flotante, 1a sobrecarga) ***/
float NGN_Math::GetAngle(float x1, float y1, float x2, float y2) {

    float x = (x2 - x1);
    float y = (y2 - y1);
    return std::atan2(y, x);

}

/*** Angulo formado por 2 puntos (2 parametros, punto flotante, 2a sobrecarga) ***/
float NGN_Math::GetAngle(Vector2 a, Vector2 b) {

    Vector2 v = (b - a);
    return std::atan2(v.y, v.x);

}



/*** Distancia Manhattan entre 2 puntos (4 parametros, punto flotante, 1ra sobrecarga) ***/
float NGN_Math::GetManhattan(float x1, float y1, float x2, float y2) {

    return (std::abs(x1 - x2) + std::abs(y1 - y2));

}

/*** Distancia Manhattan entre 2 puntos (2 parametros, punto flotante, 2da sobrecarga) ***/
float NGN_Math::GetManhattan(Vector2 a, Vector2 b) {

    Vector2 v = (a - b);
    return (std::abs(v.x) + std::abs(v.y));

}

/*** Distancia Manhattan entre 2 puntos (4 parametros, entero, 3ra sobrecarga) ***/
uint32_t NGN_Math::GetManhattan(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {

    return (std::abs(x1 - x2) + std::abs(y1 - y2));

}

/*** Distancia Manhattan entre 2 puntos (2 parametros, entero, 4a sobrecarga) ***/
uint32_t NGN_Math::GetManhattan(Vector2I32 a, Vector2I32 b) {

    Vector2I32 v = (a - b);
    return (std::abs(v.x) + std::abs(v.y));

}
