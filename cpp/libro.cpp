#include "libro.h"

Libro::Libro(const std::string& titulo, const std::string& autor, int añoEdicion, const std::string& editorial, const std::string& isbn, int paginas)
    : titulo(titulo), autor(autor), añoEdicion(añoEdicion), editorial(editorial), isbn(isbn), paginas(paginas), siguiente(nullptr) {}

std::string Libro::getTitulo() const {
    return titulo;
}

std::string Libro::getAutor() const {
    return autor;
}

int Libro::getAñoEdicion() const {
    return añoEdicion;
}

std::string Libro::getEditorial() const {
    return editorial;
}

std::string Libro::getIsbn() const {
    return isbn;
}

int Libro::getPaginas() const {
    return paginas;
}

Libro* Libro::getSiguiente() const {
    return siguiente;
}

void Libro::setSiguiente(Libro* siguiente) {
    this->siguiente = siguiente;
}
