#include "mod_indice.h"

void agregar_indice_clave(FILE *f, hashtable *ht, char *clave){
	if( f == NULL|| ht==NULL || !validar_char(clave) ){
		return;
	}
	if(fseek(f, 0, SEEK_END)!=0){
		return;
	}
	int *posicion = get(ht,clave);
	int nuevo_indice=ftell(f);
	if(posicion!=NULL){
		*posicion=nuevo_indice;
	}else{
		posicion = (int *) malloc(sizeof(int));
		*posicion = nuevo_indice;
		put(ht, strdup(clave), posicion);
	}

	
	
}

char *get_indice_archivo(FILE *f, hashtable *ht, char* clave){
	if( f == NULL|| ht==NULL || !validar_char(clave) ){
		return NULL;
	}
	int *valor=(int * ) get(ht, clave);
	if(valor == NULL){
		return NULL;
	}
	fseek(f, *valor, SEEK_SET);
	char linea[MAXLINE];
	fgets(linea, MAXLINE, f);
	
	reemplazar(linea,'\n','\0');
	char *separador=strchr(linea, ':');
	separador++;

	return strdup(separador);
}

void guardar_indices(char *archivo, hashtable *ht){
	if(archivo == NULL || ht==NULL)
		return;
	FILE *f=fopen(archivo, "w");
	if(f==NULL){
		return;
	}
	int elementos = 0;
	char **__claves = claves(ht, &elementos);
	char linea[MAXLINE];
	int *valor;

	for(int i=0; i<elementos; i++){
		valor = (int *)get(ht,__claves[i]);
		sprintf(linea, "%s:%d\n", __claves[i], *valor);

		fputs(linea, f);
	}
	fclose(f);
}

hashtable *cargar_indices(char *archivo){
	hashtable *ht=crearHashTable(TAMANO_HT);
	if(archivo == NULL ||ht==NULL){

		return NULL;
	}
	FILE *fp=fopen(archivo, "r");
	if(fp==NULL){

		return ht;
	}

	char linea[MAXLINE];
	int *indice;	
	while(fgets(linea, MAXLINE, fp)!=NULL){
		reemplazar(linea,'\n', '\0');
		char *split=strchr(linea, ':');
		*split='\0';
		split++;
		indice = (int * ) malloc(sizeof(int));
		*indice=atoi(split);

		put(ht, strdup(linea), indice);
	}
	return ht;
}

