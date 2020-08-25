#include "hashtable.h"

void **valores(hashtable *tabla, int *conteo)          //Devuelve un arreglo de punteros con todos los valores dentro de la hash table
{
    if(tabla == NULL)return NULL;
    void **valores = malloc(sizeof(void*) * tabla->elementos);
    objeto * cubeta;
    *conteo = 0;
    for(int i = 0; i < tabla->numeroBuckets; i++ ) {    
        for(cubeta = tabla->buckets[i]; cubeta != NULL; cubeta = cubeta->siguiente){
            valores[*conteo] = cubeta->valor;
            *conteo = *conteo + 1;
        }
    }   

    return valores;
}