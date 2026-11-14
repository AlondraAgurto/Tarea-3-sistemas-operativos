#ifndef INDEXADA_H
#define INDEXADA_H

#include "estructuras.h"
#include "disco.h"

// Funciones para la asignación y acceso de archivos indexados
bool asignar_indexada(Disco *disco, Archivo *archivo, int tamano_bloque_kb); // Asigna bloques en el disco para un archivo indexado específico
int acceder_indexada(Disco *disco, Archivo *archivo, int posicion_logica, int tamano_bloque_kb, int *accesos_necesarios); // Accede a un bloque específico de un archivo indexado en el disco

#endif