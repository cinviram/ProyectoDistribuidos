#include "logdb.h"

int main(int argc, char **argv){
	conexionlogdb *con = conectar_db("127.0.0.1",5555);

	printf("%d\n",crear_db(con,"prueba_proyecto"));

	printf("%d\n",abrir_db(con,"prueba_proyecto"));

	printf("%d\n",put_val(con, "materia", "promedio"));	
	printf("%d\n",put_val(con, "universidad", "espol"));	
	printf("%d\n",put_val(con, "comida", "pizza"));	


	printf("%d\n",put_val(con, "musica", "instrumento"));	
	printf("%d\n",put_val(con, "musica", "guitarra"));	
	printf("%d\n",eliminar(con,"comida"));	
	

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
	printf("%d\n",put_val(con, "cpu", "componente"));	
	printf("%d\n",put_val(con, "mouse", "periferico"));
	
	printf("%s\n",get_val(con, "teclado"));	
	printf("%s\n",get_val(con, "musica"));	
	printf("%s\n",get_val(con, "mouse"));	
	printf("%s\n",get_val(con, "cpu"));	

	cerrar_db(con);

	return 0;
}