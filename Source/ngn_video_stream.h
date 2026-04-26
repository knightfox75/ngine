/******************************************************************************

    N'gine Lib for C++
    *** Version 1.22.0-wip_0x04 ***
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


/******************************************************************************

    Codigo basado en los ejemplos de theoraplay proporcionados
    por los modelos Claude Sonnet 4.6 y Google Gemini 3.1

******************************************************************************/



#ifndef NGN_VIDEO_STREAM_H_INCLUDED
#define NGN_VIDEO_STREAM_H_INCLUDED



/*** Includes ***/

// C++
#include <cstdint>
#include <string>
#include <vector>

// SDL
#include <SDL.h>

// SFML
#include <SFML/Audio.hpp>

// theoraplay
#include "theoraplay/theoraplay.h"

// Libreria
#include "ngn_defines.h"



/*** Configuracion para el decoder de VIDEO/AUDIO ***/

// Numero de muestras PCM por canal que SFML solicita en cada callback onGetData().
// Con 1024 muestras a 44100 Hz stereo, cada chunk dura ~23ms.
// Aumentar este valor reduce la frecuencia de callbacks (mas estable),
// a costa de mayor latencia de audio.
static const uint32_t NGN_VIDEO_AUDIO_CHUNK_SAMPLES = 1024;

// Numero de chunks que se acumulan en el pre-buffer antes de arrancar SFML.
// El pre-buffer total es NGN_VIDEO_AUDIO_CHUNK_SAMPLES * channels * NGN_VIDEO_AUDIO_CHUNKS_BUFFER.
// Con 4 chunks de 1024 muestras stereo = 8192 muestras = ~93ms de colchon.
// Este margen cubre la latencia de arranque del thread de SFML y los
// posibles huecos entre llamadas a Update() en el game loop.
static const uint32_t NGN_VIDEO_AUDIO_CHUNKS_BUFFER = 4;

// Tiempo maximo de espera en ms para completar el pre-buffer de audio.
// Si el decoder no entrega suficientes datos en este tiempo (archivo corrupto,
// stream sin audio, etc.), Play() arranca con los datos disponibles.
static const uint32_t NGN_VIDEO_AUDIO_TIMEOUT = 500;

// Numero maximo de frames de video que theoraplay mantiene decodificados
// en su cola interna. Limitar este valor reduce el uso de memoria RAM.
// A 30fps con 45 frames de cola, el decoder adelanta hasta 1.5s.
static const int32_t NGN_VIDEO_DECODER_MAX_FRAMES_BUFFER = 35;

// Tiempo maximo de espera en ms para obtener el primer frame de video
// y el primer paquete de audio al abrir el decoder.
// Protege contra archivos corruptos o con streams invalidos.
static const int32_t NGN_VIDEO_DECODER_TIMEOUT = 500;



/******************************************************************************

    Clase NGN_VideoAudioStream  (uso interno de NGN_VideoStream)

    Subclase de sf::SoundStream que actua como puente entre el decoder
    Vorbis (thread principal via theoraplay) y el subsistema de audio
    SFML (thread interno de sf::SoundStream).

    Flujo de datos:
        Thread principal -> PushAudio() -> _queue (bajo mutex)
        Thread SFML      -> onGetData() -> _queue (bajo mutex) -> hardware

    Initialize() debe llamarse en Play(), no en OpenDecoder().
    Si se llamara al abrir el archivo, el thread de SFML arrancaria
    antes de que haya datos en la cola y comenzaria a servir silencio.

******************************************************************************/

class NGN_VideoAudioStream : public sf::SoundStream {

    // Segmento publico
    public:

        // Constructor de la clase
        NGN_VideoAudioStream();

        // Destructor de la clase
        // Detiene el thread de SFML antes de destruir los buffers
        ~NGN_VideoAudioStream();

        // Configura el stream de audio con los parametros del decoder Vorbis
        // y dimensiona el chunk de salida. Debe llamarse en Play(), justo
        // antes de play(), una vez conocidos channels y sample_rate.
        void Initialize(uint32_t channels, uint32_t sample_rate);

        // Encola muestras PCM recien decodificadas. Llamado desde el thread
        // principal en Update() y en el pre-buffer de Play().
        // samples     : puntero a muestras int16_t interleaved (L,R,L,R,...)
        // sample_count: numero total de muestras (frames * channels)
        void PushAudio(const int16_t* samples, uint32_t sample_count);

        // Vacia la cola de audio sin reproducir. Llamado en Stop() y Close()
        // para descartar datos pendientes antes de detener el stream.
        void Flush();

        // Devuelve el numero de muestras actualmente en la cola de audio.
        // Usado en Play() para saber cuando se ha completado el pre-buffer.
        uint32_t GetQueueSize();


    // Segmento privado
    private:

        // Callback de SFML: llamado desde el thread interno de SFML cuando
        // necesita un nuevo bloque de PCM para el hardware de audio.
        // Si la cola tiene menos datos que el chunk, rellena el resto con
        // silencio (ceros) para evitar pop/clicks. Siempre devuelve true
        // para mantener el stream activo hasta que NGN_VideoStream lo detenga.
        virtual bool onGetData(Chunk& data) override;

        // Callback de SFML: seek en el stream. No implementado porque el
        // video OGV no soporta seek en esta version del modulo.
        virtual void onSeek(sf::Time timeOffset) override;

        std::vector<int16_t> _queue;    // Cola de muestras PCM acumuladas
        std::vector<int16_t> _chunk;    // Buffer de salida entregado a SFML en cada callback
        sf::Mutex _mutex;               // Protege el acceso concurrente a _queue entre threads

};



/******************************************************************************

    Clase NGN_VideoStream

    Objeto stream de video OGV (Theora + Vorbis). Equivalente a NGN_MusicClip
    en el subsistema de audio: el usuario no lo instancia directamente, sino
    que lo obtiene de NGN_Video::Open() y lo pasa a los metodos del manager.

    Toda la interfaz publica (constructor, metodos de control, propiedades)
    es privada y accesible solo via friend class NGN_Video y NGN_Render.
    Esto garantiza que el ciclo de vida del stream lo gestiona exclusivamente
    el manager, igual que NGN_AudioClip es gestionado por NGN_Sound.

    Expone frame_texture (SDL_Texture*) con el frame actual decodificado,
    listo para que NGN_Render::Video() lo vuelque al backbuffer del renderer
    como una textura mas del pipeline de render.

    Sincronismo A/V:
        El reloj maestro (_start_ticks) arranca en Play() justo antes de
        _audio_stream->play(). En cada llamada a Update(), elapsed_ms mide
        el tiempo transcurrido desde ese instante y se usa para avanzar los
        frames de video al ritmo correcto, descartando los frames que ya
        deberian haberse mostrado (frame drop) si el game loop va lento.

******************************************************************************/

class NGN_VideoStream {

    // Segmento privado
    // Toda la interfaz es privada: el acceso externo se realiza
    // exclusivamente a traves de NGN_Video y NGN_Render.
    private:

        /*** Ajuste de permisos ***/
        friend class NGN_Video;
        friend class NGN_Render;
        friend class NGN_Sound;


        /*** Decoder theoraplay ***/
        // Handle al decoder activo. nullptr cuando no hay sesion abierta.
        THEORAPLAY_Decoder* _decoder;


        /*** Ciclo de vida del stream ***/

        // Constructor: inicializa todos los miembros a valores seguros.
        // Solo NGN_Video puede instanciar este objeto via friend.
        NGN_VideoStream();

        // Destructor: llama a Close() y destruye el audio stream.
        ~NGN_VideoStream();

        // Abre el archivo OGV, arranca el decoder y espera los primeros
        // metadatos (resolucion, framerate, channels, freq).
        // Llamado por NGN_Video::Open(). No inicia la reproduccion.
        // Devuelve false si el archivo no existe o no es un OGV valido.
        bool Open(const std::string& filepath);

        // Detiene la reproduccion, libera todos los recursos y resetea el estado.
        // Llamado por NGN_Video::Close() y por el destructor.
        void Close();


        /*** Control de reproduccion ***/

        // Inicia la reproduccion desde cero o reanuda desde pausa.
        // En inicio desde cero: llama a Initialize(), construye el pre-buffer
        // de audio, ancla el reloj maestro y arranca el stream de SFML.
        // En reanuda: desplaza _start_ticks para compensar el tiempo pausado
        // y reanuda el stream de SFML.
        void Play();

        // Pausa la reproduccion activa guardando el instante de pausa en
        // _pause_ticks para poder compensar el reloj al reanudar.
        void Pause();

        // Detiene la reproduccion, cierra el decoder y lo reabre para dejar
        // el stream rebobinado y listo para un Play() posterior, igual que
        // NGN_MusicClip::Stop() deja la musica al principio.
        void Stop();


        /*** Consulta de estado ***/

        // True si el stream se esta reproduciendo activamente (no pausado).
        bool IsPlaying();

        // True si el stream esta activo: reproduciendose o en pausa.
        // Un stream detenido con Stop() o que ha llegado al final no es alive.
        bool IsAlive();

        // True si el stream ha llegado al final del archivo OGV.
        // NGN_Video::VideoUpdate() elimina automaticamente los streams finalizados.
        bool IsFinished();

        /*** Frame actual ***/
        // Textura SDL con el ultimo frame decodificado y sincronizado.
        // Actualizada por Update() mediante SDL_UpdateTexture().
        // NGN_Render::Video() la vuelca al backbuffer via SDL_RenderCopy().
        SDL_Texture* frame_texture;

        /*** Propiedades de la textura del frame ***/
        Vector2 position;           // Posicion de la textura
        float width;                // Tamaño
        float height;
        Vector2 center;             // Centro de la textura para su rotacion
        bool visible;               // Visibilidad
        int32_t alpha;              // Nivel de alpha
        SDL_BlendMode blend_mode;   // Modo de mezcla de color
        double rotation;            // Rotacion de la textura
        bool flip_h;                // Flip de la textura
        bool flip_v;
        Size2 original_size;         		// Tamaño original
        Size2 current_scale;                // Escala actual


        /*** Volumen ***/
        // Establece el volumen del audio (0 ~ 100).
        void Volume(int32_t volume);
        // Devuelve el volumen actual.
        int32_t GetVolume();
        // Parametros del volumen
        int32_t _volume;
        uint8_t _mixer_channel;

        /*** Gestion de la tinta ***/
        Rgba tint_color;
        Rgba last_tint_color;
        bool NewTint();

        /*** Gestion del loop ***/
        bool loop;


        /*** Update ***/

        // Nucleo del sincronismo A/V. Llamado por NGN_Video::Update() una
        // vez por iteracion del game loop, antes de ngn->graphics->Update().
        //
        // En cada llamada:
        // 1. Drena todos los paquetes de audio disponibles del decoder y los
        //    encola en el stream de SFML via PushAudio().
        // 2. Avanza los frames de video hasta el que corresponde segun
        //    elapsed_ms = SDL_GetTicks() - _start_ticks. Si el game loop va
        //    lento, descarta frames intermedios (frame drop) para mantener sync.
        // 3. Actualiza frame_texture con el frame seleccionado.
        // 4. Comprueba el fin de stream (EOS) y activa _finished si procede.
        void Update();


        /*** Contexto de I/O para los callbacks del decoder ***/
        // La API de theoraplay usa seek absoluto (no fseek con whence) y
        // streamlen en lugar de ftell. file_size se calcula una sola vez
        // al abrir el archivo con fseek/ftell y se sirve en IoStreamLen.
        // Reservado: los campos from_package, package_offset y package_size
        // se usaran en el futuro para soportar streaming desde paquetes
        // cifrados cuando NGN_FileSystem exponga lectura fragmentada.

        struct IoContext {
            FILE* file;                         // Handle de archivo en modo disco directo
            long file_size;                     // Tamaño total del archivo para IoStreamLen
            bool from_package;                  // Modo paquete (cifrado) o disco
            std::string package_file;           // Archivo de empaquetado
            std::string stream_file;            // Archivo del stream de video
            uint32_t package_offset;            // Offset del archivo en el paquete
            uint32_t package_size;              // Tamaño del archivo en el paquete
            long logical_offset;                // Offset logico manual para evitar fallos de lectura
            bool package_encrypted;             // Esta encriptado el paquete?
            std::vector<uint8_t> package_key;   // Clave del paquete (si se usa un paquete encriptado)
        } _io_ctx;

        // Estructura de callbacks alojada como miembro de la clase (no en la pila).
        // Critico: theoraplay mantiene un puntero a esta estructura durante toda la
        // vida del decoder. Si se alojara en la pila de OpenDecoder() se corromperia
        // al salir de esa funcion, produciendo un crash en el thread del decoder.
        THEORAPLAY_Io _io_callbacks;

        // Callbacks estaticos de I/O con las firmas exactas requeridas por THEORAPLAY_Io.
        // El puntero io->userdata apunta siempre a _io_ctx.
        static long IoRead(THEORAPLAY_Io* io, void* buf, long buflen);      // Lee buflen bytes del archivo
        static long IoStreamLen(THEORAPLAY_Io* io);                         // Devuelve el tamaño total del archivo
        static int  IoSeek(THEORAPLAY_Io* io, long absolute_offset);        // Seek absoluto desde el inicio
        static void IoClose(THEORAPLAY_Io* io);                             // No-op: CloseDecoder() gestiona el FILE*


        /*** Stream de audio SFML ***/
        NGN_VideoAudioStream* _audio_stream;    // Puente decoder Vorbis -> hardware SFML


        /*** Estado de reproduccion ***/
        bool _playing;              // True si hay reproduccion activa (no pausada)
        bool _paused;               // True si el stream esta pausado
        bool _stopped;              // True si esta detenido (no hagas el render de este video)
        bool _finished;             // True si el stream llego al final del OGV
        bool _opened;               // True si Open() tuvo exito y el decoder esta listo
        bool _has_audio;            // True si el OGV contiene pista de audio Vorbis

        uint32_t _start_ticks;      // SDL_GetTicks() en el momento de arrancar Play()
        uint32_t _pause_ticks;      // SDL_GetTicks() en el momento de ejecutar Pause()
                                    // Al reanudar: _start_ticks += (now - _pause_ticks)


        /*** Ruta del archivo ***/
        // Guardada en Open() para que Stop() pueda reabrir el decoder
        // y dejar el stream listo para un Play() posterior.
        std::string _filepath;


        /*** Frames pendientes del decoder ***/
        // Punteros a los ultimos elementos extraidos de las colas de theoraplay
        // que aun no han sido procesados. Se guardan entre iteraciones para
        // no descartar datos al salir del bucle de drenado en Update().
        // IMPORTANTE: liberar siempre con THEORAPLAY_freeVideo/freeAudio,
        // nunca con delete.
        const THEORAPLAY_VideoFrame* _pending_video;
        const THEORAPLAY_AudioPacket* _pending_audio;


        /*** Metodos privados de ciclo de vida ***/

        // Abre el archivo, calcula file_size, configura los callbacks de I/O,
        // arranca THEORAPLAY_startDecode() y espera los primeros metadatos.
        // Reutilizado por Open() (primera apertura) y Stop() (rebobinado).
        // No llama a Initialize() del stream de audio: eso lo hace Play().
        bool OpenDecoder();

        // Libera _pending_video, _pending_audio, el decoder, el FILE* y
        // frame_texture. Seguro de llamar aunque algunos recursos sean nullptr.
        void CloseDecoder();

        // Crea o recrea frame_texture con las dimensiones del video.
        // Usa SDL_TEXTUREACCESS_STREAMING para permitir SDL_UpdateTexture() cada frame.
        // Solo actualiza width/height si aun valen 0.0f (primera creacion).
        void AllocateFrameTexture(uint32_t w, uint32_t h);

        // Convierte un paquete de audio Vorbis de float32 normalizado (-1..1)
        // a int16 lineal con clampeo explicito, y lo encola en _audio_stream.
        void PushAudioPacket(const THEORAPLAY_AudioPacket* pkt);

};



#endif // NGN_VIDEO_STREAM_H_INCLUDED
