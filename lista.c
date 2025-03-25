#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

// Cria uma lista vazia.
// Retorno: ponteiro p/ a lista ou NULL em erro.
struct lista_t *lista_cria (){
    
    struct lista_t *lst; 

    lst = malloc(sizeof(struct lista_t)); 
    if(!lst)
        return NULL;

    lst->prim = NULL;
    lst->ult = NULL;
    lst->tamanho = 0;
    return lst;
}

// Remove todos os itens da lista e libera a memória.
// Retorno: NULL.
struct lista_t *lista_destroi (struct lista_t *lst){
    
    struct item_t *aux = lst->prim;
    while(aux != lst->ult){
        lst->prim = aux->prox;
        free(aux);
        aux = lst->prim;
    }
    free(lst->ult);
	free(lst);
    return NULL;
}

// Nas operações insere/retira/consulta/procura, a lista inicia na
// posição 0 (primeiro item) e termina na posição TAM-1 (último item).

// Insere o item na lista na posição indicada;
// se a posição for além do fim da lista ou for -1, insere no fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_insere (struct lista_t *lst, int item, int pos){
    
    struct item_t *aux;

    if(!lst){
        return -1;
	}
    
    aux = malloc(sizeof(struct item_t)); 
    if(!aux){
        return -1;
	}

    aux->valor = item;
    aux->prox = NULL; 
    aux->ant = NULL;

    if(!lst->prim){ //lista vazia
        lst->prim = aux;
        lst->ult = aux;
    }
    

    else if(pos == 0){ //inicio
        aux->prox = lst->prim;
        lst->prim->ant = aux;
        lst->prim = aux;
    }

    else if((pos > ((lst->tamanho) - 1)) || pos == -1){ //fim
        aux->ant = lst->ult;
        lst->ult->prox = aux;
        lst->ult = aux;
    }

    else{ //meio
        struct item_t *posicao;
        posicao = lst->prim; //ponteiro auxiliar comeca no primeiro nodo
        for(int i = 0; i < pos - 1; i++ ){ 
            posicao = posicao->prox; //movendo o ponteiro aux(pont. posicao)
        }
        aux->prox = posicao->prox; 
        aux->ant = posicao;
        posicao->prox->ant = aux;
        posicao->prox = aux;
    }
	(lst->tamanho)++;
    return lst->tamanho;
}

// Retira o item da lista da posição indicada.
// se a posição for -1, retira do fim.
// Retorno: número de itens na lista após a operação ou -1 em erro.
int lista_retira (struct lista_t *lst, int *item, int pos){
    
    struct item_t *aux;

    if((!lst) || (!lst->prim) || (pos > lst->tamanho))//lista vazia, nao existe, ou retira de posicao nao existente
        return -1;

    if(lst->prim == lst->ult){ //apenas 1 elemento na lista
        aux = lst->prim;
        lst->prim = NULL;
        lst->ult = NULL;
        *item = aux->valor;
        free(aux);
    }

    else if(pos == 0){
        aux = lst->prim;
        lst->prim = lst->prim->prox;
        lst->prim->ant = NULL;
        *item = aux->valor;
        free(aux);
    }

    else if((pos == lst->tamanho -1) || (pos == -1)){
        aux = lst->ult;
        lst->ult = lst->ult->ant;
        lst->ult->prox = NULL;
        *item = aux->valor;
        free(aux);
    }

    else{
        struct item_t *posicao;
        posicao = lst->prim;
        for(int i = 0; i < pos; i++ ){
            posicao = posicao->prox;
        }
        posicao->ant->prox = posicao->prox;
        posicao->prox->ant = posicao->ant;
        *item = posicao->valor;
        free(posicao);
    }
	(lst->tamanho)--;
    return lst->tamanho;
}

// Informa o valor do item na posição indicada, sem retirá-lo.
// se a posição for -1, consulta do fim.
// Retorno: número de itens na lista ou -1 em erro.
int lista_consulta (struct lista_t *lst, int *item, int pos){

    if((!lst) || (!lst->prim)){
        return -1;
    }

    if(pos >= lst->tamanho){
        return -1;
    }

    if(pos == -1){
        *item = lst->ult->valor;
    }
    else{
        struct item_t *posicao;
        posicao = lst->prim;
        for(int i = 0; i < pos; i++ ){
            posicao = posicao->prox;
        }
        *item = posicao->valor;
    }
    return lst->tamanho;
}

// Informa a posição da 1ª ocorrência do valor indicado na lista.
// Retorno: posição do valor ou -1 se não encontrar ou erro.
int lista_procura (struct lista_t *lst, int valor){

    int i;

    if(!lst){
        return -1;
    }

    struct item_t *aux = lst->prim;
    for(i = 0; i < lst->tamanho; i++){
        if(valor == aux->valor){
            return i;
        }
        if(aux->prox == NULL){ //se for o ultmo elemento da lista e nao achou
            return -1;
        }
        aux = aux->prox;
    }
    return i;
}

// Informa o tamanho da lista (o número de itens presentes nela).
// Retorno: número de itens na lista ou -1 em erro.
int lista_tamanho (struct lista_t *lst){

    if(!lst){
        return -1;
    }

    return lst->tamanho;
}

// Imprime o conteúdo da lista do inicio ao fim no formato "item item ...",
// com um espaço entre itens, sem espaços antes/depois, sem newline.
void lista_imprime (struct lista_t *lst){

    struct item_t *posicao;
    posicao = lst->prim;


	if (lst->tamanho == 0) 
		return;

    for(int i = 0; i < lst->tamanho - 1; i++ ){
        printf("%d ", posicao->valor); //QUESTAO DOS ESPAÇOS
        posicao = posicao->prox;
    }
    printf("%d", lst->ult->valor);
}