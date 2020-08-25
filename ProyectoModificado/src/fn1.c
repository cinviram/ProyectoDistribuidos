#include "hashtable.h"

hashtable *crearHashTable(int numeroBuckets){

	if( numeroBuckets < 1 ) {
		printf("Tamanio no permitido");
		return NULL;
	}
	
	/* Declaracion de la estructura de tipo hastable*/
	hashtable *ht = malloc(sizeof(hashtable));
		
    /* Asignacion de los punteros  */
	ht->buckets = malloc( sizeof(objeto*) * numeroBuckets);

	for(int i = 0; i < numeroBuckets; i++ ) {
		ht->buckets[i] = NULL;
	}

	ht->elementos = 0;
	ht->numeroBuckets = numeroBuckets;
	ht->id = rand () % (0-1000+1)+0;


	return ht;	
}
