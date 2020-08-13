#include "hashtable.h"
#include <math.h>

void *remover(hashtable *tabla, char *clave){
    if(tabla == NULL || clave == NULL)return NULL; 

    long hashed=0;
    
    hashed=(long)hash((unsigned char*)clave);
    hashed=(long)fabs(hashed);
    hashed= (long)(hashed % (tabla->numeroBuckets));


    objeto *cubeta = tabla->buckets[hashed];
    if(cubeta == NULL)return NULL;

    if(strcmp(clave,cubeta->clave) == 0){//si es el primer objeto
        tabla->buckets[hashed] = cubeta->siguiente;
        cubeta->siguiente = NULL;
        tabla->elementos = tabla->elementos - 1;
        return cubeta->valor;
    }


    objeto *cubetant;
    for(cubeta = tabla->buckets[hashed]; cubeta != NULL; cubeta = cubeta->siguiente){
        if(strcmp(clave,cubeta->clave) == 0){
            if(cubeta->siguiente==NULL){//si existe y es el ultimo 
                cubetant->siguiente = NULL;
                tabla->elementos = tabla->elementos - 1;
                return cubeta->valor;
            }else{ // si no es el primero, ni el ultimo, y coincidio la clave, esta entre. 
                cubetant->siguiente = cubeta->siguiente;
                cubeta->siguiente = NULL;
                tabla->elementos = tabla->elementos - 1;
                return cubeta->valor;
            }

        }
       cubetant = cubeta; 
    }   


return NULL;

}
