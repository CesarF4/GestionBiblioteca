#ifndef LIBRO_H
#define LIBRO_H

#include <string>

class Libro {
public:
    Libro(const std::string& titulo, const std::string& autor, int añoEdicion, const std::string& editorial, const std::string& isbn, int paginas);
    std::string getTitulo() const;
    std::string getAutor() const;
    int getAñoEdicion() const;
    std::string getEditorial() const;
    std::string getIsbn() const;
    int getPaginas() const;
    Libro *getSiguiente() const;
    void setSiguiente(Libro* siguiente);
private:
    std::string titulo;
    std::string autor;
    int añoEdicion;
    std::string editorial;
    std::string isbn;
    int paginas;
    Libro* siguiente;
};

#endif  // LIBRO_H
