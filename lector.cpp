#include "lector.h"

Lector::Lector(const std::string& nombre, const std::string& dni)
    : nombre(nombre), dni(dni), libroSolicitado("") {}

std::string Lector::getNombre() const {
    return nombre;
}

std::string Lector::getDni() const {
    return dni;
}

std::string Lector::getLibroSolicitado() const {
    return libroSolicitado;
}

void Lector::setLibroSolicitado(const std::string& libroSolicitado) {
    this->libroSolicitado = libroSolicitado;
}