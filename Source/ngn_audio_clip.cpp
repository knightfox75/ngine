/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0-wip_0x04 ***
    Clips de audio

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

    // Multiplicador de volumen para la atenuacion panoramica
    _panning_attenuation = 1.0f;

    // Canal por defecto
    _mixer_channel = MIXER_EFFECTS_CH;

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
    int32_t volume,         // Volumen inicial
    int32_t pan,            // Nivel de panning (-100 a 100)
    bool loop               // Loop?
    ) {

    sound.setRelativeToListener(true);
    sound.setMinDistance(1.0f);
    sound.setAttenuation(0.0f);
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
    float mixer = ((float)ngn->sound->GetMixerLevel(MIXER_MASTER_CH) / 100.0f) * ((float)ngn->sound->GetMixerLevel(_mixer_channel) / 100.0f);
    int32_t v = (int32_t)((float)_volume * _panning_attenuation * mixer);
    sound.setVolume(v);
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
    sf::Vector3 pos(0.0f, 0.0f, 0.0f);
    float angle = ((PI * ((float)(-_panning + 100))) / 200.0f);
    pos.x = std::cos(angle);
    pos.y = 0.0f;
    pos.z = std::sin(angle);
    sound.setPosition(pos);

    // Calculos de la compensacion de la atenuacion panoramica
    _panning_attenuation = (1.0f - (std::abs(pos.x) / 2.0f));

    // Reajusta el nivel de volumen
    float mixer = ((float)ngn->sound->GetMixerLevel(MIXER_MASTER_CH) / 100.0f) * ((float)ngn->sound->GetMixerLevel(_mixer_channel) / 100.0f);
    int32_t v = (int32_t)((float)_volume * _panning_attenuation * mixer);
    sound.setVolume(v);

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


/*** Asigna un canal del mixer ***/
void NGN_AudioClip::SetMixerChannel(uint8_t channel) {

    _mixer_channel = channel;
    if ((_mixer_channel < 1) || (_mixer_channel >= MIXER_CHANNELS)) _mixer_channel = MIXER_EFFECTS_CH;
    Volume(_volume);

}



/*** Devuelve el canal asignado en el mixer ***/
uint8_t NGN_AudioClip::GetMixerChannel() {

    return _mixer_channel;

}
