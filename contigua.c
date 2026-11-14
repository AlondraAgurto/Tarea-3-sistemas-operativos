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

int acceder_contigua(Archivo *archivo, int posicion_logica, int tamano_bloque_kb, int *accesos_necesarios) { // Accede a un bloque específico de un archivo contiguo en el disco
    if (!archivo || tamano_bloque_kb <= 0 || archivo->cantidad_bloques == 0) { // Verifica si los parámetros son válidos
        if (accesos_necesarios) *accesos_necesarios = 0; // Si el puntero de accesos_necesarios no es nulo, asigna 0 indicando que no se realizaron accesos
        return -1; // Retorna -1 indicando que no se pudo acceder al bloque
    }

    int indice_bloque_logico = posicion_logica / tamano_bloque_kb; // Calcula el índice del bloque lógico correspondiente a la posición lógica proporcionada
    if (indice_bloque_logico < 0 || indice_bloque_logico >= archivo->cantidad_bloques) { // Verifica si el índice del bloque lógico está dentro de los límites del archivo
        if (accesos_necesarios) *accesos_necesarios = 0; // Si el puntero de accesos_necesarios no es nulo, asigna 0 indicando que no se realizaron accesos
        return -1; // Retorna -1 indicando que la posición lógica está fuera de los límites del archivo
    }

    if (accesos_necesarios) { // Si el puntero de accesos_necesarios no es nulo, asigna 1 indicando que se realizó un acceso
        *accesos_necesarios = 1; // Asigna 1 indicando que se realizó un acceso
    }

    return archivo->bloques_datos[indice_bloque_logico]; // Retorna la posición física del bloque correspondiente al índice del bloque lógico
}