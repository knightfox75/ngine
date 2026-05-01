/******************************************************************************

    N'gine Lib for C++
    *** Version 1.22.0+stable ***
    Video player - Stream de video

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
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// SDL
#include <SDL.h>

// SFML
#include <SFML/Audio.hpp>

// theoraplay
#include "theoraplay/theoraplay.h"

// Libreria
#include "ngn.h"



/******************************************************************************

    NGN_VideoAudioStream
    Puente entre el decoder Vorbis (thread principal) y el hardware de audio
    SFML (thread interno de sf::SoundStream).

******************************************************************************/



/*** Constructor de la clase ***/
NGN_VideoAudioStream::NGN_VideoAudioStream() {

    _queue.clear();
    _chunk.clear();

}



/*** Destructor de la clase ***/
// Detiene el thread interno de SFML antes de destruir los buffers para
// evitar que onGetData() acceda a memoria ya liberada.
NGN_VideoAudioStream::~NGN_VideoAudioStream() {

    stop();
    _queue.clear();
    _chunk.clear();

}



/*** Configura el stream de audio con los parametros del decoder Vorbis ***/
// Dimensiona el chunk de salida y llama a sf::SoundStream::initialize()
// para arrancar el thread interno de SFML. Debe llamarse en Play(), no en
// OpenDecoder(), para que el thread no arranque antes de que haya datos.
void NGN_VideoAudioStream::Initialize(uint32_t channels, uint32_t sample_rate) {

    // El chunk se dimensiona a NGN_VIDEO_AUDIO_CHUNK_SAMPLES muestras por canal.
    // Con 1024 muestras stereo a 44100 Hz, cada chunk dura ~23ms, y SFML
    // llama a onGetData() unas ~43 veces por segundo. El pre-buffer de
    // NGN_VIDEO_AUDIO_CHUNKS_BUFFER chunks garantiza que la cola siempre
    // tiene datos cuando llega el primer callback.
    _chunk.resize((NGN_VIDEO_AUDIO_CHUNK_SAMPLES * channels), 0);
    initialize(channels, sample_rate);

}



/*** Encola muestras PCM desde el thread principal ***/
// Llamado en Update() (drenado continuo) y en el pre-buffer de Play().
// Protegido por mutex para sincronizacion con el thread de SFML.
void NGN_VideoAudioStream::PushAudio(const int16_t* samples, uint32_t sample_count) {

    sf::Lock lock(_mutex);
    _queue.insert(_queue.end(), samples, (samples + sample_count));

}



/*** Devuelve el numero de muestras actualmente en la cola ***/
// Usado en Play() para comprobar si el pre-buffer esta completo.
// Protegido por mutex.
uint32_t NGN_VideoAudioStream::GetQueueSize() {

    sf::Lock lock(_mutex);
    return (uint32_t)_queue.size();

}



/*** Vacia la cola de audio sin reproducir ***/
// Llamado en Stop() y Close() para descartar datos pendientes antes
// de detener el stream. Protegido por mutex.
void NGN_VideoAudioStream::Flush() {

    sf::Lock lock(_mutex);
    _queue.clear();

}



/*** Callback de SFML: solicita el siguiente bloque de PCM para el hardware ***/
// Llamado desde el thread interno de SFML cada ~23ms (con chunk de 1024).
// Si la cola tiene datos suficientes, los copia al chunk de salida.
// Si faltan datos, rellena el resto con silencio (ceros) para evitar
// pop/clicks en el hardware de audio. Siempre devuelve true para mantener
// el stream activo: NGN_VideoStream lo detiene explicitamente via stop().
bool NGN_VideoAudioStream::onGetData(Chunk& data) {

    sf::Lock lock(_mutex);

    uint32_t available = (uint32_t)_queue.size();
    uint32_t target_size = (uint32_t)_chunk.size();
    uint32_t to_copy = (available < target_size) ? available : target_size;

    // Copia los datos disponibles al buffer de salida y los elimina de la cola
    if (to_copy > 0) {
        std::copy(_queue.begin(), (_queue.begin() + to_copy), _chunk.begin());
        _queue.erase(_queue.begin(), (_queue.begin() + to_copy));
    }

    // Rellena el resto del chunk con silencio si no habia datos suficientes
    if (to_copy < target_size) {
        std::fill((_chunk.begin() + to_copy), _chunk.end(), 0);
    }

    data.samples = _chunk.data();
    data.sampleCount = target_size;

    // Devuelve siempre true: el stream se detiene desde NGN_VideoStream::Stop()
    return true;

}



/*** Callback de SFML: seek en el stream ***/
// No implementado: el video OGV no soporta seek en esta version del modulo.
void NGN_VideoAudioStream::onSeek(sf::Time timeOffset) {

    (void)timeOffset;

}



/******************************************************************************

    NGN_VideoStream

******************************************************************************/



/*** Constructor de la clase ***/
// Inicializa todos los miembros a valores seguros antes de que se llame a Open().
NGN_VideoStream::NGN_VideoStream() {

    // Decoder y contexto de I/O
    _decoder = nullptr;
    _io_ctx.file = nullptr;
    _io_ctx.file_size = 0;
    _io_ctx.from_package = false;
    _io_ctx.package_file = "";
    _io_ctx.stream_file = "";
    _io_ctx.package_offset = 0;
    _io_ctx.package_size = 0;
    _io_ctx.logical_offset = 0;
    _io_ctx.package_key.clear();
    _io_ctx.package_encrypted = false;

    // Subsistema de audio
    _audio_stream = new NGN_VideoAudioStream();
    _volume = 100;
    _mixer_channel = MIXER_VIDEO_CH;

    // Estado del stream
    _filepath = "";
    _playing = false;
    _paused = false;
    _finished = false;
    _stopped = true;
    _opened = false;
    _start_ticks = 0;
    _pause_ticks = 0;

    // Frames pendientes del decoder
    _pending_video = nullptr;
    _pending_audio = nullptr;

    // Propiedades de render
    frame_texture = nullptr;
    position.x = 0.0f;
    position.y = 0.0f;
    width = 0.0f;
    height = 0.0f;
    center = Vector2::Zero();
    visible = true;
    alpha = 0xFF;
    blend_mode = NGN_BLENDMODE_ALPHA;
    rotation = 0.0;
    flip_h = false;
    flip_v = false;
    original_size = {0.0f, 0.0f};
    current_scale = {1.0f, 1.0f};

    // Color de tinta
    tint_color = {0xFF, 0xFF, 0xFF, 0xFF};
    last_tint_color = {0xFF, 0xFF, 0xFF, 0xFF};

    // Loop del video
    loop = false;

    // Pista de audio
    _has_audio = false;
    _force_no_audio = false;

}



/*** Destructor de la clase ***/
// Llama a Close() para liberar todos los recursos del decoder y del audio,
// luego destruye el objeto NGN_VideoAudioStream.
NGN_VideoStream::~NGN_VideoStream() {

    Close();
    if (_audio_stream) delete _audio_stream;

}



/*** Abre el archivo OGV e inicializa el decoder ***/
// Guarda la ruta, arranca el decoder y espera los primeros metadatos.
// No inicia la reproduccion: para eso hay que llamar a Play().
// Devuelve false si el archivo no existe o no es un OGV valido.
bool NGN_VideoStream::Open(const std::string& filepath) {

    _filepath = filepath;
    if (!OpenDecoder()) return false;
    _opened = true;
    return true;

}



/*** Cierra el stream y libera todos los recursos ***/
// Detiene el audio, cierra el decoder y resetea el estado completo.
// Seguro de llamar aunque el stream no este abierto o ya este cerrado.
void NGN_VideoStream::Close() {

    if (_audio_stream && _has_audio) {
        _audio_stream->stop();
        _audio_stream->Flush();
    }
    CloseDecoder();
    _filepath = "";
    _playing = false;
    _paused = false;
    _stopped = true;
    _finished = false;
    _opened = false;
    _has_audio = false;

}



/******************************************************************************
    Control de reproduccion
******************************************************************************/



/*** Inicia o reanuda la reproduccion ***/
// Si el stream esta pausado: desplaza el reloj maestro para compensar el
// tiempo pausado y reanuda el stream de SFML.
// Si el stream esta detenido: inicializa el stream de audio, construye el
// pre-buffer, ancla el reloj y arranca SFML.
void NGN_VideoStream::Play() {

    if (!_opened) return;
    if (_playing && !_paused) return;

    if (_paused) {

        // Reanuda desde pausa: desplaza _start_ticks hacia adelante
        // el tiempo que estuvo pausado para que elapsed_ms sea continuo.
        _start_ticks += (SDL_GetTicks() - _pause_ticks);
        _pause_ticks = 0;
        _paused = false;
        _playing = true;
        if (_has_audio) {
            Volume(_volume);
            _audio_stream->play();
        }

    } else {

        // Inicio desde cero: extrae channels y freq del primer paquete de
        // audio pendiente (guardado por OpenDecoder()) para inicializar SFML.
        uint32_t aud_channels = 0;
        uint32_t aud_freq = 0;

        if (_pending_audio) {
            // Caso normal: OpenDecoder() siempre guarda el primer paquete de audio
            aud_channels = (uint32_t)_pending_audio->channels;
            aud_freq = (uint32_t)_pending_audio->freq;
        } else {
            // Caso excepcional: el primer paquete ya fue consumido (no deberia ocurrir)
            const THEORAPLAY_AudioPacket* probe = THEORAPLAY_getAudio(_decoder);
            if (probe) {
                aud_channels = (uint32_t)probe->channels;
                aud_freq = (uint32_t)probe->freq;
                _pending_audio = probe;
            }
        }

        // Inicializa y arranca el subsistema de audio solo si el OGV
        // tiene pista Vorbis. Si no la tiene, se salta todo este bloque
        // y el reloj maestro arranca sin esperar pre-buffer.
        if (_has_audio) {

            // Inicializa el stream de audio SFML con los parametros del decoder Vorbis.
            // Critico: llamar aqui, no en OpenDecoder(). Si se llamara al abrir el
            // archivo, el thread de SFML arrancaria antes de que haya datos en la cola
            // y comenzaria a servir silencio, produciendo el corte inicial de audio.
            if (aud_channels > 0) {
                _audio_stream->Initialize(aud_channels, aud_freq);
            }

            // PRE-BUFFER: acumula audio suficiente para cubrir NGN_VIDEO_AUDIO_CHUNKS_BUFFER
            // chunks completos antes de arrancar SFML. Este colchon garantiza que
            // onGetData() siempre encuentra datos disponibles mientras el game loop
            // repone la cola en cada frame via Update().
            uint32_t samples_needed = (NGN_VIDEO_AUDIO_CHUNK_SAMPLES * aud_channels * NGN_VIDEO_AUDIO_CHUNKS_BUFFER);
            uint32_t prebuf_timeout = (SDL_GetTicks() + NGN_VIDEO_AUDIO_TIMEOUT);

            while ((_audio_stream->GetQueueSize() < samples_needed) && (SDL_GetTicks() < prebuf_timeout)) {

                // Consume el paquete de audio pendiente primero
                if (_pending_audio) {
                    PushAudioPacket(_pending_audio);
                    THEORAPLAY_freeAudio(_pending_audio);
                    _pending_audio = nullptr;
                }

                // Solicita el siguiente paquete al decoder
                const THEORAPLAY_AudioPacket* pkt = THEORAPLAY_getAudio(_decoder);
                if (pkt) {
                    PushAudioPacket(pkt);
                    THEORAPLAY_freeAudio(pkt);
                } else {
                    // El decoder aun no tiene datos: cede la CPU brevemente
                    // para que el thread de theoraplay pueda decodificar
                    SDL_Delay(1);
                }

            }

        }

        _finished = false;
        _playing  = true;
        _stopped = false;

        // Ancla el reloj maestro. En modo con audio, se ancla justo antes de
        // play() para sincronizar elapsed_ms con el primer sample emitido.
        // En modo sin audio, el reloj arranca libre desde este instante.
        _start_ticks = SDL_GetTicks();
        if (_has_audio) {
            Volume(_volume);
            _audio_stream->play();
        }

    }

}



/*** Pausa la reproduccion ***/
// Guarda el instante de pausa en _pause_ticks para compensar el reloj
// al reanudar con Play(). No libera ningun recurso.
void NGN_VideoStream::Pause() {

    if (!_playing || _paused) return;

    _pause_ticks = SDL_GetTicks();
    _paused = true;
    _playing = false;
    if (_has_audio) _audio_stream->pause();

}



/*** Detiene la reproduccion y rebobina el stream ***/
// Detiene el audio, cierra el decoder y lo reabre inmediatamente para
// dejar el stream en estado "listo para Play()", igual que
// NGN_MusicClip::Stop() deja la musica al principio.
void NGN_VideoStream::Stop() {

    if (!_playing && !_paused && !_finished) return;

    if (_has_audio) { _audio_stream->stop(); _audio_stream->Flush(); }
    CloseDecoder();
    _playing = false;
    _paused = false;
    _stopped = true;
    _finished = false;
    _start_ticks = 0;
    _pause_ticks = 0;

    // Reabre el decoder para que el stream quede rebobinado.
    // _opened sigue siendo true: el stream sigue valido para Play().
    if (_opened) OpenDecoder();

}


/*** Reinicia el stream para realizar el loop ***/
void NGN_VideoStream::RestartForLoop() {

    if (_has_audio) {
        _audio_stream->stop();
        _audio_stream->Flush();
    }

    CloseDecoder(true);

    _playing = false;
    _paused = false;
    _stopped = false;
    _finished = false;
    _start_ticks = 0;
    _pause_ticks = 0;

    if (_opened) OpenDecoder(true);

    Play();

}



/******************************************************************************
    Update -- nucleo del sincronismo A/V
    Llamado por NGN_Video::Update() una vez por iteracion del game loop,
    antes de ngn->graphics->Update() y ngn->sound->Update().
******************************************************************************/

void NGN_VideoStream::Update() {

    if (!_playing || _paused || _finished) return;
    if (_decoder == nullptr) return;

    // Tiempo transcurrido desde el inicio de la reproduccion (milisegundos)
    uint32_t elapsed_ms = (SDL_GetTicks() - _start_ticks);

    // ------------------------------------------------------------------
    // Drenado de audio: consume todos los paquetes Vorbis disponibles
    // en la cola del decoder y los encola en el stream de SFML.
    // Se procesan todos sin esperar al timestamp porque el hardware de
    // audio los consume a su propio ritmo independientemente del video.
    // ------------------------------------------------------------------
    while (true) {
        if (_pending_audio == nullptr) _pending_audio = THEORAPLAY_getAudio(_decoder);
        if (_pending_audio == nullptr) break;
        PushAudioPacket(_pending_audio);
        THEORAPLAY_freeAudio(_pending_audio);
        _pending_audio = nullptr;
    }

    // ------------------------------------------------------------------
    // Sincronismo de video: avanza frames hasta el que corresponde
    // segun elapsed_ms. Si el game loop va lento, descarta frames
    // intermedios (frame drop) para mantener el sync con el audio.
    // ------------------------------------------------------------------
    const THEORAPLAY_VideoFrame* next_frame = nullptr;

    while (true) {
        if (_pending_video == nullptr) _pending_video = THEORAPLAY_getVideo(_decoder);
        if (_pending_video == nullptr) break;

        // Frame del futuro: no ha llegado su momento, lo guardamos y paramos
        if (_pending_video->playms > elapsed_ms) break;

        // Frame del presente o pasado: puede haber uno mas reciente.
        // Descartamos el candidato anterior (frame drop si el loop va lento).
        if (next_frame) THEORAPLAY_freeVideo(next_frame);
        next_frame = _pending_video;
        _pending_video = nullptr;
    }

    // Actualiza la textura del renderer con el frame candidato via CPU upload.
    // pitch = width * 4 bytes/pixel (RGBA8888). Usa shift en lugar de
    // multiplicacion: width << 2 == width * 4.
    if (next_frame) {
        SDL_UpdateTexture(frame_texture, nullptr, next_frame->pixels, (int32_t)(next_frame->width << 2));
        THEORAPLAY_freeVideo(next_frame);
    }

    // ------------------------------------------------------------------
    // Deteccion de fin de stream (EOS):
    // El decoder ha terminado Y no quedan frames ni audio pendientes en nuestras variables.
    // ------------------------------------------------------------------
    if (!THEORAPLAY_isDecoding(_decoder) && !_pending_video && !_pending_audio) {
        
        // Verifica si la cola interna del decoder aun tiene remanentes
        const THEORAPLAY_AudioPacket* last_a = THEORAPLAY_getAudio(_decoder);
        const THEORAPLAY_VideoFrame*  last_v = THEORAPLAY_getVideo(_decoder);
        
        if (!last_a && !last_v) {
            if (loop) {
                RestartForLoop();
            } else {
                _playing = false;
                _finished = true;
                if (_has_audio) _audio_stream->stop();
            }
        } else {
            // Si aun queda algo, lo asignamos a pending para que se
            // reproduzca y sincronice en el siguiente frame de forma natural.
            if (last_a) _pending_audio = last_a;
            if (last_v) _pending_video = last_v;
        }
        
    }

    //std::cout << "Has audio? " << _has_audio << std::endl;

}



/******************************************************************************
    Metodos privados
******************************************************************************/



/*** Abre el archivo y arranca el decoder theoraplay ***/
// Reutilizado por Open() (primera apertura) y Stop() (rebobinado).
// Verifica si el archivo se encuentra logica o fisicamente, ajusta
// los offsets para el paquete (si aplica), y arranca el decoder.
bool NGN_VideoStream::OpenDecoder(bool keep_texture) {

    // Si se esta usando un archivo de empaquetado...
    if (ngn->load->PackageEnabled() || _io_ctx.from_package) {
        
        // Si aun no has accedido nunca al archivo, o este ha cambiado, opten los datos
        if ((_io_ctx.package_file.size() == 0) || (_io_ctx.stream_file != _filepath)) {
            // Comprueba si el archivo esta disponible dentro de un empaquetado
            uint32_t pkg_offset = 0;
            uint32_t pkg_length = 0;
            if (!ngn->load->GetFileInfoInPackage(_filepath, pkg_offset, pkg_length)) {
                ngn->log->Message("[NGN_Video error] Video file <" + _filepath + "> not found.");
                return false;
            }
            // Datos del archivo en el paquete
            _io_ctx.from_package = true;
            _io_ctx.package_offset = pkg_offset;
            _io_ctx.package_size = pkg_length;
            _io_ctx.file_size = pkg_length;         // Theoraplay vera esto como tamaño total
            // Guarda el nombre y ruta del archivo empaquetado que contiene el archivo, asi como el archivo en si
            _io_ctx.package_file = ngn->load->GetCurrentSelectedPackageFile();
            _io_ctx.stream_file = _filepath;
            // Verifica si esta el paquete encriptado
            _io_ctx.package_key = ngn->load->GetCurrentSelectedPackageFileKey();
            _io_ctx.package_encrypted = (_io_ctx.package_key.size() > 0);
        }

        // Reinicia el offset del cabezal
        _io_ctx.logical_offset = 0;                 // <-- Tracker logico manual a cero
        
        // Intenta abrir el archivo empaquetado
        _io_ctx.file = fopen(_io_ctx.package_file.c_str(), "rb");
        if (!_io_ctx.file) {
            ngn->log->Message("[NGN_Video error] Can't open <" + _filepath + "> video stream.");
            return false;
        }
        // Y coloca el cabezal de lectura en posicion
        fseek(_io_ctx.file, _io_ctx.package_offset, SEEK_SET);

    } else {

        // Archivo directo desde el disco (sin empaquetar)
        _io_ctx.from_package = false;
        _io_ctx.package_file = "";
        _io_ctx.stream_file = "";
        _io_ctx.package_offset = 0;
        _io_ctx.package_size = 0;
        _io_ctx.file_size = 0;
        _io_ctx.logical_offset = 0;
        _io_ctx.package_key.clear();
        _io_ctx.package_encrypted = false;

        _io_ctx.file = fopen(_filepath.c_str(), "rb");
        if (!_io_ctx.file) {
            ngn->log->Message("[NGN_Video error] Video file <" + _filepath + "> not found.");
            return false;
        }

        // Calcula el tamaño total del archivo para servir en IoStreamLen().
        fseek(_io_ctx.file, 0, SEEK_END);
        _io_ctx.file_size = ftell(_io_ctx.file);
        fseek(_io_ctx.file, 0, SEEK_SET);

    }

    // Configura los callbacks de I/O.
    // Critico: _io_callbacks es miembro de la clase, no variable local.
    // theoraplay mantiene el puntero &_io_callbacks durante toda la vida
    // del decoder, por lo que no puede estar en la pila de esta funcion.
    _io_callbacks.read = IoRead;
    _io_callbacks.streamlen = IoStreamLen;
    _io_callbacks.seek = IoSeek;
    _io_callbacks.close = IoClose;
    _io_callbacks.userdata = &_io_ctx;

    // Arranca el decoder:
    // - NGN_VIDEO_DECODER_MAX_FRAMES_BUFFER: max frames en la cola interna
    // - THEORAPLAY_VIDFMT_RGBA: frames en RGBA8888, directo a SDL_UpdateTexture
    // - nullptr: allocator por defecto (malloc/free)
    // - 1: modo multithreaded activado
    _decoder = THEORAPLAY_startDecode(&_io_callbacks, NGN_VIDEO_DECODER_MAX_FRAMES_BUFFER, THEORAPLAY_VIDFMT_RGBA, nullptr, 1);
    if (!_decoder) {
        fclose(_io_ctx.file);
        _io_ctx.file = nullptr;
        _io_ctx.file_size = 0;
        return false;
    }

    // Espera al primer frame de video para obtener los metadatos del stream
    // (resolucion, framerate). El audio es opcional: un OGV puede no tener
    // pista Vorbis, en cuyo caso first_a quedara nullptr y el video se
    // reproducira sin sonido. Se espera hasta el timeout en ambos casos
    // para dar tiempo al decoder a entregar lo que tenga disponible.
    const THEORAPLAY_VideoFrame*  first_v = nullptr;
    const THEORAPLAY_AudioPacket* first_a = nullptr;
    uint32_t timeout = (SDL_GetTicks() + NGN_VIDEO_DECODER_TIMEOUT);

    while (THEORAPLAY_isDecoding(_decoder)) {
        if (!first_v) first_v = THEORAPLAY_getVideo(_decoder);
        if (!first_a) first_a = THEORAPLAY_getAudio(_decoder);
        // Condicion de salida rapida: tenemos video y, si hay audio, tambien
        if (first_v && (first_a || _force_no_audio)) break;
        if (SDL_GetTicks() > timeout) break;
        SDL_Delay(1);
    }

    // El video es obligatorio. El audio es opcional.
    if (!first_v) {
        if (first_a) THEORAPLAY_freeAudio(first_a);
        CloseDecoder();
        return false;
    }

    // Registra si el OGV tiene pista de audio
    _has_audio = (first_a != nullptr);

    // Si se ha marcado como que no tiene audio y lo tiene, ignorar la pista
    if (_force_no_audio && _has_audio) {
        THEORAPLAY_freeAudio(first_a);
        first_a = nullptr;
        _has_audio = false;
    }

    // Crea la textura SDL con las dimensiones nativas del video
    AllocateFrameTexture(first_v->width, first_v->height, keep_texture);

    // Subimos los píxeles del primer frame a la GPU ahora mismo.
    // Así, la textura ya tiene imagen antes de que Play() sea llamado.
    if (first_v) {
        SDL_UpdateTexture(frame_texture, nullptr, first_v->pixels, (int32_t)(first_v->width << 2));
    }

    // Guarda los primeros datos como pendientes para que Play() y Update()
    // los procesen en el primer ciclo sin descartarlos
    _pending_video = first_v;
    _pending_audio = first_a;

    return true;

}



/*** Cierra el decoder y libera todos los recursos asociados ***/
// Libera los frames pendientes, detiene el decoder, cierra el FILE*
// y destruye frame_texture. Seguro de llamar con recursos nullptr.
void NGN_VideoStream::CloseDecoder(bool keep_texture) {

    // Libera frames pendientes ANTES de detener el decoder
    if (_pending_video) {
        THEORAPLAY_freeVideo(_pending_video);
        _pending_video = nullptr;
    }
    if (_pending_audio) {
        THEORAPLAY_freeAudio(_pending_audio);
        _pending_audio = nullptr;
    }

    // Detiene el decoder (tambien llama a IoClose internamente, que es no-op)
    if (_decoder) {
        THEORAPLAY_stopDecode(_decoder);
        _decoder = nullptr;
    }

    // Cierra el archivo: lo gestionamos nosotros, no theoraplay (IoClose es no-op)
    if (_io_ctx.file) {
        fclose(_io_ctx.file);
        _io_ctx.file      = nullptr;
        _io_ctx.file_size = 0;
    }

    // Destruye la textura SDL del frame
    if (!keep_texture) {
        if (frame_texture) {
            SDL_DestroyTexture(frame_texture);
            frame_texture = nullptr;
        }
    }

}



/*** Crea o recrea la textura SDL para almacenar el frame actual ***/
// SDL_TEXTUREACCESS_STREAMING permite actualizarla desde CPU cada frame
// via SDL_UpdateTexture() sin recrearla. Solo actualiza width/height
// si aun valen 0.0f, es decir, en la primera creacion.
void NGN_VideoStream::AllocateFrameTexture(uint32_t w, uint32_t h, bool keep_texture) {

    // Si exite la textura, destruyela
    if (frame_texture && !keep_texture) {
        SDL_DestroyTexture(frame_texture);
        frame_texture = nullptr;
    }

    // Crea una nueva textura con el tamaño del video
    if (!frame_texture) {
        frame_texture = SDL_CreateTexture(
            ngn->graphics->renderer,
            SDL_PIXELFORMAT_RGBA32,         // Coincide con THEORAPLAY_VIDFMT_RGBA
            SDL_TEXTUREACCESS_STREAMING,    // Actualizable desde CPU cada frame
            (int32_t)w,
            (int32_t)h
        );
    }

    // Ponemos la textura en negro opaco inmediatamente al crearla.
    // Esto evita que sea transparente si hay algun retraso.
    uint32_t* pixels = nullptr;
    int pitch = 0;
    if (SDL_LockTexture(frame_texture, nullptr, (void**)&pixels, &pitch) == 0) {
        // Para SDL_PIXELFORMAT_RGBA32 en maquinas Little-Endian (x86/ARM):
        // 0xFF000000 -> A:255, B:0, G:0, R:0
        uint32_t opaque_black = 0xFF000000;
        // Calculamos el numero de pixeles usando desplazamiento de bits (pitch / 4)
        uint32_t total_pixels = (uint32_t)((pitch >> 2) * h);
        // Llenado rapido del buffer
        for (uint32_t i = 0; i < total_pixels; i++) pixels[i] = opaque_black;
        SDL_UnlockTexture(frame_texture);
    }

    // Verifica que se ha podido crear con exito
    if (!frame_texture) {
        ngn->log->Message("[NGN_VideoStream error] SDL_CreateTexture failed for frame buffer.");
    }

    // Si no se ha especificado un tamaño personalizado para el render, usa el del video
    if ((width == 0.0f) || (height == 0.0f)) {
        width = (float)w;
        height = (float)h;
        original_size.width = width;
        original_size.height = height;
    }

}



/*** Convierte un paquete de audio Vorbis (float32) a int16 y lo encola ***/
// theoraplay entrega el audio Vorbis decodificado como float32 normalizado
// en el rango [-1.0, 1.0]. SFML sf::SoundStream espera int16 lineal.
// Se aplica clampeo explicito antes de la conversion para evitar overflow
// en muestras que por redondeo del decoder superen ligeramente 1.0.
void NGN_VideoStream::PushAudioPacket(const THEORAPLAY_AudioPacket* pkt) {

    if (!pkt) return;

    uint32_t total = (uint32_t)(pkt->frames * pkt->channels);
    std::vector<int16_t> buf(total);

    for (uint32_t i = 0; i < total; i++) {
        float s = pkt->samples[i];
        if (s >  1.0f) s =  1.0f;
        else if (s < -1.0f) s = -1.0f;
        buf[i] = (int16_t)(s * 32767.0f);
    }

    _audio_stream->PushAudio(buf.data(), total);

}



/*** Devuelve si el stream se esta reproduciendo activamente ***/
bool NGN_VideoStream::IsPlaying() {
    return (_playing && !_paused);
}



/*** Devuelve si el stream esta activo (reproduciendose o en pausa) ***/
bool NGN_VideoStream::IsAlive() {
    return (_playing || _paused);
}



/*** Devuelve si el stream ha llegado al final del archivo OGV ***/
bool NGN_VideoStream::IsFinished() {
    return _finished;
}



/*** Establece el volumen del audio (0 ~ 100) ***/
void NGN_VideoStream::Volume(int32_t v) {
    _volume = v;
    if (_volume < 0) _volume = 0;
    if (_volume > 100) _volume = 100;
    if (_audio_stream) {
        float _v = (float)_volume * ((float)ngn->sound->GetMixerLevel(MIXER_MASTER_CH) / 100.0f) * ((float)ngn->sound->GetMixerLevel(_mixer_channel) / 100.0f);
        _audio_stream->setVolume(_v);
    }
}



/*** Devuelve el volumen actual ***/
int32_t NGN_VideoStream::GetVolume() {
    return _volume;
}



/*** Devuelve si se ha cambiado el color de tinta en este frame ***/
bool NGN_VideoStream::NewTint() {

    bool color_mod = ((tint_color.r != last_tint_color.r) || (tint_color.g != last_tint_color.g) || (tint_color.b != last_tint_color.b));
    last_tint_color = tint_color;
    return color_mod;

}



/******************************************************************************
    Callbacks de I/O para theoraplay
    Firmas exactas requeridas por la estructura THEORAPLAY_Io.
    El thread del decoder de theoraplay llama a estas funciones.
    El puntero io->userdata apunta siempre a _io_ctx.
******************************************************************************/



/*** Lee buflen bytes del archivo y los escribe en buf ***/
long NGN_VideoStream::IoRead(THEORAPLAY_Io* io, void* buf, long buflen) {

    IoContext* ctx = (IoContext*)io->userdata;
    if (!ctx->file) return 0;

    long bytes_read = 0;

    if (ctx->from_package) {

        // Calculo robusto del remanente evitando ftell() y problemas de tipos long vs uint32_t
        long max_read = (long)ctx->package_size - ctx->logical_offset;

        // Clamp de la lectura solicitada
        if (buflen > max_read) buflen = max_read;
        if (buflen <= 0) return 0;

        // Lee directamente al buffer de destino de Theoraplay
        bytes_read = (long)fread(buf, 1, (size_t)buflen, ctx->file);

        // Si se han leido datos, aplica la desencriptacion In-Place (Zero-Copy) y avanza tracker manual
        if (bytes_read > 0) {
            if (ctx->package_encrypted) ngn->load->DecryptChunk((uint8_t*)buf, (uint32_t)bytes_read, (uint32_t)ctx->logical_offset, ctx->package_key);
            ctx->logical_offset += bytes_read;
        }

    } else {

        // Lectura de disco tradicional
        bytes_read = (long)fread(buf, 1, (size_t)buflen, ctx->file);

    }

    return bytes_read;

}



/*** Devuelve el tamaño total del archivo en bytes ***/
// Calculado una sola vez en OpenDecoder() con fseek/ftell o extraido del FAT.
long NGN_VideoStream::IoStreamLen(THEORAPLAY_Io* io) {
    return ((IoContext*)io->userdata)->file_size;
}



/*** Seek absoluto desde el inicio del archivo ***/
// La API de theoraplay usa seek absoluto exclusivamente (sin whence),
// equivalente a fseek(file, offset, SEEK_SET).
int NGN_VideoStream::IoSeek(THEORAPLAY_Io* io, long off) {

    IoContext* ctx = (IoContext*)io->userdata;
    if (!ctx->file) return -1;

    if (ctx->from_package) {
        
        // Evita underflows si Theoraplay pide un seek negativo
        if (off < 0) off = 0;
        
        // Proteccion rigurosa forzando cast a 'long' en package_size para evitar over-clamp
        if (off > (long)ctx->package_size) off = (long)ctx->package_size;
        
        // Sincroniza nuestro tracking manual
        ctx->logical_offset = off;
        
        // Traduce a offset fisico (absoluto) en el empaquetado
        return fseek(ctx->file, ((long)ctx->package_offset + off), SEEK_SET);

    } else {
        // Seek normal
        return fseek(ctx->file, off, SEEK_SET);
    }

}



/*** Cierre del archivo ***/
// No-op intencionado: CloseDecoder() gestiona el ciclo de vida del FILE*
// en _io_ctx para evitar un double-close entre IoClose() y CloseDecoder().
void NGN_VideoStream::IoClose(THEORAPLAY_Io* io) {
    (void)io;
}