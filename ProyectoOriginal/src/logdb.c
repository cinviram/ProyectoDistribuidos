#include "servidor.h"

hashtable *bases_abiertas;
hashtable *bases_archivos;
char *directorio;
sem_t semaforo_bases;
sem_t semaforo_archivo;
#define CONFIGURACION "configuracion.txt"

hashtable *cargar_direcciones(char *archivo_configuracion){
	hashtable *ht = crearHashTable(TAMANO_HT);
	char linea[MAXLINE];
	char *split;
	FILE *f = fopen(archivo_configuracion,"r");
	if(f==NULL){
		return ht;
	}
	
	
	
	while(fgets(linea, MAXLINE,f)!=NULL){
		split = strchr(linea,':');
		int n = (split-linea);
		reemplazar(linea,'\n','\0');
		reemplazar(linea, *split, '\0');
		char *arch = strdup(linea+n+1);
		put(ht,strdup(linea),strdup(arch));

	}
	return ht;
}

void agregar_direccion_base(char *archivo_configuracion, char *nombre, char* directorio){
	FILE *f = fopen(archivo_configuracion,"a");
	char linea[MAXLINE];
	if(f==NULL){
		return;
	}
	sprintf(linea,"%s:%s\n",nombre,directorio);
	fputs(linea,f);
	fclose(f);
}
//Funcion para inicializar el servidor
int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen){

	int fd;
	int err = 0;
	
	if((fd = socket(addr->sa_family, type, 0)) < 0)
		return -1;

	if(bind(fd, addr, alen) < 0)
		goto errout;

	if(type == SOCK_STREAM || type == SOCK_SEQPACKET){
		
		if(listen(fd, qlen) < 0)
			goto errout;
	}
	return fd;
	errout:
	err = errno;
	close(fd);
	errno = err;
	return (-1);
}

void verificar_compactacion(base_servidor *db){
	if(db==NULL){
		return;
	}

	if(db->compactado && db->ht_acumuladora != NULL){
		fin_compactar(db);
	}
}

char *extraer_valor(char *buffer){
	char *valor = strchr(buffer,' ');
	int n = (valor-buffer);
	valor = buffer+n;
	valor++;
	return valor;
}

void extraer_accion(char *buffer){
	char *accion = strchr(buffer,' ');
	*accion = '\0';
}

int verificar_accion(char *buffer, char *accion){
	return strcmp(buffer,accion)==0;
}

void procesar_crear_base(char *valor, int cliente){
	int respuesta = 0;
	char dir[MAXLINE];
	sprintf(dir,"%s/%s_log",directorio,valor);

	sem_wait(&semaforo_archivo);

	int esta_creada = contieneClave(bases_archivos,valor);

	sem_post(&semaforo_archivo);
	if(!esta_creada){
		sem_wait(&semaforo_bases);
		int esta_abierta = contieneClave(bases_abiertas,valor);
		sem_post(&semaforo_bases);
		if(!esta_abierta){
			respuesta = crear_base_servidor(dir);
			if(respuesta){
				sem_wait(&semaforo_archivo);
				agregar_direccion_base(CONFIGURACION, valor,dir);
				put(bases_archivos,strdup(valor),strdup(dir));
				sem_post(&semaforo_archivo);
			}
		}
	}

	rio_writen(cliente, &respuesta,sizeof(int));
}

base_servidor *procesar_abrir_base(char *valor, int cliente){

	int respuesta = 0;
	char tmp[MAXLINE];
	sprintf(tmp, "%s/%s_log",directorio,valor);
	
	base_servidor *db= NULL;
	char ind[MAXLINE];
	sprintf(ind,"%s/%s_indice",directorio,valor);
	sem_wait(&semaforo_archivo);
	int esta_creada = contieneClave(bases_archivos,valor);
	sem_post(&semaforo_archivo);
	if(esta_creada){
		sem_wait(&semaforo_bases);
		int esta_abierta =contieneClave(bases_abiertas,valor);
		sem_post(&semaforo_bases);
		if(!esta_abierta){
			db = abrir_base_servidor(tmp,ind);
			if(db!=NULL){
				respuesta=1;
				sem_wait(&semaforo_bases);
				put(bases_abiertas,strdup(tmp),db);
				sem_post(&semaforo_bases);
			}else{
				free(db);
				db=NULL;
			}
		}
		

	}
	rio_writen(cliente, &respuesta,sizeof(int));
	return db;
}

void procesar_get(base_servidor *db, char *buffer, int cliente){

	char *valor = NULL;
	char mensaje[MAXLINE];
	if(db==NULL){
		sprintf(mensaje, "@T!\n");
	}else{

		valor = get_servidor(db, buffer);

		if(valor==NULL){
			sprintf(mensaje, "@T!\n");
		}else{
			sprintf(mensaje, "%s\n",valor);
			
		}
	}
	if(valor!=NULL){

		free(valor);
	}
	rio_writen(cliente,mensaje,strlen(mensaje));

}

void procesar_put(base_servidor *db, char *valor, int cliente){
	int respuesta = 0;
	if(db!=NULL){
		char *split= strchr(valor,':');
		int index = (split-valor);
		valor[index] = '\0';
		respuesta=put_servidor(db,valor,valor+index+1);
	}
	rio_writen(cliente,&respuesta,sizeof(int));	
}

void procesar_cerrar(base_servidor *db){
	if(db!=NULL){
		cerrar_conexion_servidor(db);
		sem_wait(&semaforo_bases);
		remover(bases_abiertas,db->nombre_base);
		sem_post(&semaforo_bases);
		fclose(db->f);
		borrar(db->ht);
		free(db);

		
	}
}

void *atender(void *arg){
	int *conexion = (int *)arg;
	if(conexion == NULL){
		return NULL;
	}
	char buffer[MAXLINE];
	int sfd = *conexion;
	int n;

	base_servidor *db = NULL;
	
	
	rio_t rio;
	
	rio_readinitb(&rio, sfd);
	char *valor;
	while((n = rio_readlineb(&rio, buffer, MAXLINE)) > 0) {
		reemplazar(buffer,'\n', '\0');
		valor = extraer_valor(buffer);
		extraer_accion(buffer);
		verificar_compactacion(db);
		
		
		if(verificar_accion(buffer,"c")){
			procesar_crear_base(valor,sfd);
		}else if(verificar_accion(buffer,"a")){
			db = procesar_abrir_base(valor,sfd);
	
		}else if(verificar_accion(buffer,"g")){
			procesar_get(db, valor, sfd);

		}else if(verificar_accion(buffer,"p")){
			procesar_put(db,valor,sfd);
		}else if(verificar_accion(buffer,"el")){
			char elim[MAXLINE];
			sprintf(elim,"%s:@T!",valor);
			procesar_put(db,elim,sfd);

		}else if(verificar_accion(buffer,"ce")){
			procesar_cerrar(db);
			db=NULL;
		}else if(verificar_accion(buffer,"co")){
			if(db!=NULL){
				compactar_servidor(db);
			}
		}


	}

	if(db!=NULL){
		procesar_cerrar(db);
		db=NULL;
	}
	close(sfd);
	free(conexion);

	return NULL;

}
int main(int argc, char **argv){
	if(argc<4){
		printf("Faltan argumentos, directorio ip puerto\n");
		return -1;
	}
	
	directorio = (char * ) malloc(sizeof(char) * strlen(argv[1])+3);
	sprintf(directorio, "%s/",argv[1]);
	
	int puerto = atoi(argv[3]);
	char *ip = argv[2];
	sem_init(&semaforo_bases, 0, 1);
	sem_init(&semaforo_archivo, 0, 1);
	bases_abiertas = crearHashTable(TAMANO_HT);
	bases_archivos = cargar_direcciones(CONFIGURACION);

	 //Direccion del servidor
	struct sockaddr_in direccion_servidor;

	memset(&direccion_servidor, 0, sizeof(direccion_servidor));	//ponemos en 0 la estructura direccion_servidor

	//llenamos los campos
	direccion_servidor.sin_family = AF_INET;		//IPv4
	direccion_servidor.sin_port = htons(puerto);		//Convertimos el numero de puerto al endianness de la red
	direccion_servidor.sin_addr.s_addr = inet_addr(ip) ;	//Nos vinculamos a la interface localhost o podemos usar INADDR_ANY para ligarnos A TODAS las interfaces

	
	int sockfd;
	//inicalizamos servidor (AF_INET + SOCK_STREAM = TCP)
	if( (sockfd = initserver(SOCK_STREAM, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor), 1000)) < 0){	//Hasta 1000 solicitudes en cola 
		printf("Error al inicializar el servidor\n");
		return -1;	
	}		

	while (1) {
		int clfd = accept(sockfd, NULL, NULL);
		int *copia_socket = (int *) malloc(sizeof(int));
		*copia_socket = clfd;
		pthread_t th;
		pthread_create(&th, NULL, atender, copia_socket);
	}
	exit(0);
}