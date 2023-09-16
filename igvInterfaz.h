#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

#include <string>

/**
 * Clase para encapsular la configuración y la interfaz de la aplicación
 */
class igvInterfaz {
private:
    // Atributos
    int ancho_ventana = 0; ///< Ancho inicial de la ventana de visualización
    int alto_ventana = 0;  ///< Alto inicial de la ventana de visualización
    bool _ejes;

private:

    // Aplicación del patrón Singleton
    static igvInterfaz *_instancia;   ///< Puntero al objeto único de la clase
    /// Constructor por defecto
    igvInterfaz() = default;

public:
    // Aplicación del patrón Singleton
    static igvInterfaz &getInstancia();

    /// Destructor
    ~igvInterfaz() = default;

    // Metodos estáticos
    // callbacks de eventos
    static void keyboardFunc(unsigned char key, int x, int y); // método para control de eventos del teclado
    static void reshapeFunc(int w, int h); // método que define la camara de visión y el viewport
    // se llama automáticamente cuando se cambia el tamaño de la ventana
    static void displayFunc(); // método para visualizar la escena


    // Metodos
    // inicializa todos los parámetros para crear una ventana de visualización
    void configura_entorno(int argc, char **argv // parametros del main
            , int _ancho_ventana, int _alto_ventana // ancho y alto de la ventana de visualización
            , int _pos_X, int _pos_Y // posicion inicial de la ventana de visualización
            , std::string _titulo // título de la ventan de visualización
    );

    void inicializa_callbacks(); // inicializa todos los callbacks

    void inicia_bucle_visualizacion(); // visualiza la escena y espera a eventos sobre la interfaz

    // métodos get_ y set_ de acceso a los atributos
    int get_ancho_ventana();

    int get_alto_ventana();

    void set_ancho_ventana(int _ancho_ventana);

    void set_alto_ventana(int _alto_ventana);

    void setEjes(bool ejes);

    bool getEjes();
};

#endif   // __IGVINTERFAZ
