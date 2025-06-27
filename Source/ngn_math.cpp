/******************************************************************************

    N'gine Lib for C++
    *** Version 1.19.0-stable ***
    Funciones matematicas

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2025 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


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
float Vector2::Magnitude() const {

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

/*** Clase Vector2 (float) - Normaliza el vector ***/
Vector2 Vector2::Normal() const {

    float m = Magnitude();
    Vector2 r;
    if (m > 0.0f) {
        r.x = x / m;
        r.y = y / m;
    } else {
        r.x = 0.0f;
        r.y = 0.0f;
    }
    return r;

}

/*** Operadores de la clase Vector2 (float) ***/
Vector2 Vector2::operator+(const Vector2 &p) const {

    Vector2 v = {x, y};
    v.x += p.x;
    v.y += p.y;
    return v;

}

Vector2 Vector2::operator-(const Vector2 &p) const {

    Vector2 v = {x, y};
    v.x -= p.x;
    v.y -= p.y;
    return v;

}

Vector2 Vector2::operator*(float p) const {

    Vector2 v = {x, y};
    v.x *= p;
    v.y *= p;
    return v;

}

Vector2 Vector2::operator/(float p) const {

    Vector2 v = {x, y};
    v.x /= p;
    v.y /= p;
    return v;

}

bool Vector2::operator==(const Vector2 &p) const {

    if ((x == p.x) && (y == p.y)) return true; else return false;

}

bool Vector2::operator!=(const Vector2 &p) const {

    if ((x != p.x) || (y != p.y)) return true; else return false;

}

void Vector2::operator+=(const Vector2 &p) {

    x += p.x;
    y += p.y;

}

void Vector2::operator-=(const Vector2 &p) {

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

/*** Clase Vector2 (float) - Devuelve un vector de valor 0 ***/
Vector2 Vector2::Zero() {

    return Vector2 {0.0f, 0.0f};

}



/*** Clase Vector2I32 (int32_t) - Calcula la magnitud del vector ***/
int32_t Vector2I32::Magnitude() const {

    return std::round(std::sqrt(std::pow(x, 2) + std::pow(y, 2)));

}

/*** Operadores de la clase Vector2I32 (int32_t) ***/
Vector2I32 Vector2I32::operator+(const Vector2I32 &p) const {

    Vector2I32 v = {x, y};
    v.x += p.x;
    v.y += p.y;
    return v;

}

Vector2I32 Vector2I32::operator-(const Vector2I32 &p) const {

    Vector2I32 v = {x, y};
    v.x -= p.x;
    v.y -= p.y;
    return v;

}

Vector2I32 Vector2I32::operator*(int32_t p) const {

    Vector2I32 v = {x, y};
    v.x *= p;
    v.y *= p;
    return v;

}

Vector2I32 Vector2I32::operator/(int32_t p) const {

    Vector2I32 v = {x, y};
    v.x /= p;
    v.y /= p;
    return v;

}

bool Vector2I32::operator==(const Vector2I32 &p) const {

    if ((x == p.x) && (y == p.y)) return true; else return false;

}

bool Vector2I32::operator!=(const Vector2I32 &p) const {

    if ((x != p.x) || (y != p.y)) return true; else return false;

}

void Vector2I32::operator+=(const Vector2I32 &p) {

    x += p.x;
    y += p.y;

}

void Vector2I32::operator-=(const Vector2I32 &p) {

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

/*** Clase Vector2 (int32_t) - Devuelve un vector de valor 0 ***/
Vector2I32 Vector2I32::Zero() {

    return Vector2I32 {0, 0};

}



/*** Clase Vector2I64 (int64_t) - Calcula la magnitud del vector ***/
int64_t Vector2I64::Magnitude() const {

    return std::round(std::sqrt(std::pow(x, 2) + std::pow(y, 2)));

}

/*** Operadores de la clase Vector2I64 (int64_t) ***/
Vector2I64 Vector2I64::operator+(const Vector2I64 &p) const {

    Vector2I64 v = {x, y};
    v.x += p.x;
    v.y += p.y;
    return v;

}

Vector2I64 Vector2I64::operator-(const Vector2I64 &p) const {

    Vector2I64 v = {x, y};
    v.x -= p.x;
    v.y -= p.y;
    return v;

}

Vector2I64 Vector2I64::operator*(int64_t p) const {

    Vector2I64 v = {x, y};
    v.x *= p;
    v.y *= p;
    return v;

}

Vector2I64 Vector2I64::operator/(int64_t p) const {

    Vector2I64 v = {x, y};
    v.x /= p;
    v.y /= p;
    return v;

}

bool Vector2I64::operator==(const Vector2I64 &p) const {

    if ((x == p.x) && (y == p.y)) return true; else return false;

}

bool Vector2I64::operator!=(const Vector2I64 &p) const {

    if ((x != p.x) || (y != p.y)) return true; else return false;

}

void Vector2I64::operator+=(const Vector2I64 &p) {

    x += p.x;
    y += p.y;

}

void Vector2I64::operator-=(const Vector2I64 &p) {

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

/*** Clase Vector2I64 (int64_t) - Devuelve un vector de valor 0 ***/
Vector2I64 Vector2I64::Zero() {

    return Vector2I64 {0, 0};

}





/******************************************************************************

    Metodos de la clase NGN_Math

******************************************************************************/

/*** Puntero de la instancia a nullptr ***/
NGN_Math* NGN_Math::instance = nullptr;



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
        instance = nullptr;
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

    // Semilla del random avanzado
    mt_gen.seed(rand_dev());

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



/*** Devuelve un numero aleatorio en un rango dado (entero) [1a sobrecarga] ***/
int32_t NGN_Math::RandomInt(int32_t min_value, int32_t max_value) {

    // Si no hay rango, devuelve el valor tal cual
    if (min_value == max_value) return min_value;

    // Ordena los valores
    int32_t _min = std::min(min_value, max_value);
    int32_t _max = std::max(min_value, max_value);

    // Genera un numero en el rango y devuelvelo
    return std::uniform_int_distribution<int32_t>(_min, _max)(mt_gen);

}

/*** Devuelve un numero aleatorio en un rango dado (entero) [2a sobrecarga] ***/
int32_t NGN_Math::RandomInt() {

    return RandomInt(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());

}



/*** Devuelve un numero aleatorio en un rango dado (punto flotante) [1a sobrecarga] ***/
float NGN_Math::RandomFloat(float min_value, float max_value) {

    // Si no hay rango, devuelve el valor tal cual
    if (min_value == max_value) return min_value;

    // Ordena los valores
    float _min = std::fmin(min_value, max_value);
    float _max = std::fmax(min_value, max_value);

    // Genera un numero en el rango y devuelvelo
    return std::uniform_real_distribution<float>(_min, _max)(mt_gen);

}

/*** Devuelve un numero aleatorio en un rango dado (punto flotante) [2a sobrecarga] ***/
float NGN_Math::RandomFloat() {

    constexpr float SAFE_RANGE = 1.0e6;
    return RandomFloat(-SAFE_RANGE, SAFE_RANGE);

}



/*** Devuelve un numero aleatorio en un rango dado (punto flotante de doble precision) [1a sobrecarga] ***/
double NGN_Math::RandomDouble(double min_value, double max_value) {

    // Si no hay rango, devuelve el valor tal cual
    if (min_value == max_value) return min_value;

    // Ordena los valores
    double _min = std::fmin(min_value, max_value);
    double _max = std::fmax(min_value, max_value);

    // Genera un numero en el rango y devuelvelo
    return std::uniform_real_distribution<double>(_min, _max)(mt_gen);

}

/*** Devuelve un numero aleatorio en un rango dado (punto flotante de doble precision) [2a sobrecarga] ***/
double NGN_Math::RandomDouble() {

    constexpr double SAFE_RANGE = 1.0e6;
    return RandomDouble(-SAFE_RANGE, SAFE_RANGE);

}



// Devuelve el resultado en 32bits de un checksum de datos usando el algoritmo "Adler-32" (Maximo 4gb de datos)
// Primera sobrecarga usando los datos de un Vector.
uint32_t NGN_Math::Adler32Checksum(const std::vector<uint8_t>& buffer, uint32_t start_index) {

    // Si el indice de inicio esta fuera de los limites,
    // o si la porcion del buffer a procesar esta vacia,
    // devolvemos el checksum de una cadena vacia (s1=1, s2=0).
    if (start_index >= buffer.size()) {
        return ((0 << 16) | 1);
    }

    // Variables para la generacion del checksum
    uint32_t s1 = 1;    // s1 se inicializa a 1
    uint32_t s2 = 0;    // s2 se inicializa a 0

    // Prepara las variables para el analisis de datos
    const uint8_t* data_ptr = (buffer.data() + start_index);
    uint32_t len = buffer.size() - start_index;
    uint32_t bytes_to_process_in_block = 0;

	// Constante para el modulo de Adler-32
	const uint32_t MOD_ADLER = 65521;

    // Optimizacion NMAX (de zlib): procesar en bloques para reducir la frecuencia de las operaciones de modulo.
    // NMAX es el mayor numero de bytes que se pueden procesar antes de que s2 (como uint32_t)
    // corra el riesgo de desbordarse, incluso en el peor de los casos (todos los bytes 0xFF).
    // El valor 5552 es el estandar usado en zlib.
    // NMAX = 5552 se elige porque es el limite matemático para el numero de bytes que pueden ser sumados en s1 y s2
    // (siendo s2 el limitante) antes de que s2 corra el riesgo de desbordar un uint32_t, asumiendo el peor caso de valores de entrada.
    // No tiene que ver con alineaciones de memoria o tamaños de cache como podrian tenerlo valores como 2048 o 4096 en otros contextos.
    // Es puramente una restriccion aritmética para garantizar la correccion y optimizar el rendimiento.
    const uint32_t NMAX = 5552;

    // Bucle de analisis
    while (len > 0) {

        // Calcula el tamaño del bloque
        bytes_to_process_in_block = (len < NMAX) ? len : NMAX;
        len -= bytes_to_process_in_block;

        // Bucle interno para procesar el bloque actual
        for (uint32_t i = 0; i < bytes_to_process_in_block; i ++) {
            s1 += data_ptr[i];
            s2 += s1;
        }

        // Incrementa la posicion del puntero de datos
        data_ptr += bytes_to_process_in_block;

        // Aplicar módulo después de procesar el bloque
        // (s1 puede haber excedido MOD_ADLER multiples veces, s2 mas aun)
        s1 %= MOD_ADLER;
        s2 %= MOD_ADLER;

    }

    // El checksum final se forma combinando s2 y s1
    return (((s2 & 0xFFFF) << 16) | (s1 & 0xFFFF));

}
