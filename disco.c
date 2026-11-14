#include <stdio.h>
#include "disco.h"

// Calcula el espacio libre en el disco contando los bloques que no están ocupados
int calcular_espacio_libre(Disco *disco) {
    if (!disco) return 0; // Verifica si el puntero al disco es nulo
    int libres = 0; // Inicializa el contador de bloques libres
    for (int i = 0; i < disco->total_bloques; i++) { // Itera sobre todos los bloques del disco
        if (!disco->bloques[i].ocupado) { // Si el bloque no está ocupado, incrementa el contador
            libres++; // Incrementa el contador de bloques libres
        }
    }
    return libres; // Devuelve el número total de bloques libres en el disco
} 

// Calcula la fragmentación externa del disco contando los huecos de bloques libres
int calcular_fragmentacion_externa(Disco *disco) { // Verifica si el puntero al disco es nulo
    if (!disco) return 0; // Verifica si el puntero al disco es nulo
    
    int huecos = 0; // Inicializa el contador de huecos de bloques libres
    bool en_hueco = false; // Bandera para indicar si actualmente se está en un hueco de bloques libres
    
    for (int i = 0; i < disco->total_bloques; i++) { // Itera sobre todos los bloques del disco
        if (!disco->bloques[i].ocupado) { // Si el bloque no está ocupado, verifica si se está en un hueco
            if (!en_hueco) { // Si no se estaba en un hueco, incrementa el contador de huecos y marca que ahora se está en un hueco
                huecos++; // Incrementa el contador de huecos de bloques libres
                en_hueco = true; // Marca que ahora se está en un hueco de bloques libres
            }
        } else { // Si el bloque está ocupado, marca que ya no se está en un hueco
            en_hueco = false; // Marca que ya no se está en un hueco de bloques libres
        }
    }
    return huecos; // Devuelve el número total de huecos de bloques libres en el disco
}