#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#include "logdb.h"
#include <stdio.h>
#include <stdlib.h>
#define tamanoLineasBase 150
//Cambiar en base al tamaño de elementos de base
#define totalElementosBase 3000
//Constante a cambiar para hacer gets
#define totalGets 3000
#define inicioFalla 100

int obtenerArreglosBase(char *archivoBases,char resultado[totalElementosBase][tamanoLineasBase],int lineas);
int putElementosBase(conexionlogdb *conexion,char claves[totalElementosBase][tamanoLineasBase],char valores[totalElementosBase][tamanoLineasBase],int totalElementos);
unsigned long hash_function(char* str);
unsigned long hash(unsigned char *str);
int sharding(conexionlogdb *conexion, int *puertos,char claves[totalElementosBase][tamanoLineasBase],char valores[totalElementosBase][tamanoLineasBase],int numeroParticiones);
int obtenerArregloGet(char *archivoGets, char resultado[totalGets][tamanoLineasBase],int lineas);
int getElementosBase(int *puertos,char clavesget[totalGets][tamanoLineasBase],int totalElementosGets,int numeroParticiones);
double tiempo();
int escribirTiemposResultados(char *nombreArchivo,int cantidadGets,double tiempo);

conexionlogdb *con;
conexionlogdb *con1;
conexionlogdb *con2;
conexionlogdb *con3;
conexionlogdb *con4;

int main(int argc, char **argv){

	con= conectar_db("127.0.0.1", 5555);

	printf("%d\n",crear_db(con,"baseDatosPrincipal"));

	printf("%d\n",abrir_db(con,"baseDatosPrincipal"));

	con4= conectar_db("127.0.0.1", 9999);

	crear_db(con4,"baseDatosPrincipalResplado");
	abrir_db(con4,"baseDatosPrincipal");

	//printf("%d\n",crear_db(con,"baseDatosPrincipal2"));

	//printf("%d\n",abrir_db(con,"baseDatosPrincipal2"));

	//printf("%d\n",put_val(con,"hola","mundo"));

	char claves[totalElementosBase][tamanoLineasBase];
	char valores[totalElementosBase][tamanoLineasBase];

	obtenerArreglosBase("./base/claves.txt",claves,tamanoLineasBase);
	obtenerArreglosBase("./base/valores.txt",valores,tamanoLineasBase);

	putElementosBase(con,claves,valores,totalElementosBase);
	putElementosBase(con4,claves,valores,totalElementosBase);

	//No borrar
	printf("%s\n",get_val(con, "Mord"));


	int numeroParticiones=4;
	int puertos[4] ={5555,8888,7777,4444};
	sharding(con,puertos,claves,valores,numeroParticiones);

	compactar(con);

	char gets[totalGets][tamanoLineasBase];
	obtenerArregloGet("./base/gets.txt",gets,tamanoLineasBase);

	//printf("%s\n",get_val(con1, "Ola"));
	//printf("%s\n",get_val(con, "Ralph"));
	//printf("%s\n",get_val(con2, "Darb"));
	//printf("%s\n",get_val(con3, "Alli"));

	getElementosBase(puertos,gets,totalGets,numeroParticiones);

	//Cerrar conexión
	//cerrar_db(con3);

	cerrar_db(con);
	cerrar_db(con1);
	cerrar_db(con2);
	cerrar_db(con3);



	return 0;
}


int putElementosBase(conexionlogdb *conexion,char claves[totalElementosBase][tamanoLineasBase],char valores[totalElementosBase][tamanoLineasBase],int totalElementos){
	for(int i=0;i<totalElementos;i++){
		put_val(conexion,claves[i],valores[i]);
	}
	return 0;
}

int getElementosBase(int *puertos,char clavesget[totalGets][tamanoLineasBase],int totalElementosGets,int numeroParticiones){
	double tiempoInicial=tiempo();
	for(int i=0;i<totalElementosGets;i++){
		unsigned long prueba = hash( (unsigned char*) clavesget[i]);
		unsigned long prueba2= prueba%numeroParticiones;
		if(i==inicioFalla){
			cerrar_db(con3);
			crear_db(con,"baseDatosPrincipal2");
			abrir_db(con,"baseDatosPrincipal2");
			crear_db(con1,"particion1.2");
			abrir_db(con1,"particion1.2");
			crear_db(con2,"particion2.2");
			abrir_db(con2,"particion2.2");

		}
		else{
			if(prueba2==0){
			//printf("Get particion 0: %s\n",get_val(con,clavesget[i]));
			get_val(con,clavesget[i]);
			}
			if(prueba2==1){
				//printf("Get particion 1: %s\n",get_val(con1,clavesget[i]));
				get_val(con1,clavesget[i]);
			}
			if(prueba2==2){
				//printf("Get particion 2: %s\n",get_val(con2,clavesget[i]));
				get_val(con2,clavesget[i]);
			}
			if(prueba2==3 & i<inicioFalla){
				//printf("Get particion 3: %s\n",get_val(con3,clavesget[i]));
				get_val(con3,clavesget[i]);
			}
		}
	}
	double tiempoFinal=tiempo();
	double tiempoResultante= tiempoFinal-tiempoInicial;
	printf("%f \n",tiempoResultante);
	//Descomentar al efectuar pruebas en primer ambiente de pruebas
	//escribirTiemposResultados("./base/tiemposAmbientePruebas1.txt",totalGets,tiempoResultante);
	//Descomentar para efectuar pruebas en segundo ambiente de pruebas
	//escribirTiemposResultados("./base/tiemposAmbientePruebas2.txt",totalGets,tiempoResultante);
	return 0;
}

int obtenerArreglosBase(char *archivoBases,char resultado[totalElementosBase][tamanoLineasBase],int lineas){
    FILE *fp;
	int temporal=0;
    fp = fopen(archivoBases, "r");
    if (fp == NULL)
    {
        printf("Error al abrir el archivo \n");
        return 0;
    }

    while(fgets(resultado[temporal], lineas, fp)) 
	{
        resultado[temporal][strlen(resultado[temporal]) - 1] = '\0';
        temporal++;
    }

    fclose(fp);
    return 0;
}

int obtenerArregloGet(char *archivoGets, char resultado[totalGets][tamanoLineasBase],int lineas){
	FILE *fp;
	int temporal=0;
    fp = fopen(archivoGets, "r");

    if (fp == NULL)
    {
        printf("Error al abrir el archivo \n");
        return 0;
    }

    while(fgets(resultado[temporal], lineas, fp)) 
	{
        resultado[temporal][strlen(resultado[temporal]) - 1] = '\0';
        temporal++;
    }

    fclose(fp);
    return 0;
}

int sharding(conexionlogdb *conexion, int *puertos,char claves[totalElementosBase][tamanoLineasBase],char valores[totalElementosBase][tamanoLineasBase],int numeroParticiones){
	con1=conectar_db("127.0.0.1", puertos[1]);
	con2= conectar_db("127.0.0.1", puertos[2]);
	con3=conectar_db("127.0.0.1", puertos[3]);

	crear_db(con1,"particion1");
	crear_db(con2,"particion2");
	crear_db(con3,"particion3");
	
	abrir_db(con1,"particion1");
	abrir_db(con2,"particion2");
	abrir_db(con3,"particion3");

	for(int i = 0;i<totalElementosBase;i++){
		unsigned long prueba = hash( (unsigned char*) claves[i]);
		unsigned long prueba2= prueba%numeroParticiones;
			if(prueba2==1){
				put_val(con1,claves[i],valores[i]);
				eliminar(conexion,claves[i]);
			}
			if(prueba2==2){
				put_val(con2,claves[i],valores[i]);
				eliminar(conexion,claves[i]);
			}
			if(prueba2==3){
				put_val(con3,claves[i],valores[i]);
				eliminar(conexion,claves[i]);
			}
		}
	return 0;
}

double tiempo(){
	struct timespec tsp;

	clock_gettime(CLOCK_REALTIME, &tsp);

	double secs = (double)tsp.tv_sec;
	double nano = (double)tsp.tv_nsec / 1000000000.0;

	return secs + nano;
}

int escribirTiemposResultados(char *nombreArchivo,int cantidadGets,double tiempo){
	int fd;
	fd=open(nombreArchivo,O_CREAT|O_RDWR|O_APPEND,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH );
    if(fd < 0) {
		printf("Error al crear el archivo puede que ya exista un archivo con ese nombre o archivo no existe en el directorio especificado, ingrese un archivo valido\n");
    	return 0;
	}

	char temp1[5]={0};
    char temp2[10]={0};
    char escribir[20]={0};
    sprintf(temp1, "%d", cantidadGets); 
    sprintf(temp2, "%f", tiempo);
    strcat(escribir,temp1);
    strcat(escribir,",");
    strcat(escribir,temp2);
    strcat(escribir,"\n");
    write(fd,escribir,strlen(escribir));
	return 0;
}

