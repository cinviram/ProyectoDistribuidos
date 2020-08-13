#include "servidor.h"
#include "mod_indice.h"
#include "hashtable.h"


int crear_base_servidor(char *archivo){
	
	int fd = open(archivo, O_RDONLY, 0);
	if(fd>0){
		close(fd);
		return 0;
	}

	fd = open(archivo,O_WRONLY | O_CREAT, DEF_MODE);
	if(fd>0){

		close(fd);
		return 1;
	}
	
	return 0;
}

base_servidor *abrir_base_servidor(char *archivo_base, char *indice_base){
	
	
	hashtable *ht = cargar_indices(indice_base);
	if(ht==NULL){
		return NULL;
	}
	
	FILE *f = fopen(archivo_base, "a+");
	if(f==NULL){

		return NULL;
	}
	
	base_servidor *db = (base_servidor *)malloc(sizeof(base_servidor));
	db->nombre_base = strdup(archivo_base);
	if(db->nombre_base == NULL ){
		return NULL;
	}
	db->indice_base = strdup(indice_base);
	if(db->indice_base==NULL){
		return NULL;
	}
	db->ht=ht;
	db->f=f;
	db->ht_acumuladora = NULL;
    db->ht_temporal = NULL;
    db->nuevo_archivo = NULL;
	db->compactado = 1;
	sem_init(&db->semaforo,0,1);

    
	return db;
}

int put_servidor(base_servidor *db, char *clave, char *valor){
	if(db==NULL || !validar_char(clave)){

		return 0;
	}
	char linea[MAXLINE];
	sprintf(linea, "%s:%s\n", clave, valor);
	if(db->ht_acumuladora!=NULL){
		P(&db->semaforo);

		agregar_indice_clave(db->f, db->ht_acumuladora, clave);
		fputs(linea, db->f);
		V(&db->semaforo);
		return 1;
		
	}
	agregar_indice_clave(db->f, db->ht, clave); //para la hashtable
	fputs(linea, db->f);
	return 1;
	
}

char *get_servidor(base_servidor *db, char *clave){
	if(db==NULL || !validar_char(clave)){
		
		return NULL;
	}

	
	if(db->ht_acumuladora!=NULL){

		P(&db->semaforo);
		
		char *valor = get_indice_archivo(db->f, db->ht_acumuladora, clave);
		if(valor == NULL){

			valor = get_indice_archivo(db->f, db->ht, clave);
		}
		V(&db->semaforo);
		return valor;
	}
	return get_indice_archivo(db->f, db->ht, clave);

}


int cerrar_conexion_servidor(base_servidor *db){
	if(db==NULL){
		return 0;
	}
	if(db->ht_acumuladora==NULL){
		
		guardar_indices(db->indice_base, db->ht);
		return 1;
	}
	
	while(!db->compactado);
	fin_compactar(db);
	guardar_indices(db->indice_base, db->ht);
	return 1;
}
