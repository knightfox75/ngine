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



#ifndef NGN_VIDEO_H_INCLUDED
#define NGN_VIDEO_H_INCLUDED



/*** Includes ***/

// C++
#include <cstdint>
#include <string>
#include <vector>

// Libreria
#include "ngn_defines.h"
#include "ngn_video_stream.h"
#include "ngn_sound.h"



/*** Numero maximo de streams de video simultaneos ***/
static const uint8_t MAX_VIDEO_STREAMS = 4;



/******************************************************************************

    Clase NGN_Video

******************************************************************************/

/*** Declaracion de la clase ***/
class NGN_Video {

    // Segmento publico
    public:

        // Devuelve la instancia
        static NGN_Video* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();


        /*** Gestion de streams ***/

        // Abre un archivo OGV y crea un stream listo para reproducir.
        // auto_start : si es true, inicia la reproduccion automaticamente.
        // Si se alcanza MAX_VIDEO_STREAMS o el archivo no es valido, devuelve nullptr.
        NGN_VideoStream* Open(
            std::string filepath,                               // Archivo OGV
            bool auto_start = false,                            // Reproduccion automatica
            bool loop = false,                                  // Repite el video al finalizar
            int32_t volume = 100,                               // Volumen inicial (0-100)
            uint32_t render_width = NGN_DEFAULT_VALUE,          // Ancho de render (nativo por defecto)
            uint32_t render_height = NGN_DEFAULT_VALUE,         // Alto de render (nativo por defecto)
            int32_t position_x = NGN_DEFAULT_VALUE,             // Posicion X (centrado por defecto)
            int32_t position_y = NGN_DEFAULT_VALUE,             // Posicion Y (centrado por defecto)
            uint8_t mixer_channel = MIXER_VIDEO_CH              // Canal asignado del mixer de sonido
        );

        // Cierra un stream y libera sus recursos.
        // El puntero queda invalidado tras esta llamada.
        void Close(NGN_VideoStream* stream);

        // Cierra todos los streams activos
        void CloseAll();


        /*** Control de reproduccion (un stream) ***/
        void Play(NGN_VideoStream* stream);                 // Inicia o reanuda
        void Pause(NGN_VideoStream* stream);                // Pausa
        void Stop(NGN_VideoStream* stream);                 // Detiene y reinicia
        void SetLoop(NGN_VideoStream* stream, bool loop);   // Cambia el estado del loop del video

        /*** Control de reproduccion (varios streams) ***/
        void PauseAll();                                // Pausa todos los streams activos
        void ResumeAll();                               // Reanuda todos los streams pausados
        void StopAll();                                 // Detiene todos los streams

        /*** Consulta de estado ***/
        bool IsPlaying(NGN_VideoStream* stream);        // True si esta reproduciendose
        bool IsAlive(NGN_VideoStream* stream);          // True si esta activo (play o pause)
        bool IsFinished(NGN_VideoStream* stream);       // True si llego al final del stream

        /*** Posicion ***/
        void SetPosition(NGN_VideoStream* stream, float position_x, float position_y);
        void SetPosition(NGN_VideoStream* stream, Vector2 pos);
        Vector2 GetPosition(NGN_VideoStream* stream);
        /*** Movimiento ***/
        void Translate(NGN_VideoStream* stream, float speed_x, float speed_y);
        void Translate(NGN_VideoStream* stream, Vector2 spd);
        /*** Tamaño ***/
        void SetSize(NGN_VideoStream* stream, float w, float h);
        Size2 GetSize(NGN_VideoStream* stream);
        /*** Escala ***/
        void SetScale(NGN_VideoStream* stream, float w, float h);      // [Sobrecarga 1]       Escala los dos ejes por separado
        void SetScale(NGN_VideoStream* stream, float scale);           // [Sobrecarga 2]       Escala ambos ejes a la vez
        Size2 GetScale(NGN_VideoStream* stream);
        /*** Rotacion ***/
        void SetRotation(NGN_VideoStream* stream, double degrees); 
        void Rotate(NGN_VideoStream* stream, double degrees);
        double GetRotation(NGN_VideoStream* stream);
        void SetCenter(NGN_VideoStream* stream, float x, float y);
        Vector2 GetCenter(NGN_VideoStream* stream);
        /*** Alpha ***/
        void SetAlpha(NGN_VideoStream* stream, int32_t lv);
        int32_t GetAlpha(NGN_VideoStream* stream);
        /*** Color de tinta ***/
        void SetTintColor(NGN_VideoStream* stream, uint8_t r = 0xFF, uint8_t g = 0xFF, uint8_t b = 0xFF);
        Rgba GetTintColor(NGN_VideoStream* stream);
        /*** Ajusta el blending mode ***/
        void SetBlendMode(NGN_VideoStream* stream, SDL_BlendMode blend_mode);
        /*** Volumen ***/
        void SetVolume(NGN_VideoStream* stream, int32_t volume);
        int32_t GetVolume(NGN_VideoStream* stream);


        /*** Update ***/

        // Actualiza todos los streams activos: decodifica frames, sincroniza A/V,
        // elimina streams finalizados de la cola.
        // Debe llamarse una vez por frame, antes de ngn->graphics->Update().
        void Update();


    // Segmento privado
    private:

        // Ajuste de permisos
        friend class NGN_Sound;

        // Constructor / Destructor (singleton)
        NGN_Video();
        ~NGN_Video();

        // Puntero a la instancia
        static NGN_Video* instance;

        // Cola de streams activos
        std::vector<NGN_VideoStream*> video_cue;

        // Verifica si el stream pertenece a la cola (validacion interna)
        bool StreamInCue(NGN_VideoStream* stream);

};



#endif // NGN_VIDEO_H_INCLUDED
