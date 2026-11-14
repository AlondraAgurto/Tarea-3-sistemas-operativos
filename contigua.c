#include <stdio.h>
#include <stdlib.h>
#include "contigua.h"

bool asignar_contigua(Disco *disco, Archivo *archivo, int tamano_bloque_kb) { // Asigna bloques contiguos en el disco para un archivo específico
    if (!disco || !archivo) return false; // Verifica si los punteros son válidos

    int bloques_necesarios = calcular_bloques(archivo->tamano_kb, tamano_bloque_kb); // Calcula el número de bloques necesarios para almacenar el archivo
    if (bloques_necesarios <= 0) return false; // Verifica si el número de bloques necesarios es válido

    int inicio = -1; // Variable para almacenar la posición inicial de los bloques contiguos
    int consecutivos = 0; // Variable para contar los bloques contiguos encontrados

    for (int i = 0; i < disco->total_bloques; i++) { // Itera sobre todos los bloques del disco
        if (!disco->bloques[i].ocupado) { // Verifica si el bloque actual está libre
            if (consecutivos == 0) { // Si es el primer bloque libre encontrado, guarda la posición inicial
                inicio = i; // Guarda la posición inicial del primer bloque libre
            }
            consecutivos++; // Incrementa el contador de bloques contiguos encontrados
            if (consecutivos == bloques_necesarios) { // Si se han encontrado suficientes bloques contiguos, sale del bucle
                break; // Sale del bucle si se han encontrado suficientes bloques contiguos
            }
        } else { // Si el bloque actual está ocupado, reinicia el contador de bloques contiguos
            consecutivos = 0; // Reinicia el contador de bloques contiguos encontrados
            inicio = -1; // Reinicia la posición inicial ya que se encontró un bloque ocupado
        }
    }

    if (consecutivos < bloques_necesarios) { // Si no se encontraron suficientes bloques contiguos, retorna false
        return false; // Retorna false si no se encontraron suficientes bloques contiguos
    }

    archivo->cantidad_bloques = bloques_necesarios; // Asigna la cantidad de bloques al archivo
    archivo->bloques_datos = (int*) malloc(bloques_necesarios * sizeof(int)); // Reserva memoria para el arreglo de índices de bloques del archivo
    if (!archivo->bloques_datos) return false; // Verifica si la memoria fue asignada correctamente

    for (int i = 0; i < bloques_necesarios; i++) { // Itera sobre los bloques contiguos encontrados y los reserva en el disco
        int pos_fisica = inicio + i; // Calcula la posición física del bloque a reservar
        reservar_bloque(disco, pos_fisica, archivo->nombre); // Llama a la función para reservar el bloque en el disco
        archivo->bloques_datos[i] = pos_fisica; // Guarda la posición física del bloque reservado en el arreglo de índices del archivo
    }

    return true; // Retorna true indicando que la asignación fue exitosa
}