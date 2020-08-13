#include "logdb.h"

int main(int argc, char **argv){
	conexionlogdb *con = conectar_db("127.0.0.1",5555);

	conexionlogdb *con1 = conectar_db("127.0.0.1",8888);

	conexionlogdb *con2 = conectar_db("127.0.0.1",7777);

	printf("%d\n",crear_db(con,"prueba_proyecto"));
	printf("%d\n",crear_db(con1,"prueba_proyecto1"));
	printf("%d\n",crear_db(con2,"prueba_proyecto2"));

	printf("%d\n",abrir_db(con,"prueba_proyecto"));
	printf("%d\n",abrir_db(con1,"prueba_proyecto1"));
	printf("%d\n",abrir_db(con2,"prueba_proyecto2"));

	printf("%d\n",put_val(con, "materia", "promedio"));	
	printf("%d\n",put_val(con1, "universidad", "espol"));	
	printf("%d\n",put_val(con2, "comida", "pizza"));	


	printf("%d\n",put_val(con, "musica", "instrumento"));	
	printf("%d\n",put_val(con1, "musica", "guitarra"));	
	printf("%d\n",put_val(con2,"util","cuaderno"));	
	

	char *valor = get_val(con, "materia");
	if(valor==NULL){
		printf("%s\n","null");
	}
	else{
		printf("%s\n",valor);
	}

	valor = get_val(con, "musica");
	if(valor==NULL){
		printf("%s\n","comida");
	}
	else{
		printf("%s\n",valor);
	}
	compactar(con);
	
	printf("%d\n",put_val(con, "teclado", "periferico"));	
	printf("%d\n",put_val(con1, "cpu", "componente"));	
	printf("%d\n",put_val(con2, "mouse", "periferico"));
	
	printf("%s\n",get_val(con, "teclado"));	
	printf("%s\n",get_val(con, "musica"));	
	printf("%s\n",get_val(con2, "mouse"));	
	printf("%s\n",get_val(con1, "cpu"));	

	//Pruebas 12 Agosto 2020
	printf("%d\n",put_val(con, "toxica", "cindy"));
	printf("%d\n",put_val(con1, "softboy", "jeremy"));
	printf("%d\n",put_val(con2, "fuckboy", "josue"));

	printf("%s\n",get_val(con, "toxica"));	
	printf("%s\n",get_val(con2, "fuckboy"));	
	printf("%s\n",get_val(con1, "softboy"));

	compactar(con);
	compactar(con1);
	compactar(con2);

	cerrar_db(con);
	cerrar_db(con1);
	cerrar_db(con2);

	return 0;
}