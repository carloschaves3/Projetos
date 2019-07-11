#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <stdio_ext.h>
#include "listas.h"

int main(void) {

	unsigned int igual_quantum;
	unsigned int menor_quantum;
	usleep(igual_quantum);
	srand(time(NULL));
	printf("\n\n	SIMULADOR DE ESCALONADOR DE SISTEMA OPERACIONAL\n\n");
	int quantidade;
	do {
		printf("Digite a quantidade de processos que deseja criar para iniciar o simulador(minimo 2): ");
		scanf(" %d", &quantidade);
		__fpurge(stdin);
	} while (quantidade < 2);	
	putchar('\n');
	int i;
	Lista_Circular* lc = cria_circular();
	Lista_Dupla* ld = cria_dupla();
	Lista_Dupla* ldd = cria_dupla();
	putchar('\n');
	printf("\nDigite o valor do quantum em milisegundos: ");
	float quantum;
	scanf("%f", &quantum);
	igual_quantum = quantum * 1000;
	quantum /= 1000.0;
	ciclo_escalonador(lc,ld,ldd,quantum,igual_quantum,menor_quantum,quantidade);
	destroi_circular(lc);
	destroi_dupla(ld);
	destroi_dupla(ldd);

	return EXIT_SUCCESS;
}
