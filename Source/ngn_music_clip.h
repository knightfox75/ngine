/******************************************************************************

    N'gine Lib for C++
    *** Version 1.20.0-wip_0x04 ***
    Clips de musica

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



#ifndef NGN_MUSIC_CLIP_H_INCLUDED
#define NGN_MUSIC_CLIP_H_INCLUDED


/*** Includes ***/

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>
#include <string>

// SFML
#include <SFML/Audio.hpp>

// Includes de la libreria
#include "ngn_defines.h"


/*** Definicion de la clase NGN_MusicClip ***/
class NGN_MusicClip {

    // Public
    public:

        // Constructor de la clase
        NGN_MusicClip();

        // Destructor de la clase
        ~NGN_MusicClip();

        // Abre un archivo para su streaming
        bool Open(std::string filepath);

        // Reproduce la musica [1a sobrecarga]
        void Play(
                  int32_t volume,      // Volumen inicial
                  bool loop            // Loop?
                  );

        void SetLoop(
                    int32_t loop_start,                     // Punto inicial del loop (milisegundos)
                    int32_t loop_end = NGN_DEFAULT_VALUE    // Punto final del loop (milisengundos)
                    );

        // Continua la reproduccion pausada de la musica [2a sobrecarga]
        void Resume();

        // Verifica si aun se este reproduciendo
        bool IsPlaying();

        // Verifica si aun esta activo (en play o pause)
        bool IsAlive();

        // Deten el sonido
        void Stop();

        // Pausa sonido
        void Pause();

        // Cambia el nivel de volumen
        void Volume(int32_t volume);

        // Devuelve el nivel de volumen
        int32_t GetVolume();

        // Cambia el pitch
        void Pitch(float pitch);

        // Devuelve el pitch actual
        float GetPitch();

        // Cambia el loop
        void Loop(bool loop);

        // Devuelve el loop
        bool GetLoop();

        // Rebobina el stream de audio
        void Rewind();

        // Asigna un canal del mixer
        void SetMixerChannel(uint8_t channel);

        // Devuelve el canal asignado actualmente
        uint8_t GetMixerChannel();

        // Buffer para el archivo
        std::vector<uint8_t> buffer;


    // Private
    private:

        // Stream de musica
        sf::Music music;

        // Parametros;
        int32_t _volume;
        uint8_t _mixer_channel;


};



#endif // NGN_MUSIC_CLIP_H_INCLUDED
