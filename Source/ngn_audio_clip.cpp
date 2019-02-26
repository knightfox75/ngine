/******************************************************************************

    N'gine Lib for C++
    *** Version 0.6.2-alpha ***
    Clips de audio

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2019 by Cesar Rincon "NightFox"
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
#include <cmath>

// SFML
#include <SFML/Audio.hpp>

// Libreria
#include "ngn.h"



/*** Constructor de la clase  NGN_AudioClip ***/
NGN_AudioClip::NGN_AudioClip() {

    // Nivel de panning
    _panning = 0;

    // Volumen
    _volume = 0;

}



/*** Destructor de la clase NGN_AudioClip ***/
NGN_AudioClip::~NGN_AudioClip() {

}



/*** Asigna un clip de audio al sonido ***/
void NGN_AudioClip::Clip(NGN_AudioClipData* clip) {
    sound.setBuffer(clip->data);
}



/*** Reproduce el sonido ***/
void NGN_AudioClip::Play(
    int32_t volume,        // Volumen inicial
    int32_t pan,            // Nivel de panning (-100 a 100)
    bool loop               // Loop?
    ) {

    sound.setRelativeToListener(true);
    Panning(pan);

    _volume = volume;
    if (_volume < 0) _volume = 0;
    if (_volume > 100) _volume = 100;

    Volume(_volume);
    Loop(loop);
    Rewind();

    sound.play();

}

/*** Continua la reproduccion de un sonido ***/
void NGN_AudioClip::Resume() {
    if (!IsPlaying()) sound.play();
}



/***  Verifica si aun se este reproduciendo ***/
bool NGN_AudioClip::IsPlaying() {
    if (sound.getStatus() == sf::Sound::Playing) {
        return true;
    } else {
        return false;
    }
}



/*** Verifica si aun esta activo (en play o pause) ***/
bool NGN_AudioClip::IsAlive() {
    if ((sound.getStatus() == sf::Sound::Playing) || (sound.getStatus() == (sf::Sound::Paused))) {
        return true;
    } else {
        return false;
    }
}



/*** Deten el sonido ***/
void NGN_AudioClip::Stop() {
    sound.stop();
}



/*** Pausa el sonido ***/
void NGN_AudioClip::Pause() {
    if (IsPlaying()) sound.pause();
}



/*** Cambia el nivel de volumen ***/
void NGN_AudioClip::Volume(int32_t volume) {
    _volume = volume;
    if (_volume < 0) _volume = 0;
    if (_volume > 100) _volume = 100;
    sound.setVolume(_volume);
}



/*** Devuelve el nivel de volumen ***/
int32_t NGN_AudioClip::GetVolume() {
    return _volume;
}



/*** Cambia el pitch del sonido ***/
void NGN_AudioClip::Pitch(float pitch) {
    sound.setPitch(pitch);
}



/*** Devuelve el pitch actual ***/
float NGN_AudioClip::GetPitch() {
    return sound.getPitch();
}



/*** Cambia el loop ***/
void NGN_AudioClip::Loop(bool loop) {
    sound.setLoop(loop);
}



/*** Devuelve el loop ***/
bool NGN_AudioClip::GetLoop() {
    return sound.getLoop();
}



/*** Cambia el panning de un sonido ***/
void NGN_AudioClip::Panning(int32_t pan) {

    // Guarda el nivel de panning
    _panning = pan;
    if (_panning < -100) _panning = -100;
    if (_panning > 100) _panning = 100;

    // Calculos de angulo
    float angle = ((3.141592f * ((float)(-_panning + 100))) / 200.0f);
    float x = std::cos(angle);
    float z = std::sin(angle);
    sound.setPosition(x, 0.0f, z);

}



/*** Devuelve el nivel actual de panning de un sonido ***/
int32_t NGN_AudioClip::GetPanning() {
    return _panning;
}



/*** Revobina el archivo de audio ***/
void NGN_AudioClip::Rewind() {

    sf::Time t;
    sound.setPlayingOffset(t.Zero);

}
