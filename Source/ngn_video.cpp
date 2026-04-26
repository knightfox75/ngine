/******************************************************************************

    N'gine Lib for C++
    *** Version 1.22.0-wip_0x04 ***
    Video player - Manager de streams de video

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
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

// SDL
#include <SDL.h>

// Libreria
#include "ngn.h"



/*** Puntero de la instancia a nullptr ***/
NGN_Video* NGN_Video::instance = nullptr;



/*** Metodo para crear/obtener la instancia ***/
NGN_Video* NGN_Video::GetInstance() {

    if (!instance) instance = new NGN_Video();
    return instance;

}



/*** Metodo para eliminar la instancia ***/
void NGN_Video::RemoveInstance() {

    if (instance) {
        delete instance;
        instance = nullptr;
    }

}



/*** Constructor de la clase ***/
NGN_Video::NGN_Video() {

    video_cue.clear();

}



/*** Destructor de la clase ***/
NGN_Video::~NGN_Video() {

    CloseAll();

}



/*** Procesos iniciales despues de crear la instancia ***/
void NGN_Video::BootUp() {
}



/******************************************************************************
    Gestion de streams
******************************************************************************/

/*** Abre un archivo OGV y crea un stream listo para reproducir ***/
NGN_VideoStream* NGN_Video::Open(
    std::string filepath,
    bool auto_start,
    bool loop,
    int32_t volume,
    uint32_t render_width,
    uint32_t render_height,
    int32_t position_x,
    int32_t position_y,
    uint8_t mixer_channel
) {

    // Comprueba si se ha alcanzado el limite de streams simultaneos
    if (video_cue.size() >= MAX_VIDEO_STREAMS) {
        ngn->log->Message("[NGN_Video error] Maximum simultaneous video streams reached (" + std::to_string(MAX_VIDEO_STREAMS) + ").");
        return nullptr;
    }

    // Crea el stream e intenta abrirlo
    NGN_VideoStream* stream = new NGN_VideoStream();

    if (!stream->Open(filepath)) {
        ngn->log->Message("[NGN_Video error] Error opening <" + filepath + "> for streaming.");
        delete stream;
        return nullptr;
    }

    // Aplica el loop
    stream->loop = loop;

    // Guarda el canal del mixer
    stream->_mixer_channel = mixer_channel;
    // Aplica el volumen
    stream->Volume(volume);

    // Aplica las dimensiones de render si se han especificado
    if ((render_width != (uint32_t)NGN_DEFAULT_VALUE) && (render_height != (uint32_t)NGN_DEFAULT_VALUE)) {
        stream->width = (float)render_width;
        stream->height = (float)render_height;
    } 
    
    // Aplica la posicion: si no se especifica, centra en la resolucion nativa
    if ((position_x != NGN_DEFAULT_VALUE) && (position_y != NGN_DEFAULT_VALUE)) {
        stream->position.x = (float)position_x;
        stream->position.y = (float)position_y;
    } else {
        stream->position.x = (float)((ngn->graphics->native_w - (int32_t)stream->width)  / 2);
        stream->position.y = (float)((ngn->graphics->native_h - (int32_t)stream->height) / 2);
    }

    // Anade el stream a la cola
    video_cue.push_back(stream);

    // Fuerza que el video este disponible inmediatamente (reproducelo y dejalo en pausa)
    stream->Play();
    stream->Pause();

    // Inicia la reproduccion si auto_start esta activo
    if (auto_start) stream->Play();

    // Devuelve el puntero al stream
    return stream;

}



/*** Cierra un stream y libera sus recursos ***/
void NGN_Video::Close(NGN_VideoStream* stream) {

    // Si el stream no es valido, sal
    if (!stream) return;

    // Si el stream no esta en cola, sal
    if (!StreamInCue(stream)) return;

    // Detiene la reproduccion si esta activa
    if (stream->IsAlive()) stream->Stop();

    // Eliminalo de la cola
    bool repeat = false;
    do {
        repeat = false;
        for (uint32_t i = 0; i < video_cue.size(); i++) {
            if (video_cue[i] != stream) continue;
            delete video_cue[i];
            video_cue.erase(video_cue.begin() + i);
            repeat = true;
            break;
        }
    } while (repeat);

}



/*** Cierra todos los streams activos ***/
void NGN_Video::CloseAll() {

    for (uint32_t i = 0; i < video_cue.size(); i++) {
        video_cue[i]->Close();
        delete video_cue[i];
    }
    video_cue.clear();

}



/******************************************************************************
    Control de reproduccion de un stream
******************************************************************************/

/*** Inicia (o reanuda) la reproduccion de un stream de video ***/
void NGN_Video::Play(NGN_VideoStream* stream) {
    if (!stream) return;
    if (!StreamInCue(stream)) return;
    stream->Play();
}



/*** Pausa el stream de video especificado ***/
void NGN_Video::Pause(NGN_VideoStream* stream) {
    if (!stream) return;
    if (!StreamInCue(stream)) return;
    stream->Pause();
}



/*** Deten el stream de video especificado ***/
void NGN_Video::Stop(NGN_VideoStream* stream) {
    if (!stream) return;
    if (!StreamInCue(stream)) return;
    stream->Stop();
}



/*** Cambia el estado del loop del video ***/
void NGN_Video::SetLoop(NGN_VideoStream* stream, bool loop) {
    if (!stream) return;
    if (!StreamInCue(stream)) return;
    stream->loop = loop;
}



/******************************************************************************
    Control de reproduccion de un stream
******************************************************************************/

/*** Pausa todos los stream abiertos ***/
void NGN_Video::PauseAll() {
    for (uint32_t i = 0; i < video_cue.size(); i++) {
        if (!video_cue[i]) continue;
        video_cue[i]->Pause();
    }
}



/*** Reanuda la reproduccion de todos los stream en pausa ***/
void NGN_Video::ResumeAll() {
    for (uint32_t i = 0; i < video_cue.size(); i++) {
        if (!video_cue[i]) continue;
        if (!video_cue[i]->_paused) continue;
        video_cue[i]->Play();
    }
}



/*** Deten todos los stream de video ***/
void NGN_Video::StopAll() {
    for (uint32_t i = 0; i < video_cue.size(); i++) {
        if (!video_cue[i]) continue;
        video_cue[i]->Stop();
    }
}



/******************************************************************************
    Consulta de estado de un stream
******************************************************************************/

/*** El stream esta en reproduccion ? ***/
bool NGN_Video::IsPlaying(NGN_VideoStream* stream) {
    if (!stream) return false;
    if (!StreamInCue(stream)) return false;
    return stream->IsPlaying();
}


/*** El stream esta abierto ***/
bool NGN_Video::IsAlive(NGN_VideoStream* stream) {
    if (!stream) return false;
    if (!StreamInCue(stream)) return false;
    return stream->IsAlive();
}


/*** La reproduccion del stream ha finalizado ***/
bool NGN_Video::IsFinished(NGN_VideoStream* stream) {
    if (!stream) return false;
    if (!StreamInCue(stream)) return false;
    return stream->IsFinished();
}



/******************************************************************************
    Propiedades visuales
******************************************************************************/

/*** Posiciona el video ***/
void NGN_Video::SetPosition(NGN_VideoStream* stream, float position_x, float position_y) {

    // Proteccion
    if (!stream) return;
    if (!StreamInCue(stream)) return;

    // Asigna la posicion a la Textura del video
    stream->position.x = position_x;
    stream->position.y = position_y;

}
void NGN_Video::SetPosition(NGN_VideoStream* stream, Vector2 pos) {

    // Asigna la posicion a la Textura
    SetPosition(stream, pos.x, pos.y);

}
/*** Devuelve la posicion actual del video ***/
Vector2 NGN_Video::GetPosition(NGN_VideoStream* stream) {
    
    // Proteccion
    if (!stream) return Vector2::Zero();
    if (!StreamInCue(stream)) return Vector2::Zero();

    // Devuelve la posicion
    return stream->position;

}



/*** Mueve el video ***/
void NGN_Video::Translate(NGN_VideoStream* stream, float speed_x, float speed_y) {

    // Proteccion
    if (!stream) return;
    if (!StreamInCue(stream)) return;

    // Calcula la nueva posicion
    stream->position.x += speed_x;
    stream->position.y += speed_y;

}
void NGN_Video::Translate(NGN_VideoStream* stream, Vector2 spd) {

    // Calcula la nueva posicion
    Translate(stream, spd.x, spd.y);

}



/*** Establece un nuevo tamaño ***/
void NGN_Video::SetSize(NGN_VideoStream* stream, float w, float h) {

    // Proteccion
    if (!stream) return;
    if (!StreamInCue(stream)) return;

    // Aplica el nuevo tamaño
    stream->width = w;
    stream->original_size.width = stream->width;
    stream->height = h;
    stream->original_size.height = stream->height;
    stream->current_scale.width = 1.0f;
    stream->current_scale.height = 1.0f;

}

/*** Devuelve el tamaño del video ***/
Size2 NGN_Video::GetSize(NGN_VideoStream* stream) {

    // Dato a devolver
    Size2 s = {0.0f, 0.0f};
    
    // Proteccion
    if (!stream) return s;
    if (!StreamInCue(stream)) return s;

    // Devuelve el tamaño actual
    s = {stream->width, stream->height};
    return s;

}



/*** Escala el video [Sobrecarga 1 - Ejes por separado] ***/
void NGN_Video::SetScale(NGN_VideoStream* stream, float w, float h) {

    // Proteccion
    if (!stream) return;
    if (!StreamInCue(stream)) return;

    // Aplica la escala
    stream->width = stream->original_size.width * w;
    stream->height = stream->original_size.height * h;

    // Guarda la escala
    stream->current_scale.width = w;
    stream->current_scale.height = h;

}

/*** Escala el video [Sobrecarga 2 - Ambos ejes a la vez] ***/
void NGN_Video::SetScale(NGN_VideoStream* stream, float scale) {

    // Aplica la escala
    SetScale(stream, scale, scale);

}

/*** Devuelve la escala del video ***/
Size2 NGN_Video::GetScale(NGN_VideoStream* stream) {

    // Dato a devolver
    Size2 s = {1.0f, 1.0f};
    
    // Proteccion
    if (!stream) return s;
    if (!StreamInCue(stream)) return s;

    // Devuelve el tamaño actual
    s = stream->current_scale;
    return s;

}



/*** Escala el video [Sobrecarga 1 - Ejes por separado] ***/
void NGN_Video::SetRotation(NGN_VideoStream* stream, double degrees) {

    // Proteccion
    if (!stream) return;
    if (!StreamInCue(stream)) return;

    // Clamp del angulo
    double r = std::fmod(degrees, 360.0);
    if (r < 0.0) r += 360.0;

    // Aplica la nueva rotacion
    stream->rotation = r;

}



/*** Rota una textura los grados solicitados ***/
void NGN_Video::Rotate(NGN_VideoStream* stream, double degrees) {

    // Proteccion
    if (!stream) return;
    if (!StreamInCue(stream)) return;

    stream->rotation = std::fmod((stream->rotation + degrees), 360.0);
    if (stream->rotation < 0.0) stream->rotation += 360.0;

}


/*** Devuelve rotacion del video ***/
double NGN_Video::GetRotation(NGN_VideoStream* stream) {
    
    // Proteccion
    if (!stream) return 0.0;
    if (!StreamInCue(stream)) return 0.0;

    // Devuelve el tamaño actual
    return stream->rotation;

}



/*** Offset del centro para las rotaciones ***/
void NGN_Video::SetCenter(NGN_VideoStream* stream, float x, float y) {

    // Proteccion
    if (!stream) return;
    if (!StreamInCue(stream)) return;

    // Establece el centro
    stream->center.x = x;
    stream->center.y = y;

}

/*** Devuelve el offset del centro para las rotaciones ***/
Vector2 NGN_Video::GetCenter(NGN_VideoStream* stream) {

    // Proteccion
    if (!stream) return Vector2::Zero();
    if (!StreamInCue(stream)) return Vector2::Zero();

    // Devuelve el offset actual
    return stream->center;

}


/*** Establece el alpha del video ***/
void NGN_Video::SetAlpha(NGN_VideoStream* stream, int32_t lv) {

    // Proteccion
    if (!stream) return;
    if (!StreamInCue(stream)) return;

    // Clamp
    int32_t level = lv;
    if (level < -1) level = -1;
    if (level > 255) level = 255;

    // Asigna el valor
    stream->alpha = level;

}

/*** Devuelve el nivel actual de alpha ***/
int32_t NGN_Video::GetAlpha(NGN_VideoStream* stream) {

    // Proteccion
    if (!stream) return 0;
    if (!StreamInCue(stream)) return 0;

    // Valor
    return stream->alpha;

}


/*** Establece el color de tinta (si no es especifica ninguno, se restaura) ***/
void NGN_Video::SetTintColor(NGN_VideoStream* stream, uint8_t r, uint8_t g, uint8_t b) {

    // Proteccion
    if (!stream) return;
    if (!StreamInCue(stream)) return;

    // Asigna el color
    stream->tint_color.r = r;
    stream->tint_color.g = g;
    stream->tint_color.b = b;

}

/*** Devuelve el color actual de tinta ***/
Rgba NGN_Video::GetTintColor(NGN_VideoStream* stream) {

    // Proteccion
    Rgba color = {0xFF, 0xFF, 0xFF, 0xFF};
    if (!stream) return color;
    if (!StreamInCue(stream)) return color;

    // Devuelve el color actual
    return stream->tint_color;

}



/*** Ajusta el blending mode ***/
void NGN_Video::SetBlendMode(NGN_VideoStream* stream, SDL_BlendMode blend_mode) {

    // Proteccion
    if (!stream) return;
    if (!StreamInCue(stream)) return;

    // Asigna el modo
    stream->blend_mode = blend_mode;

}



/******************************************************************************
    Propiedades del sonido
******************************************************************************/

/*** Ajusta el volumen del video (0 ~ 100) ***/
void NGN_Video::SetVolume(NGN_VideoStream* stream, int32_t volume) {
    if (!stream) return;
    if (!StreamInCue(stream)) return;
    stream->Volume(volume);
}



/*** Devuelve el nivel de volumen del stream solicitado, si no existe, devuelve 0 ***/
int32_t NGN_Video::GetVolume(NGN_VideoStream* stream) {
    if (!stream) return 0;
    if (!StreamInCue(stream)) return 0;
    return stream->GetVolume();
}



/******************************************************************************
    Actualiza todos los stream abiertos
******************************************************************************/

/*** Actualiza todos los streams activos ***/
void NGN_Video::Update() {

    // Actualiza cada stream activo
    for (uint32_t i = 0; i < video_cue.size(); i++) {
        if (!video_cue[i]) continue; 
        video_cue[i]->Update();
        if (video_cue[i]->IsFinished() && !video_cue[i]->IsAlive() && video_cue[i]->loop) {
            video_cue[i]->Stop();
            video_cue[i]->Play();
        }
    }

}



/******************************************************************************
    Metodos privados
******************************************************************************/

/*** Verifica si un stream pertenece a la cola activa ***/
bool NGN_Video::StreamInCue(NGN_VideoStream* stream) {

    for (uint32_t i = 0; i < video_cue.size(); i++) {
        if (video_cue[i] == stream) return true;
    }
    return false;

}
