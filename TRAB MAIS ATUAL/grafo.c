/*****************************************************************/
/*   Grafo direcionado | PROG2 | MIEEC | 2019/20        */      
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

//Observaçoes sobre o trabalho:
//Meu colega de grupo não participou;
//A minha stnova nao da-me bem os resultados, mas creio que esta correto o modo de pensar
//nao consegui realizar corretamente a inserção dos elementos


grafo* grafo_novo()
{
    grafo* grf = (grafo*)malloc(sizeof(grafo)); //aloca memória pro grafo
    grf->nos = (no_grafo**)malloc(sizeof(no_grafo*)); //aloca memória para os nós
    grf->tamanho = 0; //novo grafo inicializa com tamanho nulo, pois nao há intancias dentro dele ainda
    if(grf == NULL || grf->nos == NULL)//condição de erro
    {
       return NULL;
    }
    return grf;
}


void grafo_apaga(grafo* g)
{
  if(g==NULL)//condição de erro, se estiiver vazio não há oq apagar
  {
      return;           
  }
  int i,j, tam;
  tam = g->tamanho; // atribui a uma variavel o tamanho do grafo
  if(g->nos == NULL)// condicao de erro, se nos estao vazios nao ha oq apagar
  {
      return;
  }
  else 
  {
      for(i=0;i<tam;i++)//abaixo os ciclos percorrem todas as posições de dos nos
      {
          for(j=0;j<g->nos[i]->tamanho; j++)//e para cada posiçao do no, tambem percorre suas ligaçoes
                                            //ou seja, para cada i, temos ligaçoes um ciclo completo de j
          {
            free(g->nos[i]->ligacoes[j]);     //liberta a memoria de todas as posiçoes das ligaçoes
          }   

          free(g->nos[i]->ligacoes);// liberta memoria da instancia ligaçoes
          free(g->nos[i]);   //liberta memoria de todas a posiçoes dos nos
      }  
      free(g->nos);    //liberta memoria da instancia nos
  }
  free(g); //liberta memoria do grafo
}


no_grafo * no_insere(grafo *g, char *user)
{
    if(g==NULL || user==NULL)//condiçao de erro
    {
        return NULL;    
    }
    int i, tamOri = g->tamanho;//atribui a tamOri o tamanho do grafo
    for(i=0; i< tamOri; i++)
        {
            if(strcmp(g->nos[i]->nome_user,user)==0)//se o utilizador ja existir retorna NULL, pois ja esta inserido
            {
                return NULL;
            }     
         
        }
    if(g->nos == NULL)//se os nos estiverem vazios
    {                 
        g->tamanho++;//aumenta o tamanho do grafo, para "caber o no"
        g->nos[0]->nome_user = user; //insere na primeira posiçao o user               
        g->nos[0]->ligacoes=NULL;   //como foi inserido agora ainda nao tem ligaçao com outros users
        g->nos[0]->tamanho=0; //logo, o tamanho é 0
        return g->nos[0];  //retorna o no               
     
    }
    else//caso os nos ja existam
    {                  
        g->nos=(no_grafo**)realloc(g->nos, (tamOri+1)*sizeof(no_grafo*));// realoca a memoria desse no para "caber" o novo user 
        g->tamanho++;//aumenta o tamanho do grafo para caber esse novo no
        g->nos[g->tamanho-1]=(no_grafo*)malloc(sizeof(no_grafo));//como o no ja existia, e necessario inserir na ultima posicao
        g->nos[g->tamanho-1]->ligacoes=NULL; //agora é igual, so inserir na ultima posicao 
        g->nos[g->tamanho-1]->tamanho=0;
        g->nos[g->tamanho-1]->nome_user=user;
        return g->nos[g->tamanho-1];
    }

    return NULL;
}

int  cria_ligacao(no_grafo *origem, no_grafo *destino, int peso)
{
    ligacao *lig = (ligacao*)malloc(sizeof(ligacao));//aloca memoria
    if (origem == NULL || destino == NULL || peso <= 0 || lig == NULL)//condicao de erro
    {
        free(lig);
        return -1;
        
    }
    for(int i = 0; i < origem->tamanho; i++)
    {
        if(origem->ligacoes[i]->destino == destino)//condiçao para ver se a ligaçao existe ja
        {
            free(lig);
            return -1;
            
        }    
    }
    int tamOri;
    tamOri = origem->tamanho;
    
    if (origem->tamanho == 0)// se o tamanho do no de origem for 0(ou seja, nao ter nada la), aloca memoria
    {
        origem->ligacoes = (ligacao**)malloc(sizeof(ligacao *));
    }
    else//caso ja exista
    {
        void *newmem = realloc(origem->ligacoes,  (tamOri + 1)*sizeof(ligacao*));//realoca a memoria de uma varivel auxiliar sem retorno
        if (newmem == NULL)                                                       // para o "tamanho" das ligacoes do no de origgem
        {
            free(newmem);
            return -1;
        }
        else
        {
            origem->ligacoes = newmem;//atribui essa nova memoria aligacao
        }
    }

    lig->destino = destino;//mete os parametros no apontador de ligacoes
    lig->peso_ligacao = peso;
    origem->ligacoes[tamOri] = lig;//atribui essa ligacao a origem tambem
    origem->tamanho++;//aumenta o tamanho para "caber" essa nova ligacao
    return 0; 

}



no_grafo * encontra_no(grafo *g, char *nomeU)
{
    if(g== NULL || nomeU == NULL)//conficao de erro
    {
        return NULL;
    }
    int i, tam = g->tamanho;
    for(i=0; i< tam; i++)//ciclo para percorre os nos
    {
       if(strcmp(g->nos[i]->nome_user, nomeU)==0)//se o que esta no no na posicao i for igual ao que se pretende
       {
           
           return g->nos[i];//retorna esse no
       }
       
    }
    return NULL;
}




grafo * criaGrafo(tabela_dispersao *td)
{
    elemento *aux;//cria um elemento auxiliar para ajudar nos ciclos
    grafo *grf = grafo_novo();// cria um novo grafo
    grf->tamanho = 0;
    if (td == NULL )// condicao de erro
    {
        return NULL;
    }
    int totMsg[2] = {0, 0}, tamTAB, i;//cria um parametro totMsg para a funcai ligacao2
    tamTAB = td->tamanho;//armazena os tamanho originais da tabela 
    for (i=0; i<tamTAB ; i++)//percorre a tabela 
    {
        aux = td->elementos[i];//atribui ao auxiliar os elementos da tabela
            while (aux != NULL)//enquando houver algo na posicao i do elemento
            {
                if (encontra_no(grf, aux->msg->remetente) == NULL)//se nao encontrar no grafo o user na posicao elemaux, insere no grafo
                {
                    no_insere(grf, aux->msg->remetente); 
                }
                if (encontra_no(grf, aux->msg->destinatario) == NULL)//se nao encontrar no grafo o user na posicao elemaux, insere no grafo
                {
                    no_insere(grf, aux->msg->destinatario); 
                }
                ligacao2(td, aux->msg->remetente, aux->msg->destinatario, totMsg); //ve as ligaçoes entre os nos e armazena o numero de mensagens trocadas entre eles
                cria_ligacao(encontra_no(grf, aux->msg->remetente), encontra_no(grf, aux->msg->destinatario), totMsg[0]); 
                cria_ligacao(encontra_no(grf, aux->msg->destinatario), encontra_no(grf, aux->msg->remetente), totMsg[1]);//atribui o peso  (numero de mensagens) as ligaçoes entre os nos

                aux = aux->proximo;//avança para o proximo elemento 
            }
            

    }
    return grf;
}



no_grafo **lista_amigos(grafo *g, char *nomeU, int *n)
{
  if (g == NULL || nomeU == NULL) // condicao de erro
    {
        return NULL;
    }
    no_grafo *aux = encontra_no(g, nomeU), *aux2;//aux é um no com o nomeU, que chamo de x
    if(aux == NULL)// se nao encontrar nada retorna NULL
    {
        free(aux);
        return NULL;
    }

    
    no_grafo **no = (no_grafo**)malloc(sizeof(no_grafo*));
    int i, j, cont=0;
    for ( i=0; i <aux->tamanho; i++)//ciclo que percorre as ligacoes do no x
    {
    
         if (aux->ligacoes[i]->peso_ligacao >= 4)//se o numero de mensagens q x mandou p y for maior ou igual a 4
          {
            aux2 = encontra_no(g, aux->ligacoes[i]->destino->nome_user);//aux2 é um no com o nome do destino de x, que chamo de y
            for (j = 0; j < aux2->tamanho; j++)//ciclo que percorre as ligacoes do no y
            {
                if ((aux2->ligacoes[j]->peso_ligacao >= 4) && (aux2->ligacoes[j]->destino == aux))//se o numero de mensagens q y mandou p x for maior ou igual a 4
                {                                                                                  //e o destino dessa ligacao for o nosso x
                    no = (no_grafo**)realloc(no, sizeof(no_grafo*)*(cont+1));//realoca a memoria para "caber" mais uma instancia
                    no[cont] = aux2;// atribui ao meu vetor de apontadores esse amigos
                    cont++;//incrementa um contador que é usado para marcar as posiçoes e o tamanho
                }
                
            }
         }
         
    }
    if(cont == 0)// se o contador for 0, significa que nao tem amigos
    {
        free(no);
        return NULL;
    }
    
    *n= cont;//o tamanho e igual ao numero final do contador
    return no;
}



///////estrutura auxiliar//////////
#define TRUE 1
#define FALSE 0
typedef struct
{
	no_grafo **raiz;//raizes das pilhas
	int tamanho;//o quanto cabe
    int max;//onde esta
} pilha;


pilha* pilha_nova(int M)
{
	/* cria pilha */
	pilha *p = (pilha*) malloc(sizeof(pilha));
	if(p == NULL || M < 0)
    {
        free(p);
		return NULL;
    }
    p->raiz =(no_grafo**)malloc(sizeof(no_grafo*)*M);
    if (p->raiz == NULL)
    {
        free(p->raiz);
        return NULL;
    }
    for(int i=0; i<M;i++)
    {
        p->raiz[i] = NULL;
    }
    p->tamanho = M;
	p->max = -1;
    

  return p;
}

void pilha_push(no_grafo *no, pilha *p)
{
    if (no == NULL || p == NULL)
    {
        free(p);
        return;
    }
    if (p->max == p->tamanho - 1)//se estiver no limite
    {
        return;//sai pq nao pode adicionar mais
    }
    else
    {
        p->max++;
        p->raiz[p->max] = no;//Como e fifo, entra no top
    }   
    
    
}

void pilha_pop(pilha *p)
{
    if (p == NULL)
    {
        free(p);
        return;
    }
    
    p->raiz[p->max] = NULL;//retira da ultima posiçao pq é FIFO
	if (p->max != 0)
    {
        p->max--;
    }
}

void pilha_busca(grafo *g, no_grafo *no, no_grafo *aux, int *n, char *nomeU, pilha *pilhaux )
{
    if(g== NULL || no==NULL || aux == NULL || nomeU == NULL || pilhaux == NULL)
    {
        free(pilhaux);
        return;
    }
    static int cont=0, i, stop;
    for(i=0; i < g->tamanho; i++)// ciclo para percorrer todas as posições do grafo
    { 
        if(g->nos[i] == no)//se o no na posicao i for o que eu procuro
        {       
            pilha_push(no, pilhaux);//adiciona do topo da pilha
            cont++; 
        }
    }
    if(pilhaux->max == pilhaux->tamanho-1)//se o que estiver no topo for mesmo no limite
    { 
        for(i=0; i < no->tamanho; i++)//percorre as ligacoes
        { 
            if(no->ligacoes[i]->destino == aux)//caso o destino do no seja o que se procura
            {
                stop = TRUE;//para o ciclo na posiçao
                return;
            }   
        }

    }
    if(cont <= 2)//caso seja menor que o minimo
    {
        for(i=0; i<no->tamanho; i++)
        {
            pilha_busca(g, no->ligacoes[i]->destino, aux ,  n, nomeU, pilhaux);
            if(stop == TRUE)
            {
                return;
            }
        }
    }
    else if(cont > 2)//senao, caso seja maior
    {
       if(pilhaux->max != pilhaux->tamanho-1)//se oq esta no topo nao estiver no limite da capacidade
       {
          for(int i=0; i<no->tamanho; i++)
          {                        
            if(no->ligacoes[i]->destino == aux)//caso o destino do no seja o que se procura
            {                                              
                stop=TRUE;//para a funçao
                return;
            }
            pilha_busca(g, no->ligacoes[i]->destino, aux ,  n, nomeU, pilhaux);
            if(stop == TRUE)
            {            
                return;
            }
          }
       }
    }
   
    pilha_pop(pilhaux);  //se o nó nao tiver ligaçoes
    cont--;
    return;
}


no_grafo ** identifica_ciclo(grafo *g, char *nomeU, int M, int *n)
{
    if (g==NULL || nomeU==NULL)//condicao de erro
    {
        free(g);
        free(nomeU);
        return NULL;
    }
    pilha *pilhaux;
    no_grafo *no = encontra_no(g, nomeU);
    if (no == NULL )//condicao de erro
    {
        free(no);
        return NULL;
    }
    pilhaux = pilha_nova(M);
    pilha_busca(g, no , no , n, nomeU, pilhaux);
    if(pilhaux->max != 0)//se no topo da pilha existir algo
    {
        *n = (pilhaux->max+1);// numero de nos do ciclo vai ser o valor maior que alcança
        return pilhaux->raiz;
    }
        free(pilhaux);//caso contrario liberta a memoria da pilha;
        return NULL;
}





