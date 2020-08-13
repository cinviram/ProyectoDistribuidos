#include "hashtable.h"
#include <math.h>
void put(hashtable *tabla, char *clave, void *valor){
    long hashed=0;
    if(tabla==NULL || clave == NULL)return;

    hashed=(long)hash((unsigned char*)clave);
    hashed=(long)fabs(hashed);


    hashed = (long)(hashed % (tabla->numeroBuckets));
    
    objeto *newcubeta = malloc(sizeof(objeto));
    newcubeta->clave = clave;
    newcubeta->valor = valor;
    newcubeta->siguiente=NULL;


    //obtenemos la cubeta correspondiente
    objeto *cubeta;
    cubeta = tabla->buckets[hashed];

    if(cubeta == NULL){
        tabla->buckets[hashed] = newcubeta;
        tabla->elementos = tabla->elementos + 1;
    }else{

        while(cubeta->siguiente != NULL){
            if(strcmp(cubeta->clave, clave)==0){
                cubeta->valor = valor;
                return;
            }
            cubeta = cubeta->siguiente;

        }

        if(strcmp(cubeta->clave, clave)==0){
                cubeta->valor = valor;
                return;
        }


        cubeta->siguiente = newcubeta;
        tabla->elementos = tabla->elementos + 1;

    }


}
