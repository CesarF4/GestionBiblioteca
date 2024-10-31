#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "libro.cpp"
#include "lector.cpp"
#include "biblioteca.cpp"
#include <algorithm>
const std::string ARCHIVO_BIBLIOTECA = "biblioteca.txt";
const std::string ARCHIVO_SOLICITUDES = "solicitudes.txt";

void limpiarConsola() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int inputInt(const std::string& mensaje, bool permitirNegativo) {
    int valor;
    while (true) {
        std::cout << mensaje;
        std::cin >> valor;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cout << "Error al procesar el valor ingresado." << std::endl;
        } else if (!permitirNegativo && valor < 0) {
            std::cout << "El valor ingresado no puede ser negativo." << std::endl;
        } else {
            return valor;
        }
    }
}

std::string inputISBN(const std::string& mensaje) {
    std::string valor;
    while (true) {
        std::cout << mensaje;
        std::cin >> valor;
        std::string isbn = valor;
        
        isbn.erase(std::remove(isbn.begin(), isbn.end(), '-'), isbn.end());
        if (isbn.length() != 13 || !std::all_of(isbn.begin(), isbn.end(), ::isdigit)) {
            std::cout << "El valor ISBN no es valido." << std::endl;
        } else {
            return valor;
        }
    }
}


int main() {
    std::vector<Biblioteca> pilaBiblioteca;
    std::vector<std::string> pilaAcciones;

    Biblioteca biblioteca;
    biblioteca.cargarLibrosDesdeArchivo();

    while (true) {
        std::cout << "Presione Enter para continuar." << std::endl;
        std::cin.get();

        limpiarConsola();

        std::cout << "---------------------" << std::endl;
        if (!pilaAcciones.empty() && !pilaBiblioteca.empty()) {
            std::cout << "|0| Deshacer ultima acción (" << pilaAcciones.back() << ")" << std::endl;
        }
        std::cout << "|1| Agregar libro" << std::endl;
        std::cout << "|2| Ordenar libros" << std::endl;
        std::cout << "|3| Buscar libro" << std::endl;
        std::cout << "|4| Solicitar libro" << std::endl;
        std::cout << "|5| Devolver libro" << std::endl;
        std::cout << "|6| Guardar y salir" << std::endl;
        std::cout << "---------------------" << std::endl;

        int opcion;
        std::cout << "Ingrese una opción numerica: ";
        std::cin >> opcion;

        switch (opcion) {
            case 0:
                if (!pilaAcciones.empty() && !pilaBiblioteca.empty()) {
                    biblioteca = pilaBiblioteca.back();
                    pilaBiblioteca.pop_back();
                    pilaAcciones.pop_back();
                }
                break;
            case 1: {
                pilaBiblioteca.push_back(biblioteca);
                pilaAcciones.push_back("Agregar libro");

                std::string titulo, autor, editorial, isbn;
                int añoEdicion, paginas;
                
                std::cout << "Ingrese título: ";
                std::cin.ignore(256, '\n');
                std::getline(std::cin, titulo);
                
                std::cout << "Ingrese autor: ";
                std::getline(std::cin, autor);
                
                std::cout << "Ingrese año de edición: ";
                añoEdicion = inputInt("", false);
                
                std::cout << "Ingrese editorial: ";
                std::cin.ignore(256, '\n');
                std::getline(std::cin, editorial);
                
                std::cout << "Ingrese ISBN: ";
                isbn = inputISBN("");
                
                std::cout << "Ingrese número de páginas: ";
                paginas = inputInt("", false);

                biblioteca.agregarLibroAlFinal(titulo, autor, añoEdicion, editorial, isbn, paginas);
                break;
            }
            case 2:
                pilaBiblioteca.push_back(biblioteca);
                pilaAcciones.push_back("Ordenar libros");

                biblioteca.ordenarLibrosPorTitulo();
                std::cout << "Los libros fueron ordenados por título." << std::endl;
                break;
            case 3: {
                std::string titulo;
                std::cout << "Ingrese título del libro a buscar: ";
                std::cin.ignore(256, '\n');
                std::getline(std::cin, titulo);

                Libro* libro = biblioteca.buscarLibroPorTitulo(titulo);
                if (libro) {
                    std::cout << "Título: " << libro->getTitulo() << std::endl;
                    std::cout << "Autor: " << libro->getAutor() << std::endl;
                    std::cout << "Año de edición: " << libro->getAñoEdicion() << std::endl;
                    std::cout << "Editorial: " << libro->getEditorial() << std::endl;
                    std::cout << "ISBN: " << libro->getIsbn() << std::endl;
                    std::cout << "Páginas: " << libro->getPaginas() << std::endl;
                } else {
                    std::cout << "Libro no encontrado" << std::endl;
                }
                break;
            }
            case 4: {
                pilaBiblioteca.push_back(biblioteca);
                pilaAcciones.push_back("Solicitar libro");

                std::string nombre, dni, tituloLibro;
                std::cin.ignore(256, '\n');
                std::cout << "Ingrese nombre del lector: ";
                std::getline(std::cin, nombre);
                std::cout << "Ingrese DNI del lector: ";
                std::getline(std::cin, dni);
                std::cout << "Ingrese título del libro a solicitar: ";
                std::getline(std::cin, tituloLibro);

                Lector lector(nombre, dni);
                biblioteca.solicitarLibro(lector, tituloLibro);
                break;
            }
            case 5: {
                pilaBiblioteca.push_back(biblioteca);
                pilaAcciones.push_back("Devolver libro");

                bool lector_encontrado = false;

                std::string nombre, dni;
                std::cout << "Ingrese nombre del lector: ";
                std::cin.ignore();
                std::getline(std::cin, nombre);
                std::cout << "Ingrese DNI del lector: ";
                std::getline(std::cin, dni);

                //Lector = obtenemos el lector segun el nombre y dni
                
                Lector lector(nombre, dni);
                for (auto& solicitud : biblioteca.getSolicitudes()) { // Asegúrate que getSolicitudes() devuelva una referencia a la lista de solicitudes
                    if (solicitud.getNombre() == nombre && solicitud.getDni() == dni) {
                        lector_encontrado = true;
                        lector = solicitud;
                        break;
                    }
                }

                if (lector_encontrado) {
                    if (biblioteca.devolverLibro(lector)) {
                        std::cout << "El libro ha sido devuelto." << std::endl;
                    } else {
                        std::cout << "El libro no pudo ser devuelto." << std::endl;
                    }
                } else {
                    std::cout << "No se encontro el lector, o el DNI es incorrecto." << std::endl;
                }
                break;
            }
            case 6:
                biblioteca.guardarDatos();
                std::cout << "Presione Enter para salir." << std::endl;
                std::cin.get();
                return 0;
            default:
                std::cout << "Opción inválida..." << std::endl;
                std::cin.ignore();
        }

        // Limitar la cantidad de memoria que toma la pila
        while (pilaBiblioteca.size() > 32) {
            pilaBiblioteca.erase(pilaBiblioteca.begin());
        }
        while (pilaAcciones.size() > 32) {
            pilaAcciones.erase(pilaAcciones.begin());
        }
    }

    return 0;
}