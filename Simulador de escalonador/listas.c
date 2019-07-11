#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdio_ext.h>
#include "listas.h"

float tempo_em_pronto = 0.0;
int tempo_total_de_processador_h = 0, tempo_total_de_processador_m = 0, tempo_total_de_processador_s = 0;

// TAD LISTA CIRCULAR 

struct noh {
	Processo proc;
	struct noh* proximo;
};
typedef struct noh Noh_Circular;

struct lista_circular {
	Noh_Circular* cauda;
	size_t total_nohs;
};

Lista_Circular* cria_circular(void) {
	Lista_Circular* l = malloc(sizeof(Lista_Circular));
	l->cauda = NULL;
	l->total_nohs = 0;
	return l;
}

bool underflow_circular(const Lista_Circular* l) {
	return l->cauda == NULL;
}

void destroi_circular(Lista_Circular* l) {
	while(!underflow_circular(l)) {
		Noh_Circular* n = malloc(sizeof(Noh_Circular));
		n = l->cauda->proximo;
		if(l->cauda == n) {
			l->cauda = NULL;
		} else {
			l->cauda->proximo = n->proximo;
		}
		free(n);
		--l->total_nohs;
	}
	printf("Lista Destruida !\n");
}

void insere_circular(Lista_Circular* l, Processo proc) {
	Noh_Circular* n = malloc(sizeof(Noh_Circular));
	n->proc = proc;
	if(l->cauda == NULL) {
		l->cauda = n;
		n->proximo = n;
		++l->total_nohs;
	} else {
		n->proximo = l->cauda->proximo;
		l->cauda->proximo = n;
		l->cauda = n;
		++l->total_nohs;
	}
}

Processo remove_circular(Lista_Circular* l) {
	Processo aux;
	if(l->cauda == NULL) {
		printf("Lista vazia !\n");
	} else {
		Noh_Circular* n = l->cauda->proximo;
		if(n == n->proximo) {
			l->cauda = NULL;
			aux = n->proc;
			free(n);
			--l->total_nohs;
			return aux;
		} else {
			n = l->cauda->proximo;
			l->cauda->proximo = n->proximo;
			aux = n->proc;
			--l->total_nohs;
			free(n);
			return aux;
		}
	}
}

void dump_circular(const Lista_Circular* l) {
	if(underflow_circular(l)) {
		printf("Lista vazia !\n");
		return;
	} else {
		Noh_Circular* n = malloc(sizeof(Noh_Circular));
		n = l->cauda->proximo;
		do {
			printf("Processo numero: %d\n", n->proc.PID);
			printf("Tempo necessario de processamento para conclusao do processo (em segundos): %f\n", n->proc.tempo_necessario_processador);
			printf("Horario de entrada do processo: %02d:%02d:%02d\n", n->proc.h_entrada, n->proc.m_entrada, n->proc.s_entrada);
			printf("Estado = %s\n\n", n->proc.estado);
			n = n->proximo;
		} while (n != l->cauda->proximo);
	}
	printf("Total de nohs da circular : %zd\n", l->total_nohs);
}

// TAD LISTA DUPLA ENCADEADA 

struct noh_dupla {
	struct noh_dupla* proximo;
	struct noh_dupla* anterior;
	Processo proc;
};
typedef struct noh_dupla Noh_Dupla;

struct lista_dupla {
	Noh_Dupla* cabeca;
	Noh_Dupla* cauda;
	size_t total_nohs_dupla;
};

Lista_Dupla* cria_dupla(void) {
	Lista_Dupla* ld = malloc(sizeof(Lista_Dupla));
	ld->cabeca = NULL;
	ld->cauda = NULL;
	ld->total_nohs_dupla = 0;
	return ld;
}

bool underflow_dupla(Lista_Dupla* ld) {
	return ld->cabeca == NULL;
}

void insere_dupla(Lista_Dupla* ld, Processo proc) {
	Noh_Dupla* n_ld = malloc(sizeof(Noh_Dupla));
	if(underflow_dupla(ld)) {
		n_ld->proc = proc;
		n_ld->proximo = ld->cabeca;
		ld->cabeca = n_ld;
		++ld->total_nohs_dupla;
		n_ld->anterior = NULL;
		if(n_ld->proximo != NULL) {
			n_ld->proximo->anterior = n_ld;
		} else {
			ld->cauda = n_ld;
		}
	} else {
		n_ld->proc = proc;
		n_ld->proximo = NULL;
		n_ld->anterior = ld->cauda;
		n_ld->anterior->proximo = n_ld;
		ld->cauda = n_ld;
		++ld->total_nohs_dupla;
	}
}

Processo remove_dupla(Lista_Dupla* ld, Processo proc) {
	if(underflow_dupla(ld)) {
		printf("Lista Vazia !\n");
	} else {
		Noh_Dupla* n_ld = ld->cabeca;
		while(n_ld != NULL) {
			if(n_ld->proc.PID == proc.PID) {
				if(n_ld->anterior != NULL) {
					n_ld->anterior->proximo = n_ld->proximo;
				} else {
					ld->cabeca = n_ld->proximo;
				}
				if(n_ld->proximo != NULL) {
					n_ld->proximo->anterior = n_ld->anterior;
				} else {
					ld->cauda = n_ld->anterior;
				}
				Noh_Dupla* aux = n_ld;
				free(n_ld);
				--ld->total_nohs_dupla;
				return aux->proc;
			} else {
				n_ld = n_ld->proximo;
			}
		}
	}
}

void destroi_dupla(Lista_Dupla* ld) {
	if(underflow_dupla(ld)) {
		printf("Lista vazia !\n");
		return;
	} else {
		while(!underflow_dupla(ld)) {
			Noh_Dupla* n_ld = ld->cabeca;
			ld->cabeca = n_ld->proximo;
			free(n_ld);
			if(ld->cabeca == NULL) {
				ld->cauda = NULL;
			} else {
				ld->cabeca->anterior = NULL;
			}
			--ld->total_nohs_dupla;
		}
	}
	free(ld);
	printf("Lista Destruida !\n");
}

void dump_dupla(Lista_Dupla* ld) {
	if(underflow_dupla(ld)) {
		printf("Lista vazia !\n");
		return;
	}
	Noh_Dupla* n_ld;
	for(n_ld = ld->cabeca; n_ld != NULL; n_ld = n_ld->proximo) {
		printf("Processo numero: %d\n", n_ld->proc.PID);
		printf("Tempo necessario de processamento para conclusao do processo (em segundos): %f\n", n_ld->proc.tempo_necessario_processador);
		printf("Horario de entrada do processo: %02d:%02d:%02d\n", n_ld->proc.h_entrada, n_ld->proc.m_entrada, n_ld->proc.s_entrada);
		printf("Estado = %s\n\n", n_ld->proc.estado);
	}
	printf("Total de nohs da dupla: %zd", ld->total_nohs_dupla);
}

// FUNCOES UTILIZANDO LISTAS

unsigned controle = 0;

// LIMITA O TEMPO MINIMO E MAXIMO DO TEMPO NECESSARIO PARA A CONCLUSAO DE UM PROCESSO
int tempo_rand(void) {
	return ((rand() % 5) + 5);	
}

Processo cria_processo(void) {
	Processo temp;
	controle += 1;
	temp.PID = controle;
	strcpy(temp.estado, "PRONTO");
	temp.tempo_necessario_processador = tempo_rand();
	temp.tempo_executando = 0;
	temp.h_entrada = hora_atual()->tm_hour;
	temp.m_entrada = hora_atual()->tm_min;
	temp.s_entrada = hora_atual()->tm_sec;
	temp.tempo_espera_h = 0;
	temp.tempo_espera_m = 0;
	temp.tempo_espera_s = 0;

	return temp;
}

struct tm* hora_atual(void) {
	time_t temp;
	time(&temp);
	struct tm* tempo = localtime(&temp);
	return tempo;
}

void ciclo_escalonador(Lista_Circular* lc, Lista_Dupla* ld, Lista_Dupla* ldd, float quantum, unsigned int igual_quant, unsigned int menor_quant, int quantos_processos) {
	int v;
	for(v = 0; v < quantos_processos; ++v) {
		insere_circular(lc, cria_processo());
	}
	char teste;
	float i;
	bool redireciona;
	float acumulado_processador = 0.0;
	while (!underflow_circular(lc) || !underflow_dupla(ld)) {
		if(!underflow_circular(lc)) {
			strcpy(lc->cauda->proximo->proc.estado, "EXECUTANDO");
		}
		redireciona = true;
		system("clear");
		print_escalonador(lc,ld,ldd);
		if (underflow_circular(lc) && !underflow_dupla(ld)) {
			usleep(igual_quant);
			for(i = 0; i <= quantum; i += quantum / 10) {
				if(evento_random_circular(lc,ld,ld->cabeca->proc)) {
					break;// evento random aqui
				}
			}
			continue;
		}
		if (!underflow_circular(lc) && lc->cauda->proximo->proc.tempo_necessario_processador > quantum) {
			strcpy(lc->cauda->proximo->proc.estado, "PRONTO");
			for(i = 0; i <= quantum; i += quantum / 10) {
				if(evento_random_circular(lc,ld,lc->cauda->proximo->proc)) {
					redireciona = false;
					break;// evento random aqui
				}
			}
			if(redireciona) {
				usleep(igual_quant);
				acumulado_processador += quantum;
				lc->cauda->proximo->proc.tempo_necessario_processador -= quantum;
				lc->cauda->proximo->proc.tempo_executando = lc->cauda->proximo->proc.tempo_executando + quantum;
				insere_circular(lc,remove_circular(lc));
				system("clear");
				print_escalonador(lc,ld,ldd);
				continue;
			} else {
				continue;
			}
		}
		if (!underflow_circular(lc) && lc->cauda->proximo->proc.tempo_necessario_processador == quantum) {
			strcpy(lc->cauda->proximo->proc.estado, "PRONTO");
			for(i = 0; i <= quantum; i += quantum / 10) {
				if(evento_random_circular(lc,ld,lc->cauda->proximo->proc)) {
					redireciona = false;
					break;// evento random aqui
				}
			}
			if(redireciona) {
				usleep(igual_quant);
				acumulado_processador += quantum;
				lc->cauda->proximo->proc.tempo_necessario_processador = 0;
				lc->cauda->proximo->proc.tempo_executando = lc->cauda->proximo->proc.tempo_executando + quantum;
				lc->cauda->proximo->proc.h_saida = hora_atual()->tm_hour;
				lc->cauda->proximo->proc.m_saida = hora_atual()->tm_min;
				lc->cauda->proximo->proc.s_saida = hora_atual()->tm_sec;	
				strcpy(lc->cauda->proximo->proc.estado, "FINALIZADO");
				insere_dupla(ldd,remove_circular(lc));
				system("clear");
				print_escalonador(lc,ld,ldd);
				printf("\n");
// IMPRIME RELATORIO DO PROCESSO TERMINADO E ESPERA DIGITAR ALGUMA COISA PARA CONTINUAR
				relatorio_processo(ldd,acumulado_processador);
				__fpurge(stdin);	
				getchar();						
				continue;
			} else {
				continue;
			}
		} 
		if (!underflow_circular(lc) && lc->cauda->proximo->proc.tempo_necessario_processador < quantum) {
			strcpy(lc->cauda->proximo->proc.estado, "PRONTO");
			for(i = 0; i <= lc->cauda->proximo->proc.tempo_necessario_processador; i += lc->cauda->proximo->proc.tempo_necessario_processador) {
				if(evento_random_circular(lc,ld,lc->cauda->proximo->proc)) {
					redireciona = false;					
					break;// evento random aqui
				}
			}
			if(redireciona) {
				menor_quant = lc->cauda->proximo->proc.tempo_necessario_processador * 1000;
				usleep(menor_quant);
				lc->cauda->proximo->proc.tempo_executando = lc->cauda->proximo->proc.tempo_executando + lc->cauda->proximo->proc.tempo_necessario_processador;
				acumulado_processador += lc->cauda->proximo->proc.tempo_necessario_processador;
				lc->cauda->proximo->proc.tempo_necessario_processador = 0;
				lc->cauda->proximo->proc.h_saida = hora_atual()->tm_hour;
				lc->cauda->proximo->proc.m_saida = hora_atual()->tm_min;
				lc->cauda->proximo->proc.s_saida = hora_atual()->tm_sec;
				strcpy(lc->cauda->proximo->proc.estado, "FINALIZADO");
				insere_dupla(ldd,remove_circular(lc));
				system("clear");
				print_escalonador(lc,ld,ldd);
				printf("\n");
// IMPRIME RELATORIO DO PROCESSO TERMINADO E ESPERA DIGITAR ALGUMA COISA PARA CONTINUAR
				relatorio_processo(ldd, acumulado_processador);
				__fpurge(stdin);
				getchar();
				continue;
			} else {
				continue;
			}
		}
	}
}

void relatorio_processo(Lista_Dupla* ldd, float acumulado) {

	tempo_total_de_processador_h += ((int)ldd->cauda->proc.tempo_executando / 3600);
	tempo_total_de_processador_m += ((int)ldd->cauda->proc.tempo_executando / 60);
	tempo_total_de_processador_s += ((int)ldd->cauda->proc.tempo_executando % 60);
	if (tempo_total_de_processador_s > 59) {
		tempo_total_de_processador_s -= 60;
		tempo_total_de_processador_m += 1;
		if (tempo_total_de_processador_m > 59) {
			tempo_total_de_processador_m -= 60;
			tempo_total_de_processador_h += 1;
		}
	}
	printf(" Processo numero %d terminado ....\n", ldd->cauda->proc.PID);
	int aux_m, aux2_m;
	int aux_s, aux2_s;
	int aux2_h, aux_h = abs(ldd->cauda->proc.h_saida - ldd->cauda->proc.h_entrada);
	if(ldd->cauda->proc.m_entrada > ldd->cauda->proc.m_saida) {
		if(aux_h > 0) {
			aux_h -= 1;
		}
		aux_m = 60 - abs(ldd->cauda->proc.m_entrada - ldd->cauda->proc.m_saida);
		if(ldd->cauda->proc.s_entrada > ldd->cauda->proc.s_saida) {
			if(aux_m > 0) {
				aux_m -= 1;
			}
			aux_s = 60 - abs(ldd->cauda->proc.s_entrada - ldd->cauda->proc.s_saida);
		} else {
			aux_s = abs(ldd->cauda->proc.s_entrada - ldd->cauda->proc.s_saida);
		}
	} else {
		aux_m = abs(ldd->cauda->proc.m_entrada - ldd->cauda->proc.m_saida);
		if(ldd->cauda->proc.s_entrada > ldd->cauda->proc.s_saida) {
			if(aux_m > 0) {
				aux_m -= 1;
			}
			aux_s = 60 - abs(ldd->cauda->proc.s_entrada - ldd->cauda->proc.s_saida);
		} else {
			aux_s = abs(ldd->cauda->proc.s_entrada - ldd->cauda->proc.s_saida);
		}
	}
	printf(" Tempo de atendimento : %02d:%02d:%02d\n", aux_h, aux_m, aux_s);	
	aux2_h = abs(aux_h - ((int)ldd->cauda->proc.tempo_executando / 3600));
	if(aux_m < ((int)ldd->cauda->proc.tempo_executando / 60)) {
		if(aux2_h > 0) {
			aux2_h -= 1;
		}
		aux2_m = 60 - abs(aux_m - ((int)ldd->cauda->proc.tempo_executando / 60));
		if(aux_s < ((int)ldd->cauda->proc.tempo_executando % 60)) {
			if(aux2_m > 0) {
				aux2_m -= 1;
			}
			aux2_s = 60 - abs(aux_s - ((int)ldd->cauda->proc.tempo_executando % 60));
		} else {
			aux2_s = abs(aux_s - ((int)ldd->cauda->proc.tempo_executando % 60));
		}
	} else {
		aux2_m = abs(aux_m - ((int)ldd->cauda->proc.tempo_executando / 60));
		if(aux_s < ((int)ldd->cauda->proc.tempo_executando % 60)) {
			if(aux2_m > 0) {
				aux2_m -= 1;
			}
			aux2_s = 60 - abs(aux_s - ((int)ldd->cauda->proc.tempo_executando % 60));
		} else {
			aux2_s = abs(aux_s - ((int)ldd->cauda->proc.tempo_executando % 60));
		}
	}


	printf(" Tempo total de espera : %02d:%02d:%02d\n", aux2_h, aux2_m, aux2_s);
	printf(" Tempo acumulado de uso do processador : %02d:%02d:%02d\n", (int)acumulado/3600, (int)acumulado/60, (int)acumulado%60);
	
	/*
		ESTAVA UTILIZANDO ESTA PARTE DO CODIGO PARA TESTES SOMENTE
		MAS RESOLVI DEIXAR COMENTADO CASO NECESSÁRIO MAIS TARDE
	
	printf("\n\n*********************************************\n\n");	
	printf(" Horario de entrada = %02d:%02d:%02d\n", ldd->cauda->proc.h_entrada, ldd->cauda->proc.m_entrada, ldd->cauda->proc.s_entrada);
	printf(" Horario de saida = %02d:%02d:%02d\n", ldd->cauda->proc.h_saida, ldd->cauda->proc.m_saida, ldd->cauda->proc.s_saida);
	printf(" Tempo em 'executando' = %02d:%02d:%02d\n", (int)ldd->cauda->proc.tempo_executando / 3600, (int)ldd->cauda->proc.tempo_executando / 60, (int)ldd->cauda->proc.tempo_executando % 60);
	printf(" Tempo em 'espera' = %02d:%02d:%02d\n", ldd->cauda->proc.tempo_espera_h, ldd->cauda->proc.tempo_espera_m, ldd->cauda->proc.tempo_espera_s);	
	*/
}

bool evento_random_circular(Lista_Circular* lc, Lista_Dupla* ld, Processo p) {
	int random = (rand() % 2000) +1;
	switch (random) {
		case 446:
		case 828:
			// CRIA PROCESSO RANDOM
			if(!underflow_circular(lc)) {
				insere_circular(lc,cria_processo());
				return true;
			} else {
				return false;
			}
		case 131:
		case 777:
			// SOLICITACAO E/S
			if(!underflow_circular(lc)) {
				strcpy(lc->cauda->proximo->proc.estado, "ESPERA");
				lc->cauda->proximo->proc.entrada_espera_h = hora_atual()->tm_hour;
				lc->cauda->proximo->proc.entrada_espera_m = hora_atual()->tm_min;
				lc->cauda->proximo->proc.entrada_espera_s = hora_atual()->tm_sec;
				insere_dupla(ld,remove_circular(lc));
				return true;
			} else {
				return false;
			}
		case 20:
		case 240:
		case 590:
		case 600:
		case 870:
			// VOLTA DA E/S
			if(!underflow_dupla(ld)) {
				Noh_Dupla* n_ld = ld->cabeca;
				int i;
				while(n_ld != NULL) {
					for(i = 0; i <= controle; ++i) {
						if(n_ld->proc.PID == i) {
							insere_circular(lc,remove_dupla(ld,n_ld->proc));
							lc->cauda->proc.tempo_espera_h = abs((hora_atual()->tm_hour) - lc->cauda->proc.entrada_espera_h);
							lc->cauda->proc.tempo_espera_m = abs((hora_atual()->tm_min) - lc->cauda->proc.entrada_espera_m);
							lc->cauda->proc.tempo_espera_s = abs((hora_atual()->tm_sec) - lc->cauda->proc.entrada_espera_s);
							strcpy(lc->cauda->proc.estado, "PRONTO");
							return true;
						}
					}
					n_ld = n_ld->proximo;
				}
			} else {
				return false;
			}
		default:
			// NAO FAZ NADA
			return false;
	}
}		

void print_escalonador(Lista_Circular* lc, Lista_Dupla* ld, Lista_Dupla* ldd) {

	int i, j, quantidade_circular = 0, quantidade_dupla = 0, quantidade_ddupla = 0;
	int cabecalho;
	int a, b, c;
	
	printf("       ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("       |");
	cabecalho = printf("            Processos executando ( Total = %zd )", lc->total_nohs);
	for(i = 0; i <= (59 - cabecalho); ++i) {
		printf(" ");
	}
	putchar('|');
	cabecalho = printf("            Processos em espera E/S ( Total = %zd )", ld->total_nohs_dupla);
	for(i = 0; i <= (59 - cabecalho); ++i) {
		printf(" ");
	}
	putchar('|');
	cabecalho = printf("            Processos terminados ( Total = %zd )", ldd->total_nohs_dupla);
	for(i = 0; i <= (55 - cabecalho); ++i) {
		printf(" ");
	}
	printf("|\n");
	printf("       ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	int totaldenohs;
	if(lc->total_nohs >= ld->total_nohs_dupla && lc->total_nohs >= ldd->total_nohs_dupla){
		totaldenohs = lc->total_nohs;
	} else if (ld->total_nohs_dupla >= lc->total_nohs && ld->total_nohs_dupla >= ldd->total_nohs_dupla) {
		totaldenohs = ld->total_nohs_dupla;
	} else if(ldd->total_nohs_dupla >= lc->total_nohs && ldd->total_nohs_dupla >= ld->total_nohs_dupla) {
		totaldenohs = ldd->total_nohs_dupla;
	}

	int quant = 0;

	Noh_Circular* nc = malloc(sizeof(Noh_Circular));
	nc = lc->cauda;
	Noh_Dupla* nd = malloc(sizeof(Noh_Dupla));
	nd = ld->cabeca;
	Noh_Dupla* ndd = malloc(sizeof(Noh_Dupla));
	ndd = ldd->cabeca;
	

	do {
		// LISTA PRONTO
		if(underflow_circular(lc)) {
			printf("       |");
			for(i = 0; i <= 59; ++i) {
				printf(" ");
			}
			putchar('|');			
		} else if(quantidade_circular < lc->total_nohs) {
			nc = nc->proximo;
			a = printf("       | Processo numero: %d", nc->proc.PID);
			for(i = 0; i < (32 - a); ++i) {
				printf(" ");
			}
			b = printf("Estado = %s", nc->proc.estado);
			for(j = 0; j < (52 - (a + b + i)); ++j) {
				printf(" ");
			}
			c = printf("Tempo : %02d:%02d:%02d", (int)nc->proc.tempo_necessario_processador / 3600, (int)nc->proc.tempo_necessario_processador / 60, (int)nc->proc.tempo_necessario_processador % 60);
			for(i = 0; i < ( 59 - (a + b + c + i + 1 ) ); ++i) {
				printf(" ");
			}
			putchar('|');			
			++quantidade_circular;
		} else {
			printf("       |");
			for(i = 0; i <= 59; ++i) {
				printf(" ");
			}
			putchar('|');
		}

		// LISTA ESPERA
		if(underflow_dupla(ld)) {
			for(i = 0; i <= 59; ++i) {
				printf(" ");
			}
			putchar('|');			
		} else if(quantidade_dupla < ld->total_nohs_dupla) {
			a = printf(" Processo numero: %d", nd->proc.PID);
			for(i = 0; i < (24 - a); ++i) {
				printf(" ");
			}
			b = printf("Estado = %s", nd->proc.estado);
			for(j = 0; j < (44 - (a + b + i)); ++j) {
				printf(" ");
			}
			c = printf("Tempo : %02d:%02d:%02d", (int)nd->proc.tempo_necessario_processador / 3600, (int)nd->proc.tempo_necessario_processador / 60, (int)nd->proc.tempo_necessario_processador % 60);
			putchar('|');			
			nd = nd->proximo;
			++quantidade_dupla;
		} else {
			printf(" ");
			for(i = 0; i < 59; ++i) {
				printf(" ");
			}
			putchar('|');
		}

		// LISTA TERMINADO
		if(underflow_dupla(ldd)) {
			for(i = 0; i <= 55; ++i) {
				printf(" ");
			}
			putchar('|');			
		} else if(quantidade_ddupla < ldd->total_nohs_dupla) {
			a = printf(" Processo numero: %d", ndd->proc.PID);
			for(i = 0; i < (23 - a); ++i) {
				printf(" ");
			}
			b = printf("%s", ndd->proc.estado);
			for(j = 0; j < (40 - (a + b + i)); ++j) {
				printf(" ");
			}
			c = printf("Tempo : %02d:%02d:%02d", (int)ndd->proc.tempo_necessario_processador / 3600, (int)ndd->proc.tempo_necessario_processador / 60, (int)ndd->proc.tempo_necessario_processador % 60);
			putchar('|');			
			ndd = ndd->proximo;
			++quantidade_ddupla;
		} else {
			printf(" ");
			for(i = 0; i < 55; ++i) {
				printf(" ");
			}
			putchar('|');
		}		

		putchar('\n');
		++quant;
	} while (quant < totaldenohs && quant <= 45);
		printf("       ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("\n");
	if(!underflow_circular(lc)) {
		printf(" Executando ...\n");
	}
	if(underflow_circular(lc) && !underflow_dupla(ld)) {
		printf(" Ocioso ...\n");
	}
}

