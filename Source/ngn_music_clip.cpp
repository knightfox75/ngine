/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0+10th-anniversary ***
    Clips de musica

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2026 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016 - 2026 by Cesar Rincon "NightFox"

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
#include <string>

// SFML
#include <SFML/Audio.hpp>

// Libreria
#include "ngn.h"


/*** Contructor de la clase ***/
NGN_MusicClip::NGN_MusicClip() {

    _volume = 0;
    _mixer_channel = MIXER_MUSIC_CH;

}



/*** Destructor de la clase ***/
NGN_MusicClip::~NGN_MusicClip() {

    // Asegurate de que se ha vaciado el buffer de archivo antes de salir
    buffer.clear();

}



/*** Abre un archivo para su streaming ***/
bool NGN_MusicClip::Open(std::string filepath) {

    // Segun se use o no el empaquetado de datos...
    if (ngn->load->PackageEnabled()) {

        // Prepara el buffer temporal para la carga del archivo
        buffer.clear();

        // Intenta cargar el archivo
        int32_t file_length = ngn->load->LoadFile(filepath, buffer);
        if (file_length <= 0) {
            // Error leyendo el archivo solicitado
            ngn->log->Message("[NGN_MusicClip error] Error opening <" + filepath + "> for read.");
            buffer.clear();
            return false;
        }

        // Transfiere los datos en RAM para el stream
        if (music.openFromMemory((uint8_t*)&buffer[0], file_length)) {
            return true;
        } else {
            ngn->log->Message("[NGN_MusicClip error] Error opening <" + filepath + "> from memory.");
            buffer.clear();
            return false;
        }

    } else {

        // Abre el archivo para hacer streaming
        if (music.openFromFile(filepath.c_str())) {
            return true;
        } else {
            std::cout << "Error opening " << filepath << "  for streaming." << std::endl;
            return false;
        }

    }

}



/*** Reproduce una musica por streaming de un archivo ***/
void NGN_MusicClip::Play(
        int32_t volume,        // Volumen inicial
        bool loop              // Loop?
    ) {

    _volume = volume;
    if (_volume < 0) _volume = 0;
    if (_volume > 100) _volume = 100;

    Loop(loop);
    Volume(_volume);
    Rewind();

    music.play();

}



/*** Establece el los puntos de loop de un stream ***/
void NGN_MusicClip::SetLoop(
    int32_t loop_start,     // Punto inicial del loop (milisegundos)
    int32_t loop_end        // Punto final del loop (milisengundos)
    ) {

    #if !defined (DISABLE_SFML_AUDIO_ADVANCED_FEATURES)

        // Ajuste del punto final
        sf::Time duration = music.getDuration();
        if (loop_end == NGN_DEFAULT_VALUE) loop_end = duration.asMilliseconds();

        // Proteccion
        if (
            (loop_end > duration.asMilliseconds())  // Si el punto final de loop esta mas alla del final
            ||
            (loop_start >= loop_end)                // Si el punto de inicio supera el punto final
        ) return;

        // Convierte el tiempo de los puntos al formato SF::TIME
        sf::Time _offset = sf::milliseconds(loop_start);
        sf::Time _length = sf::milliseconds(loop_end - loop_start);

        // Prepara los datos del loop
        sf::Music::TimeSpan loop;
        loop.offset = _offset;
        loop.length = _length;

        // Aplicalos
        music.setLoopPoints(loop);

    #endif

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
    float v = (float)_volume * ((float)ngn->sound->GetMixerLevel(MIXER_MASTER_CH) / 100.0f) * ((float)ngn->sound->GetMixerLevel(_mixer_channel) / 100.0f);
    music.setVolume(v);
}



/*** Devuelve el nivel de volumen ***/
int32_t NGN_MusicClip::GetVolume() {
    return _volume;
}



/*** Cambia el pitch de la musica ***/
void NGN_MusicClip::Pitch(float pitch) {
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



/*** Asigna un canal del mixer ***/
void NGN_MusicClip::SetMixerChannel(uint8_t channel) {

    _mixer_channel = channel;
    if ((_mixer_channel < 1) || (_mixer_channel >= MIXER_CHANNELS)) _mixer_channel = MIXER_MUSIC_CH;
    Volume(_volume);

}



/*** Devuelve el canal asignado en el mixer ***/
uint8_t NGN_MusicClip::GetMixerChannel() {

    return _mixer_channel;

}

