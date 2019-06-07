#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Cliente.h"
#include "Servidor.h"


int main () {

	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port = htons (1866);


	clienteMemoria = socket(AF_INET, SOCK_STREAM, 0);

	if(connect(clienteMemoria, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) != 0)
	{
	perror("No se pudo conectar\n");
	return 1;
	}
	else printf("Conexion Exitosa :3\n");
 	mensaje = malloc(100);

while(1) {
	printf("->Mensaje:  ");
	 scanf("%s", mensaje);
	 send(servidor,mensaje, strlen(mensaje), 0);
	}
}
