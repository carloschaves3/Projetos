#ifndef LISTAS_H
#define LISTAS_H

#include <stdbool.h>
#include "processo.h"

typedef struct lista_circular Lista_Circular;
typedef struct lista_dupla Lista_Dupla;
typedef struct processo Processo;

Lista_Circular* cria_circular(void);
bool underflow_circular(const Lista_Circular*);
void destroi_circular(Lista_Circular*);
void insere_circular(Lista_Circular*, Processo);
Processo remove_circular(Lista_Circular*);
void dump_circular(const Lista_Circular*);
void ciclo_circular(Lista_Circular*, Lista_Dupla*, float);

Lista_Dupla* cria_dupla(void);
bool underflow_dupla(Lista_Dupla*);
void insere_dupla(Lista_Dupla*, Processo);
Processo remove_dupla(Lista_Dupla*, Processo);
void destroi_dupla(Lista_Dupla*);
void dump_dupla(Lista_Dupla*);

int tempo_rand(void);
Processo cria_processo(void);
struct tm* hora_atual(void);
void ciclo_escalonador(Lista_Circular*, Lista_Dupla*, Lista_Dupla*, float, unsigned int, unsigned int, int);
void relatorio_processo(Lista_Dupla*, float);
bool evento_random_circular(Lista_Circular*, Lista_Dupla*, Processo);
void print_escalonador(Lista_Circular*, Lista_Dupla*, Lista_Dupla*);

#endif
