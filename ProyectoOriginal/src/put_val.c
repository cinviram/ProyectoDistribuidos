#include "logdb.h"
#include "csapp.h"
#include "validaciones.h"



int put_val(conexionlogdb *conexion, char *clave, char *valor){
	if(conexion==NULL || !validar_char(valor) || !validar_char(clave)){
		return 0;
	}
	rio_t rio;

    rio_readinitb(&rio, conexion->sockdf);

	char message[MAXLINE] = "p ";
	strcat(message,clave);
	strcat(message,":");
	strcat(message,valor);
	strcat(message,"\n");

	int resultado;
	if(rio_writen(conexion->sockdf, message, strlen(message))>0 && rio_readnb(&rio,&resultado,sizeof(int))>0){
		return resultado;
	}
	return 0;

}