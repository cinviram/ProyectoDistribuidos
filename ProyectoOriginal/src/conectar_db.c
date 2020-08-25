#include "logdb.h"
#include <sys/types.h>          /* some systems still require this */
#include <sys/stat.h>
#include <signal.h>             /* for SIG_ERR */ 
#include <netdb.h> 
#include <errno.h> 
#include <syslog.h> 
#include <sys/socket.h> 
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/resource.h>

#define MAXSLEEP 64

int semilla = 0;
static int connect_retry( int domain, int type, int protocol, 	const struct sockaddr *addr, socklen_t alen){
	
	int numsec, fd; /* * Try to connect with exponential backoff. */ 

	for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1) { 

		if (( fd = socket( domain, type, protocol)) < 0) 
			return -1; 

		if (connect( fd, addr, alen) == 0) { /* * Conexión aceptada. */ 
			return(fd); 
		} 
		close(fd);

		if (numsec <= MAXSLEEP/2)
			sleep( numsec); 
	} 
	return -1; 
}



conexionlogdb * conectar_db(char *ip, int puerto){
	if(ip==NULL || puerto<0){
		return NULL;
	}
	struct sockaddr_in direccion_cliente;

	memset(&direccion_cliente, 0, sizeof(direccion_cliente));

	direccion_cliente.sin_family = AF_INET;
	direccion_cliente.sin_port = htons(puerto);		
	direccion_cliente.sin_addr.s_addr = inet_addr(ip) ;
	int sockfd;
	if (( sockfd = connect_retry( direccion_cliente.sin_family, SOCK_STREAM, 0, (struct sockaddr *)&direccion_cliente, sizeof(direccion_cliente))) < 0) { 
		return NULL;
	} 
	conexionlogdb *conexion = (conexionlogdb *) malloc(sizeof(conexionlogdb));
	conexion->ip= strdup(ip);
	conexion->puerto=puerto;
	conexion->sockdf=sockfd;
	if(!semilla){
		srand(time(NULL));
	}
	conexion->id_sesion=rand();
	conexion->nombredb=NULL;
	
	return conexion;
}