/******************************************************************************

    N'gine Lib for C++
    *** Version 0.2.1-alpha ***
    Clips de musica

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2017 by Cesar Rincon "NightFox"
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



/*** Includes ***/
// C++
#include <cstdio>
#include <iostream>
#include <vector>

// SFML
#include <SFML/Audio.hpp>

// Libreria
#include "ngn.h"


/*** Contructor de la clase ***/
NGN_MusicClip::NGN_MusicClip() {

    _volume = 0;

}



/*** Destructor de la clase ***/
NGN_MusicClip::~NGN_MusicClip() {

}



/*** Abre un archivo para su streaming ***/
bool NGN_MusicClip::Open(const char* filepath) {

    // Abre el archivo para hacer streaming
    if (music.openFromFile(filepath)) {
        return true;
    } else {
        std::cout << "Error opening " << filepath << "  for streaming." << std::endl;
        return false;
    }

}



/*** Reproduce una musica por streaming de un archivo ***/
void NGN_MusicClip::Play(
        int32_t volume,        // Volumen inicial
        bool loop               // Loop?
    ) {

    _volume = volume;
    if (_volume < 0) _volume = 0;
    if (_volume > 100) _volume = 100;

    Loop(loop);
    Volume(_volume);
    Rewind();

    music.play();

}

/*** Continua la reproduccion de una musica por streaming de un archivo ***/
void NGN_MusicClip::Resume() {

    if (!IsPlaying()) music.play();

}



/***  Verifica si aun se este reproduciendo ***/
bool NGN_MusicClip::IsPlaying() {
    if (music.getStatus() == sf::Sound::Playing) {
        return true;
    } else {
        return false;
    }
}



/*** Verifica si aun esta activo (en play o pause) ***/
bool NGN_MusicClip::IsAlive() {
    if ((music.getStatus() == sf::Sound::Playing) || (music.getStatus() == (sf::Sound::Paused))) {
        return true;
    } else {
        return false;
    }
}



/*** Deten la musica ***/
void NGN_MusicClip::Stop() {
    music.stop();
}



/*** Pausa la musica ***/
void NGN_MusicClip::Pause() {
    if (IsPlaying()) music.pause();
}



/*** Cambia el nivel de volumen ***/
void NGN_MusicClip::Volume(int32_t volume) {
    _volume = volume;
    if (_volume < 0) _volume = 0;
    if (_volume > 100) _volume = 100;
    music.setVolume(_volume);
}



/*** Devuelve el nivel de volumen ***/
int32_t NGN_MusicClip::GetVolume() {
    return _volume;
}



/*** Cambia el pitch de la musica ***/
void NGN_MusicClip::Pitch(float pitch = 1.0f) {
    music.setPitch(pitch);
}



/*** Devuelve el pitch actual ***/
float NGN_MusicClip::GetPitch() {
    return music.getPitch();
}



/*** Cambia el loop ***/
void NGN_MusicClip::Loop(bool loop) {
    music.setLoop(loop);
}



/*** Devuelve el loop ***/
bool NGN_MusicClip::GetLoop() {
    return music.getLoop();
}



/*** Revobina el stream de audio ***/
void NGN_MusicClip::Rewind() {

    sf::Time t;
    music.setPlayingOffset(t.Zero);

}

