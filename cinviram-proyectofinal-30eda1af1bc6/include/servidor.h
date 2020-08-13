#ifndef SERVIDOR_H
#define SERVIDOR_H

#include "validaciones.h"
#include "hashtable.h"
#include "csapp.h"


typedef struct{
    char *nombre_base;
    hashtable *ht;
    char *indice_base;
    FILE *f;
    hashtable *ht_acumuladora;
    hashtable *ht_temporal;
    sem_t semaforo;
    FILE *nuevo_archivo;
    volatile int compactado;
} base_servidor;

//funciones
int crear_base_servidor(char *archivo);

base_servidor *abrir_base_servidor(char *archivo_base, char *indice_base);

int put_servidor(base_servidor *db, char *clave, char *valor);

char *get_servidor(base_servidor *db, char *clave);

int cerrar_conexion_servidor(base_servidor *db);

void compactar_servidor(base_servidor *db);

void fin_compactar(base_servidor *db);


#endif