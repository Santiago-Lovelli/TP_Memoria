#include "Requests.h"

int parsear (char *request);

void ejecutarselect (int cliente, char *request, int tamaniorequest) {
	char *mensaje = malloc (tamaniorequest + 1);
	char codigo = aceptacionrequest [0];

	*mensaje = codigo + *request;

	send (cliente, mensaje, tamaniorequest + 1, 0);

	free (mensaje);

	cantselect++;
}

void ejecutarinsert (int cliente, char *request, int tamaniorequest) {
	char *mensaje = malloc (tamaniorequest + 1);
	char codigo = aceptacionrequest [1];

	*mensaje = codigo + *request;

	send (cliente, mensaje, tamaniorequest + 1, 0);

	free (mensaje);

	cantinsert++;
}

void ejecutarcreate (int cliente, char *request, int tamaniorequest) {
	char *mensaje = malloc (tamaniorequest + 1);
	char codigo = aceptacionrequest [2];

	*mensaje = codigo + *request;

	send (cliente, mensaje, tamaniorequest + 1, 0);

	free (mensaje);
}

void ejecutardescribe (int cliente, char *request, int tamaniorequest) {
	char *mensaje = malloc (tamaniorequest + 1);
	char codigo = aceptacionrequest [3];

	*mensaje = codigo + *request;

	send (cliente, mensaje, tamaniorequest + 1, 0);

	free (mensaje);
}

void ejecutardrop (int cliente, char *request, int tamaniorequest) {
	char *mensaje = malloc (tamaniorequest + 1);
	char codigo = aceptacionrequest [4];

	*mensaje = codigo + *request;

	send (cliente, mensaje, tamaniorequest + 1, 0);

	free (mensaje);
}

void ejecutarjournal (int cliente) {
	struct sockaddr_in direccionmemoria;
	struct Memoria *receptor;
	char *codigo = malloc (1);

	*codigo = aceptacionrequest [5];

	direccionmemoria.sin_family = AF_INET;

	if (consistenciafuerte.id) {
		direccionmemoria.sin_addr.s_addr = INADDR_ANY; //inet (consistenciafuerte.ip);

		direccionmemoria.sin_port = htons (consistenciafuerte.puerto);

		if (connect (cliente, (void*) &direccionmemoria, sizeof (direccionmemoria)))
			printf ("No me pude conectar a la memoria %i.\n", consistenciafuerte.id);

		else
			send (cliente, codigo, 1, 0);
	}

	receptor = consistenciahashfuerte;

	while (receptor) {
		direccionmemoria.sin_addr.s_addr = INADDR_ANY; //inet (receptor -> ip);

		direccionmemoria.sin_port = htons (receptor -> puerto);

		if (connect (cliente, (void*) &direccionmemoria, sizeof (direccionmemoria)))
			printf ("No me pude conectar a la memoria %i.\n", receptor -> id);

		else
			send (cliente, codigo, 1, 0);

		receptor = receptor -> siguiente;
	}

	receptor = consistenciaeventual;

	while (receptor) {
		direccionmemoria.sin_addr.s_addr = INADDR_ANY; //inet (receptor -> ip);

		direccionmemoria.sin_port = htons (receptor -> puerto);

		if (connect (cliente, (void*) &direccionmemoria, sizeof (direccionmemoria)))
			printf ("No me pude conectar a la memoria %i.\n", receptor -> id);

		else
			send (cliente, codigo, 1, 0);

		receptor = receptor -> siguiente;
	}

	direccionmemoria.sin_addr.s_addr = INADDR_ANY;

	direccionmemoria.sin_port = htons (1024);

	send (cliente, codigo, 1, 0);

	free (codigo);
}

int potencia (int base, int exponente) {
	if (! exponente)
		return 1;

	for (int i = 1; i < exponente; i++)
		base *= base;

	return base;
}

int castear (char *numero) {
	int numerocasteado = 0;

	for (int i = 0; i < strlen (numero); i++)
		numerocasteado += (numero [i] - 48) * potencia (10, strlen (numero) - i - 1);

	return numerocasteado;
}

void ejecutaradd (char *request) {
	char *numero = malloc (1);
	struct Memoria memoria;
	int numerocasteado;
	int actual = 0;
	int i = 11;
	int j = 0;

	while (request [i] != ' ') {
		numero = realloc (numero, i - 10);

		numero [i - 11] = request [i];

		i++;
	}

	numerocasteado = castear (numero);

	i += 4;

	while (request [i]) {
		while (request [i] != caracteresconsistencia [j] && j < 4)
			j++;

		actual = transicionesconsistencia [actual] [j];

		i++;
	}

	switch (actual) {
		case 2: memoria.id = numerocasteado; consistenciafuerte = memoria; break;
		case 4: memoria.id = numerocasteado; memoria.siguiente = consistenciahashfuerte; *consistenciahashfuerte = memoria; break;
		case 6: memoria.id = numerocasteado; memoria.siguiente = consistenciaeventual; *consistenciaeventual = memoria; break;
		default: break;
	}
}

void sacarproceso (struct PCB pcb) {
	comienzoready = pcb.siguiente;

	free (pcb.path);
}

void enviaraready (struct PCB pcb) {
	if (! finready) {
		comienzoready = &pcb;

		pcb.siguiente = finready;

		finready = &pcb;
	}

	else {
		finready -> siguiente = &pcb;

		pcb.siguiente = NULL;

		finready = &pcb;
	}
}

void ejecutararchivo (struct PCB pcb) {
	char *linea = malloc (1);
	FILE *archivo;
	char caracter;
	int i = 0;
	int q = 0;

	archivo = fopen (pcb.path, "rb");

		if (! archivo) {
			perror ("El archivo no existe.");

			exit (EXIT_FAILURE);
		}

		fseek (archivo, pcb.caracteractual, 0);

		fread (&caracter, 1, 1, archivo);

		pcb.caracteractual++;

	// TODO config de quantum.
		while (! feof (archivo) && q < 4) {
			while (! feof (archivo) && caracter != '\n') {
				linea = realloc (linea, i + 1);

				linea [i] = caracter;

				i++;

				fread (&caracter, 1, 1, archivo);

				pcb.caracteractual++;
			}

			if (q > 4) {
				enviaraready (pcb);

				comienzoready = comienzoready -> siguiente;

				break;
			}

			linea [i] = '\0';

			parsear (linea);

			i = 0;

			q++;

			if (feof (archivo)) {
				sacarproceso (pcb);

				break;
			}

			fread (&caracter, 1, 1, archivo);
		}

		free (linea);

		fclose (archivo);
}

void ejecutarrun (char *request) {
	char *path = malloc (1);
	struct PCB pcb;
	int i = 4;

	pcb.caracteractual = 0;

	while (request [i]) {
		path = realloc (path, i - 3);

		path [i - 4] = request [i];

		i++;
	}

	pcb.path =  malloc (strlen (path));

	pcb.path = path;

	free (path);

	enviaraready (pcb);
}

void ejecutarmetrics () {

}

int esalfanumerico (char caracter) {
	if (caracter >= '0' && caracter <= '9')
		return 1;

	else if (caracter >= 'A' && caracter <= 'Z')
		return 1;

	else if (caracter >= 'a' && caracter <= 'z')
		return 1;

	return 0;
}

int parsearselect (char* palabra) {
	int i = 7;

	if (isdigit (palabra [i]))
		return aceptacionrequest [0] + 1;

	do {
		if (! esalfanumerico (palabra [i]))
			return aceptacionrequest [0] + 1;

		i++;
	} while (palabra [i] != ' ' && palabra [i]);

	i++;

	do {
		if (! isdigit (palabra [i]))
			return aceptacionrequest [0] + 1;

		i++;
	} while (palabra [i]);

	return aceptacionrequest [0];
}

int parsearinsert (char *palabra) {
	int i = 7;

	if (isdigit (palabra [i]))
		return aceptacionrequest [1] + 1;

	do {
		if (! esalfanumerico (palabra [i]))
			return aceptacionrequest [1] + 1;

		i++;
	} while (palabra [i] != ' ' && palabra [i]);

	i++;

	do {
		if (! isdigit (palabra [i]))
			return aceptacionrequest [1] + 1;

		i++;
	} while (palabra [i] != ' ' && palabra [i]);

	i++;

	if (palabra [i] != '"')
		return aceptacionrequest [1] + 1;

	do {
		i++;

		if (! palabra [i])
			return aceptacionrequest [1] + 1;
	} while (palabra [i] != '"');

	i++;

	if (palabra [i] != ' ')
		return aceptacionrequest [1] + 1;

	i++;

	do {
		if (! isdigit (palabra [i]))
			return aceptacionrequest [1] + 1;

		i++;
	} while (palabra [i]);

	return aceptacionrequest [1];
}

int esdeaceptacionconsistencia (int actual) {
	for (int i = 0; i < 3; i++) {
		if (actual == aceptacionconsistencia [i])
			return 1;
	}

	return 0;
}

int parsearcreate (char *palabra) {
	int actual = 0;
	int i = 7;
	int j;

	if (isdigit (palabra [i]))
		return aceptacionrequest [2] + 1;

	do {
		if (! esalfanumerico (palabra [i]))
			return aceptacionrequest [2] + 1;

		i++;
	} while (palabra [i] != ' ' && palabra [i]);

	i++;

	do {
		j = 0;

		while (j < 4 && palabra [i] != caracteresconsistencia [j])
			j++;

		actual = transicionesconsistencia [actual] [j];

		i++;
	} while (palabra [i] != ' ' && palabra [i]);

	i++;

	if (! esdeaceptacionconsistencia (actual))
		return aceptacionrequest [2] + 1;

	do {
		if (! isdigit (palabra [i]))
			return aceptacionrequest [2] + 1;

		i++;
	} while (palabra [i] != ' ' && palabra [i]);

	i++;

	do {
		if (! isdigit (palabra [i]))
			return aceptacionrequest [2] + 1;

		i++;
	} while (palabra [i]);

	return aceptacionrequest [2];
}

int parseardescribe (char *palabra) {
	int i = 8;

	if (! palabra [i])
		return aceptacionrequest [3];

	i++;

	if (isdigit (palabra [i]))
		return aceptacionrequest [3] + 1;

	do {
		if (! esalfanumerico (palabra [i]))
			return aceptacionrequest [3] + 1;

		i++;
	} while (palabra [i]);

	return aceptacionrequest [3];
}

int parseardrop (char *palabra) {
	int i = 5;

	if (isdigit (palabra [i]))
			return aceptacionrequest [4] + 1;

	do {
		if (! esalfanumerico (palabra [i]))
			return aceptacionrequest [4] + 1;

		i++;
	} while (palabra [i]);

	return aceptacionrequest [4];
}

int parsearjournal (char *palabra) {
	if (palabra [7])
		return aceptacionrequest [5] + 1;

	return aceptacionrequest [5];
}

int parsearadd (char *palabra) {
	int actual = 0;
	int i = 11;
	int j;

	do {
		if (! isdigit (palabra [i]))
			return aceptacionrequest [6] + 1;

		i++;
	} while (palabra [i] != ' ' && palabra [i]);

	i++;

	if (palabra [i] != 't')
		return aceptacionrequest [6] + 1;

	i++;

	if (palabra [i] != 'o')
		return aceptacionrequest [6] + 1;

	i++;

	if (palabra [i] != ' ')
		return aceptacionrequest [6] + 1;

	i++;

	while (palabra [i]) {
		j = 0;

		if (palabra [i] == ' ')
			return aceptacionrequest [6] + 1;

		while (j < 4 && palabra [i] != caracteresconsistencia [j])
			j++;

		actual = transicionesconsistencia [actual] [j];

		i++;
	}

	if (! esdeaceptacionconsistencia (actual))
		return aceptacionrequest [6] + 1;

	return aceptacionrequest [6];
}

int parsearrun (char *palabra) {
	int i = 3;

	if (! palabra [i])
		return aceptacionrequest [7] + 1;

	i++;

	if (! palabra [i])
		return aceptacionrequest [7] + 1;

	do {
		if (palabra [i] == ' ')
			return aceptacionrequest [7] + 1;

		i++;
	} while (palabra [i]);

	return aceptacionrequest [7];
}

int parsearmetrics (char *palabra) {
	if (palabra [7])
		return aceptacionrequest [8] + 1;

	return aceptacionrequest [8];
}

int esdeaceptacionrequest (int estado) {
	for (int i = 0; i < 9; i++) {
		if (estado == aceptacionrequest [i])
			return 1;
	}

	return 0;
}

int parsear (char *palabra) {
	int actual = 0;
	int i = 0;
	int j;

	do {
		j = 0;

		while (j < 18 && palabra [i] != caracteresrequest [j]) {
			j++;
		}

		actual = transicionesrequest [actual] [j];

		if (esdeaceptacionrequest (actual) && (! palabra [i + 1] || palabra [i + 1] == ' '))
			break;

		i++;
	} while (actual);

	switch (actual) {
		case 6: actual = parsearselect (palabra); break;
		case 12: actual = parsearinsert (palabra); break;
		case 18: actual = parsearcreate (palabra); break;
		case 26: actual = parseardescribe (palabra); break;
		case 29: actual = parseardrop (palabra); break;
		case 36: actual = parsearjournal (palabra); break;
		case 46: actual = parsearadd (palabra); break;
		case 49: actual = parsearrun (palabra); break;
		case 56: actual = parsearmetrics (palabra); break;
		default: break;
	}

	return actual;
}

void insertarrequest () {
	//t_config *config = config_create ("Conexion.config");
	//int puerto = config_get_int_value (config, "Puerto");
	int cliente = socket (AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in direccionservidor;
	char *request = malloc (1);
	//char *clave = malloc (20);
	//char *ip = malloc (11);
	int tamaniorequest = 0;
	char caracter;
	int codigo;

	//clave = config_get_string_value (config, "Clave");

	//ip = config_get_string_value (config, "IP");

	direccionservidor.sin_family = AF_INET;

	direccionservidor.sin_addr.s_addr = INADDR_ANY; //inet (ip);

	direccionservidor.sin_port = htons (1024);

	if (connect (cliente, (void*) &direccionservidor, sizeof (direccionservidor))) {
		perror ("No me pude conectar.\n");

		exit (EXIT_FAILURE);
	}

	printf ("Inserte una request: ");

	caracter = getchar ();

	while (caracter != '\n') {
		while (caracter != '\n') {
			request = realloc (request, tamaniorequest + 1);

			request [tamaniorequest] = caracter;

			tamaniorequest++;

			caracter = getchar ();
		}

		request = realloc (request, tamaniorequest + 1);

		request [tamaniorequest] = '\0';

		codigo = parsear (request);

		switch (codigo) {
			case 6: ejecutarselect (cliente, request, tamaniorequest); break;
			case 7: printf ("Formato: select [tabla] [key]\n"); break;
			case 12: ejecutarinsert (cliente, request, tamaniorequest); break;
			case 13: printf ("Formato: insert [tabla] [key] \"[valor]\" [timestamp]\n"); break;
			case 18: ejecutarcreate (cliente, request, tamaniorequest); break;
			case 19: printf ("Formato: create [tabla] [consistencia] [particiones] [tiempo de compactación]\n"); break;
			case 26: ejecutardescribe (cliente, request, tamaniorequest); break;
			case 27: printf ("Formato: describe / describe [tabla]\n"); break;
			case 29: ejecutardrop (cliente, request, tamaniorequest); break;
			case 30: printf ("Formato: drop [tabla]\n"); break;
			case 36: ejecutarjournal (cliente); break;
			case 37: printf ("Formato: journal\n"); break;
			case 46: ejecutaradd (request); break;
			case 47: printf ("Formato: add memory [numero] to [consistencia]\n"); break;
			case 49: ejecutarrun (request); break;
			case 50: printf ("Formato: run [path]\n"); break;
			case 56: ejecutarmetrics (); break;
			case 57: printf ("Formato: metrics\n"); break;
			default: printf ("Comando inválido.\n"); break;
		}

		tamaniorequest = 0;

		printf ("Inserte una request: ");

		caracter = getchar ();
	}

	free (request);

	close (cliente);
}

void ciclodemetrics() {
	while (1) {
		if (! (time (NULL) - tiempodeinicio) % 30)
			ejecutarmetrics ();
	}
}

void ejecutarready () {
	while (1) {
		if (finready)
			ejecutararchivo (*comienzoready);
	}
}

void main () {
// TODO crear hilos y sincronizar.
// TODO logs y configs.
// TODO comentar.
	tiempodeinicio = time (NULL);

	ejecutarready ();

	ciclodemetrics ();

	insertarrequest ();
}
