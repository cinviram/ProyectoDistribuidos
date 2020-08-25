#include "hashtable.h"
#include "servidor.h"
#include "mod_indice.h"


void *thread_compactar(void *arg);
void compactar_servidor(base_servidor *db){

	
	db->ht_acumuladora = crearHashTable(db->ht->numeroBuckets);
	
	char nombres[MAXLINE];
	sprintf(nombres,"%s%s", db->nombre_base, "_compactado");
	FILE *f = fopen(nombres,"a+");
	db->nuevo_archivo = f;
	pthread_t thread;
	db->ht_temporal = crearHashTable(db->ht->numeroBuckets);
	
	db->compactado=0;
	pthread_create(&thread, NULL, thread_compactar, db);
}


void *thread_compactar(void *arg){
	pthread_detach(pthread_self());
	base_servidor *db= (base_servidor *) arg;

	if(db==NULL){
		return NULL;
	}
	
	
	int elementos = 0;
	char **__claves = claves(db->ht,&elementos);
	char *valor;
	char linea[MAXLINE];
	for(int i = 0;i<elementos;i++){
		valor = get_servidor(db,__claves[i]);
		if(valor!=NULL && strcmp(valor,"@T!")!=0){
			sprintf(linea,"%s:%s\n",__claves[i],valor);
			agregar_indice_clave(db->nuevo_archivo, db->ht_temporal, __claves[i]);
			fputs(linea, db->nuevo_archivo);
			
		}
		
	}
	db->compactado=1;
	return NULL;

}

void fin_compactar(base_servidor *db){
	char linea[MAXLINE];
	char *valor;
	int elementos=0;
	char **__claves = claves(db->ht_acumuladora,&elementos);
	for(int i = 0; i < elementos; i++){
		valor = get_servidor(db,__claves[i]);
		
		if(valor!=NULL){
			sprintf(linea,"%s:%s\n",__claves[i],valor);
			agregar_indice_clave(db->nuevo_archivo, db->ht_temporal,__claves[i]);
			fputs(linea, db->nuevo_archivo);
		}

	}
	free(__claves);
	borrar(db->ht);
	borrar(db->ht_acumuladora);
	fclose(db->f);
	fclose(db->nuevo_archivo);

	db->ht = db->ht_temporal;
	db->ht_acumuladora = NULL;
	sprintf(linea, "%s%s",db->nombre_base,"_compactado");
	remove(db->nombre_base);
	rename(linea, db->nombre_base);
	db->f = fopen(db->nombre_base,"a+");
	

}

