#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include "disco.h"
#include "contigua.h"
#include "indexada.h"

// Función principal del programa
int main() { 
    int total_bloques = 20; // Número total de bloques en el disco
    int tamano_bloque_kb = 4; // Tamaño de cada bloque en kilobytes

    Disco *disco = crear_disco(total_bloques, tamano_bloque_kb); // Crea un disco con el número de bloques y tamaño de bloque especificados
    if (!disco) { // Verifica si el disco fue creado correctamente
        printf("Error al crear el disco\n"); // Imprime un mensaje de error si no se pudo crear el disco
        return 1; // Retorna un código de error
    }

    printf("Disco creado con exito. Bloques libres: %d\n", calcular_espacio_libre(disco)); // Imprime el número de bloques libres en el disco

    // Prueba de asignacion indexada
    Archivo *archivo2 = crear_archivo("video.mp4", 16); // Crea un archivo con el nombre "video.mp4" y tamaño de 16 KB
    if (asignar_indexada(disco, archivo2, tamano_bloque_kb)) { // Intenta asignar bloques en el disco para el archivo de forma indexada
        printf("Archivo '%s' asignado de forma indexada con exito.\n", archivo2->nombre); // Imprime un mensaje indicando que el archivo fue asignado correctamente
    } else { // Si no se pudo asignar el archivo, imprime un mensaje de error
        printf("No se pudo asignar el archivo de forma indexada.\n"); // Imprime un mensaje indicando que no se pudo asignar el archivo
    }

    printf("Bloques libres finales: %d\n", calcular_espacio_libre(disco)); // Imprime el número de bloques libres en el disco después de la asignación

    liberar_archivo(archivo2); // Libera la memoria ocupada por el archivo y sus bloques de datos

    liberar_disco(disco); // Libera la memoria ocupada por el disco y sus bloques
    return 0; // Retorna 0 indicando que el programa finalizó correctamente
}