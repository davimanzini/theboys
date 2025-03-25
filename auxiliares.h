
//ifdef aqui? tem que colocar alguma coisa?

#ifndef AUXILIARES
#define AUXILIARES

#include <stdbool.h> //colocar isso? quais includes usar?

struct heroi_t{
  int id_heroi;
  struct cjto_t *habilidades; //checar
  int paciencia;
  int velocidade;
  int experiencia;
  int base;
  bool status; //1 para vivo e 0 pra morto
};

struct base_t{
  int id_base;
  int lotacao;
  struct cjto_t *herois_presentes;
  struct lista_t *espera;
  int x_base;
  int y_base;
  int maximo;
  int missoes_participadas;
};

struct missao_t{
  int id_missao;
  struct cjto_t *habilidades_necess;
  int perigo;
  int x_miss;
  int y_miss;
  int tentativas;
};


struct mundo_t{
  int num_herois;
  struct heroi_t *herois;
  int num_bases;
  struct base_t *bases;
  int num_missoes;
  struct missao_t *missoes;
  int num_habilidades;
  int x_mundo;
  int y_mundo;
  int relogio;
  struct fprio_t *lef;
  int ev_tratados;
  int missoes_cumpridas;
} ;

//funcoes

//Gera e retorna um numero aleatório entre minimo e maximo
int gera_aleat(int minimo, int maximo);

//Calcula a distância entre dois pontos dadas suas coordenadas
double calcula_distancia(int x1, int y1, int x2, int y2);

//Gera o conjunto união das habilidade dos herois em uma base
//Retorna ponteiro para o conjunto união
struct cjto_t *uniao_habs_herois(struct mundo_t *m, int base);

//Calcula e retorna o numero minimo de tentativas
//que uma missao obteve
int minimo_tent(struct mundo_t *m);

//Calcula e retorna o numero maximo de tentativas
//que uma missao obteve
int maximo_tent(struct mundo_t *m);

//Calcula o numero medio de tentativas
//que uma missao obteve
int media_tent(struct mundo_t *m);

//Calcula a taxa de mortalidade da simulacao
double taxa_mortalidade(struct mundo_t *m);

//Inicializa um herói com suas informações iniciais
//Retorna um ponteiro para o herói criado
struct heroi_t *inicializa_heroi(struct heroi_t *h, int id);

//Inicializa uma base com suas informações iniciais
//Retorna um ponteiro para a base criada
struct base_t *inicializa_base(struct base_t *b, int id);

//Inicializa uma missão com suas informações iniciais
//Retorna um ponteiro para a missão criada
struct missao_t *inicializa_missao(struct missao_t *m, int id);

//Cria o mundo da simulação
//Retorna um ponteiro para o mundo criado
struct mundo_t *mundo_cria();


// Libera todas as estruturas de dados do mundo
// Retorno: NULL
struct mundo_t *mundo_destroi(struct mundo_t *m);

#endif