#include "logdb.h"
#include "csapp.h"
#include "validaciones.h"

void compactar(conexionlogdb *conexion){
	if(conexion==NULL){
		return;
	}


	char accion[MAXLINE] = "co ";
	
	strcat(accion,conexion->nombredb);
	strcat(accion,"\n");	
	rio_writen(conexion->sockdf, accion, strlen(accion));

}