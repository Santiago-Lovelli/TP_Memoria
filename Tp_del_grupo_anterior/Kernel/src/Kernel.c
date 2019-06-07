#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <readline/readline.h>
#include <commons/collections/list.h>
#include "Kernel.h"

int main(){
	t_list *strongConsistency = list_create();
	t_list *strongHashConsistency = list_create();
	t_list *eventualConsistency = list_create();
	char* memoriaPrincipal;
	t_list *memoriasTotales = list_create();
	t_list *metadataDeTablas = list_create();

	t_log* g_logger=log_create("Kernel.log", "Kernel", 1, LOG_LEVEL_INFO);
	log_info(g_logger, "Log creado\n");
	t_config* g_config;
	g_config=config_create("Kernel.config");
	log_info(g_logger, "Config recibido\n");
	char* IpMemoria=config_get_string_value(g_config, "IP_MEMORIA");
	log_info(g_logger, "IP memoria para conexion: %s \n", IpMemoria);
	char* puerto_memoria=config_get_string_value(g_config, "PUERTO_MEMORIA");
	log_info(g_logger, "Puerto para conexion: %s \n", puerto_memoria);

	memoriaPrincipal=IpMemoria;
	log_info(g_logger, "memoria principal: %s \n", memoriaPrincipal);
	list_add(memoriasTotales,memoriaPrincipal);
	log_info(g_logger, "memomoria total apunta a memoria principal: %s \n", list_get(memoriasTotales,0));

	/*
	int conexion=crear_conexion(IpMemoria,puerto_memoria,g_logger);

	enviar("IP: \n",conexion);
	enviar(IpMemoria,conexion);

	leerPaqueteParaEnviar(conexion);
	 */

//libero memoria
	list_destroy_and_destroy_elements(strongConsistency,(void*) destruirIpMemoria);
	list_destroy_and_destroy_elements(strongHashConsistency,(void*) destruirIpMemoria);
	list_destroy_and_destroy_elements(eventualConsistency,(void*) destruirIpMemoria);
	list_destroy_and_destroy_elements(memoriasTotales,(void*) destruirIpMemoria);
	list_destroy_and_destroy_elements(metadataDeTablas,(void*) destruirMetadata);

	log_destroy(g_logger);
	config_destroy(g_config);

	return EXIT_SUCCESS;
}
