#ifndef PROCESSO_H
#define PROCESSO_H

#include <stdbool.h>

#define MAX 12

struct processo{
	unsigned PID;
	float tempo_necessario_processador;
	float tempo_executando;
	int h_entrada, m_entrada, s_entrada;
	int h_saida, m_saida, s_saida;
	int entrada_espera_h, entrada_espera_m, entrada_espera_s, tempo_espera_h, tempo_espera_m, tempo_espera_s;
	int pronto_h, pronto_m, pronto_s;
	char estado[MAX];
};
typedef struct processo Processo;

#endif
