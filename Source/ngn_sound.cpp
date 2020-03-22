/******************************************************************************

    N'gine Lib for C++
    *** Version 0.12.0-wip_1 ***
    Sonido

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2020 by Cesar Rincon "NightFox"
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



/*** Constructor de la clase  NGN_Sound ***/
NGN_Sound::NGN_Sound() {

    // Limpia la cola de sonidos
    sfx_cue.clear();
    // Limpia la cola de musicas
    music_cue.clear();

}

/*** Destructor de la clase NGN_Sound ***/
NGN_Sound::~NGN_Sound() {

    // Limpia la cola de sonidos
    if (sfx_cue.size() > 0) {
        for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
            // Deten el sonido
            sfx_cue[i]->Stop();
            // Borralo
            delete sfx_cue[i];
        }
    }
    sfx_cue.clear();

    // Limpia la cola de musicas
    if (music_cue.size() > 0) {
        for (uint32_t i = 0; i < music_cue.size(); i ++) {
            // Deten el sonido
            music_cue[i]->Stop();
            // Borralo
            delete music_cue[i];
        }
    }
    music_cue.clear();

}



/*** Reproduce un sonido ***/
NGN_AudioClip* NGN_Sound::PlaySfx(
        NGN_AudioClipData* sound,       // Clip de audio
        int32_t volume,            // Volumen
        int32_t panning,                    // Panning (-100 a 100)
        bool loop                       // Loop ?
    ) {

    // Si no se ha alcanzado el limite de SFX simultaneos...
    if (sfx_cue.size() < MAX_SFX_CHANNELS) {

        // Crea un nuevo clip en la cola de SFX
        sfx_cue.push_back(new NGN_AudioClip());

        // ID
        uint32_t id = (sfx_cue.size() - 1);

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



/*** Continua la reproduccion de un sonido ***/
void NGN_Sound::ResumeSfx(NGN_AudioClip* sound) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    sound->Resume();
                    break;
                }
            }
        }
    }

}



/*** Pausa un sonido ***/
void NGN_Sound::PauseSfx(NGN_AudioClip* sound) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    sound->Pause();
                    break;
                }
            }
        }
    }

}



/*** Deten un sonido ***/
void NGN_Sound::StopSfx(NGN_AudioClip* sound) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    sound->Stop();
                    break;
                }
            }
        }
    }

}



/*** Cambia el volumen de un sonido ***/
void NGN_Sound::SfxVolume(NGN_AudioClip* sound, int32_t volume) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    sound->Volume(volume);
                    break;
                }
            }
        }
    }

}



/*** Devuelve el nivel de volumen de un sonido ***/
uint32_t NGN_Sound::SfxGetVolume(NGN_AudioClip* sound) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    return sound->GetVolume();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve 0
    return 0;

}



/*** Cambia el pitch de un sonido ***/
void NGN_Sound::SfxPitch(NGN_AudioClip* sound, float pitch) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    sound->Pitch(pitch);
                    break;
                }
            }
        }
    }
}



/*** Devuelve el pitch actual de un sonido ***/
float NGN_Sound::SfxGetPitch(NGN_AudioClip* sound) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    return sound->GetPitch();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve 0
    return 0.0f;

}



/*** Cambia el loop ***/
void NGN_Sound::SfxLoop(NGN_AudioClip* sound, bool loop) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    sound->Loop(loop);
                    break;
                }
            }
        }
    }

}



/*** Devuelve el loop actual de un sonido ***/
bool NGN_Sound::SfxGetLoop(NGN_AudioClip* sound) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    return sound->GetLoop();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve FALSE
    return false;

}



/*** Cambia el panning ***/
void NGN_Sound::SfxPanning(NGN_AudioClip* sound, int32_t panning) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    sound->Panning(panning);
                    break;
                }
            }
        }
    }

}



/*** Devuelve el panning actual de un sonido ***/
int32_t NGN_Sound::SfxGetPanning(NGN_AudioClip* sound) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    return sound->GetPanning();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve FALSE
    return false;

}



/*** Devuelve si se esta reproduciendo un sonido ***/
bool NGN_Sound::SfxIsPlaying(NGN_AudioClip* sound) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    return sound->IsPlaying();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve FALSE
    return false;

}



/*** Devuelve si se esta reproduciendo un sonido ***/
bool NGN_Sound::SfxIsAlive(NGN_AudioClip* sound) {

    // Si el sonido es valido
    if (sound != NULL) {
        if (sfx_cue.size() > 0) {
            // Buscalo en la cola de sonidos
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                if (sfx_cue[i] == sound) {
                    return sound->IsAlive();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve FALSE
    return false;

}



/*** Actualiza la cola de SFX ***/
void NGN_Sound::SfxUpdate() {

    bool repeat = false;

    do {
        // No repitas por defecto
        repeat = false;
        // Si hay sonidos en cola...
        if (sfx_cue.size() > 0) {
            for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
                // Si el sonido, ya no esta activo, eliminalo
                if (!sfx_cue[i]->IsAlive()) {
                    delete sfx_cue[i];
                    sfx_cue[i] = NULL;
                    sfx_cue.erase(sfx_cue.begin() + i);
                    repeat = true;
                    break;
                }
            }
        }
    } while (repeat);

    //std::cout << "SFX cue: " << sfx_cue.size() << std::endl;

}



/*** Abre una musica para streaming (sobrecarga 1) ***/
NGN_MusicClip* NGN_Sound::OpenMusic(
        const char* filepath,   // Archivo de audio
        bool auto_start,        // Reproduccion automatica
        int32_t volume,         // Volumen
        bool loop               // Loop ?
    ) {

    // Si no se ha alcanzado el numero
    if (music_cue.size() < MAX_MUSIC_CHANNELS) {

        // Crea un nuevo clip en la cola de musicas
        music_cue.push_back(new NGN_MusicClip());

        // ID
        uint32_t id = (music_cue.size() - 1);

        // Abre el archivo
        if (!music_cue[id]->Open(filepath)) {
            // Si hay un error, eliminalo y sal
            sfx_cue.erase(sfx_cue.begin() + id);
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
        const char* filepath,           // Archivo de audio
        int32_t loop_start,             // Inicio del loop (milisegundos)
        int32_t loop_end,               // Fin del loop (milisegundos)
        bool auto_start,                // Reproduccion automatica
        int32_t volume                  // Volumen
    ) {

    // Si no se ha alcanzado el numero
    if (music_cue.size() < MAX_MUSIC_CHANNELS) {

        // Crea un nuevo clip en la cola de musicas
        music_cue.push_back(new NGN_MusicClip());

        // ID
        uint32_t id = (music_cue.size() - 1);

        // Abre el archivo
        if (!music_cue[id]->Open(filepath)) {
            // Si hay un error, eliminalo y sal
            sfx_cue.erase(sfx_cue.begin() + id);
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
                        NGN_MusicClip* music,
                        int32_t volume,    // Volumen
                        bool loop               // Loop ?
                        ) {

    // Si la musica es valida
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    music->Play(volume, loop);
                    break;
                }
            }
        }
    }

}



/*** Cierra el stream de audio abierto ***/
void NGN_Sound::CloseMusic(NGN_MusicClip* music) {

    if (MusicIsAlive(music)) music->Stop();

}



/*** Continua con la reproduccion de una musica ***/
void NGN_Sound::ResumeMusic(NGN_MusicClip* music) {
    // Si la musica es valida
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    music->Resume();
                    break;
                }
            }
        }
    }
}



/*** Pausa la musica ***/
void NGN_Sound::PauseMusic(NGN_MusicClip* music) {
    // Si la musica es valida
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    music->Pause();
                    break;
                }
            }
        }
    }
}



/*** Deten la musica ***/
void NGN_Sound::StopMusic(NGN_MusicClip* music) {
    // Si la musica es valida
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    music->Pause();
                    music->Rewind();
                    break;
                }
            }
        }
    }
}



/*** Cambia el volumen de una musica ***/
void NGN_Sound::MusicVolume(NGN_MusicClip* music, int32_t volume) {

    // Si el sonido es valido
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    music->Volume(volume);
                    break;
                }
            }
        }
    }

}



/*** Devuelve el nivel de volumen de una musica ***/
uint32_t NGN_Sound::MusicGetVolume(NGN_MusicClip* music) {

    // Si el sonido es valido
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    return music->GetVolume();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve 0
    return 0;

}



/*** Cambia el pitch de una musica ***/
void NGN_Sound::MusicPitch(NGN_MusicClip* music, float pitch) {

    // Si el sonido es valido
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    music->Pitch(pitch);
                    break;
                }
            }
        }
    }

}



/*** Devuelve el pitch actual de una musica ***/
float NGN_Sound::MusicGetPitch(NGN_MusicClip* music) {

    // Si el sonido es valido
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    return music->GetPitch();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve 0
    return 0;

}



/*** Cambia el loop de una musica ***/
void NGN_Sound::MusicLoop(NGN_MusicClip* music, bool loop) {

    // Si el sonido es valido
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    music->Loop(loop);
                    break;
                }
            }
        }
    }

}



/*** Devuelve el loop actual de una musica ***/
bool NGN_Sound::MusicGetLoop(NGN_MusicClip* music) {

    // Si el sonido es valido
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    return music->GetLoop();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve FALSE
    return false;

}



/*** Devuelve si se esta reproduciendo ***/
bool NGN_Sound::MusicIsPlaying(NGN_MusicClip* music) {

    // Si el sonido es valido
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    return music->IsPlaying();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve FALSE
    return false;

}



/*** Devuelve si aun existe ***/
bool NGN_Sound::MusicIsAlive(NGN_MusicClip* music) {

    // Si el sonido es valido
    if (music != NULL) {
        if (music_cue.size() > 0) {
            // Buscalo en la cola de musicas
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                if (music_cue[i] == music) {
                    return music->IsAlive();
                }
            }
        }
    }

    // Si no lo es, o no se encuentra, devuelve FALSE
    return false;

}



/*** Actualiza la cola de Musicas ***/
void NGN_Sound::MusicUpdate() {

    bool repeat = false;

    do {
        // No repitas por defecto
        repeat = false;
        // Si hay sonidos en cola...
        if (music_cue.size() > 0) {
            for (uint32_t i = 0; i < music_cue.size(); i ++) {
                // Si el sonido, ya no esta activo, eliminalo
                if (!music_cue[i]->IsAlive()) {
                    delete music_cue[i];
                    music_cue[i] = NULL;
                    music_cue.erase(music_cue.begin() + i);
                    repeat = true;
                    break;
                }
            }
        }
    } while (repeat);

    //std::cout << "Music cue: " << music_cue.size() << std::endl;

}



/*** Pausa todos los sonidos ***/
void NGN_Sound::PauseAll() {

    // Intenta pausar los SFX
    if (sfx_cue.size() > 0) {
        for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
            if (sfx_cue[i] != NULL) sfx_cue[i]->Pause();
        }
    }

    // Intenta pausar las musicas
    if (music_cue.size() > 0) {
        for (uint32_t i = 0; i < music_cue.size(); i ++) {
            if (music_cue[i] != NULL) music_cue[i]->Pause();
        }
    }

}



/*** Continua la reproduccion de todos los sonidos ***/
void NGN_Sound::ResumeAll() {

    // Intenta pausar los SFX
    if (sfx_cue.size() > 0) {
        for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
            if (sfx_cue[i] != NULL) sfx_cue[i]->Resume();
        }
    }

    // Intenta pausar las musicas
    if (music_cue.size() > 0) {
        for (uint32_t i = 0; i < music_cue.size(); i ++) {
            if (music_cue[i] != NULL) music_cue[i]->Resume();
        }
    }

}



/*** Deten todos los sonidos ***/
void NGN_Sound::StopAll() {

    // Intenta detener los SFX
    if (sfx_cue.size() > 0) {
        for (uint32_t i = 0; i < sfx_cue.size(); i ++) {
            if (sfx_cue[i] != NULL) sfx_cue[i]->Stop();
        }
    }

    // Intenta cerrar los strams de musica
    if (music_cue.size() > 0) {
        for (uint32_t i = 0; i < music_cue.size(); i ++) {
            if (music_cue[i] != NULL) music_cue[i]->Stop();
        }
    }

}



/*** Actualiza el modulo de sonido ***/
void NGN_Sound::Update() {

    SfxUpdate();
    MusicUpdate();

}
