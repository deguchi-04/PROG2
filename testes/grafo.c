/*****************************************************************/
/*   Grafo direcionado | PROG2 | MIEEC | 2019/20        */
/*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

grafo *grafo_novo()
{
    grafo *novo = (grafo *)malloc(sizeof(grafo));

    if (novo == NULL)
    {
        return NULL; //em caso de erro returnar NULL
    }

    else
    {
        novo->tamanho = 0; //atribuir número de nós
        novo->nos = NULL;  //inicializar vetor de nós

        return novo;
    }

    return NULL;
}

void grafo_apaga(grafo *g)
{
    if (g == NULL)
    { //caso de erro
        return;
    }

    int i;
    for (i = 0; i < g->tamanho; i++)// ciclo que dá free a cada nó do grafo
    {
        no_grafo *ng = g->nos[i];
        free(ng->nome_user);
        for (int a = 0; a < ng->tamanho; a++)//liberta cada instancia do vetor ligaçoes
        {
            free(ng->ligacoes[a]);
        }
        free(ng->ligacoes);
        free(ng);
    }
    free(g->nos);
    free(g);
}

no_grafo *no_insere(grafo *g, char *user)
{

    no_grafo *novo = (no_grafo *)malloc(sizeof(no_grafo));

    if (g == NULL || user == NULL)//em caso de erro returnar NULL
    {
        return NULL; 
    }

    int a = g->tamanho;

    if (a == 0)
    {
        g->nos = (no_grafo **)malloc(sizeof(no_grafo *));//aloca memória para o 1o no do grafo
    }
    else
    {
        g->nos = realloc(g->nos, sizeof(no_grafo *) * (g->tamanho + 1)); // aumentar a memoria alocada para o vetor de nos;
    }

    if (user != NULL)
    {
        novo->nome_user = (char *)malloc(strlen(user) + 1);//aloca memoria para a string do nome do usuario
        strcpy(novo->nome_user, user);
    }

    g->tamanho++;          //aumentar o numero de nos no grafo
    novo->tamanho = 0;     //atribuir numero de ligacoes
    novo->ligacoes = NULL; //inicializar vetor de ligacoes
    g->nos[a] = novo;

    return g->nos[a];
}

int cria_ligacao(no_grafo *origem, no_grafo *destino, int peso)
{

    if (origem == NULL || destino == NULL || peso <= 0)
    {
        return -1; //em caso de erro retornar -1
    }

    ligacao *novo = malloc(sizeof(ligacao));

    if (novo == NULL)
    {
        return -1;
    }

    if (origem->tamanho == 0)//se nao tiver ligacoes aloca memoria para a 1a ligaçao
    {
        origem->ligacoes = malloc(sizeof(ligacao *));
    }
    else
    {
        void *tmp = realloc(origem->ligacoes, sizeof(ligacao *) * (origem->tamanho + 1));
        if (tmp == NULL)
        {
            return -1; //caso de erro
        }
        else
        {
            origem->ligacoes = tmp;
        }
    }

    novo->destino = destino; // atribui as ligaçoes e as novas caractericticas do vetor ligaçoes
    novo->peso_ligacao = peso;
    origem->ligacoes[origem->tamanho] = novo;
    origem->tamanho++;

    return 0;
}

no_grafo *encontra_no(grafo *g, char *nomeU)
{
    if (g == NULL || nomeU == NULL)
    {
        return NULL; //em caso de erro returnar NULL
    }

    if (g->tamanho != 0) //se grafo tiver nós
    {
        for (int i = 0; i < g->tamanho; i++)
        {
            if (!strcmp(g->nos[i]->nome_user, nomeU))//procura nó do user
            {
                return g->nos[i];
            }
        }
    }

    return NULL;
}

grafo *criaGrafo(tabela_dispersao *td)
{
    grafo *g;

    int totMsg[2] = {0, 0};
    
    elemento *eleaux;
    
    g = grafo_novo(); //invocar funcao que cria nova instancia tipo grafo
    g->tamanho = 0;

    if (td == NULL) // caso de erro
    {
        return NULL;
    }

    for (int i = 0; i < td->tamanho; i++)//percorre a tabela toda
    {
        eleaux = td->elementos[i];
        if (eleaux != NULL)
        {

            while (eleaux != NULL)
            {

                if (encontra_no(g, eleaux->msg->remetente) == NULL)
                {
                    no_insere(g, eleaux->msg->remetente); //inserir nó  do remetento no grafo
                }

                if (encontra_no(g, eleaux->msg->destinatario) == NULL)
                {
                    no_insere(g, eleaux->msg->destinatario); //inserir nó do destinatario no grafo
                }
                eleaux = eleaux->proximo;//avança para o proximo elemento
            }
        }
    }

    for (int i = 0; i < g->tamanho; i++)//percorre os nós
    {

        for (int a = i + 1; a < (g->tamanho); a++)
        {
            int totMsg[2] = {0, 0};// inicilizia vetor a zeros

            ligacao2(td, g->nos[i]->nome_user, g->nos[a]->nome_user, totMsg);//procura peso das ligacoes
            cria_ligacao(g->nos[i], g->nos[a], totMsg[0]);// cria as ligaçoes
            cria_ligacao(g->nos[a], g->nos[i], totMsg[1]);
        }
    }

    if (g == NULL) // caso de erro
    {
        return NULL;
    }
    return g;
}

no_grafo **lista_amigos(grafo *g, char *nomeU, int *n)
{
    no_grafo *novo, *novodest;

    int aux = 0;
    if ((g == NULL) && (nomeU == NULL))//caso de erro
    {
        return NULL;
    }

    no_grafo **vetor = malloc(sizeof(no_grafo *));
    if (vetor == NULL)//caso de erro
    {
        return NULL;
    }

    novo = encontra_no(g, nomeU);// atribui a novo o nó a que nomeU pertence
    for (int i = 0; i < novo->tamanho; i++)
    {

        if (novo->ligacoes[i]->peso_ligacao > 3)//se o peso for superior a 3
        {
            novodest = encontra_no(g, novo->ligacoes[i]->destino->nome_user);// encontra destinatario e atribui-o a novodest

            for (int a = 0; a < novodest->tamanho; a++)
            {

                if (novodest->ligacoes[a]->destino == novo && novodest->ligacoes[a]->peso_ligacao >= 4)// se o peso maior do que 3 e trocarem mensagens
                {

                    vetor = realloc(vetor, sizeof(no_grafo *) * (aux + 1));

                    vetor[aux] = novo->ligacoes[i]->destino;
                    aux++;
                }
            }
        }
    }

    if (aux == 0)//caso de nao ter amigos
    {
        return NULL;
    }

    *n = aux;
    return vetor;
}

typedef struct
{
    /* capacidade da stack */
    int capacity;
    /* índice do topo da stack */
    int topValue;
    /* vetor com os elementos */
    no_grafo **elements;
}Stack;
  

/* cria uma nova stack */
Stack* CreateStack(int maxSize);
/* insere um novo elemento no topo */
void Push(no_grafo *X, Stack *S);
/* remove o elemento do topo */
void Pop(Stack * S);
/* obtém o valor do elemento do topo */
no_grafo* Top(Stack *S);
/* depth first research*/
void dfsearch(grafo *g, no_grafo *novo, no_grafo *aux , int *visited, int *n, char *nomeU, Stack *recStack );

#define EMPTY_STACK -1

Stack * CreateStack(int maxSize)
{
    
    Stack *S = (Stack *) malloc(sizeof(Stack));

    if (S == NULL && maxSize<0)//caso de erro
    {
        exit(EXIT_FAILURE);
    }

    S->elements = (no_grafo **) malloc(sizeof(no_grafo * ) * maxSize);//aloca memoria para os elementos
   
    if (S->elements == NULL)//caso de erro
    {
        exit(EXIT_FAILURE);
    }

    S->capacity = maxSize;
    S->topValue = EMPTY_STACK;

    for(int i=0; i<maxSize;i++){
        S->elements[i]=NULL;//inicializa vetor a NULL
    }

    return S;
}

void Push(no_grafo* X, Stack *S)
{
    if (S->topValue == S->capacity - 1)
    {
        exit(EXIT_FAILURE);//caso de erro
    }

    else{
        S->topValue++;
        S->elements[S->topValue] = X;
    }    
}

void Pop(Stack * S){

    if (S->topValue != EMPTY_STACK){
        S->topValue--;
    }
}

no_grafo **identifica_ciclo(grafo *g, char *nomeU, int M, int *n)
{
    int visited [g->tamanho];
    Stack *recStack;
    no_grafo *novo = encontra_no(g, nomeU);
    
    //casos de erro
    if (novo == NULL || g==NULL || nomeU==NULL || M<3)
    {
        return NULL;
    }
    
    // Marcar todos os vértices como não visitados e  criar a stack
    for (int t = 0; t < g->tamanho; t++)
    {
        visited[t] = -1;
    }
    recStack = CreateStack(M);

    
    dfsearch(g, novo , novo , visited, n, nomeU, recStack);
    
    // se nao houver ciclo
    if(recStack->topValue == -1){
        free(recStack);
        return NULL;
    }
    
    else{// se houver ciclo
        *n = (recStack->topValue+1);
        return recStack->elements;
    }
}

void dfsearch(grafo *g, no_grafo *novo, no_grafo *aux , int *visited, int *n, char *nomeU, Stack *recStack )
{
   static int contador=0 , breaker=0;
    
    //preenche a stack
    for(int i=0; i<g->tamanho; i++){ 
        if(g->nos[i] == novo){   // se for o nó pretendido insere o na stack      
            Push(novo, recStack);
            contador++;   
        }
        
        else if(visited[i]==0 && g->nos[i]==novo){ //se faz parte do ciclo do e nao foi visitado marca-o como visitado e dá break na função
            visited[i]=1;
            break;
        }
        
        else if(visited[i]==1 && g->nos[i]==novo){//se ja foi visitado e faz parte do ciclo, remove o elemento de topo da stack e baixa o valor do contador
            Pop(recStack);
            contador--;
            return;
        }
    }
    
    if(recStack->topValue == recStack->capacity - 1){  // se o indice de topo for o ultimo possivel devido a capacity
        for(int i=0; i<novo->tamanho; i++){ 
            if(novo->ligacoes[i]->destino == aux){//se o destino for igual ao nó de inicio
                breaker=1;//break na função
                return;
            }   
        }
        Pop(recStack);//remove o no da stack 
        contador--;
        return;
    }
     
    if(contador<3){//se o tamanho da stack for maior do que o tamanho minimo
        for(int i=0; i<novo->tamanho; i++){

            dfsearch(g, novo->ligacoes[i]->destino, aux , visited, n, nomeU, recStack);

            if(breaker == 1){//se nó ja foi visitado dá break na função
                return;
            }
        }
    }
    
    if(contador>=3 && recStack->topValue != recStack->capacity - 1){// se o indice de topo for o ultimo possivel
        for(int i=0; i<novo->tamanho; i++){                         // e o tamanho for maior do que o minimo
            
            if(novo->ligacoes[i]->destino == aux){                                              
                breaker=1;
                return;
            }
            
            dfsearch(g, novo->ligacoes[i]->destino, aux , visited, n, nomeU, recStack);
            
            if(breaker == 1){//termina a função            
                return;
            }
        }
    }
    
    
    Pop(recStack);  //se o nó nao tiver ligaçoes
    contador--;
    return;
}



