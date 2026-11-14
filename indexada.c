#include <stdio.h>
#include <stdlib.h>
#include "indexada.h"

// Función para asignar bloques en el disco para un archivo indexado específico
bool asignar_indexada(Disco *disco, Archivo *archivo, int tamano_bloque_kb) { // Asigna bloques en el disco para un archivo indexado específico
    if (!disco || !archivo) return false; // Verifica si los punteros son válidos

    int bloques_datos_necesarios = calcular_bloques(archivo->tamano_kb, tamano_bloque_kb); // Calcula el número de bloques de datos necesarios para almacenar el archivo
    if (bloques_datos_necesarios <= 0) return false; // Verifica si el número de bloques de datos necesarios es válido

    int total_necesarios = bloques_datos_necesarios + 1; // Total de bloques necesarios (bloques de datos + 1 bloque de índice)
    int libres = 0; // Contador de bloques libres en el disco
    for (int i = 0; i < disco->total_bloques; i++) { // Itera sobre todos los bloques del disco
        if (!disco->bloques[i].ocupado) libres++; // Incrementa el contador si el bloque actual está libre
    }
    if (libres < total_necesarios) return false; // Verifica si hay suficientes bloques libres en el disco para asignar al archivo

    int pos_indice = -1; // Variable para almacenar la posición del bloque de índice
    for (int i = 0; i < disco->total_bloques; i++) { // Itera sobre todos los bloques del disco para encontrar un bloque libre para el índice
        if (!disco->bloques[i].ocupado) { // Verifica si el bloque actual está libre
            pos_indice = i; // Guarda la posición del bloque libre para el índice
            break; // Sale del bucle después de encontrar el primer bloque libre
        }
    }

    int *indices_temp = (int*) malloc(bloques_datos_necesarios * sizeof(int)); // Reserva memoria para un arreglo temporal de índices de bloques de datos
    if (!indices_temp) return false; // Verifica si la memoria fue asignada correctamente

    int encontrados = 0; // Contador de bloques de datos encontrados
    for (int i = 0; i < disco->total_bloques && encontrados < bloques_datos_necesarios; i++) { // Itera sobre todos los bloques del disco para encontrar bloques libres para los datos
        if (i != pos_indice && !disco->bloques[i].ocupado) { // Verifica si el bloque actual está libre y no es el bloque de índice
            indices_temp[encontrados] = i; // Guarda la posición del bloque libre en el arreglo temporal de índices
            encontrados++; // Incrementa el contador de bloques de datos encontrados
        }
    }

    if (encontrados < bloques_datos_necesarios) { // Si no se encontraron suficientes bloques de datos libres, libera la memoria temporal y retorna false
        free(indices_temp); // Libera la memoria temporal asignada para los índices de bloques de datos
        return false; // Retorna false indicando que no se pudieron asignar suficientes bloques de datos
    }

    reservar_bloque(disco, pos_indice, archivo->nombre); // Reserva el bloque de índice en el disco para el archivo
    disco->bloques[pos_indice].indices = indices_temp; // Asigna el arreglo temporal de índices al bloque de índice en el disco
    disco->bloques[pos_indice].num_indices = bloques_datos_necesarios; // Asigna el número de índices al bloque de índice en el disco

    for (int i = 0; i < bloques_datos_necesarios; i++) { // Itera sobre los bloques de datos encontrados y los reserva en el disco
        reservar_bloque(disco, indices_temp[i], archivo->nombre); // Llama a la función para reservar cada bloque de datos en el disco
    }

    archivo->bloque_indice = pos_indice; // Asigna la posición del bloque de índice al archivo
    archivo->cantidad_bloques = bloques_datos_necesarios; // Asigna la cantidad de bloques de datos al archivo
    archivo->bloques_datos = (int*) malloc(bloques_datos_necesarios * sizeof(int)); // Reserva memoria para el arreglo de índices de bloques de datos del archivo
    for (int i = 0; i < bloques_datos_necesarios; i++) { // Itera sobre los bloques de datos encontrados y los asigna al arreglo de índices del archivo
        archivo->bloques_datos[i] = indices_temp[i]; // Guarda la posición física del bloque de datos reservado en el arreglo de índices del archivo
    }

    return true; // Retorna true indicando que la asignación fue exitosa
}

// Función para acceder a un bloque específico de un archivo indexado en el disco
int acceder_indexada(Disco *disco, Archivo *archivo, int posicion_logica, int tamano_bloque_kb, int *accesos_necesarios) { // Accede a un bloque específico de un archivo indexado en el disco
    if (!disco || !archivo || tamano_bloque_kb <= 0 || archivo->cantidad_bloques == 0) { // Verifica si los punteros son válidos y si el tamaño de bloque es positivo y si el archivo tiene bloques asignados
        if (accesos_necesarios) *accesos_necesarios = 0; // Si el puntero de accesos_necesarios no es NULL, establece el valor a 0
        return -1;
    }

    int indice_bloque_logico = posicion_logica / tamano_bloque_kb; // Calcula el índice del bloque lógico correspondiente a la posición lógica solicitada
    if (indice_bloque_logico < 0 || indice_bloque_logico >= archivo->cantidad_bloques) { // Verifica si el índice del bloque lógico está dentro del rango válido de bloques asignados al archivo
        if (accesos_necesarios) *accesos_necesarios = 0; // Si el puntero de accesos_necesarios no es NULL, establece el valor a 0
        return -1; // Retorna -1 indicando que la posición lógica solicitada está fuera del rango válido de bloques asignados al archivo
    }

    if (accesos_necesarios) { // Si el puntero de accesos_necesarios no es NULL, establece el valor a 2 (1 acceso al bloque índice y 1 acceso al bloque de datos)
        *accesos_necesarios = 2; // 1 acceso al bloque índice y 1 acceso al bloque de datos
    }

    int pos_bloque_indice = archivo->bloque_indice; // Obtiene la posición del bloque índice del archivo
    if (pos_bloque_indice < 0 || pos_bloque_indice >= disco->total_bloques) { // Verifica si la posición del bloque índice está dentro del rango válido de bloques del disco
        return -1; // Retorna -1 indicando que la posición del bloque índice está fuera del rango válido de bloques del disco
    }

    return disco->bloques[pos_bloque_indice].indices[indice_bloque_logico]; // Retorna la posición física del bloque de datos correspondiente al índice del bloque lógico solicitado
}