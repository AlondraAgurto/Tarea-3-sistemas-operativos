#ifndef DISCO_H
#define DISCO_H

#include "estructuras.h"

// Prototipos de funciones para calcular espacio libre y fragmentación externa
int calcular_espacio_libre(Disco *disco);
int calcular_fragmentacion_externa(Disco *disco);

#endif