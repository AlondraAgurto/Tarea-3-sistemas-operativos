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

    libera_disco(disco); // Libera la memoria asignada para el disco
    return 0; // Retorna 0 indicando que el programa finalizó correctamente
}