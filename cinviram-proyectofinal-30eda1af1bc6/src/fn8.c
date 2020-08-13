
#include "hashtable.h"
#include <math.h>


int contieneClave(hashtable *tabla, char *clave)       //Devuelve 1 si la clave existe en la hashtable, 0 si no.
{
    objeto * nuevo;
    long hashed=0;
    hashed=(long)hash((unsigned char*)clave);
    hashed=(long)fabs(hashed);
    hashed= (long)(hashed % (tabla->numeroBuckets));
    nuevo= tabla->buckets[hashed];

    while(nuevo!=NULL){
        if(strcmp(nuevo->clave,clave) == 0){
            return 1;
        }
        nuevo=nuevo->siguiente;
    }

    
    return 0;
}
