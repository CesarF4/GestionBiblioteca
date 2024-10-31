import copy
import os

ARCHIVO_BIBLIOTECA = "biblioteca.txt"
ARCHIVO_SOLICITUDES = "solicitudes.txt"


class Libro:
    def __init__(self, titulo, autor, año_edicion, editorial, isbn, paginas):
        self.titulo = titulo
        self.autor = autor
        self.año_edicion = año_edicion
        self.editorial = editorial
        self.isbn = isbn
        self.paginas = paginas
        self.siguiente = None
    
    def obtener_siguiente(self):
        return self.siguiente
    

class Lector:
    def __init__(self, nombre, dni):
        self.nombre = nombre
        self.dni = dni
        self.libro_solicitado = None


class Biblioteca:
    def __init__(self):
        self.cabeza = None
        self.solicitudes = []

    def agregar_libro_al_final(self, titulo, autor, año_edicion, editorial, isbn, paginas):
        libro = Libro(titulo, autor, año_edicion, editorial, isbn, paginas)
        
        if self.cabeza == None:
            self.cabeza = libro
            return
        
        libro_actual = self.cabeza
        while (libro_actual.siguiente != None):
            libro_actual = libro_actual.siguiente

        libro_actual.siguiente = libro

    def buscar_libro_por_titulo(self, titulo):
        libro_actual = self.cabeza
        while libro_actual is not None:
            if libro_actual.titulo == titulo:
                return libro_actual
            libro_actual = libro_actual.siguiente
        return None

    def solicitar_libro(self, lector, titulo_libro):
        libro = self.buscar_libro_por_titulo(titulo_libro)
        if libro:
            
            for solicitud in self.solicitudes:
                if solicitud.libro_solicitado == libro:
                    print(f"Este libro ya esta solicitado por {solicitud.nombre}")
                    return
            
            lector.libro_solicitado = libro.titulo
            self.solicitudes.append(lector)
        else:
            print("El libro no existe")

    def devolver_libro(self, lector):
        if lector.libro_solicitado:
            lector.libro_solicitado = None
            self.solicitudes.remove(lector)
            return True
        else:
            print("El lector no tiene un libro solicitado")
            return False

    def guardar_datos(self):
        try:
            with open(ARCHIVO_BIBLIOTECA, "w") as archivo:
                
                libro = self.cabeza
                while libro is not None:

                    archivo.write(f"{libro.titulo},{libro.autor},{libro.año_edicion},{libro.editorial},{libro.isbn},{libro.paginas}\n")
                    
                    libro = libro.siguiente
            with open(ARCHIVO_SOLICITUDES, "w") as archivo:
                for lector in self.solicitudes:
                    archivo.write(f"{lector.nombre},{lector.dni},{lector.libro_solicitado if lector.libro_solicitado else ''}\n")
            print("Los datos de biblioteca y solicitudes fueron guardados correctamente.")
        
        except Exception as error:
            print(f"Ocurrio un error al guardar los archivos ({type(error)})")
    
    def cargar_libros_desde_archivo(self):
        try:
            if os.path.exists(ARCHIVO_BIBLIOTECA):
                with open(ARCHIVO_BIBLIOTECA, "r") as archivo:
                    for linea in archivo:
                        datos = linea.strip().split(",")
                        self.agregar_libro_al_final(datos[0], datos[1], datos[2], datos[3], datos[4], datos[5])
                    print(f"Los datos de biblioteca han sido cargados.")
            
            if os.path.exists(ARCHIVO_SOLICITUDES):
                with open(ARCHIVO_SOLICITUDES, "r") as archivo:
                    for linea in archivo:
                        datos = linea.strip().split(",")
                        
                        lector = Lector(datos[0], datos[1])
                        if datos[2] != '':
                            lector.libro_solicitado = datos[2]
                        self.solicitudes.append(lector)
                    print(f"Los datos de solicitud han sido cargados.")
        
        except Exception as error:
            print(f"Ocurrio un error al cargar los archivos de datos ({type(error)})")

#quicksort recursivo
def quicksort_libros(head):
    if head is None or head.siguiente is None:
        return head

    pivot = head
    menor_cabeza = menor_cola = None
    mayor_cabeza = mayor_cola = None
    current = head.siguiente

    while current is not None:
        next_node = current.siguiente
        current.siguiente = None
        if current.titulo < pivot.titulo:
            if menor_cabeza is None:
                menor_cabeza = menor_cola = current
            else:
                menor_cola.siguiente = current
                menor_cola = current
        else:
            if mayor_cabeza is None:
                mayor_cabeza = mayor_cola = current
            else:
                mayor_cola.siguiente = current
                mayor_cola = current
        current = next_node

    ordenado_menor = quicksort_libros(menor_cabeza)
    ordenado_mayor = quicksort_libros(mayor_cabeza)

    if ordenado_menor is not None:
        menor_cola.siguiente = pivot
        pivot.siguiente = ordenado_mayor
        return ordenado_menor
    else:
        pivot.siguiente = ordenado_mayor
        return pivot


#input con checkeo de errores (int)
def input_int(mensaje, permitir_negativo):
    while True:
        try:
            valor = input(mensaje)
            conversion = int(valor)
            if not permitir_negativo and conversion < 0:
                print("El valor ingresado no puede ser negativo.")
                continue
            return conversion
        except ValueError:
            print("Error al procesar el valor ingresado.")
            continue

#input con checkeo de errores (isbn)
def input_isbn(mensaje):
    while True:
        try:
            valor = input(mensaje)
           
            isbn = valor.replace("-", "").upper()

            if len(isbn) != 13 or not isbn.isdigit():
                raise ValueError

            digito_de_control = isbn[-1]
            if  0 >= int(digito_de_control) >= 9:
                raise ValueError

            return valor
        except ValueError:
            print("El valor ISBN no es valido.")
            continue

def limpiar_consola():
    if os.name == "nt":  # Windows
        os.system("cls")
    else:  # Linux, MacOS, etc.
        os.system("clear")

def incrementar_pila(pila_biblioteca, pila_acciones, biblioteca, accion):
    pila_biblioteca.append(copy.deepcopy(biblioteca))
    pila_acciones.append(accion)

def limitar_pila(pila_biblioteca, pila_acciones, limite):
    if len(pila_biblioteca) > limite:
        for i in range(len(pila_biblioteca) - limite):
            del pila_biblioteca[0]
    if len(pila_acciones) > limite:
        for i in range(len(pila_acciones) - limite):
            del pila_acciones[0]
            
def main():
    pila_biblioteca = []
    pila_acciones = []

    biblioteca = Biblioteca()
    biblioteca_auxiliar = Biblioteca()
    biblioteca.cargar_libros_desde_archivo()

    while True:
        input("Presione Enter para continuar.")
        
        limpiar_consola()
        
        print("---------------------")
        if len(pila_acciones) > 0 and len(pila_biblioteca) > 0:
            print(f"|0| Deshacer ultima acción ({pila_acciones[-1]})")
        print("|1| Agregar libro")
        print("|2| Ordenar libros")
        print("|3| Buscar libro")
        print("|4| Solicitar libro")
        print("|5| Devolver libro")
        print("|6| Guardar y salir")
        print("---------------------")

        opcion = input("Ingrese una opción numerica: ")

        if opcion == "0" and len(pila_acciones) > 0 and len(pila_biblioteca) > 0: #Deshacer ultima acción
            biblioteca = pila_biblioteca[-1]
            del pila_biblioteca[-1]
            del pila_acciones[-1]

        elif opcion == "1": #Agregar libro
            incrementar_pila(pila_biblioteca, pila_acciones, biblioteca, "Agregar libro")

            titulo = input("Ingrese título: ")
            autor = input("Ingrese autor: ")
            año_edicion = input_int("Ingrese año de edición: ", False)
            editorial = input("Ingrese editorial: ")
            isbn = input_isbn("Ingrese ISBN: ")
            paginas = input_int("Ingrese número de páginas: ", False)
            biblioteca.agregar_libro_al_final(titulo, autor, año_edicion, editorial, isbn, paginas)

        
        elif opcion == "2": #Ordenar libros
            incrementar_pila(pila_biblioteca, pila_acciones, biblioteca, "Ordenar libros")
            libros_ordenados = quicksort_libros(biblioteca.cabeza)
            biblioteca.cabeza = libros_ordenados
            print("Los libros fueron ordenados por titulo.")

        elif opcion == "3": #Buscar libros
            titulo = input("Ingrese título del libro a buscar: ")
            libro = biblioteca.buscar_libro_por_titulo(titulo)
            if libro:
                print(f"Título: {libro.titulo}")
                print(f"Autor: {libro.autor}")
                print(f"Año de edición: {libro.año_edicion}")
                print(f"Editorial: {libro.editorial}")
                print(f"ISBN: {libro.isbn}")
                print(f"Páginas: {libro.paginas}")
            else:
                print("Libro no encontrado")

        elif opcion == "4": #Solicitar libro
            incrementar_pila(pila_biblioteca, pila_acciones, biblioteca, "Solicitar libro")
            nombre = input("Ingrese nombre del lector: ")
            dni = input("Ingrese DNI del lector: ")
            titulo_libro = input("Ingrese título del libro a solicitar: ")
            lector = Lector(nombre, dni)
            biblioteca.solicitar_libro(lector, titulo_libro)
        
        elif opcion == "5": #Devolver libro
            incrementar_pila(pila_biblioteca, pila_acciones, biblioteca, "Devolver libro")
            nombre = input("Ingrese nombre del lector: ")
            dni = input("Ingrese DNI del lector: ")

            devuelto = False
            lector_encontrado = False
            for solicitud in biblioteca.solicitudes:
                if solicitud.nombre == nombre and solicitud.dni == dni:
                    lector_encontrado = True
                    devuelto = biblioteca.devolver_libro(solicitud)

            if not lector_encontrado:
                print("No se encontro el lector, o el DNI es incorrecto.")
                continue
            if not devuelto:
                print("El libro no pudo ser devuelto.")
                continue
            print("El libro ha sido devuelto.")

        elif opcion == "6": #Guardar datos
            biblioteca.guardar_datos()
            input("Presione Enter para salir.")
            break
        
        else:
            print("Opción inválida...")
        
        # para limitar la cantidad de memoria que tomara la pila
        # despues de una cierta cantidad de acciones borramos las acciones mas viejas
        limitar_pila(pila_biblioteca, pila_acciones, 32)

main()
