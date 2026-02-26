/******************************************************************************

    Project Template: Musica

    Proyecto iniciado el 13 de Noviembre del 2022
    (cc) 2021 by Cesar Rincon "NightFox"
    https://nightfoxandco.com
    contact@nightfoxandco.com

	Requiere N'gine 1.20.0+10th-anniversary o superior
	(c) 2016 - 2026 by Cesar Rincon "NightFox"
	https://nightfoxandco.com

    Requiere GCC 14.2.0 MinGW64 (SEH) - 64-bits
    https://www.mingw-w64.org/

    Requiere SDL2 (2.30.11) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SDL2_TTF (2.24.0) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.6.2) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG (20241228)
    (c) 2005 - 2024 by Lode Vandevenne
    http://lodev.org/lodepng/

******************************************************************************/



#ifndef SOUNDTRACK_H_INCLUDED
#define SOUNDTRACK_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>



/*** Declaracion de la clase ***/
class Soundtrack {

    public:

        // Devuelve la instancia
        static Soundtrack* GetInstance();
        // Elimina la instancia
        static void RemoveInstance();

        // Procesos iniciales despues de crear la instancia
        void BootUp();

        // Actualiza el estado del soundtrack
        void Update();

        // Listado de canciones disponibles
        struct {
            enum {
                title,                  // Musica del titulo
                none                    // ID especial, ninguna pista
            };
        } TRACK_ID;

        // Carga un archivo de audio
        bool Load(uint32_t id);

        // Descarga el archivo actual
        void Unload();

        // Inicia la reproduccion de la musica de fondo
        void Play();

        // Deten la repoduccion de la musica de fondo
        void Stop();

        // Gestion del volumen de la musica de fondo
        void SetVolumeTo(int8_t level, int8_t speed = 0);
        int8_t GetVolume();

        // Devuelve la pista cargada actualmente
        uint32_t GetCurrentTrackId();

        // Control del fade-out entre niveles
        void SetFadeOut(bool st);
        bool GetFadeOut();


    private:

        // Constructor de la clase
        Soundtrack();

        // Destructor de la clase
        ~Soundtrack();

        // Puntero de memoria a la instancia
        static Soundtrack* instance;

        // Puntero al stream de musica
        NGN_MusicClip* bgm;

        // Datos para el control del stream
        uint32_t current_track_id;          // ID de la pista cargada actualmente
        bool is_playing;                    // Esta en reproduccion?

        // Datos para el control del volumen
        struct {
            int8_t current;                 // Volumen de la musica (actual)
            int8_t target;                  // Volumen que has de alcanzar
            int8_t speed;                   // Velocidad del cambio
        } volume;
        void AdjustVolume();                // Ajusta el volumen al nivel solicitado

        // Control del fade-out
        bool fade_out;

};



#endif // SOUNDTRACK_H_INCLUDED
