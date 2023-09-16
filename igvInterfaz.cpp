#include <cstdlib>
#include <iostream>

#include "igvInterfaz.h"

// Aplicación del patrón Singleton
igvInterfaz *igvInterfaz::_instancia = nullptr;

// Métodos públicos ----------------------------------------

/**
 * Método para acceder al objeto único de la clase, en aplicación del patrón de
 * diseño Singleton
 * @return Una referencia al objeto único de la clase
 */
igvInterfaz &igvInterfaz::getInstancia() {
    if (!_instancia) {
        _instancia = new igvInterfaz;
    }

    return *_instancia;
}

/**
 * Inicializa todos los parámetros para crear una ventana de visualización
 * @param argc Número de parámetros por línea de comandos al ejecutar la
 *             aplicación
 * @param argv Parámetros por línea de comandos al ejecutar la aplicación
 * @param _ancho_ventana Ancho inicial de la ventana de visualización
 * @param _alto_ventana Alto inicial de la ventana de visualización
 * @param _pos_X Coordenada X de la posición inicial de la ventana de
 *               visualización
 * @param _pos_Y Coordenada Y de la posición inicial de la ventana de
 *               visualización
 * @param _titulo Título de la ventana de visualización
 * @pre Se asume que todos los parámetros tienen valores válidos
 * @post Cambia el alto y ancho de ventana almacenado en el objeto
 */
void
igvInterfaz::configura_entorno(int argc, char **argv, int _ancho_ventana, int _alto_ventana, int _pos_X, int _pos_Y,
                               std::string _titulo) {  // inicialización de los atributos de la interfaz
    ancho_ventana = _ancho_ventana;
    alto_ventana = _alto_ventana;
    _ejes = false;

    // inicialización de la ventana de visualización
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_ancho_ventana, _alto_ventana);
    glutInitWindowPosition(_pos_X, _pos_Y);
    glutCreateWindow(_titulo.c_str());

    glEnable(GL_DEPTH_TEST); // activa el ocultamiento de caras por Z-buffer
    glClearColor(1.0, 1.0, 1.0, 0.0); // establece el color de fondo de la ventana
}

/**
 * Método para visualizar la escena y esperar a eventos sobre la interfaz
 */
void igvInterfaz::inicia_bucle_visualizacion() {
    glutMainLoop(); // inicia el bucle de visualización de GLUT
}

/**
 * Método para control de eventos del teclado
 * @param key Código de la tecla pulsada
 * @param x Coordenada X de la posición del cursor del ratón en el momento del
 *          evento de teclado
 * @param y Coordenada Y de la posición del cursor del ratón en el momento del
 *          evento de teclado
 * @pre Se asume que todos los parámetros tienen valores válidos
 * @post El atributo que indica si se tienen que pintar los _ejes o no puede
 *       cambiar
 */
void igvInterfaz::keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // tecla de escape para SALIR
            exit(1);
            break;
        case 101:
            if (_instancia->getEjes()) {
                _instancia->setEjes(false);
            } else {
                _instancia->setEjes(true);
            }
            break;
    }
    glutPostRedisplay(); // renueva el contenido de la ventana de visión
}

/**
 * Método que define la cámara de visión y el viewport. Se llama automáticamente
 * cuando se cambia el tamaño de la ventana.
 * @param w Nuevo ancho de la ventana
 * @param h Nuevo alto de la ventana
 * @pre Se asume que todos los parámetros tienen valores válidos
 */
void igvInterfaz::reshapeFunc(int w, int h) {  // dimensiona el viewport al nuevo ancho y alto de la ventana
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    // guardamos valores nuevos de la ventana de visualización
    _instancia->set_ancho_ventana(w);
    _instancia->set_alto_ventana(h);

    // establece el tipo de proyección a utilizar
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1, 1, -1, 1, -1, 200);

    // se define la camara de vision
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1.5, 1.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

/**
 * Método para visualizar la escena
 */
void igvInterfaz::displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra la ventana y el Z-buffer
    glPushMatrix(); // guarda la matriz de modelado

    // Apartado A: pintar los _ejes

    if (_instancia->getEjes()) {

        glColor3f(1, 0, 0);

        glBegin(GL_LINES);
        glVertex3f(-100, 0, 0);
        glVertex3f(100, 0, 0);
        glEnd();

        glColor3f(0, 1, 0);

        glBegin(GL_LINES);
        glVertex3f(0, -100, 0);
        glVertex3f(0, 100, 0);
        glEnd();

        glColor3f(0, 0, 1);

        glBegin(GL_LINES);
        glVertex3f(0, 0, -100);
        glVertex3f(0, 0, 100);
        glEnd();

    }

    // Apartado B: visualizar primitiva cubo en modo alambre

    glColor3f(0, 0, 0);

    glutWireCube(1);

    glColor3f(1, 0, 0);

    glBegin(GL_TRIANGLES);
    glVertex3f(-0.50, 0.50, 0.50);
    glVertex3f(0.50, 0.50, -0.50);
    glVertex3f(0.50, 0.50, 0.50);
    glEnd();

    glColor3f(0, 1, 0);

    glBegin(GL_TRIANGLES);
    glVertex3f(-0.50, 0.50, 0.50);
    glVertex3f(0.50, 0.50, -0.50);
    glVertex3f(-0.50, 0.50, -0.50);
    glEnd();

    // Apartado C: cara superior con triángulos

    glPopMatrix(); // restaura la matriz de modelado
    glutSwapBuffers(); // se utiliza, en vez de glFlush(), para evitar el parpadeo
}

/**
 * Método para inicializar los callbacks
 */
void igvInterfaz::inicializa_callbacks() {
    glutKeyboardFunc(keyboardFunc);
    glutReshapeFunc(reshapeFunc);
    glutDisplayFunc(displayFunc);
}

/**
 * Método para consultar el ancho de la ventana de visualización
 * @return El valor almacenado como ancho de la ventana de visualización
 */
int igvInterfaz::get_ancho_ventana() {
    return ancho_ventana;
}

/**
 * Método para consultar el alto de la ventana de visualización
 * @return El valor almacenado como alto de la ventana de visualización
 */
int igvInterfaz::get_alto_ventana() {
    return alto_ventana;
}

/**
 * Método para cambiar el ancho de la ventana de visualización
 * @param _ancho_ventana Nuevo valor para el ancho de la ventana de visualización
 * @pre Se asume que el parámetro tiene un valor válido
 * @post El ancho de ventana almacenado en la aplicación cambia al nuevo valor
 */
void igvInterfaz::set_ancho_ventana(int _ancho_ventana) {
    ancho_ventana = _ancho_ventana;
}

/**
 * Método para cambiar el alto de la ventana de visualización
 * @param _alto_ventana Nuevo valor para el alto de la ventana de visualización
 * @pre Se asume que el parámetro tiene un valor válido
 * @post El alto de ventana almacenado en la aplicación cambia al nuevo valor
 */
void igvInterfaz::set_alto_ventana(int _alto_ventana) {
    alto_ventana = _alto_ventana;
}

void igvInterfaz::setEjes(bool ejes) {
    _ejes = ejes;
}

bool igvInterfaz::getEjes() {
    return _ejes;
}
