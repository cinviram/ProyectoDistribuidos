#include "logdb.h"
#include <stdio.h>
#include <stdlib.h>

int leerLinea(char *nombreArchivo);
int sharding(int salto,conexionlogdb *conexion, int *puertos,int totalParticiones,char claves[8][25],char valores[8][25]);
int obtenerArreglosBase(char *archivoBases,char resultado[8][25],int lineas);
int putElementosBase(conexionlogdb *conexion,char claves[8][25],char valores[8][25],int totalElementos);
conexionlogdb *con;
conexionlogdb *con1;
conexionlogdb *con2;
conexionlogdb *con3;



int main(int argc, char **argv){

	con= conectar_db("127.0.0.1", 5555);

	printf("%d\n",crear_db(con,"baseDatosPrincipal"));

	printf("%d\n",abrir_db(con,"baseDatosPrincipal"));

	//Establecer dimensiones como constantes
	char claves[8][25];
	char valores[8][25];
	int tamanoLinea=25;

	obtenerArreglosBase("./base/claves.txt",claves,tamanoLinea);
	obtenerArreglosBase("./base/valores.txt",valores,tamanoLinea);

	putElementosBase(con,claves,valores,8);

	//for(int i=0;i<8;i++){
	//	printf(" clave %s \n",claves[i]);
	//	printf(" valor %s \n",valores[i]);
	//}

	//printf("%d\n",put_val(con, "materia", "promedio"));	
	//printf("%d\n",put_val(con1, "universidad", "espol"));	
	//printf("%d\n",put_val(con2, "comida", "pizza"));	


	//printf("%d\n",put_val(con, "musica", "instrumento"));	
	//printf("%d\n",put_val(con1, "musica", "guitarra"));	
	//printf("%d\n",put_val(con2,"util","cuaderno"));	
	
	//compactar(con);
	
	//printf("%d\n",put_val(con, "teclado", "periferico"));	
	//printf("%d\n",put_val(con1, "cpu", "componente"));	
	//printf("%d\n",put_val(con2, "mouse", "periferico"));
	
	//printf("%s\n",get_val(con, "teclado"));

	//Pruebas 12 Agosto 2020
	//printf("%d\n",put_val(con, "hola", "distribuidos"));
	//printf("%d\n",put_val(con, "toxica", "cindy"));
	//printf("%d\n",put_val(con, "libro", "ciencias"));
	//printf("%d\n",put_val(con, "cuaderno", "sociales"));
	//printf("%d\n",put_val(con, "pluma", "roja"));

	//printf("%d\n",put_val(con1, "softboy", "jeremy"));
	//printf("%d\n",put_val(con2, "fuckboy", "josue"));

	printf("%s\n",get_val(con, "hola"));	
	//printf("%s\n",get_val(con2, "fuckboy"));	
	//printf("%s\n",get_val(con1, "softboy"));

	int totalElementos = leerLinea("./prueba/baseDatosPrincipal_log");

	printf("%d \n",totalElementos);
	int numeroParticiones=4;
	int puertos[4] ={5555,8888,7777,4444};
	int salto=totalElementos/numeroParticiones;
	printf("%d \n",salto);
	sharding(salto,con,puertos,numeroParticiones,claves,valores);

	compactar(con);
	//compactar(con1);
	//compactar(con2);
	//compactar(con3);

	printf("%s\n",get_val(con1, "hola"));
	printf("%s\n",get_val(con, "materia"));
	printf("%s\n",get_val(con2, "libro"));
	printf("%s\n",get_val(con3, "pluma"));

	cerrar_db(con);
	cerrar_db(con1);
	cerrar_db(con2);
	cerrar_db(con3);


	return 0;
}
int leerLinea(char *nombreArchivo)
{
    int totalLineas = 0;
    FILE *fp;
    char *linea = NULL;
    size_t longitud = 0;
    fp = fopen(nombreArchivo, "r");
    if (fp == NULL)
    {
        return 0;
    }
    while ((getline(&linea, &longitud, fp)) != -1)
    {
		//printf("%s",linea);
        totalLineas++;
    }
    fclose(fp);
    return totalLineas;
}

int putElementosBase(conexionlogdb *conexion,char claves[8][25],char valores[8][25],int totalElementos){
	for(int i=0;i<totalElementos;i++){
		put_val(conexion,claves[i],valores[i]);
	}
	return 0;
}

int obtenerArreglosBase(char *archivoBases,char resultado[8][25],int lineas){
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

int sharding(int salto,conexionlogdb *conexion, int *puertos,int totalParticiones,char claves[8][25],char valores[8][25]){
	con1=conectar_db("127.0.0.1", puertos[1]);
	con2= conectar_db("127.0.0.1", puertos[2]);
	con3=conectar_db("127.0.0.1", puertos[3]);

	crear_db(con1,"particion1");
	crear_db(con2,"particion2");
	crear_db(con3,"particion3");
	
	abrir_db(con1,"particion1");
	abrir_db(con2,"particion2");
	abrir_db(con3,"particion3");

	for(int i = 1;i<totalParticiones;i++){
		int tope=i+1;
		for(int j = salto*i;j<salto*tope;j++){
			if(i==1){
				printf("Hola Mundo1\n");
				put_val(con1,claves[j],valores[j]);
				eliminar(conexion,claves[j]);
			}
			if(i==2){
				printf("Hola Mundo2\n");
				put_val(con2,claves[j],valores[j]);
				eliminar(conexion,claves[j]);
			}
			if(i==3){
				printf("Hola Mundo3\n");
				put_val(con3,claves[j],valores[j]);
				eliminar(conexion,claves[j]);
			}
		}
	}
	return 0;
}