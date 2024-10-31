#include "biblioteca.h"
#include <iostream>
#include <fstream>
#include <sstream>

Biblioteca::Biblioteca() : cabeza(nullptr) {}

void Biblioteca::agregarLibroAlFinal(const std::string& titulo, const std::string& autor, int añoEdicion, const std::string& editorial, const std::string& isbn, int paginas) {
    Libro* nuevoLibro = new Libro(titulo, autor, añoEdicion, editorial, isbn, paginas);

    if (cabeza == nullptr) {
        cabeza = nuevoLibro;
    } else {
        Libro* actual = cabeza;
        while (actual->getSiguiente() != nullptr) {
            actual = actual->getSiguiente();
        }
        actual->setSiguiente(nuevoLibro);
    }
}

// Función recursiva de quicksort
Libro* quicksortLibros(Libro* cabeza) {
    if (cabeza == nullptr || cabeza->getSiguiente() == nullptr) {
        return cabeza;
    }

    Libro* pivot = cabeza;
    Libro* menorCabeza = nullptr;
    Libro* menorCola = nullptr;
    Libro* mayorCabeza = nullptr;
    Libro* mayorCola = nullptr;
    Libro* actual = cabeza->getSiguiente();

    while (actual != nullptr) {
        Libro* siguienteLibro = actual->getSiguiente();
        actual->setSiguiente(nullptr);
        if (actual->getTitulo() < pivot->getTitulo()) {
            if (menorCabeza == nullptr) {
                menorCabeza = menorCola = actual;
            } else {
                menorCola->setSiguiente(actual);
                menorCola = actual;
            }
        } else {
            if (mayorCabeza == nullptr) {
                mayorCabeza = mayorCola = actual;
            } else {
                mayorCola->setSiguiente(actual);
                mayorCola = actual;
            }
        }
        actual = siguienteLibro;
    }

    Libro* ordenadoMenor = quicksortLibros(menorCabeza);
    Libro* ordenadoMayor = quicksortLibros(mayorCabeza);

    if (ordenadoMenor != nullptr) {
        menorCola->setSiguiente(pivot);
        pivot->setSiguiente(ordenadoMayor);
        return ordenadoMenor;
    } else {
        pivot->setSiguiente(ordenadoMayor);
        return pivot;
    }
}

void Biblioteca::ordenarLibrosPorTitulo() {
    // Implementación del algoritmo de ordenamiento quicksort
    Libro* ordenado = quicksortLibros(cabeza);
    cabeza = ordenado;
}

Libro* Biblioteca::buscarLibroPorTitulo(const std::string& titulo) {
    Libro* actual = cabeza;
    while (actual != nullptr) {
        if (actual->getTitulo() == titulo) {
            return actual;
        }
        actual = actual->getSiguiente();
    }
    return nullptr;
}

void Biblioteca::solicitarLibro(Lector& lector, const std::string& tituloLibro) {
    Libro* libro = buscarLibroPorTitulo(tituloLibro);
    if (libro != nullptr) {
        for (const Lector& solicitud : solicitudes) {
            if (solicitud.getLibroSolicitado() == tituloLibro) {
                std::cout << "Este libro ya está solicitado por " << solicitud.getNombre() << std::endl;
                return;
            }
        }
        lector.setLibroSolicitado(tituloLibro);
        solicitudes.push_back(lector);
    } else {
        std::cout << "El libro no existe" << std::endl;
    }
}

bool Biblioteca::devolverLibro(Lector& lector) {
    if (!lector.getLibroSolicitado().empty()) {
        lector.setLibroSolicitado("");
        for (auto it = solicitudes.begin(); it != solicitudes.end(); ++it) {
            if (it->getNombre() == lector.getNombre() && it->getDni() == lector.getDni()) {
                solicitudes.erase(it);
                return true;
            }
        }
    }
    std::cout << "El lector no tiene un libro solicitado" << std::endl;
    return false;
}

void Biblioteca::guardarDatos() {
    std::ofstream archivoBiblioteca("biblioteca.txt");
    std::ofstream archivoSolicitudes("solicitudes.txt");

    if (archivoBiblioteca.is_open() && archivoSolicitudes.is_open()) {
        Libro* actual = cabeza;
        while (actual != nullptr) {
            archivoBiblioteca << actual->getTitulo() << "," << actual->getAutor() << "," << actual->getAñoEdicion() << "," 
            << actual->getEditorial() << "," << actual->getIsbn() << "," << actual->getPaginas() << std::endl;
            actual = actual->getSiguiente();
        }

        for (const Lector& solicitud : solicitudes) {
            archivoSolicitudes << solicitud.getNombre() << "," << solicitud.getDni() << "," << solicitud.getLibroSolicitado() << std::endl;
        }

        std::cout << "Los datos de biblioteca y solicitudes fueron guardados correctamente." << std::endl;
    } else {
        std::cout << "Error al abrir archivos para guardar datos." << std::endl;
    }

    archivoBiblioteca.close();
    archivoSolicitudes.close();
}

void Biblioteca::cargarLibrosDesdeArchivo() {
    std::ifstream archivoBiblioteca;
    archivoBiblioteca.open("biblioteca.txt");
    if (archivoBiblioteca.is_open()) {
        std::string linea;
        while (std::getline(archivoBiblioteca, linea)) {
            std::istringstream iss(linea);
            std::string titulo, autor, editorial, añoEdicionStr, isbn, paginasStr;
            int añoEdicion, paginas;

            std::getline(iss, titulo, ',');
            std::getline(iss, autor, ',');
            std::getline(iss, añoEdicionStr, ',');
            std::getline(iss, editorial, ',');
            std::getline(iss, isbn, ',');
            std::getline(iss, paginasStr, ',');

            añoEdicion = std::stoi(añoEdicionStr);
            paginas = std::stoi(paginasStr);

            agregarLibroAlFinal(titulo, autor, añoEdicion, editorial, isbn, paginas);
        }

        std::cout << "Los libros fueron cargados correctamente desde el archivo." << std::endl;
    } else {
        std::cout << "Error al abrir archivo de biblioteca. (biblioteca.txt)" << std::endl;
    }


    std::ifstream archivoSolicitudes("solicitudes.txt");
    if (archivoSolicitudes.is_open()) {
        std::string linea;
        while (std::getline(archivoSolicitudes, linea)) {
            std::istringstream iss(linea);
            std::string nombre, dni, libroSolicitado;
            std::getline(iss, nombre, ',');
            std::getline(iss, dni, ',');
            std::getline(iss, libroSolicitado, ',');

            Lector lector(nombre, dni);
            if (!libroSolicitado.empty()) {
                lector.setLibroSolicitado(libroSolicitado);
            }
            solicitudes.push_back(lector);
        }
        std::cout << "Las solicitudes fueron cargadas correctamente desde el archivo." << std::endl;
        archivoSolicitudes.close();
    } else {
        std::cout << "Error al abrir archivo de solicitudes. (solicitudes.txt)" << std::endl;
    }
}

// Función para ordenar libros utilizando quicksort
Libro* Biblioteca::quicksortLibros(Libro* lista) {
    if (lista == nullptr || lista->getSiguiente() == nullptr) {
        return lista;
    }
    
    Libro* pivote = lista;
    Libro* menor = nullptr;
    Libro* menorFin = nullptr;
    Libro* mayor = nullptr;
    Libro* mayorFin = nullptr;
    Libro* actual = lista->getSiguiente();
    pivote->setSiguiente(nullptr);

    while (actual != nullptr) {
        Libro* siguiente = actual->getSiguiente();
        actual->setSiguiente(nullptr);
        if (actual->getTitulo() < pivote->getTitulo()) {
            if (menor == nullptr) {
                menor = actual;
                menorFin = menor;
            } else {
                menorFin->setSiguiente(actual);
                menorFin = actual;
            }
        } else {
            if (mayor == nullptr) {
                mayor = actual;
                mayorFin = mayor;
            } else {
                mayorFin->setSiguiente(actual);
                mayorFin = actual;
            }
        }
        actual = siguiente;
    }

    menor = quicksortLibros(menor);
    mayor = quicksortLibros(mayor);

    if (menor != nullptr) {
        Libro* ultimo = menor;
        while (ultimo->getSiguiente() != nullptr) {
            ultimo = ultimo->getSiguiente();
        }
        ultimo->setSiguiente(pivote);
    } else {
        menor = pivote;
    }
    pivote->setSiguiente(mayor);
    return menor;
}

std::vector<Lector> Biblioteca::getSolicitudes(){
    return solicitudes;
}