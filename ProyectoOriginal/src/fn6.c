#include "hashtable.h"


void borrar(hashtable *tabla){
    if(tabla == NULL)return;
    int cant1;
    char **lista_claves = claves(tabla, &cant1);
    char *removido;

    for(int i = 0; i<cant1;i++){
        removido = (char *) remover(tabla,lista_claves[i]);
        free(removido);
    }


}