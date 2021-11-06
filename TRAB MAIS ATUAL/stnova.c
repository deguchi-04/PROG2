/*****************************************************************/
/*   Estrutura nova a implementar | PROG2 | MIEEC | 2019/20   */      
/*****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stnova.h"

#define RAIZ 		(1)
#define PAI(x) 		(x/2)
#define FILHO_ESQ(x) 	(x*2)
#define FILHO_DIR(x) 	(x*2+1)




int maior_que(user * e1, user * e2)
{
	if (e1 == NULL || e2 == NULL)
	{
		return 0;
	}

	return e1->msgtrocadas > e2->msgtrocadas;
}

estrutura* st_nova()
{
    estrutura* st = (estrutura *) malloc(sizeof(estrutura));
	if (!st)
	{
        return NULL;
	}
	st->tamanho = 0;
	st->user = (user**) malloc(sizeof(user *));
	if (!st->user) 
	{
		free(st);
		return NULL;
	}
	return st;
}

//ve o numero de mensagens trocadas por um user, aka prioridade da heap
int msgtroca(user *u)
{
	if (u == NULL)
	{
		return -1;
	}
    int cont;
       cont =0;
       while(u->mensa != NULL)
       {
           cont++;
           u->mensa = u->mensa->proximo;
       }
       u->msgtrocadas = cont;
    return u->msgtrocadas;
   
}

//cria um novo user
user * user_novo(const char * remet, const char * dest, char * texto)
{
	user * use = (user *) malloc(sizeof(user));
    int i;
	if (!use )
		return NULL;

	use->nome_rem = (char*)calloc(strlen(remet)+1, sizeof(char));
    use->mensa->remet->nome_rem = (char*)calloc(strlen(remet)+1, sizeof(char));
    use->mensa->destin->nome_rem = (char*)calloc(strlen(dest)+1, sizeof(char));
    use->mensa->texto = (char*)calloc(strlen(texto)+1, sizeof(char));
	if (!use->nome_rem)
	{
		free(use);
		return NULL;
	}

	strcpy(use->nome_rem, remet);
    strcpy(use->mensa->destin->nome_rem, dest);
    strcpy(use->mensa->remet->nome_rem, remet);
    strcpy(use->mensa->texto, texto);
    i = msgtroca(use);
    use->msgtrocadas = i;
	

	return use;
}

int st_insere(estrutura *st, elemento *elem)
{
    user * aux, * usua;
	int i, prioridade;
    usua = user_novo(elem->msg->remetente,  elem->msg->destinatario, elem->msg->texto);
    prioridade = msgtroca(usua);
	usua->msgtrocadas = prioridade;
	if (!elem)
    {
       return 0;
    }
	st->tamanho++;
	i = st->tamanho;
	st->user[i] = usua;
	while (i != RAIZ && maior_que(st->user[i], st->user[PAI(i)]))
	{
		aux = st->user[PAI(i)];
		st->user[PAI(i)] = st->user[i];
		st->user[i] = aux;
		i = PAI(i);
	}
        
	return 0;

}


int st_importa_tabela(estrutura *st, tabela_dispersao *td)
{
    int i,cont2=0,j ;
   if (st == NULL || td == NULL)
    {
        return -1;
    }
    
    for ( i = 0; i < st->tamanho; i++)
    {
        j = st_insere(st, td->elementos[i]);
        if (j==0)
        {
           
            cont2++;
        }
    }
    if (cont2 == td->tamanho)
    {
        
        return 0;
    }
    else
    {
        return -1;
    }

    

}

elemento *st_remove(estrutura *st, char *remetente)
{
    
	int i, filho_maior, cont=0;
	user * aux;
    elemento **el = (elemento**)malloc(sizeof(elemento*));

	if (!st || st->tamanho <= 0)
    {
        free(el);
        return NULL;
    }

	remetente = st->user[RAIZ]->nome_rem;
	free(st->user[RAIZ]);

	st->user[RAIZ] = st->user[st->tamanho];
	st->user[st->tamanho] = NULL;
	st->tamanho--;
    
 	i = RAIZ;

	while(FILHO_ESQ(i) <= st->tamanho)
	{
		filho_maior = FILHO_ESQ(i);

		
		if (FILHO_DIR(i) <= st->tamanho && maior_que(st->user[FILHO_DIR(i)], st->user[FILHO_ESQ(i)]))
        {
            filho_maior = FILHO_DIR(i);
        }
		if (maior_que(st->user[filho_maior], st->user[i]))
		{
			aux = st->user[filho_maior];
			st->user[filho_maior] = st->user[i];
			st->user[i] = aux;
			i = filho_maior;
		}
        else
        {
            break;
        }
        strcpy(el[cont]->msg->remetente,st->user[filho_maior]->mensa->remet->nome_rem);
        strcpy(el[cont]->msg->destinatario,st->user[filho_maior]->mensa->destin->nome_rem);
        strcpy(el[cont]->msg->texto,st->user[filho_maior]->mensa->texto);
        cont++;
        
	
	}
    el[cont]->proximo = NULL;

	return el[0];

}


int st_apaga(estrutura *st)
{
   int i;

	if(!st)
		return -1;

	/* apaga todos os elementos e respetivas strings */
	for(i=RAIZ; i<=st->tamanho; i++)
	{
		free(st->user[i]->nome_rem);
        free(st->user[i]->mensa);
		free(st->user[i]);
		st->user[i]=NULL;
	}

	free(st->user);
	free(st);

    return 0;
}




