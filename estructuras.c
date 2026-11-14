#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estructuras.h"

// Función para crear un disco con un número específico de bloques y tamaño de bloque
Disco* crear_disco(int total_bloques, int tamano_bloque_kb) {
    Disco *disco = (Disco*) malloc(sizeof(Disco)); // Asignar memoria para la estructura Disco
    if (!disco) return NULL;// Verificar si la asignación de memoria fue exitosa

    disco->total_bloques = total_bloques; // Asignar el número total de bloques al disco
    disco->tamano_bloque_kb = tamano_bloque_kb; // Asignar el tamaño de cada bloque al disco
    disco->bloques = (Bloque*) malloc(total_bloques * sizeof(Bloque)); // Asignar memoria para el arreglo de bloques del disco

    if (!disco->bloques) { // Verificar si la asignación de memoria para los bloques fue exitosa
        free(disco); // Liberar la memoria del disco si la asignación de bloques falla
        return NULL; // Retornar NULL para indicar que la creación del disco falló
    }

    for (int i = 0; i < total_bloques; i++) { // Inicializar cada bloque del disco
        disco->bloques[i].ocupado = false; // Marcar el bloque como no ocupado
        memset(disco->bloques[i].nombre_archivo, 0, sizeof(disco->bloques[i].nombre_archivo)); // Limpiar el nombre del archivo asociado al bloque
        disco->bloques[i].siguiente = -1; // Inicializar el índice del siguiente bloque como -1 (no hay siguiente bloque)
        disco->bloques[i].indices = NULL; // Inicializar el puntero a los índices de bloques adicionales como NULL
        disco->bloques[i].num_indices = 0; // Inicializar el número de índices en el arreglo como 0
    }

    return disco; // Retornar el puntero al disco recién creado yeyy
}

void liberar_disco(Disco *disco) { // Función para liberar la memoria ocupada por el disco y sus bloques [inserts escoba emoji jaja]
    if (disco) { // Verificar si el puntero al disco no es NULL
        if (disco->bloques) { // Verificar si el puntero a los bloques del disco no es NULL
            for (int i = 0; i < disco->total_bloques; i++) { // Iterar sobre cada bloque del disco
                if (disco->bloques[i].indices) { // Verificar si el puntero a los índices de bloques adicionales no es NULL
                    free(disco->bloques[i].indices); // Liberar la memoria ocupada por los índices de bloques adicionales
                }
            }
            free(disco->bloques); // Liberar la memoria ocupada por el arreglo de bloques del disco
        }
        free(disco); // Liberar la memoria ocupada por la estructura Disco
    }
}

Archivo* crear_archivo(const char *nombre, int tamano_kb) { // Función para crear un archivo con un nombre y tamaño especificados
    Archivo *archivo = (Archivo*) malloc(sizeof(Archivo)); // Asignar memoria para la estructura Archivo
    if (!archivo) return NULL; // Verificar si la asignación de memoria fue exitosa

    strncpy(archivo->nombre, nombre, sizeof(archivo->nombre) - 1); // Copiar el nombre del archivo al campo correspondiente, asegurando que no se desborde el buffer
    archivo->nombre[sizeof(archivo->nombre) - 1] = '\0'; // Asegurar que el nombre del archivo esté terminado en NULL
    archivo->tamano_kb = tamano_kb; // Asignar el tamaño del archivo en kilobytes al campo correspondiente
    archivo->bloques_datos = NULL; // Inicializar el puntero a los bloques de datos como NULL
    archivo->cantidad_bloques = 0; // Inicializar la cantidad de bloques que contiene el archivo como 0

    return archivo; // Retornar el puntero al archivo recién creado
}

void liberar_archivo(Archivo *archivo) { // Función para liberar la memoria ocupada por el archivo y sus bloques de datos
    if (archivo) { // Verificar si el puntero al archivo no es NULL
        if (archivo->bloques_datos) { // Verificar si el puntero a los bloques de datos del archivo no es NULL
            free(archivo->bloques_datos);// Liberar la memoria ocupada por el arreglo de bloques de datos del archivo
        }
        free(archivo); // Liberar la memoria ocupada por la estructura Archivo
    }
}

bool reservar_bloque(Disco *disco, int posicion, const char *nombre) { // Función para reservar un bloque en el disco para un archivo específico
    if (!disco || posicion < 0 || posicion >= disco->total_bloques) return false; // Verificar si el puntero al disco es NULL o si la posición del bloque es inválida

    if (!disco->bloques[posicion].ocupado) { // Verificar si el bloque en la posición especificada no está ocupado
        disco->bloques[posicion].ocupado = true; // Marcar el bloque como ocupado
        strncpy(disco->bloques[posicion].nombre_archivo, nombre, sizeof(disco->bloques[posicion].nombre_archivo) - 1); // Copiar el nombre del archivo al campo correspondiente del bloque, asegurando que no se desborde el buffer
        return true; // Retornar true para indicar que la reserva del bloque fue exitosa
    }
    return false; // Retornar false para indicar que el bloque ya estaba ocupado y no se pudo reservar
}

bool liberar_bloque(Disco *disco, int posicion) { // Función para liberar un bloque en el disco, marcándolo como disponible y eliminando cualquier referencia a un archivo
    if (!disco || posicion < 0 || posicion >= disco->total_bloques) return false;   // Verificar si el puntero al disco es NULL o si la posición del bloque es inválida

    disco->bloques[posicion].ocupado = false; // Marcar el bloque como no ocupado
    memset(disco->bloques[posicion].nombre_archivo, 0, sizeof(disco->bloques[posicion].nombre_archivo)); // Limpiar el nombre del archivo asociado al bloque
    disco->bloques[posicion].siguiente = -1; // Reiniciar el índice del siguiente bloque a -1 (no hay siguiente bloque)
    
    if (disco->bloques[posicion].indices) { // Verificar si el puntero a los índices de bloques adicionales no es NULL
        free(disco->bloques[posicion].indices); // Liberar la memoria ocupada por los índices de bloques adicionales
        disco->bloques[posicion].indices = NULL; // Reiniciar el puntero a los índices de bloques adicionales a NULL
        disco->bloques[posicion].num_indices = 0; // Reiniciar el número de índices en el arreglo a 0
    }
    return true; // Retornar true para indicar que la liberación del bloque fue exitosa
}

int calcular_bloques(int tamano_kb, int tamano_bloque_kb) { // Función para calcular el número de bloques necesarios para almacenar un archivo de un tamaño dado, considerando el tamaño de bloque especificado
    if (tamano_bloque_kb <= 0) return 0; // Verificar si el tamaño de bloque es inválido (menor o igual a 0) y retornar 0 en ese caso
    return (tamano_kb + tamano_bloque_kb - 1) / tamano_bloque_kb; // Calcular y retornar el número de bloques necesarios utilizando la fórmula de techo para la división
}