#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <readline/readline.h>
#include "../../ComunParaTodos/src/Comun.c"

#ifndef KERNEL_H_
#define KERNEL_H_


void menuApi(){
	printf("Elija la opciones que desee: \n");
	printf("1- SELECT \n");
	printf("2- INSERT \n");
	printf("3- CREATE \n");
	printf("4- DESCRIBE \n");
	printf("5- DROP \n");
	printf("6- JOURNAL \n");
	printf("7- ADD \n");
	printf("8- RUN \n");
	printf("9- METRICS \n");
}



void* elejir_accion(codigoRequest unCodigo){

	void selecccionar(char* nombreDeTabla, char* key);
	void insert(char* nombreDeTabla, char* key, char* value, void* timetamp);
	void create(char* nombreDeTabla, unaConsistencia consistencia,int particiones, int tiempoDeCompactacion);
	void describe(void* nombreTabla);
	void drop(char* nombreDeTabla);
	void journal();
	void add(int numeroDeMemoria,unaConsistencia consistencia);
	void run(char* path);
	void metrics();

	switch (unCodigo)
	{
	    case 0:
	    	return (void*)selecccionar;
	    	break;
	    case 1:
	    	return (void*)insert;
	    	break;
	    case 2:
	    	return (void*)create;
	    	break;
	    case 3:
	    	return (void*)describe;
	    	break;
	    case 4:
	    	return (void*)drop;
	    	break;
	    case 5:
	    	return (void*)journal;
	    	break;
	    case 6:
	    	return (void*)add;
	    	break;
	    case 7:
	    	return (void*)run;
	    	break;
	    case 8:
	    	return (void*)metrics;
	    	break;
	    default:
	    	//informar error

	    	break;
	}
}

#endif
