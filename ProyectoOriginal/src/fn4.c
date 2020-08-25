#include "hashtable.h"
#include <math.h>

void *get(hashtable *tabla, char *clave){

    if(tabla == NULL || clave == NULL)return NULL; 

    long hashed=0;
    
    hashed=(long)hash((unsigned char*)clave);
    hashed=(long)fabs(hashed);

    hashed = (long)(hashed % (tabla->numeroBuckets));
    
    objeto *cubeta;
    for(cubeta = tabla->buckets[hashed]; cubeta != NULL; cubeta = cubeta->siguiente){
        if(strcmp(clave,cubeta->clave) == 0)return cubeta->valor;
    }

    return NULL;

}
