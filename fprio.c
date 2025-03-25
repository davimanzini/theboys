#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

// Cria uma fila vazia.
// Retorno: ponteiro para a fila criada ou NULL se erro.
struct fprio_t *fprio_cria (){
    
    struct fprio_t *f = malloc(sizeof(struct fprio_t));
    if(!f)
        return NULL;

    f->prim = NULL;
    f->num = 0;
    return f;
}

// Libera todas as estruturas de dados da fila, inclusive os itens.
// Retorno: NULL.
struct fprio_t *fprio_destroi (struct fprio_t *f){

    if(!fprio_tamanho(f)){
        free(f);
        return NULL;
    }

    struct fpnodo_t *aux = f->prim;
    
    for(int i = 0; i < f->num -1; i++){
        f->prim = f->prim->prox;
        free(aux->item);
        free(aux);
        aux = f->prim;
    }
    free(aux->item);
    free(aux);

    free(f);

    return NULL;

}

// Insere o item na fila, mantendo-a ordenada por prioridades crescentes.
// Itens com a mesma prioridade devem respeitar a politica FIFO (retirar
// na ordem em que inseriu).
// Inserir duas vezes o mesmo item (o mesmo ponteiro) é um erro.
// Retorno: número de itens na fila após a operação ou -1 se erro.
int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio){

    struct fpnodo_t *aux;
    struct fpnodo_t *posicao;
    struct fpnodo_t *confere;

    if(item == NULL){
        return -1;
    }

    if(!f){
        return -1;
    }
    
    aux = malloc(sizeof(struct fpnodo_t));
    if(!aux)
        return -1;

    posicao = f->prim;
    
    aux->prio = prio;
    aux->tipo = tipo;
    aux->item = item;


    if(f->prim == NULL){ //fila vazia
        f->prim = aux;
        aux->prox = NULL;
    }

    else if(prio < f->prim->prio){ //inserir na primeira posicao
        confere = posicao;
        while(confere != NULL){
            if(confere->item == item){
                free(aux);
                return -1;
            }
            confere = confere->prox;
        }
        aux->prox = f->prim;
        f->prim = aux;
    }

    else{ //inserir no meio/final
        while(posicao->prox != NULL && posicao->prox->prio <= prio){
            if(posicao->item == item){
                free(aux);
                return -1;
            }
            posicao = posicao->prox;
        }

        if(posicao->prox == NULL){ //final
            if(posicao->item == item){
                free(aux);
                return -1;
            }
            posicao->prox = aux;
            aux->prox = NULL;
        }
        else{ //meio
            confere = posicao;
            while(confere != NULL){
                if(confere->item == item){
                    free(aux);
                    return -1;
                }
                confere = confere->prox;
            }
            aux->prox = posicao->prox;
            posicao->prox = aux;
        }
    }
    (f->num) ++;
    return f->num;

}

// Retira o primeiro item da fila e o devolve; o tipo e a prioridade
// do item são devolvidos nos parâmetros "tipo" e "prio".
// Retorno: ponteiro para o item retirado ou NULL se fila vazia ou erro.
void *fprio_retira (struct fprio_t *f, int *tipo, int *prio){

    struct fpnodo_t *aux;
    void *retorno;

    if((!f) || (!f->prim) || (tipo == NULL) || (prio == NULL)) //teste de erros
        return NULL;
    
    aux = f->prim;
    f->prim = f->prim->prox;
    *tipo = aux->tipo;
    *prio = aux->prio;

    retorno = aux->item;
    free(aux);
    (f->num) --; 
    return retorno;
}

// Informa o número de itens na fila.
// Retorno: N >= 0 ou -1 se erro.
int fprio_tamanho (struct fprio_t *f){

    if(!f)
        return -1;
    
    return f->num;
}

// Imprime o conteúdo da fila no formato "(tipo prio) (tipo prio) ..."
// Para cada item deve ser impresso seu tipo e sua prioridade, com um
// espaço entre valores, sem espaços antes ou depois e sem nova linha.
void fprio_imprime (struct fprio_t *f){

    struct fpnodo_t *posicao;
    
    if(f->num == 0) //vazia
        return;
    
    posicao = f->prim;

    for(int i = 0; i < f->num -1; i++){
        printf("(%d %d) ",posicao->tipo, posicao->prio);
        posicao = posicao->prox;
    }
    printf("(%d %d)",posicao->tipo, posicao->prio);

}