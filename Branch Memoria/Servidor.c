#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Servidor.h"

int main()
{


	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = INADDR_ANY;
	direccionServidor.sin_port = htons (1866);

	servidor = socket (AF_INET, SOCK_STREAM, 0);

	activado = 1;

	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof (activado));

	if (bind (servidor, (void*) &direccionServidor, sizeof (direccionServidor)) != 0) {
		perror ("Falló el bind.\n");
		return 1;
	}

	printf ("Estoy escuchando.\n");

	listen (servidor, 10);

//--------------------------- ACEPTAR CLIENTE


	tamanioDireccionCliente = sizeof(struct sockaddr_in);
	cliente = accept(servidor, (void*) &direccionCliente, &tamanioDireccionCliente);

	if(cliente == -1)
	{
		printf("Ocurrio un error :( \n");
		return 1;
	}

	printf("Recibí una coneccion de %i  ", cliente);



	//----------------------------------ACEPTAR MENSAJES

	int tamanioMensaje = 15;
	char* buffer = malloc(tamanioMensaje);
	int bytesRecibidos = recv(cliente, buffer, tamanioMensaje - 1, 0);

	if(bytesRecibidos <= 0) {
		printf("Cliente desconectado");
		return 1;
	}

	buffer[bytesRecibidos] = '\0';

	printf("Recibí %i bytes con el mensaje %s \n", bytesRecibidos, buffer);

	free(buffer);

//---------------------------RECIBIR MUCHOS MENSAJES

	uint32_t tamanioPaquete;
	recv(cliente, &tamanioPaquete, 4, 0);

	char* bufferParaPaquetes = malloc(tamanioPaquete);
	recv(cliente, bufferParaPaquetes, tamanioPaquete, MSG_WAITALL);

	close(servidor);

	return 0;
}
