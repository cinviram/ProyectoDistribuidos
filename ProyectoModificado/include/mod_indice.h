#ifndef MODINDICE_H
#define MODINDICE_H

#include "hashtable.h"
#include "validaciones.h"
#include "csapp.h"
#include <stdio.h>

char *get_indice_archivo(FILE *f, hashtable *ht, char* clave);
void agregar_indice_clave(FILE *f, hashtable *ht, char *clave);
void guardar_indices(char *archivo, hashtable *ht);
hashtable *cargar_indices(char *archivo);
#endif