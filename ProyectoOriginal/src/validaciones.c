#include "validaciones.h"
#include <string.h>
#include <ctype.h>


int validar_char(char *s){
	if(s==NULL){
		return 0;
	}
	int tamano = strlen(s);
	if(tamano==0 || tamano>=MAXIMO){
		return 0;
	}
	if(strcmp(s,"@T!")==0){
		return 0;
	}


	while(*s!='\0'){
		if(*s==':'){
			return 0;
		}
		s++;
	}
	return 1;
}
void reemplazar(char *s, char c, char nuevo_c){
	while(*s){
		if(*s==c){
			*s = nuevo_c;
		}
		s++;
	}
}