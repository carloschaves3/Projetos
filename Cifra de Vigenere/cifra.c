#include <stdlib.h>
#include <stdio.h>

int cifra(int, int, char*);

int main(int argc, char* argv[]) {
	if(argc < 2 || argc > 3) {
		puts("Utilize o seguinte modelo >> ./a.out <chave>");
		return EXIT_FAILURE;
	}
	char* chave = argv[1];
	FILE* arquivo_entrada_cifra = fopen("arquivo_original.txt", "r");
	if (arquivo_entrada_cifra == 0) {
		perror("Erro na abertura do arquivo de entrada da cifra!");
		return EXIT_FAILURE;
	}
	FILE* arquivo_saida_cifra = fopen("arquivo_cifrado.txt", "w");
	if (arquivo_saida_cifra == 0) {
		perror("Erro na abertura do de saida da cifra!");
		return EXIT_FAILURE;
	}
	// Obtendo o file descriptor de cada arquivo
	int fd_entrada_cifra	= fileno(arquivo_entrada_cifra);
	int fd_saida_cifra	= fileno(arquivo_saida_cifra);
	int cifrado = cifra(fd_entrada_cifra, fd_saida_cifra, chave);
	if (cifrado == 0) {
		puts("Cifragem bem sucedida!");
	}
	if (cifrado == -1) {
		puts("Cifragem nao realizada devido ao arquivo de entrada estar vazio, em branco ou conter algum caractere especial..");
		fclose(arquivo_entrada_cifra);
	        fclose(arquivo_saida_cifra);
		return EXIT_FAILURE;
	}
	fclose(arquivo_entrada_cifra);
	fclose(arquivo_saida_cifra);
	return EXIT_SUCCESS;
}
