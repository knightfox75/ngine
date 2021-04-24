/******************************************************************************

    N'gine Lib for C++
    *** Version 1.5.0-wip3 ***
    Clips de audio

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2021 by Cesar Rincon "NightFox"
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
