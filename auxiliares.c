#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conjunto.h"
#include "fprio.h"
#include "auxiliares.h"
#include "lista.h"


#define T_FIM_DO_MUNDO 525600
#define T_INICIO 0
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)  // Cálculo direto
#define N_BASES (N_HEROIS / 5)       // Cálculo direto
#define N_MISSOES (T_FIM_DO_MUNDO / 100)  // Cálculo direto



//funcoes auxiliares

int gera_aleat(int minimo, int maximo) { //checar
    return minimo + rand() % (maximo - minimo + 1);
}

double calcula_distancia(int x1, int y1, int x2, int y2){
    return(sqrt( (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1) )) ;
} 

struct cjto_t *uniao_habs_herois(struct mundo_t *m, int base){ //calcula uniao de habs de heroi em uma certa base

    struct cjto_t *result, *aux;
    result = cjto_cria(N_BASES); //precisa ter os defines no evento.c/.h para funcionar qnd chamar essa func la?

    for(int h = 0; h < N_HEROIS; h++){
        if(cjto_pertence(m->bases[base].herois_presentes, m->herois[h].id_heroi)){ //checa se o heroi esta na base.
            aux = cjto_uniao(result, m->herois[h].habilidades);
            cjto_destroi(result);
            result = aux;
        }
    }
    return result;
}

int minimo_tent(struct mundo_t *m){

    int min_tent = m->missoes[0].tentativas;
    for(int i = 1; i < m->num_missoes; i++){

       if(m->missoes[i].tentativas < min_tent){
            min_tent = m->missoes[i].tentativas;
       }
   }

   return min_tent;
}

int maximo_tent(struct mundo_t *m){

    int max_tent = m->missoes[0].tentativas;
    for(int i = 1; i < m->num_missoes; i++){

       if(m->missoes[i].tentativas > max_tent){
            max_tent = m->missoes[i].tentativas;
       }
   }

    return max_tent;
}


int media_tent(struct mundo_t *m){

    int soma = 0;

    for(int i = 0; i < m->num_missoes; i++){
        soma = soma + m->missoes[i].tentativas;
    }

    return soma / m->num_missoes;
}


double taxa_mortalidade(struct mundo_t *m){

    int mortos = 0;

    for (int i = 0; i < m->num_herois; i++) {
        if (!m->herois[i].status) {
            mortos++;
        }
    }

    return (double)mortos / m->num_herois * 100; // Conversão para double para evitar divisão inteira
}

   

// inicializa um heroi
struct heroi_t *inicializa_heroi(struct heroi_t *h, int id){

    h->id_heroi = id;
    h->experiencia = 0;
    h->paciencia = gera_aleat(0,100);
    h->velocidade = gera_aleat(50, 5000);
    h->status = 1; //vivo
    h->habilidades = cjto_aleat(gera_aleat(1,3), N_HABILIDADES);

    return h;

}

struct base_t *inicializa_base(struct base_t *b, int id){

    b->id_base = id;
    b->x_base = gera_aleat(0, N_TAMANHO_MUNDO - 1);
    b->y_base = gera_aleat(0, N_TAMANHO_MUNDO - 1);
    b->lotacao = gera_aleat(3, 10);
    b->herois_presentes = cjto_cria(50); //mudar
    b->espera = lista_cria(); //fila vazia
    b->maximo = 0;
    b->missoes_participadas = 0;

    return b;

}

struct missao_t *inicializa_missao(struct missao_t *m, int id){

    m->id_missao = id;
    m->tentativas = 0; //checar
    m->x_miss = gera_aleat(0, N_TAMANHO_MUNDO - 1);
    m->y_miss = gera_aleat(0, N_TAMANHO_MUNDO - 1);
    m->habilidades_necess = cjto_aleat(gera_aleat(6, 10), N_HABILIDADES);
    m->perigo = gera_aleat(0, 100);

    return m;
}

// Cria um mundo
// Retorno: ponteiro para a o mundo criado ou NULL se erro.
struct mundo_t *mundo_cria(){

    struct mundo_t *mundo = malloc(sizeof(struct mundo_t));
    if(!mundo)
        return NULL;

    //todos os mallocs iniciais vao aqui 

    //tempo final e inicial? usar essa variavel relogio?
    mundo->relogio = T_INICIO;
    mundo->x_mundo = N_TAMANHO_MUNDO;
    mundo->y_mundo = N_TAMANHO_MUNDO;
    mundo->num_habilidades = N_HABILIDADES;
    mundo->num_herois = N_HEROIS;
    mundo->num_bases = N_BASES;
    mundo->num_missoes = N_MISSOES;
    mundo->ev_tratados = 0;
    mundo->missoes_cumpridas = 0;

    //mallocs

    mundo->herois = malloc (N_HEROIS * sizeof (struct heroi_t)) ;
    if(!mundo->herois)
        return NULL; //criar funcao para erro?
    
    mundo->bases = malloc (N_BASES * sizeof(struct base_t));
    if(!mundo->bases)
        return NULL;

    mundo->missoes = malloc(N_MISSOES * sizeof(struct missao_t));
    if(!mundo->missoes)
        return NULL;
    
    //lef
    mundo->lef = fprio_cria();

    //inicializacoes

    for(int i = 0; i < N_HEROIS; i++){
        inicializa_heroi(&mundo->herois[i], i); //por que usar o endereço?
    }

    for(int i = 0; i < N_BASES; i++){
        inicializa_base(&mundo->bases[i], i);
    }

    for(int i = 0; i < N_MISSOES; i++){
        inicializa_missao(&mundo->missoes[i], i);
    }

    return mundo;
}

struct mundo_t *mundo_destroi(struct mundo_t *m){
    
    for(int i = 0; i < N_HEROIS; i++){
        cjto_destroi(m->herois[i].habilidades);
    }

    for(int i = 0; i < N_BASES; i++){
        lista_destroi(m->bases[i].espera);
        cjto_destroi(m->bases[i].herois_presentes);
    }

    for(int i = 0; i < N_MISSOES; i++){
        cjto_destroi(m->missoes[i].habilidades_necess);
    }

    fprio_destroi(m->lef);
    free(m->herois);
    free(m->bases);
    free(m->missoes);
    free(m);

    return NULL;
}