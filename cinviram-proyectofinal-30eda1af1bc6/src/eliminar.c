#include "logdb.h"
#include "csapp.h"
#include "validaciones.h"


int eliminar(conexionlogdb *conexion, char *clave){
	if(conexion == NULL || !validar_char(clave)){
		return 0;
	}

	rio_t rio;

    rio_readinitb(&rio, conexion->sockdf);

	char accion[MAXLINE] = "el ";
	strcat(accion,clave);
	strcat(accion,"\n");

	int resultado;
	if(rio_writen(conexion->sockdf, accion, strlen(accion))>0 && rio_readnb(&rio,&resultado,sizeof(int))>0){
		return resultado;	
	}
	return 0;
}