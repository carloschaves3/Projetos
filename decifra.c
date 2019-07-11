#include <stdlib.h>
#include <stdio.h>

int decifra(int, int, char*);

int main(int argc, char* argv[]) {
	if(argc < 2 || argc > 3) {
		puts("Utilize o seguinte modelo >> ./a.out <chave>");
		return EXIT_FAILURE;
	}
	char* chave = argv[1];
	FILE* arquivo_entrada_decifra = fopen("arquivo_cifrado.txt", "r");
	if (arquivo_entrada_decifra == 0) {
		perror("Erro na abertura do arquivo de entrada da decifra!");
		return EXIT_FAILURE;
	}
	FILE* arquivo_saida_decifra = fopen("arquivo_decifrado.txt", "w");
	if (arquivo_saida_decifra == 0) {
		perror("Erro na abertura do arquivo de saida da decifra!");
		return EXIT_FAILURE;
	}
	// Obtendo o file descriptor de cada arquivo
	int fd_entrada_decifra	= fileno(arquivo_entrada_decifra);
	int fd_saida_decifra	= fileno(arquivo_saida_decifra);
	int decifrado = decifra(fd_entrada_decifra, fd_saida_decifra, chave);
	if (decifrado == 0) {
		puts("Decifragem bem sucedida!");
	}
       	if (decifrado == -1) {
		puts("Decifragem nao realizada devido ao arquivo de etntrada estar vazio, em branco ou conter algum caractere especial..");
        	fclose(arquivo_entrada_decifra);
	        fclose(arquivo_saida_decifra);
		return EXIT_FAILURE;
	}
	fclose(arquivo_entrada_decifra);
	fclose(arquivo_saida_decifra);

	return EXIT_SUCCESS;
}

