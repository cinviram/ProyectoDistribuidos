#include "logdb.h"
#include "csapp.h"
#include "validaciones.h"

int crear_db(conexionlogdb *conexion, char *nombre_db){
	if(conexion==NULL || !validar_char(nombre_db)){
		return 0;
	}
	rio_t rio;

    rio_readinitb(&rio, conexion->sockdf);

	char accion[MAXLINE] = "c ";
	
	strcat(accion,nombre_db);
	strcat(accion,"\n");

	int resultado;
	if(rio_writen(conexion->sockdf, accion, strlen(accion))>0 && rio_readnb(&rio,&resultado,sizeof(int))>0){
			return resultado;
	}
	return 0;

}