/******************************************************************************

    Pixel Art Project Template: Musica

    Proyecto iniciado el xx de xx del xx
    (c) xxxx by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.20.0+10th-anniversary o superior
	(c) 2016 - 2026 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
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

******************************************************************************/



/*** Includes ***/
// c++
#include <cstdio>
#include <iostream>
// Includes de la libreria
#include <ngn.h>
// Includes del programa
#include "soundtrack.h"



/*** Puntero de la instancia a nullptr ***/
Soundtrack* Soundtrack::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
Soundtrack* Soundtrack::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new Soundtrack();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void Soundtrack::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Constructor de la clase ***/
Soundtrack::Soundtrack() {

    // Punteros de memoria
    bgm = nullptr;

}



/*** Destructor de la clase ***/
Soundtrack::~Soundtrack() {

    // Verifica si hay algun stream de musica cargado, si lo hay, cierralo
    if (bgm) {
        ngn->sound->CloseMusic(bgm);
        bgm = nullptr;
    }

}



/*** Procesos iniciales despues de crear la instancia ***/
void Soundtrack::BootUp() {

    // Datos iniciales
    current_track_id = TRACK_ID.none;
    is_playing = false;

    // Control de volumen
    volume.current = 100;
    volume.target = 100;
    volume.speed = 0;

    // Control del fade-out
    fade_out = false;

}



/*** Actualiza el estado del soundtrack ***/
void Soundtrack::Update() {

    AdjustVolume();

}



/*** Carga un archivo de audio ***/
bool Soundtrack::Load(uint32_t id) {

    // Si el archivo solicitado ya esta cargado, ignora la orden, pero devuelve exito
    if (id == current_track_id) return true;

    // Si no, elimina cualquier archivo cargado anteriormente
    Unload();

    // E intenta cargar el solicitado
    switch (id) {
        case TRACK_ID.title:
            bgm = ngn->sound->OpenMusic("data/title/music/title.ogg");
            break;
    }

    // Si no se ha podido cargar con exito, error
    if (!bgm) {
        current_track_id = TRACK_ID.none;
        return false;
    }

    // Carga correcta
    current_track_id = id;
    return true;

}



/*** Descarga el archivo actual ***/
void Soundtrack::Unload() {

    // Si no esta cargada ninguna pista, ignora la orden
    if (!bgm) return;

    // Deten la reproduccion del archivo actual y cierralo
    ngn->sound->CloseMusic(bgm);
    bgm = nullptr;
    is_playing = false;

}



/*** Inicia la reproduccion de la musica de fondo ***/
void Soundtrack::Play() {

    // Si la reproduccion esta en curso, ignora la orden
    if (is_playing) return;

    // Si no esta cargada ninguna pista, ignora la orden
    if (!bgm) return;

    // Reproduce la pista
    bool loop = ngn->sound->MusicGetLoop(bgm);
    uint32_t vol = ngn->sound->MusicGetVolume(bgm);
    ngn->sound->PlayMusic(bgm, vol, loop);
    is_playing = true;

}



/*** Deten la repoduccion de la musica de fondo ***/
void Soundtrack::Stop() {

    // Si no esta en curso, ignora la orden
    if (!is_playing) return;

    // Si no esta cargada ninguna pista, ignora la orden
    if (!bgm) return;

    // Deten la pista
    ngn->sound->StopMusic(bgm);
    is_playing = false;

}



/*** Cambia el volumen de la musica (speed = 0 cambio al instante) ***/
void Soundtrack::SetVolumeTo(int8_t level, int8_t speed) {

    if (level < 0) level = 0;
    if (level > 100) level = 100;
    if (speed < 0) speed = 0;
    if (speed > 100) speed = 100;

    volume.target = level;
    volume.speed = speed;

}



/*** Obten el volumen actual ***/
int8_t Soundtrack::GetVolume() {

    return volume.current;

}



/*** Devuelve la pista cargada actualmente ***/
uint32_t Soundtrack::GetCurrentTrackId() {

    return current_track_id;

}



/*** Ajusta el volumen al nivel solicitado ***/
void Soundtrack::AdjustVolume() {

    // Ajusta el nivel de volumen
    if (volume.speed == 0) {
        volume.current = volume.target;
    } else if (volume.current < volume.target) {
        volume.current += volume.speed;
        if (volume.current > volume.target) volume.current = volume.target;
    } else if (volume.current > volume.target) {
        volume.current -= volume.speed;
        if (volume.current < volume.target) volume.current = volume.target;
    }

    // Aplicalo (si existe la musica)
    if (bgm) ngn->sound->MusicVolume(bgm, volume.current);

}



/*** Configura el estado del fade-out ***/
void Soundtrack::SetFadeOut(bool st) {

    fade_out = st;

}



/*** Devuelve el estado del fade-out ***/
bool Soundtrack::GetFadeOut() {

    return fade_out;

}
