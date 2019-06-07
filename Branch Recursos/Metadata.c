#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct MetadataFS {
	int block_size;
	int blocks;
	char* magic_number;
};

int main() {
	FILE *ArchivoMetadata;

	struct MetadataFS metaPrueba;
	metaPrueba.block_size = 44;
	metaPrueba.blocks = 9999;
	metaPrueba.magic_number = "LFS";

	int carpetaCreada;
	struct stat datosDeCarpeta = { 0 };
	if (stat("/home/utnso/workspace/LFS/FileSystem/Metadata", &datosDeCarpeta)
			== -1) {
		carpetaCreada = mkdir("/home/utnso/workspace/LFS/FileSystem/Metadata",
				0777);
		if (carpetaCreada == -1) {
			perror("No se pudo crear la carpeta\n");
			return 1;
		}

	}

	ArchivoMetadata = fopen(
			"/home/utnso/workspace/LFS/FileSystem/Metadata/metadata.bin",
			"w+b");

	if (!ArchivoMetadata) {
		perror("No se pudo abrir el archivo de escritura");
		return 1;
	}

	fwrite(&metaPrueba, sizeof(metaPrueba), 1, ArchivoMetadata);
	printf("Archivo Creado\n");

	struct MetadataFS metaLeido = { 0, 0 };

	fseek(ArchivoMetadata, 0, SEEK_SET);
	int fueLeido;
	fueLeido = fread(&metaLeido, sizeof(metaLeido), sizeof(metaLeido),
			ArchivoMetadata);

	if (!fueLeido) {
		perror("No se pudo leer del archivo\n");
		return 1;
	}

	printf("Datos reales \n TB = %i  B = %i\n Palabra mágica = %s",
			metaPrueba.block_size, metaPrueba.blocks, metaPrueba.magic_number);
	printf(
			"Datos Leidos \n Tamaño de bloque: %i \n Bloques: %i\n Palabra mágica = %s",
			metaLeido.block_size, metaLeido.blocks, metaPrueba.magic_number);
	fclose(ArchivoMetadata);
	return 0;
}
