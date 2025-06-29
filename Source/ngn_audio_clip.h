/******************************************************************************

    N'gine Lib for C++
    *** Version 1.19.1-stable ***
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



#ifndef NGN_AUDIO_CLIP_H_INCLUDED
#define NGN_AUDIO_CLIP_H_INCLUDED



/*** Includes ***/

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>

// SFML
#include <SFML/Audio.hpp>

// Includes de la libreria
#include "ngn_defines.h"



/*** Definicion de la clase NGN_AudioClip ***/
class NGN_AudioClip {

    // Public
    public:

        // Constructor de la clase
        NGN_AudioClip();

        // Destructor de la clase
        ~NGN_AudioClip();

        // Asigna el clip de audio
        void Clip(NGN_AudioClipData* clip);

        // Reproduce el sonido
        void Play(
                  int32_t volume,           // Volumen inicial
                  int32_t pan,              // Nivel de panning (-100 a 100)
                  bool loop                 // Loop?
                  );

        // Continua la reproduccion de un sonido
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

        // Cambia el panning de un sonido
        void Panning(int32_t pan);

        // Devuelve el nivel actual de panning de un sonido
        int32_t GetPanning();

        // Revobina el archivo de audio
        void Rewind();

        // Asigna un canal del mixer
        void SetMixerChannel(uint8_t channel);

        // Devuelve el canal asignado actualmente
        uint8_t GetMixerChannel();



    // Private
    private:

        // Instancia del sonido
        sf::Sound sound;

        // Parametros
        int32_t _panning;
        int32_t _volume;
        float _panning_attenuation;
        uint8_t _mixer_channel;

};




#endif // NGN_AUDIO_CLIP_H_INCLUDED
