/******************************************************************************

    N'gine Lib for C++
    *** Version 1.3.0-beta ***
    Sonido

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



#ifndef NGN_SOUND_H_INCLUDED
#define NGN_SOUND_H_INCLUDED



/*** Includes ***/

// C++
#include <cstdint>                  // Tipos de datos INTXX_T de C++ 11
#include <vector>
#include <string>


// SFML
#include <SFML/Audio.hpp>


// Includes de la libreria
#include "ngn_defines.h"
#include "ngn_audio_clip.h"
#include "ngn_music_clip.h"



/*** Definiciones ***/
static const uint8_t MAX_SFX_CHANNELS = 64;     // Numero maximo de efectos simultaneos
static const uint8_t MAX_MUSIC_CHANNELS = 4;    // Numero maximo de musicas por streaming simultaneas




/*** Definicion de la clase NGN_Sound ***/
class NGN_Sound {

    // Public
    public:

        // Constructor de la clase
        NGN_Sound();

        // Destructor de la clase
        ~NGN_Sound();


        /*** Efectos de sonido (SFX) [64 simultaneos maximo por defecto] ***/

        // Reproduce un sonido
        NGN_AudioClip* PlaySfx(
                               NGN_AudioClipData* sound,        // Clip de audio
                               int32_t volume = 100,       // Volumen
                               int32_t panning = 0,                 // Panning (-100 a 100)
                               bool loop = false                // Loop ?
                               );

        // Continua la reproduccion de un sonido
        void ResumeSfx(NGN_AudioClip* sound);

        // Pausa un sonido
        void PauseSfx(NGN_AudioClip* sound);

        // Deten un sonido
        void StopSfx(NGN_AudioClip* sound);

        // Cambia el volumen de un sonido
        void SfxVolume(NGN_AudioClip* sound, int32_t volume = 100);

        // Devuelve el nivel de volumen de un sonido
        uint32_t SfxGetVolume(NGN_AudioClip* sound);

        // Cambia el pitch de un sonido
        void SfxPitch(NGN_AudioClip* sound, float pitch = 1.0f);

        // Devuelve el pitch actual de un sonido
        float SfxGetPitch(NGN_AudioClip* sound);

        // Cambia el loop
        void SfxLoop(NGN_AudioClip* sound, bool loop = true);

        // Devuelve el loop
        bool SfxGetLoop(NGN_AudioClip* sound);

        // Cambia el panning
        void SfxPanning(NGN_AudioClip* sound, int32_t panning = 0);

        // Devuelve el panning del sonido
        int32_t SfxGetPanning(NGN_AudioClip* sound);

        // Devuelve si el sonido se esta reproduciendo
        bool SfxIsPlaying(NGN_AudioClip* sound);

        // Devuelve si el sonido aun existe
        bool SfxIsAlive(NGN_AudioClip* sound);



        /*** Musicas por streaming (BGM) [4 simultaneas maximo por defecto] ***/

        // Abre una musica para streaming (Sobrecarga 1)
        NGN_MusicClip* OpenMusic(
                                const char* filepath,           // Archivo de audio
                                bool auto_start = true,         // Reproduccion automatica
                                int32_t volume = 100,           // Volumen
                                bool loop = true                // Loop ?
                                );

        // Abre una musica para streaming (Sobrecarga 2)
        NGN_MusicClip* OpenMusic(
                                const char* filepath,                   // Archivo de audio
                                int32_t loop_start,                     // Inicio del loop (milisegundos)
                                int32_t loop_end = NGN_DEFAULT_VALUE,   // Fin del loop (milisegundos)
                                bool auto_start = true,                 // Reproduccion automatica
                                int32_t volume = 100                    // Volumen
                                );

        // Cierra el stream de audio
        void CloseMusic(NGN_MusicClip* music);

        // Reproduce una musica desde el principio o quita la pausa
        void PlayMusic(
                    NGN_MusicClip* music,           // Clip de musica
                    int32_t volume = 100,      // Volumen
                    bool loop = true                // Loop ?
                    );

        // Continua la reproduccion de una musica
        void ResumeMusic(NGN_MusicClip* music);

        // Pausa la musica
        void PauseMusic(NGN_MusicClip* music);

        // Deten la musica
        void StopMusic(NGN_MusicClip* music);

        // Cambia el volumen de una musica
        void MusicVolume(NGN_MusicClip* music, int32_t volume = 100);

        // Devuelve el nivel de volumen de una musica
        uint32_t MusicGetVolume(NGN_MusicClip* music);

        // Cambia el pitch de una musica
        void MusicPitch(NGN_MusicClip* music, float pitch = 1.0f);

        // Devuelve el pitch actual de una musica
        float MusicGetPitch(NGN_MusicClip* music);

        // Cambia el loop
        void MusicLoop(NGN_MusicClip* music, bool loop = true);

        // Devuelve el loop
        bool MusicGetLoop(NGN_MusicClip* music);

        // Devuelve si se esta reproduciendo
        bool MusicIsPlaying(NGN_MusicClip* music);

        // Devuelve si aun existe
        bool MusicIsAlive(NGN_MusicClip* music);



        /*** Procesos comunes ***/

        // Pausa todos los sonidos y musicas que esten sonando en ESE MOMENTO (no las posteriores)
        void PauseAll();

        // Continua la reproduccion de todos los sonidos y musicas pausadas con anterioridad
        void ResumeAll();

        // Deten todos los efectos de sonido, si hay streams de musica, cierralos
        void StopAll();

        // Actualiza la cola del audio
        void Update();



    // Private
    private:

        // Lista de sonidos disponibles
        std::vector <NGN_AudioClip*> sfx_cue;

        // Lista de musicas disponibles
        std::vector <NGN_MusicClip*> music_cue;


        // Actualiza la cola de SFX
        void SfxUpdate();

        // Actualiza la cola de musicas
        void MusicUpdate();

};



#endif // NGN_SOUND_H_INCLUDED
