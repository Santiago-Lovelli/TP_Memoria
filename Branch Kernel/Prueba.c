#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct entero {
	struct entero *siguiente;
	int valor;
};

struct entero *fin = NULL;

struct entero *comienzo;

void main () {
	struct entero e1;
	struct entero e2;
	struct entero e3;

	e1.valor = 1;

	e2.valor = 2;

	e3.valor = 3;

	if (! fin) {
		comienzo = &e1;

		e1.siguiente = fin;

		fin = &e1;
	}

	else {
		fin -> siguiente = &e1;

		e1.siguiente = NULL;

		fin = &e1;
	}

	if (! fin) {
		comienzo = &e2;

		e2.siguiente = fin;

		fin = &e2;
	}

	else {
		fin -> siguiente = &e2;

		e2.siguiente = NULL;

		fin = &e2;
	}

	if (! fin) {
		comienzo = &e3;

		e3.siguiente = fin;

		fin = &e3;
	}

	else {
		fin -> siguiente = &e3;

		e3.siguiente = NULL;

		fin = &e3;
	}

	while (comienzo) {
		printf ("%i\n", comienzo -> valor);

		comienzo = comienzo -> siguiente;
	}
}
