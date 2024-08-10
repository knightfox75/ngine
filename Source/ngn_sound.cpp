/******************************************************************************

    N'gine Lib for C++
    *** Version 1.18.0-wip_0x01 ***
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



/*** Includes ***/
// C++
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/*** Puntero de la instancia a NULL ***/
NGN_Sound* NGN_Sound::instance = NULL;



/*** Metodo para crear/obtener la instancia ***/
NGN_Sound* NGN_Sound::GetInstance() {

    // Verifica si la instancia ya se ha creado
    // Si no es asi, creala
    if (!instance) instance = new NGN_Sound();

    // Devuelve la instancia
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Sound::RemoveInstance() {

    // Si la instancia aun existe, eliminala
    if (instance) {
        delete instance;
        instance = NULL;
    }

}



/*** Constructor de la clase  NGN_Sound ***/
NGN_Sound::NGN_Sound() {

    // Limpia la cola de sonidos
    sfx_cue.clear();
    // Limpia la cola de musicas
    music_cue.clear();

    // Valores por defecto del mixer
    for (uint8_t i = 0; i < MIXER_CHANNELS; i ++) {
        mixer_channel_level[i] = 100;
        last_mixer_channel_level[i] = 100;
        backup_mixer_channel_level[i] = 100;
    }

}

/*** Destructor de la clase NGN_Sound ***/
NGN_Sound::~NGN_Sound() {

    // Limpia la cola de sonidos
    ClearSfx();
    // Limpia la cola de musicas
    ClearMusic();

}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Sound::BootUp() {
}



/*** Reproduce un sonido (1ra sobrecarga) ***/
NGN_AudioClip* NGN_Sound::PlaySfx(
        NGN_AudioClipData* sound,       // Clip de audio
        int32_t volume,                 // Volumen
        int32_t panning,                // Panning (-100 a 100)
        bool loop,                      // Loop ?
        uint8_t mixer_channel           // Canal asignado en el mixer
    ) {

    return _PlaySfx(
        sound,              // Clip de audio
        volume,             // Volumen
        panning,            // Panning (-100 a 100)
        loop,               // Loop ?
        mixer_channel       // Canal por defecto en el mixer
    );

}



/*** Reproduce un sonido (2da sobrecarga) ***/
NGN_AudioClip* NGN_Sound::PlaySfx(
        std::string repo_name,          // Nombre del repositorio
        std::string resource_name,      // Nombre del recurso
        int32_t volume,                 // Volumen
        int32_t panning,                // Panning (-100 a 100)
        bool loop,                      // Loop ?
        uint8_t mixer_channel           // Canal asignado en el mixer
    ) {

    return _PlaySfx(
        ngn->resources->GetSfx(repo_name, resource_name),   // Clip de audio
        volume,             // Volumen
        panning,            // Panning (-100 a 100)
        loop,               // Loop ?
        mixer_channel       // Canal por defecto en el mixer
    );

}



/*** Continua la reproduccion de un sonido ***/
void NGN_Sound::ResumeSfx(NGN_AudioClip* sound) {

    // Si no es un sonido valido...
    if (!sound) return;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) {
            sound->Resume();
            break;
        }
    }

}



/*** Pausa un sonido ***/
void NGN_Sound::PauseSfx(NGN_AudioClip* sound) {

    // Si no es un sonido valido...
    if (!sound) return;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) {
            sound->Pause();
            break;
        }
    }

}



/*** Deten un sonido ***/
void NGN_Sound::StopSfx(NGN_AudioClip* sound) {

    // Si no es un sonido valido...
    if (!sound) return;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) {
            sound->Stop();
            break;
        }
    }

}



/*** Cambia el volumen de un sonido ***/
void NGN_Sound::SfxVolume(NGN_AudioClip* sound, int32_t volume) {

    // Si no es un sonido valido...
    if (!sound) return;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) {
            sound->Volume(volume);
            break;
        }
    }

}



/*** Devuelve el nivel de volumen de un sonido ***/
uint32_t NGN_Sound::SfxGetVolume(NGN_AudioClip* sound) {

    // Si no es un sonido valido...
    if (!sound) return 0;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) return sound->GetVolume();
    }

    // Si no se encuentra, devuelve 0
    return 0;

}



/*** Cambia el pitch de un sonido ***/
void NGN_Sound::SfxPitch(NGN_AudioClip* sound, float pitch) {

    // Si no es un sonido valido...
    if (!sound) return;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) {
            sound->Pitch(pitch);
            break;
        }
    }

}



/*** Devuelve el pitch actual de un sonido ***/
float NGN_Sound::SfxGetPitch(NGN_AudioClip* sound) {

    // Si no es un sonido valido...
    if (!sound) return 0.0f;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) return sound->GetPitch();
    }

    // Si no se encuentra, devuelve 0
    return 0.0f;

}



/*** Cambia el loop ***/
void NGN_Sound::SfxLoop(NGN_AudioClip* sound, bool loop) {

    // Si no es un sonido valido...
    if (!sound) return;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) {
            sound->Loop(loop);
            break;
        }
    }

}



/*** Devuelve el loop actual de un sonido ***/
bool NGN_Sound::SfxGetLoop(NGN_AudioClip* sound) {

    // Si no es un sonido valido...
    if (!sound) return false;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) return sound->GetLoop();
    }

    // Si no se encuentra, devuelve FALSE
    return false;

}



/*** Cambia el panning ***/
void NGN_Sound::SfxPanning(NGN_AudioClip* sound, int32_t panning) {

    // Si no es un sonido valido...
    if (!sound) return;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) {
            sound->Panning(panning);
            break;
        }
    }

}



/*** Devuelve el panning actual de un sonido ***/
int32_t NGN_Sound::SfxGetPanning(NGN_AudioClip* sound) {

    // Si no es un sonido valido...
    if (!sound) return 0;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) return sound->GetPanning();
    }

    // Si no se encuentra, devuelve 0
    return 0;

}



/*** Devuelve si se esta reproduciendo un sonido ***/
bool NGN_Sound::SfxIsPlaying(NGN_AudioClip* sound) {

    // Si no es un sonido valido...
    if (!sound) return false;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) return sound->IsPlaying();
    }

    // Si no se encuentra, devuelve FALSE
    return false;

}



/*** Devuelve si se esta reproduciendo un sonido ***/
bool NGN_Sound::SfxIsAlive(NGN_AudioClip* sound) {

    // Si no es un sonido valido...
    if (!sound) return false;

    // Buscalo en la cola de sonidos
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] == sound) return sound->IsAlive();
    }

    // Si no se encuentra, devuelve FALSE
    return false;

}



/*** Actualiza la cola de SFX ***/
void NGN_Sound::SfxUpdate() {

    bool repeat = false;

    // Eliminacion de sonidos finalizados
    do {
        // No repitas por defecto
        repeat = false;
        // Recorre la cola de sonidos
        for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
            // Si el sonido, ya no esta activo, eliminalo
            if (!sfx_cue[i]->IsAlive()) {
                delete sfx_cue[i];
                sfx_cue.erase(sfx_cue.begin() + i);
                repeat = true;
                break;
            }
        }
    } while (repeat);

    // Ajuste de volumen si hay cambios en el mixer
    uint8_t ch = 0;
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        ch = sfx_cue[i]->GetMixerChannel();
        if ((mixer_channel_level[ch] != last_mixer_channel_level[ch]) || (mixer_channel_level[MIXER_MASTER_CH] != last_mixer_channel_level[MIXER_MASTER_CH])) {
            sfx_cue[i]->Volume(sfx_cue[i]->GetVolume());
        }
    }

    //std::cout << "SFX cue: " << sfx_cue.size() << std::endl;

}


/*** Elimina todos los SFX en la cola ***/
void NGN_Sound::ClearSfx() {

    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        // Deten el sonido
        sfx_cue[i]->Stop();
        // Borralo
        delete sfx_cue[i];
    }
    sfx_cue.clear();

}



/*** Abre una musica para streaming (sobrecarga 1) ***/
NGN_MusicClip* NGN_Sound::OpenMusic(
        std::string filepath,   // Archivo de audio
        bool auto_start,        // Reproduccion automatica
        int32_t volume,         // Volumen
        bool loop,              // Loop ?
        uint8_t mixer_channel   // Canal por defecto en el mixer
    ) {

    // Si no se ha alcanzado el numero
    if (music_cue.size() < MAX_MUSIC_CHANNELS) {

        // Crea un nuevo clip en la cola de musicas
        music_cue.push_back(new NGN_MusicClip());

        // ID
        uint32_t id = (music_cue.size() - 1);

        // Canal del mixer
        if ((mixer_channel > 0) && (mixer_channel < MIXER_CHANNELS)) music_cue[id]->SetMixerChannel(mixer_channel);

        // Abre el archivo
        if (!music_cue[id]->Open(filepath)) {
            // Si hay un error, eliminalo y sal
            delete music_cue[id];
            music_cue.erase(music_cue.begin() + id);
            return NULL;
        }

        // Y reproducelo si esta el flag de autostart
        music_cue[id]->Play(volume, loop);
        if (!auto_start) music_cue[id]->Pause();

        // Devuelve la referencia al clip creado
        return music_cue[id];

    } else {

        return NULL;

    }

}



/*** Abre una musica para streaming (sobrecarga 2) ***/
NGN_MusicClip* NGN_Sound::OpenMusic(
        std::string filepath,           // Archivo de audio
        int32_t loop_start,             // Inicio del loop (milisegundos)
        int32_t loop_end,               // Fin del loop (milisegundos)
        bool auto_start,                // Reproduccion automatica
        int32_t volume,                 // Volumen
        uint8_t mixer_channel           // Canal por defecto en el mixer
    ) {

    // Si no se ha alcanzado el numero
    if (music_cue.size() < MAX_MUSIC_CHANNELS) {

        // Crea un nuevo clip en la cola de musicas
        music_cue.push_back(new NGN_MusicClip());

        // ID
        uint32_t id = (music_cue.size() - 1);

        // Canal del mixer
        if ((mixer_channel > 0) && (mixer_channel < MIXER_CHANNELS)) music_cue[id]->SetMixerChannel(mixer_channel);

        // Abre el archivo
        if (!music_cue[id]->Open(filepath)) {
            // Si hay un error, eliminalo y sal
            delete music_cue[id];
            music_cue.erase(music_cue.begin() + id);
            return NULL;
        }

        // Aplicale los puntos de loop
        music_cue[id]->SetLoop(loop_start, loop_end);

        // Reproducelo si esta el flag de autostart
        music_cue[id]->Play(volume, true);
        if (!auto_start) music_cue[id]->Pause();

        // Devuelve la referencia al clip creado
        return music_cue[id];

    } else {

        return NULL;

    }

}



/*** Reproduce una musica desde el principio o quita la pausa ***/
void NGN_Sound::PlayMusic(
    NGN_MusicClip* music,   // Stream de audio
    int32_t volume,         // Volumen
    bool loop               // Loop ?
) {

    // Si el stream no es valido
    if (!music) return;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) {
            music->Play(volume, loop);
            break;
        }
    }

}



/*** Cierra el stream de audio abierto ***/
void NGN_Sound::CloseMusic(NGN_MusicClip* music) {

    // Si el stream no es valido
    if (!music) return;

    // Si la musica esta sonando, detenla
    if (MusicIsAlive(music)) {
        music->Stop();
    }

    // Actualiza la cola de musicas
    MusicUpdate();

    // Anula la referencia a al stream
    music = NULL;

}



/*** Continua con la reproduccion de una musica ***/
void NGN_Sound::ResumeMusic(NGN_MusicClip* music) {

    // Si el stream no es valido
    if (!music) return;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) {
            music->Resume();
            break;
        }
    }

}



/*** Pausa la musica ***/
void NGN_Sound::PauseMusic(NGN_MusicClip* music) {

    // Si el stream no es valido
    if (!music) return;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) {
            music->Pause();
            break;
        }
    }

}



/*** Deten la musica ***/
void NGN_Sound::StopMusic(NGN_MusicClip* music) {

    // Si el stream no es valido
    if (!music) return;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) {
            music->Pause();
            music->Rewind();
            break;
        }
    }

}



/*** Cambia el volumen de una musica ***/
void NGN_Sound::MusicVolume(NGN_MusicClip* music, int32_t volume) {

    // Si el stream no es valido
    if (!music) return;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) {
            music->Volume(volume);
            break;
        }
    }

}



/*** Devuelve el nivel de volumen de una musica ***/
uint32_t NGN_Sound::MusicGetVolume(NGN_MusicClip* music) {

    // Si el stream no es valido
    if (!music) return 0;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) return music->GetVolume();
    }

    // Si no se encuentra, devuelve 0
    return 0;

}



/*** Cambia el pitch de una musica ***/
void NGN_Sound::MusicPitch(NGN_MusicClip* music, float pitch) {

    // Si el stream no es valido
    if (!music) return;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) {
            music->Pitch(pitch);
            break;
        }
    }

}



/*** Devuelve el pitch actual de una musica ***/
float NGN_Sound::MusicGetPitch(NGN_MusicClip* music) {

    // Si el stream no es valido
    if (!music) return 0.0f;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) return music->GetPitch();
    }

    // Si no se encuentra, devuelve 0
    return 0.0f;

}



/*** Cambia el loop de una musica ***/
void NGN_Sound::MusicLoop(NGN_MusicClip* music, bool loop) {

    // Si el stream no es valido
    if (!music) return;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) {
            music->Loop(loop);
            break;
        }
    }

}



/*** Devuelve el loop actual de una musica ***/
bool NGN_Sound::MusicGetLoop(NGN_MusicClip* music) {

    // Si el stream no es valido
    if (!music) return false;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) return music->GetLoop();
    }

    // Si no se encuentra, devuelve FALSE
    return false;

}



/*** Devuelve si se esta reproduciendo ***/
bool NGN_Sound::MusicIsPlaying(NGN_MusicClip* music) {

    // Si el stream no es valido
    if (!music) return false;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) return music->IsPlaying();
    }

    // Si no se encuentra, devuelve FALSE
    return false;

}



/*** Devuelve si aun existe ***/
bool NGN_Sound::MusicIsAlive(NGN_MusicClip* music) {

    // Si el stream no es valido
    if (!music) return false;

    // Buscalo en la cola de musicas
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] == music) return music->IsAlive();
    }

    // Si no se encuentra, devuelve FALSE
    return false;

}



/*** Actualiza la cola de Musicas ***/
void NGN_Sound::MusicUpdate() {

    bool repeat = false;

    // Control de la cola de las musicas
    do {
        // No repitas por defecto
        repeat = false;
        // Recorre la cola de streams de sonido
        for (uint32_t i = 0; i < music_cue.size(); i ++) {
            // Si el sonido, ya no esta activo, eliminalo
            if (!music_cue[i]->IsAlive()) {
                delete music_cue[i];
                music_cue.erase(music_cue.begin() + i);
                repeat = true;
                break;
            }
        }
    } while (repeat);

    // Ajuste de volumen si hay cambios en el mixer
    uint8_t ch = 0;
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        ch = music_cue[i]->GetMixerChannel();
        if ((mixer_channel_level[ch] != last_mixer_channel_level[ch]) || (mixer_channel_level[MIXER_MASTER_CH] != last_mixer_channel_level[MIXER_MASTER_CH])) {
            music_cue[i]->Volume(music_cue[i]->GetVolume());
        }
    }

    //std::cout << "Music cue: " << music_cue.size() << std::endl;

}


/*** Elimina todas las musicas en la cola ***/
void NGN_Sound::ClearMusic() {

    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        // Deten el sonido
        music_cue[i]->Stop();
        // Borralo
        delete music_cue[i];
    }
    music_cue.clear();

}



/*** Pausa todos los sonidos ***/
void NGN_Sound::PauseAll() {

    // Intenta pausar los SFX
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] != NULL) sfx_cue[i]->Pause();
    }

    // Intenta pausar los streams de sonido
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] != NULL) music_cue[i]->Pause();
    }

}



/*** Continua la reproduccion de todos los sonidos ***/
void NGN_Sound::ResumeAll() {

    // Intenta quitar la pausa a los SFX
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] != NULL) sfx_cue[i]->Resume();
    }

    // Intenta quitar la pausa a los streams de sonido
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] != NULL) music_cue[i]->Resume();
    }

}



/*** Deten todos los sonidos ***/
void NGN_Sound::StopAll() {

    // Intenta detener los SFX
    for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
        if (sfx_cue[i] != NULL) sfx_cue[i]->Stop();
    }

    // Intenta cerrar los streams de musica
    for (uint32_t i = 0; i < music_cue.size(); i ++) {
        if (music_cue[i] != NULL) music_cue[i]->Stop();
    }

}



/*** Actualiza el modulo de sonido ***/
void NGN_Sound::Update() {

    SfxUpdate();
    MusicUpdate();

    for (uint8_t i = 0; i < MIXER_CHANNELS; i ++) last_mixer_channel_level[i] = mixer_channel_level[i];

}



/*** Ajusta el volumen de un canal del mixer ***/
void NGN_Sound::SetMixerLevel(uint8_t channel, int32_t level) {

    if (channel >= MIXER_CHANNELS) return;

    mixer_channel_level[channel] = level;
    if (mixer_channel_level[channel] < 0) mixer_channel_level[channel] = 0;
    if (mixer_channel_level[channel] > 100) mixer_channel_level[channel] = 100;

}



/*** Obten el volumen de un canal del mixer ***/
int32_t NGN_Sound::GetMixerLevel(uint8_t channel) {

    if (channel >= MIXER_CHANNELS) return 0;

    return mixer_channel_level[channel];

}



/*** Guarda los valores actuales del mixer ***/
void NGN_Sound::PushMixer() {

    for (uint8_t i = 0; i < MIXER_CHANNELS; i ++) {
        backup_mixer_channel_level[i] = mixer_channel_level[i];
    }

}



/*** Restaura los valores del mixer guardados ***/
void NGN_Sound::PopMixer() {

    for (uint8_t i = 0; i < MIXER_CHANNELS; i ++) {
        mixer_channel_level[i] = backup_mixer_channel_level[i];
    }

}



/*** Reproduce un sonido (procesos comunes en las sobrecargas) ***/
NGN_AudioClip* NGN_Sound::_PlaySfx(
    NGN_AudioClipData* sound,       // Clip de audio
    int32_t volume,                 // Volumen
    int32_t panning,                // Panning (-100 a 100)
    bool loop,                      // Loop ?
    uint8_t mixer_channel           // Canal por defecto en el mixer
) {

    // Si no se ha alcanzado el limite de SFX simultaneos...
    if (sfx_cue.size() < MAX_SFX_CHANNELS) {

        // Crea un nuevo clip en la cola de SFX
        sfx_cue.push_back(new NGN_AudioClip());

        // ID
        uint32_t id = (sfx_cue.size() - 1);

        if ((mixer_channel > 0) && (mixer_channel < MIXER_CHANNELS)) sfx_cue[id]->SetMixerChannel(mixer_channel);

        // Asignale el sonido a la instancia creada
        sfx_cue[id]->Clip(sound);

        // Y reproducelo
        sfx_cue[id]->Play(volume, panning, loop);

        // Devuelve la referencia al clip creado
        return sfx_cue[id];

    } else {

        return NULL;

    }

}
