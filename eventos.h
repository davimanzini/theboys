

#ifndef EVENTOS
#define EVENTOS

#include <stdbool.h> //usar isso? quais includes usar?s


#define CHEGA 1
#define ESPERA 2
#define DESISTE 3
#define AVISA 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define FIM 10


struct evento_t{
    int tipo;
    int tempo;
    int heroi;
    int base;
    int destino;
    int missao;
};

//Função que trata do evento Chega, criando os devidos novos
//eventos e fazendo as devidas alterações na LEF
void trata_ev_chega(struct mundo_t *m, struct evento_t *ev);

//Função que trata do evento Chega, criando os devidos novos
//eventos e fazendo as devidas alterações na LEF
void trata_ev_espera(struct mundo_t *m, struct evento_t *ev);

//Função que trata do evento Chega, criando os devidos novos
//eventos e fazendo as devidas alterações na LEF
void trata_ev_desiste(struct mundo_t *m, struct evento_t *ev);

//Função que trata do evento Chega, criando os devidos novos
//eventos e fazendo as devidas alterações na LEF
void trata_ev_avisa(struct mundo_t *m, struct evento_t *ev);

//Função que trata do evento Chega, criando os devidos novos
//eventos e fazendo as devidas alterações na LEF
void trata_ev_entra(struct mundo_t *m, struct evento_t *ev);

//Função que trata do evento Chega, criando os devidos novos
//eventos e fazendo as devidas alterações na LEF
void trata_ev_sai(struct mundo_t *m, struct evento_t *ev);

//Função que trata do evento Chega, criando os devidos novos
//eventos e fazendo as devidas alterações na LEF
void trata_ev_viaja(struct mundo_t *m, struct evento_t *ev);

//Função que trata do evento Chega, criando os devidos novos
//eventos e fazendo as devidas alterações na LEF
void trata_ev_morre(struct mundo_t *m, struct evento_t *ev);

//Função que trata do evento Chega, criando os devidos novos
//eventos e fazendo as devidas alterações na LEF
void trata_ev_missao(struct mundo_t *m, struct evento_t *ev);

//Função que trata do evento Chega, criando os devidos novos
//eventos e fazendo as devidas alterações na LEF
void trata_ev_fim(struct mundo_t *m, struct evento_t *ev);

//Funcao que trata os eventos iniciais da simulacao
void eventos_iniciais(struct mundo_t *m);

#endif