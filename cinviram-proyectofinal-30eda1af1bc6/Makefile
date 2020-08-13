#Escriba su makefiile en este archivo
#
# Integrante 1
# Integrante 2
#Escriba su makefiile en este archivo
#
# Integrante 1
# Integrante 2
#
#Si quieren llamar reglas adicionales, agregenlas como pre-requisitos a la regla all
#La regla logdb.so deben modificarla con su comando para generar dicho ejecutable.
#El archivo prueba.c esta vacio. NO deben llenarlo. Este se reemplazara con el codigo del profesor
#al probar su proyecto

IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR) -Wall

ODIR=obj

LIBS=-lm -lpthread

_OBJ = abrir_db.o cerrar_db.o compactar.o conectar_db.o crear_db.o eliminar.o get_val.o put_val.o validaciones.o csapp.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all:prueba lib/liblogdb.so bin/logdb  bin/prueba_est

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -fPIC -o $@ $< $(CFLAGS)


prueba.o: src/prueba.c
	gcc -Wall -Iinclude/ -c src/prueba.c -o obj/prueba.o

prueba: prueba.o lib/liblogdb.so
	gcc obj/prueba.o -lhashtabprof -llogdb -Llib/ -o bin/prueba
lib/libhashtab.so:	obj/fn1_din.o obj/fn2_din.o obj/fn3_din.o obj/fn4_din.o obj/fn5_din.o  obj/fn6_din.o obj/fn7_din.o obj/fn8_din.o obj/fn9_din.o obj/fn10_din.o
		gcc -shared -o $@ $^
obj/fn1_din.o: src/fn1.c
	gcc -Wall -Iinclude -fPIC $< -c -o $@
obj/fn2_din.o: src/fn2.c
	gcc -Wall -Iinclude -fPIC $< -c -o $@
obj/fn3_din.o: src/fn3.c
	gcc -Wall -Iinclude -fPIC $< -c -o $@
obj/fn4_din.o: src/fn4.c
	gcc -Wall -Iinclude -fPIC $< -c -o $@
obj/fn5_din.o: src/fn5.c
	gcc -Wall -Iinclude -fPIC $< -c -o $@
obj/fn6_din.o: src/fn6.c
	gcc -Wall -Iinclude -fPIC $< -c -o $@
obj/fn7_din.o: src/fn7.c
	gcc -Wall -Iinclude -fPIC $< -c -o $@
obj/fn8_din.o: src/fn8.c
	gcc -Wall -Iinclude -fPIC $< -c -o $@
obj/fn9_din.o: src/fn9.c
	gcc -Wall -Iinclude -fPIC $< -c -o $@
obj/fn10_din.o: src/fn10.c
	gcc -Wall -Iinclude -fPIC $< -c -o $@

obj/logdb.o: src/logdb.c
	gcc -Wall -Iinclude src/logdb.c -c -o obj/logdb.o
obj/mod_indice.o: src/mod_indice.c
	gcc -Wall -Iinclude src/mod_indice.c -c -o obj/mod_indice.o
obj/compactacion_servidor.o: src/compactacion_servidor.c
	gcc -Wall -Iinclude src/compactacion_servidor.c -c -o obj/compactacion_servidor.o
obj/servidor.o: src/servidor.c
	gcc -Wall -Iinclude src/servidor.c -c -o obj/servidor.o
lib/liblogdb.so: $(OBJ)
	$(CC) -shared -fPIC -o $@ $^ $(CFLAGS) $(LIBS)
bin/logdb: obj/logdb.o obj/mod_indice.o obj/servidor.o obj/csapp.o obj/compactacion_servidor.o obj/validaciones.o lib/libhashtab.so
	gcc -Wall -o bin/logdb obj/logdb.o obj/mod_indice.o obj/servidor.o obj/compactacion_servidor.o obj/validaciones.o obj/csapp.o   -I./include $(LIBS) lib/libhashtab.so -Llib



bin/prueba_est: src/prueba_est.c obj/csapp.o lib/libhashtab.so lib/liblogdb.so
	gcc -Wall  src/prueba_est.c obj/csapp.o -I./include  -lpthread lib/libhashtab.so lib/liblogdb.so -L./lib -o $@

.PHONY: clean

clean:
	rm -f obj/* bin/*

