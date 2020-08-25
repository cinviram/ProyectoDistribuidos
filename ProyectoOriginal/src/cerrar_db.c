#include "logdb.h"
#include "csapp.h"
#include "validaciones.h"

void cerrar_db(conexionlogdb *conexion){
	if(conexion==NULL){
		return;
	}
	
	char accion[MAXLINE]= "ce ";
	strcat(accion,conexion->nombredb);
	strcat(accion,"\n");

	
	rio_writen(conexion->sockdf, accion, strlen(accion));
	
	free(conexion->ip);
	free(conexion->nombredb);
	close(conexion->sockdf);
	
}