#ifndef LECTOR_H
#define LECTOR_H

#include <string>

class Lector {
public:
    Lector(const std::string& nombre, const std::string& dni);
    std::string getNombre() const;
    std::string getDni() const;
    std::string getLibroSolicitado() const;
    void setLibroSolicitado(const std::string& libroSolicitado);

private:
    std::string nombre;
    std::string dni;
    std::string libroSolicitado;
};

#endif  // LECTOR_H
