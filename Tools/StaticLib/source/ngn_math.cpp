/******************************************************************************

    N'gine Lib for C++
    *** Version 1.9.0-stable ***
    Funciones matematicas

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2022 by Cesar Rincon "NightFox"
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



/******************************************************************************

    Metodos de la clase NGN_Math

******************************************************************************/

/*** Constructor ***/
NGN_Math::NGN_Math() {

}



/*** Destructor ***/
NGN_Math::~NGN_Math() {

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
