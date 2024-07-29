/******************************************************************************

    N'gine Lib for C++
    *** Version 1.17.0-stable ***
    Sonido

    Proyecto iniciado el 1 de Febrero del 2016
    (c) 2016 - 2024 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com


	N'gine Lib is under MIT License

	Copyright (c) 2016-2024 by Cesar Rincon "NightFox"

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
static const uint8_t MIXER_CHANNELS = 5;        // Numero de canales del mixer
static const uint8_t MIXER_MASTER_CH = 0;       // Canal maestro del mixer
static const uint8_t MIXER_MUSIC_CH = 1;        // Canal de musica del mixer
static const uint8_t MIXER_EFFECTS_CH = 2;      // Canal de efectos del mixer
static const uint8_t MIXER_AUX1_CH = 3;         // Canal de efectos del mixer
static const uint8_t MIXER_AUX2_CH = 4;         // Canal de efectos del mixer




/*** Definicion de la clase NGN_Sound ***/
class NGN_Sound {

    // Public
    public:

        // Devuelve la instancia
        static NGN_Sound* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();



        /*** Efectos de sonido (SFX) [64 simultaneos maximo por defecto] ***/

        // Reproduce un sonido (1ra sobrecarga)
        NGN_AudioClip* PlaySfx(
            NGN_AudioClipData* sound,                    // Clip de audio
            int32_t volume = 100,                        // Volumen
            int32_t panning = 0,                         // Panning (-100 a 100)
            bool loop = false,                           // Loop ?
            uint8_t mixer_channel = MIXER_EFFECTS_CH     // Canal por defecto en el mixer
        );

        // Reproduce un sonido (2da sobrecarga)
        NGN_AudioClip* PlaySfx(
            std::string repo_name,                       // Nombre del repositorio
            std::string resource_name,                   // Nombre del recurso
            int32_t volume = 100,                        // Volumen
            int32_t panning = 0,                         // Panning (-100 a 100)
            bool loop = false,                           // Loop ?
            uint8_t mixer_channel = MIXER_EFFECTS_CH     // Canal por defecto en el mixer
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

        // Elimina todos los SFX en la cola
        void ClearSfx();



        /*** Musicas por streaming (BGM) [4 simultaneas maximo por defecto] ***/

        // Abre una musica para streaming (Sobrecarga 1)
        NGN_MusicClip* OpenMusic(
                                std::string filepath,                   // Archivo de audio
                                bool auto_start = true,                 // Reproduccion automatica
                                int32_t volume = 100,                   // Volumen
                                bool loop = true,                       // Loop ?
                                uint8_t mixer_channel = MIXER_MUSIC_CH  // Canal por defecto en el mixer
                                );

        // Abre una musica para streaming (Sobrecarga 2)
        NGN_MusicClip* OpenMusic(
                                std::string filepath,                   // Archivo de audio
                                int32_t loop_start,                     // Inicio del loop (milisegundos)
                                int32_t loop_end = NGN_DEFAULT_VALUE,   // Fin del loop (milisegundos)
                                bool auto_start = true,                 // Reproduccion automatica
                                int32_t volume = 100,                   // Volumen
                                uint8_t mixer_channel = MIXER_MUSIC_CH  // Canal por defecto en el mixer
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

        // Elimina todas las musicas en la cola
        void ClearMusic();



        /*** Procesos comunes ***/

        // Pausa todos los sonidos y musicas que esten sonando en ESE MOMENTO (no las posteriores)
        void PauseAll();

        // Continua la reproduccion de todos los sonidos y musicas pausadas con anterioridad
        void ResumeAll();

        // Deten todos los efectos de sonido, si hay streams de musica, cierralos
        void StopAll();

        // Actualiza la cola del audio
        void Update();

        // Ajusta el volumen del canal del mixer
        void SetMixerLevel(uint8_t channel, int32_t level);
        int32_t GetMixerLevel(uint8_t channel);
        // Guarda o restaura los valores del mixer
        void PushMixer();
        void PopMixer();


    // Private
    private:

        // Constructor de la clase
        NGN_Sound();

        // Destructor de la clase
        ~NGN_Sound();

        // Puntero de memoria a la instancia
        static NGN_Sound* instance;


        // Lista de sonidos disponibles
        std::vector <NGN_AudioClip*> sfx_cue;

        // Lista de musicas disponibles
        std::vector <NGN_MusicClip*> music_cue;


        // Actualiza la cola de SFX
        void SfxUpdate();

        // Actualiza la cola de musicas
        void MusicUpdate();

        // Parametros globales del volumen del sonido (mezclador de sonido)
        int32_t mixer_channel_level[MIXER_CHANNELS];
        int32_t last_mixer_channel_level[MIXER_CHANNELS];
        int32_t backup_mixer_channel_level[MIXER_CHANNELS];

        // Reproduce un sonido (procesos comunes en las sobrecargas)
        NGN_AudioClip* _PlaySfx(
            NGN_AudioClipData* sound,       // Clip de audio
            int32_t volume,                 // Volumen
            int32_t panning,                // Panning (-100 a 100)
            bool loop,                      // Loop ?
            uint8_t mixer_channel           // Canal por defecto en el mixer
        );

};



#endif // NGN_SOUND_H_INCLUDED
