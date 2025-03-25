#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conjunto.h"
#include "fprio.h"
#include "auxiliares.h"
#include "eventos.h"
#include "lista.h"



void trata_ev_chega(struct mundo_t *m, struct evento_t *ev){

    bool espera;

    m->herois[ev->heroi].base = ev->base;

    if((cjto_card(m->bases[ev->base].herois_presentes) < m->bases[ev->base].lotacao) && (!lista_tamanho(m->bases[ev->base].espera))){
        espera = 1;
    }

    else{
        espera = (m->herois[ev->heroi].paciencia) > (10 * lista_tamanho(m->bases[ev->base].espera));
    }


    if(espera){
        struct evento_t *novo_ev = malloc(sizeof(struct evento_t));
        if(!novo_ev)
            return;
        novo_ev->tipo  = ESPERA;
        novo_ev->tempo = m->relogio;
        novo_ev->heroi = ev->heroi;
        novo_ev->base  = ev->base;
        fprio_insere(m->lef, novo_ev, novo_ev->tipo, novo_ev->tempo); //agora?
        
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", //é aqui mesmo?
        ev->tempo, ev->heroi, ev->base,
        cjto_card(m->bases[ev->base].herois_presentes), m->bases[ev->base].lotacao);//checar

    }

    else{
        struct evento_t *novo_ev = malloc(sizeof(struct evento_t));
        if(!novo_ev)
            return;
        
        novo_ev->tipo  = DESISTE;
        novo_ev->tempo = m->relogio;
        novo_ev->heroi = ev->heroi;
        novo_ev->base  = ev->base;
        fprio_insere(m->lef, novo_ev, novo_ev->tipo, novo_ev->tempo);

        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n",
        ev->tempo, ev->heroi, ev->base,
        cjto_card(m->bases[ev->base].herois_presentes), m->bases[ev->base].lotacao);//checar
    }

}


void trata_ev_espera(struct mundo_t *m, struct evento_t *ev){

    lista_insere(m->bases[ev->base].espera, m->herois[ev->heroi].id_heroi, -1);

    if(m->bases[ev->base].maximo < lista_tamanho(m->bases[ev->base].espera)){
        m->bases[ev->base].maximo = lista_tamanho(m->bases[ev->base].espera);
        }
    
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", 
    ev->tempo, ev->heroi, ev->base, lista_tamanho(m->bases[ev->base].espera));

    struct evento_t *novo_ev = malloc(sizeof(struct evento_t));
    if(!novo_ev)
        return;
    novo_ev->tipo  = AVISA;
    novo_ev->tempo = m->relogio;
    novo_ev->base  = ev->base;
    novo_ev->heroi = ev->heroi;
    fprio_insere(m->lef, novo_ev, novo_ev->tipo, novo_ev->tempo); 

}


void trata_ev_desiste(struct mundo_t *m, struct evento_t *ev){

    printf("%6d: DESIST HEROI %2d BASE %d\n", ev->tempo, ev->heroi, ev->heroi);

    struct evento_t *novo_ev = malloc(sizeof(struct evento_t));
    if(!novo_ev)
        return;
    novo_ev->tipo    = VIAJA;
    novo_ev->tempo   = m->relogio;
    novo_ev->heroi   = ev->heroi;
    novo_ev->destino = gera_aleat(0, m->num_bases - 1);
	novo_ev->base = ev->base;
	novo_ev->missao = ev->missao;
	fprio_insere(m->lef, novo_ev, novo_ev->tipo, novo_ev->tempo);
}


void trata_ev_avisa(struct mundo_t *m, struct evento_t *ev){

   printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [", 
       ev->tempo, 
       ev->base, 
       cjto_card(m->bases[ev->base].herois_presentes), 
       m->bases[ev->base].lotacao);

    lista_imprime(m->bases[ev->base].espera); // Exibe os IDs dos heróis na fila

    printf(" ]\n");


    while((cjto_card(m->bases[ev->base].herois_presentes) < m->bases[ev->base].lotacao) && 
          (lista_tamanho(m->bases[ev->base].espera) > 0)){ //checar
        int id_heroi;
        lista_retira(m->bases[ev->base].espera, &id_heroi, 0); //olhar lisa.c, id_heroi vai receber o item retirado
        cjto_insere(m->bases[ev->base].herois_presentes, id_heroi);

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", ev->tempo, ev->base, id_heroi); //checar, principalmente esse id heroi


        struct evento_t *novo_ev = malloc(sizeof(struct evento_t));
        if(!novo_ev)
            return; //nao da problema por estar dentro de um loop?
        novo_ev->tipo  = ENTRA;
        novo_ev->tempo = m->relogio;
        novo_ev->heroi = id_heroi;
        novo_ev->base  = ev->base; //checar
        fprio_insere(m->lef, novo_ev, novo_ev->tipo, novo_ev->tempo);

    }
}


void trata_ev_entra(struct mundo_t *m, struct evento_t *ev){

    int tpb; //tempo de permanencia na base
    tpb = 15 + (m->herois[ev->heroi].paciencia * gera_aleat(1, 20));

    struct evento_t *novo_ev = malloc(sizeof(struct evento_t));
    if(!novo_ev)
        return;
    novo_ev->tipo  = SAI;
    novo_ev->tempo = m->relogio + tpb; //to atualizando o mundo relogio?
    novo_ev->heroi = ev->heroi;
    novo_ev->base  = ev->base;
    fprio_insere(m->lef, novo_ev, novo_ev->tipo, novo_ev->tempo);

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", 
        ev->tempo, ev->heroi, ev->base, cjto_card(m->bases[ev->base].herois_presentes),
        m->bases[ev->base].lotacao, novo_ev->tempo); //checar, principalmente esse novo_ev

}


void trata_ev_sai(struct mundo_t *m, struct evento_t *ev){

    if(!m->herois[ev->heroi].status){
        m->ev_tratados --;
        return;
    }

    cjto_retira(m->bases[ev->base].herois_presentes, ev->heroi);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", 
        ev->tempo, ev->heroi, ev->base, cjto_card(m->bases[ev->base].herois_presentes),
        m->bases[ev->base].lotacao);


    struct evento_t *novo_ev1 = malloc(sizeof(struct evento_t));
    if(!novo_ev1)
        return;
    novo_ev1->tipo    = VIAJA;
    novo_ev1->tempo   = m->relogio;
    novo_ev1->heroi   = ev->heroi;
	novo_ev1->base	  = ev->base;
    novo_ev1->destino = gera_aleat(0, 9); //calcular antes?
    fprio_insere(m->lef, novo_ev1, novo_ev1->tipo, novo_ev1->tempo);

    struct evento_t *novo_ev2 = malloc(sizeof(struct evento_t));
    if(!novo_ev2)
        return;
    novo_ev2->tipo    = AVISA;
    novo_ev2->tempo   = m->relogio;
	novo_ev2->heroi   = ev->heroi;
    novo_ev2->base    = ev->base;
	novo_ev2->destino = ev->destino;
    fprio_insere(m->lef, novo_ev2, novo_ev2->tipo, novo_ev2->tempo);
}


void trata_ev_viaja(struct mundo_t *m, struct evento_t *ev){

    //float duracao, distancia;
    int distancia;
    int duracao;

    // calcula distancia e duracao da viagem entre base e destino
    distancia = calcula_distancia (m->bases[ev->base].x_base,
                                   m->bases[ev->base].y_base,
                                   m->bases[ev->destino].x_base,
                                   m->bases[ev->destino].y_base);
    duracao = distancia / m->herois[ev->heroi].velocidade;

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", 
        ev->tempo, ev->heroi, ev->base, ev->destino, distancia,
        m->herois[ev->heroi].velocidade, ev->tempo + duracao); //checar. checar velocidade tb

    ev->base = ev->destino;

    struct evento_t *novo_ev = malloc(sizeof(struct evento_t));
    if(!novo_ev)
        return;
    novo_ev->tipo    = CHEGA;
    novo_ev->tempo   = m->relogio + duracao;
    novo_ev->heroi   = ev->heroi;
    novo_ev->destino = ev->destino; //evento chega trata destino?? checar
    novo_ev->base    = ev->base;
    fprio_insere(m->lef, novo_ev, novo_ev->tipo, novo_ev->tempo);


}


void trata_ev_morre(struct mundo_t *m, struct evento_t *ev){

    cjto_retira(m->bases[ev->base].herois_presentes, ev->heroi);
    m->herois[ev->heroi].status = 0;

    printf("%6d: MORRE  HEROI %2d MISSAO %d\n", ev->tempo, ev->heroi, ev->missao); //checar


    struct evento_t *novo_ev = malloc(sizeof(struct evento_t));
    if(!novo_ev)
        return;
    novo_ev->tipo  = AVISA;
    novo_ev->tempo = m->relogio;
    novo_ev->base  = ev->base;
    fprio_insere(m->lef, novo_ev, novo_ev->tipo, novo_ev->tempo);
}


void trata_ev_missao(struct mundo_t *m, struct evento_t *ev){

    m->missoes[ev->missao].tentativas ++; //checar

    printf("%6d: MISSAO %d TENT %d HAB REQ: [", ev->tempo, ev->missao,
        m->missoes[ev->missao].tentativas);
        cjto_imprime(m->missoes[ev->missao].habilidades_necess); // checar
        printf(" ]\n");


    int *dist = malloc(m->num_bases * sizeof(int)); // distancia respectiva para a missao
    if(!dist)
        return;
    int *bases_ord = malloc(m->num_bases * sizeof(int)); //id das bases
    if(!bases_ord)
        return;

    for(int i = 0; i < m->num_bases; i++){
        bases_ord[i] = m->bases[i].id_base;
        dist[i]= calcula_distancia (m->bases[i].x_base,
                                   m->bases[i].y_base,
                                   m->missoes[ev->missao].x_miss,
                                   m->missoes[ev->missao].y_miss);
    }

    //Bubble Sort implementado para ordenar os vetores dist e bases
    //simultaneamente. Cria um link entre cada base e sua distancia 
    //para a missao
    int aux; // declarar em cima? 
    for (int a = 0; a < m->num_bases; a++) {
        for (int b = 0; b < m->num_bases - a - 1; b++) {
            if (dist[b] > dist[b + 1]) {
                aux = dist[b];
                dist[b] = dist[b + 1];
                dist[b + 1] = aux;

                aux = bases_ord[b];
                bases_ord[b] = bases_ord[b + 1];
                bases_ord[b + 1] = aux;
            }
        }
    }

    int bmp = -1;
    struct cjto_t *habs_base; //checar se é aqui que faço a delcaração

    for(int b = 0; b < m->num_bases; b++){
        habs_base = uniao_habs_herois(m, bases_ord[b]);
        if(cjto_contem(habs_base, m->missoes[ev->missao].habilidades_necess)){  
            bmp = bases_ord[b];
            cjto_destroi(habs_base);
            break;
        } 
        else {
            cjto_destroi(habs_base);
        }
    }

    if(bmp != -1){
		struct cjto_t *uniao = uniao_habs_herois(m, bmp);

        //marcar missao como concluida
        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [", ev->tempo, ev->missao, bmp); //checar
            cjto_imprime(uniao); // checar
            printf(" ]\n");
	
		cjto_destroi(uniao);

        m->bases[bmp].missoes_participadas ++;
        m->missoes_cumpridas ++;


        double risco;
        for(int h = 0; h < m->num_herois; h++){
            if(cjto_pertence(m->bases[bmp].herois_presentes, h)){
                risco = m->missoes[ev->missao].perigo / (m->herois[h].paciencia +
                m->herois[h].experiencia + 1.0); //verificar identa. e valores
                
                if(risco > gera_aleat(1, 30)){ //sujestao do nathan
                    struct evento_t *novo_ev1 = malloc(sizeof(struct evento_t));
                    if(!novo_ev1)
                        return;
                    novo_ev1->tipo   = MORRE;
                    novo_ev1->tempo  = m->relogio;
                    novo_ev1->heroi  = h; //checar
                    novo_ev1->base   = bmp;
                    novo_ev1->missao = ev->missao;
                    fprio_insere(m->lef, novo_ev1, novo_ev1->tipo, novo_ev1->tempo);
                }
                else{
                    m->herois[h].experiencia ++; //checar
                }
            }
        }
    }
    else{
        //missao impossivel
        printf("%6d: MISSAO %d IMPOSSIVEL\n", ev->tempo, ev->missao);


        struct evento_t *novo_ev2 = malloc(sizeof(struct evento_t));
        if(!novo_ev2)
            return;
        novo_ev2->tipo   = MISSAO;
        novo_ev2->tempo  = m->relogio + 24 * 60; //checar esse mrelogio sus
        novo_ev2->missao = ev->missao;
        fprio_insere(m->lef, novo_ev2, novo_ev2->tipo, novo_ev2->tempo);

    }

	free(dist);
	free(bases_ord);
}

void trata_ev_fim(struct mundo_t *m, struct evento_t *ev){

    printf("%6d: FIM\n", ev->tempo);

    //Estatisticas dos herois
    for(int i = 0; i < m->num_herois; i++){

        if(m->herois[i].status){ //herois vivos
            printf("HEROI %2d %s PAC %3d VEL %4d EXP %4d HABS [",
            i, "VIVO", m->herois[i].paciencia, m->herois[i].velocidade, m->herois[i].experiencia);
            cjto_imprime(m->herois[i].habilidades);
            printf(" ]\n");
        }
        else if(!m->herois[i].status){ //herois mortos
            printf("HEROI %2d %s PAC %3d VEL %4d EXP %4d HABS [",
            i, "MORTO", m->herois[i].paciencia, m->herois[i].velocidade, m->herois[i].experiencia);
            cjto_imprime(m->herois[i].habilidades);
            printf(" ]\n");
        }
        
    }
    
    //estatisticas das bases
    for(int j = 0; j < m->num_bases; j++){

        printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n",
        j, m->bases[j].lotacao, m->bases[j].maximo, m->bases[j].missoes_participadas);
    }
    
    // Relatório geral
    
    printf("EVENTOS TRATADOS: %d\n", m->ev_tratados);

    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", m->missoes_cumpridas, 5256, 
       100.0 * m->missoes_cumpridas / 5256);

    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1d\n", minimo_tent(m), maximo_tent(m),
        media_tent(m)); //checar se passar m assim da certo

    printf("TAXA MORTALIDADE: %.1f%%\n", taxa_mortalidade(m));
   
    struct evento_t *libera_ev; //checar
    for(int i = 0; i < fprio_tamanho(m->lef); i++){
        int prio, tipo;
        libera_ev = fprio_retira(m->lef, &tipo, &prio);

        free(libera_ev);
        libera_ev = NULL;
    }
   
}

void eventos_iniciais(struct mundo_t *m){

    for(int i = 0; i < m->num_herois; i++){
        struct evento_t *ev = malloc(sizeof(struct evento_t));

        ev->tipo  = CHEGA;
        ev->tempo = gera_aleat(0, 4320);
        ev->heroi = i;
        ev->base  = gera_aleat(0,m->num_bases - 1);
        fprio_insere(m->lef, ev, ev->tipo, ev->tempo);
    }

    for(int i = 0; i < m->num_missoes; i++){
        struct evento_t *ev = malloc(sizeof(struct evento_t));

        ev->tipo   = MISSAO;
        ev->tempo  = gera_aleat(0, 525600);
        ev->missao = i ;
        fprio_insere(m->lef, ev, ev->tipo, ev->tempo);
    }
    
    struct evento_t *ev = malloc(sizeof(struct evento_t)); //checar
    ev->tipo  = FIM;
    ev->tempo = 525600;
    fprio_insere(m->lef, ev, ev->tipo, ev->tempo);
    
}
