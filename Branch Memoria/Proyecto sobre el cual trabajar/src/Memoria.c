#include <commons/config.h>
#include <commons/string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <readline/readline.h>
#include "../../ComunParaTodos/src/Comun.c"
#include <commons/collections/list.h>

int main()
{

	t_log* g_logger=log_create("Memory.log", "Memory", 1, LOG_LEVEL_INFO);
	log_info(g_logger, "Log creado\n");
	t_config* g_config;
	g_config=config_create("Memory.config");
	log_info(g_logger, "Config recibido\n");
	char* IpMemoria=config_get_string_value(g_config, "IP_FILESYSTEM");
	log_info(g_logger, "IP memoria para conexion: %s \n", IpMemoria);
	char* puerto_memoria=config_get_string_value(g_config, "PUERTO_FILESYSTEM");
	log_info(g_logger, "Puerto para conexion: %s \n", puerto_memoria);
/*
	int conexion=crear_conexion(IpMemoria,puerto_memoria,g_logger);

	enviar("IP: \n",conexion);
	enviar(IpMemoria,conexion);

	leerPaqueteParaEnviar(conexion);
*/
	//Servidor


	int servidor = iniciar_servidor(IpMemoria,puerto_memoria,g_logger);
	log_info(g_logger, "Servidor listo");
	int cliente = esperar_cliente(servidor, g_logger);

	void iterator(char* value)
		{
			printf("%s\n", value);
		}

	t_list* lista;
	while(1)//!!!!!!!!!!!!!NO VA, CONSUME MEMORIA A LO PAVO, RECURSA¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡
	{
		int cod_op = recibir_operacion(cliente);
		switch(cod_op)
		{
		case MENSAJE:
			recibir_mensaje(cliente, g_logger);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente);
			printf("Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(g_logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(g_logger, "Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

//libero memoria
	log_destroy(g_logger);
	config_destroy(g_config);

	return EXIT_SUCCESS;
}
