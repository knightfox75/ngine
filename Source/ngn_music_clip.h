/******************************************************************************

    N'gine Lib for C++
    *** Version 0.4.2-alpha ***
    Clips de musica

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
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



#ifndef NGN_MUSIC_CLIP_H_INCLUDED
#define NGN_MUSIC_CLIP_H_INCLUDED


/*** Includes ***/

// C++
#include <vector>

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
        bool Open(const char* filepath);

        // Reproduce la musica [1ª sobrecarga]
        void Play(
                  int32_t volume,      // Volumen inicial
                  bool loop                 // Loop?
                  );

        // Reproduce el la musica [2ª sobrecarga]
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

        // Revobina el stream de audio
        void Rewind();



    // Private
    private:

        // Stream de musica
        sf::Music music;

        // Parametros;
        int32_t _volume;


};



#endif // NGN_MUSIC_CLIP_H_INCLUDED
