// programa principal do projeto "The Boys - 2024/2"
// Autor: Davi Chaves Rodrigues Dutra Manzini, GRR 20245482

// seus #includes vão aqui

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // necessario?
#include <math.h>
#include <time.h>
#include "lista.h"
#include "fprio.h"
#include "conjunto.h"
#include "auxiliares.h"
#include "eventos.h"


// seus #defines vão aqui

#define T_FIM_DO_MUNDO 525600
#define T_INICIO 0
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)  // Cálculo direto
#define N_BASES (N_HEROIS / 5)       // Cálculo direto
#define N_MISSOES (T_FIM_DO_MUNDO / 100)  // Cálculo direto


// programa principal
int main (){

  srand (1);

  struct mundo_t *mundo = mundo_cria(); //tem que dar malloc?

  eventos_iniciais(mundo);

//executar o laço de simulação
  struct evento_t *ev;
  bool fim = false;
  

  while(!fim){

    int tipo;
    int prioridade;

    ev = fprio_retira(mundo->lef, &tipo, &prioridade);
    mundo->relogio = ev->tempo;
  
    switch(ev->tipo){

      case CHEGA:
        trata_ev_chega(mundo, ev);
        mundo->ev_tratados ++;
        break;

      case ESPERA:
        trata_ev_espera(mundo, ev);
        mundo->ev_tratados ++;
        break;
      
      case DESISTE:
        trata_ev_desiste(mundo, ev);
        mundo->ev_tratados ++;
        break;

      case AVISA:
        trata_ev_avisa(mundo, ev);
        mundo->ev_tratados ++;
        break;
      
      case ENTRA:
        trata_ev_entra(mundo, ev);
        mundo->ev_tratados ++;
        break;
      
      case SAI:
        trata_ev_sai(mundo, ev);
        mundo->ev_tratados ++;
        break;

      case VIAJA:
        trata_ev_viaja(mundo, ev);
        mundo->ev_tratados ++;
        break;

      case MORRE:
        trata_ev_morre(mundo, ev);
        mundo->ev_tratados ++;
        break;
      
      case MISSAO:
        trata_ev_missao(mundo, ev);
        mundo->ev_tratados ++;
        break;

      case FIM:
        trata_ev_fim(mundo, ev);
        mundo->ev_tratados ++;
        fim = true;
    }
    free(ev);
  }

  mundo_destroi(mundo); //checar


  return (0) ;
}

