#include "logdb.h"
#include "csapp.h"
#include "validaciones.h"

char *get_val(conexionlogdb *conexion, char *clave){
	if(conexion == NULL || !validar_char(clave)){
		return NULL;
	}

	rio_t rio;

    rio_readinitb(&rio, conexion->sockdf);

	char accion[MAXIMO+3]= "g ";
	strcat(accion,clave);
	strcat(accion,"\n");
	if(rio_writen(conexion->sockdf, accion, strlen(accion))>0 && rio_readlineb(&rio,accion,MAXIMO+1)>0){
		reemplazar(accion,'\n', '\0');
		if(strcmp(accion,"@T!")==0){

			return NULL;
		}
		return strdup(accion);
					
	}
	return NULL;
}