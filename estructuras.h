#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <stdbool.h>

// Definición de la estructura de un BLOQUE
typedef struct {
    bool ocupado; // Indica si el bloque está ocupado
    char nombre_archivo[50]; // Nombre del archivo que ocupa el bloque
    int siguiente; // Índice del siguiente bloque en caso de que el archivo ocupe varios bloques
    int *indices;// Puntero a un arreglo de índices de bloques adicionales
    int num_indices; // Número de índices en el arreglo
} Bloque;

// Definición de la estructura del DISCO
typedef struct {
    int total_bloques; // Número total de bloques en el disco
    int tamano_bloque_kb; // Tamaño de cada bloque en kilobytes
    Bloque *bloques; // Puntero a un arreglo de bloques
} Disco;

// Definición de la estructura de un ARCHIVO
typedef struct {
    char nombre[50]; // Nombre del archivo
    int tamano_kb; // Tamaño del archivo en kilobytes
    int *bloques_datos; // Puntero a un arreglo de índices de bloques que contienen los datos del archivo
    int cantidad_bloques; // Número de bloques que contiene el archivo
    int bloque_indice; // Bloque índice utilizado para archivos indexados, que contiene los índices de los bloques de datos
} Archivo;

// Funciones para manejar el disco y los archivos
Disco* crear_disco(int total_bloques, int tamano_bloque_kb); // Crea un disco con el número de bloques y tamaño de bloque especificados
void liberar_disco(Disco *disco); // Libera la memoria ocupada por el disco y sus bloques
Archivo* crear_archivo(const char *nombre, int tamano_kb); // Crea un archivo con el nombre y tamaño especificados
void liberar_archivo(Archivo *archivo); // Libera la memoria ocupada por el archivo y sus bloques de datos
bool reservar_bloque(Disco *disco, int posicion, const char *nombre); // Reserva un bloque en el disco para un archivo específico
bool liberar_bloque(Disco *disco, int posicion); // Libera un bloque en el disco, marcándolo como disponible y eliminando cualquier referencia a un archivo
int calcular_bloques(int tamano_kb, int tamano_bloque_kb); // Calcula el número de bloques necesarios para almacenar un archivo de un tamaño dado, considerando el tamaño de bloque especificado

#endif