#include "hashtable.h"


char **claves(hashtable *tabla, int *conteo){

    if(tabla == NULL)return NULL;
    char **claves = malloc(sizeof(char*) * tabla->elementos);
    objeto * cubeta;

    *conteo = 0;
    for(int i = 0; i < tabla->numeroBuckets; i++ ) {
        for(cubeta = tabla->buckets[i]; cubeta != NULL; cubeta = cubeta->siguiente){
            claves[*conteo] = cubeta->clave;
            *conteo = *conteo + 1;
        }
    }

    
    return claves;

}