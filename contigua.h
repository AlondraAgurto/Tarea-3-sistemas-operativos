#ifndef CONTIGUA_H
#define CONTIGUA_H

#include "estructuras.h"
#include "disco.h"

// Funciones para la asignación y acceso a archivos contiguos
bool asignar_contigua(Disco *disco, Archivo *archivo, int tamano_bloque_kb);
int acceder_contigua(Archivo *archivo, int posicion_logica, int tamano_bloque_kb, int *accesos_necesarios);

#endif