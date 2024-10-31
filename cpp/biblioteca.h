#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H


#include <string>
#include <vector>
#include "libro.h"
#include "lector.h"

class Biblioteca {
public:
    Biblioteca();
    void agregarLibroAlFinal(const std::string& titulo, const std::string& autor, int añoEdicion, const std::string& editorial, const std::string& isbn, int paginas);
    void ordenarLibrosPorTitulo();
    Libro* buscarLibroPorTitulo(const std::string& titulo);
    void solicitarLibro(Lector& lector, const std::string& tituloLibro);
    bool devolverLibro(Lector& lector);
    void guardarDatos();
    void cargarLibrosDesdeArchivo();
    Libro* quicksortLibros(Libro* lista);
    std::vector<Lector> getSolicitudes();
private:
    Libro* cabeza;
    std::vector<Lector> solicitudes;
};

#endif  // BIBLIOTECA_H
