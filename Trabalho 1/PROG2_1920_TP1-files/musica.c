/*****************************************************************/
/*Biblioteca musica | PROG2 | MIEEC | 2019/20     */
/*****************************************************************/

#include "musica.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

instante* instante_novo(int duracao, char *notas, int bateria[],  char* instrumentoExtra)
{
	instante *novo = (instante*)malloc(sizeof(instante));
    int i=0, k=0;
    strcpy(novo->notasGuitarra, "--"); //inicia sem nota
    strcpy(novo->notasPiano, "--");

    if(novo == NULL || duracao < 1 || duracao >16) //testes de erro
    {
        return NULL;
    }

    novo->duracao = duracao; //atribui o parametro 
    if(novo->duracao == 0) //teste de erro
    {
        free(novo);
        return NULL;
    }

    while(k<strlen(notas))//2 e 3 sao para guitarra, 4 e 5 para piano, as posiçoes dos numeros é sempre a segunda
    {
        switch (notas[k])
        {
        case '2':
            novo->notasGuitarra[0] = notas[k-1];
        
        case '3':
            novo->notasGuitarra[1] = notas[k-1];
        
        case '4':
            novo->notasPiano[0] = notas[k-1];
        
        case '5':
            novo->notasPiano[1] = notas[k-1];
        }
        k++;
    }
    
    while(i<8)
    {
       novo->bateria[i] = bateria[i];//atribui o parametro
       i++;
    } 

    novo->instrumentoExtra = instrumentoExtra;//atribui o parametro

    novo->proximo = NULL;// como e o instante novo, nao ha proximo


    return novo;
}

musica* musica_nova()
{
	 musica *musi =(musica*)malloc(sizeof(musica));//mesmo raciocionia de listas, basta alocar memoria
    if(musi == NULL)
    {
        return NULL;
    }
    musi->inicio = NULL;
    musi->cursor = NULL;
    
	return musi;
}


int instante_insere(musica *m , instante *t, int pos )
{
	instante *aux;                
    int tam,i=0;   
    tam = musica_tamanho(m);      
    aux = m->inicio;
    
    
    if(m == NULL || t == NULL || pos > tam || pos < -1 )
    {
        return -1; 
    }
                          
    if(m->inicio == NULL)//testes de erro
    {
        m->inicio = t;
        t->proximo = m->inicio;
    }
    
    else if(pos == -1)//se for -1, mete  na ultima posiçao 
    {
        while(aux->proximo != m->inicio)
        {
            aux = aux->proximo;//vai ate o fim
        }
        t->proximo=m->inicio;//adiciona
        aux->proximo=t;
       
    }

    else if(pos == 0){
        while(aux->proximo != m->inicio)
        {
            aux = aux->proximo;//vai ate o fim
        }
        t->proximo=m->inicio;//adiciona
        m->inicio = t;
        aux->proximo=m->inicio;
    }    

    else// caso nao seja pra inserir no fim
    {
        aux = m->inicio;
        while(i<pos-1)
        {
            aux = aux->proximo;
            i++;
        }
        t->proximo = aux->proximo;
        aux->proximo = t;
    }
    
	return 0;
}

int musica_tamanho(musica *m)
{
	instante *curr;
    curr = m->inicio;
    m->cursor = m->inicio;//atribui um cursor para andar pela musica 
    int i=0;
	if (m->cursor == NULL)//testes de erro
    {
        return -1;
    }

    while( m->cursor->proximo != curr)
    {
        m->cursor = m->cursor->proximo;
        i++;//contador pro tamanho
    }

    return i+1;// +1 porque começa em 0
}

instante* musica_pesquisa_nota(musica *m, char nota, int instrumento)
{
	instante *aux;
    aux = m->inicio;
    int i=0;
    int tam =musica_tamanho(m);


	if(m == NULL || nota < 'A'|| nota > 'G' || (instrumento != 1 && instrumento !=0))//teste de erro
    {
       return NULL;
    }
    if(instrumento == 0)// se for guitarra
    {
        while(i<tam)//percorre a musica
        {
                if (aux->notasGuitarra[0] == nota || aux-> notasGuitarra[1] == nota)//se na posicao 0 ou 1 acharem a nota
                    {
                        return aux;
                    }
                aux= aux->proximo;   //se nao, vai pro proximo
                i++; 
        }
        
    }
    if(instrumento == 1)// igual da guitarra
    {
        	while(i<tam)
        {
            if (aux->notasPiano[0] == nota || aux->notasPiano[1] == nota)
                {
                    return aux;
                }
                aux= aux-> proximo;
            i++; 
        }
         
    }
	return NULL;
}



instante* instante_remove(musica *m, int pos)
{
	int i=0, tam;
    instante *aux, *antes;
    aux = m->inicio;
    m->cursor = m->inicio;//cursor pra ajudar
    tam = musica_tamanho(m);

	if(m == NULL || pos < 0 || pos >= tam)// testes de erro
    {
        return NULL;
    }
    if( pos == 0)
    {
        while(m->cursor->proximo != m->inicio)//percorre a musica
        {
            m->cursor = m->cursor->proximo;
        }
        m->cursor->proximo = m->inicio->proximo;
        m->inicio= m->cursor->proximo;
    }
    else
    {
       for(i = 0; i < pos; i++)//percorre a musica
       {
          antes = m->cursor;//atribui o antes pra posiçao atual do cursor(inicio)
          m->cursor = m->cursor->proximo;//cursor é sempre o a frente
       } 
       aux = m->cursor;
       m->cursor = aux->proximo;//e passa a frente
       antes->proximo = m->cursor;//idem
        
    }

    return aux;
}


int instante_apaga( instante *t )//libera a memoria 
{
	if(t == NULL)
    {
      return -1;
    }
	    
	free(t);
    
    return 0;
}

int musica_apaga(musica *m)
{   
	instante  *antes;
    int tam = musica_tamanho(m), i=0;
    if(m == NULL)
     {
        return -1;
     }

     while(i<tam)
     {
         antes = instante_remove(m, 0);
         instante_apaga(antes);
         i++;
     }
        free(m);//liberta a memoria
        return 0;
}

musica *musica_importa(const char *nomef){
    FILE *f = fopen(nomef, "r");
    if(f == NULL) return NULL;
    
    musica *m = musica_nova();
    if(m == NULL){
        fclose(f);
        return NULL;

    }

    int duracao = 0, i, j;
    char str_aux[100] = {};
    int bateria[8] = {0};
    char notas[9] = {0};
    char instrumentoExtra[100] = {0};
   
    instante *aux;

    while(fscanf(f, "%d", &duracao) != EOF){
        fgets(str_aux, 100*sizeof(char), f);
        str_aux[strlen(str_aux)-1] = '\0';

        /* lê os 8 carateres correspondentes à bateria */
        i = 0;
        j = 1;
        while(i < 8){
            
            bateria[i] = atoi(str_aux+j);
            i++;
            j+=2;
        }

        /* lê o resto da linha do ficheiro */
        if(strlen(str_aux+17) > 1){
            j = 0;
            for(i = 17; i < strlen(str_aux+17)+18; i++){
                if(str_aux[i] == ' ') continue;
                else if(str_aux[i] == '"'){
                    strcpy(instrumentoExtra, str_aux+i+1);
                    instrumentoExtra[strlen(instrumentoExtra) - 1] = '\0';
                    break;
                }
                else if(j < 8){
                    notas[j++] = str_aux[i];
                }
            }
           
        }
       
        /* cria novo instante (caso não exista instrumentoExtra coloca NULL */
        if(strlen(instrumentoExtra) <= 1){
            aux = instante_novo(duracao, notas, bateria, NULL);
        }
        else{
            aux = instante_novo(duracao, notas, bateria, instrumentoExtra);
        }
        
        if(aux == NULL){
            musica_apaga(m);
            fclose(f);
            return NULL;
        }
        /* insere instante na lista */
        if(instante_insere(m, aux, -1) == -1){
            musica_apaga(m);
            fclose(f);
            return NULL;
        }
        /* reinicia instrumentoExtra e notas */
        memset(instrumentoExtra, '\0', 100);
        memset(notas, '\0', 9);

    }
    fclose(f);
    return m;

}
int musica_toca(musica *m, int duracao)
{
    int aux =0;

    if ( m==NULL || duracao < 0)//teste de erro
    {
        return -1;
    }

    while (aux < duracao)
    {
        m->cursor = m->cursor->proximo;
        aux += m->cursor->duracao;//incrementa a posicao
    }
    m->cursor = m->cursor->proximo;
    return 0;
}

int musica_corrige(musica *m, char *ficheiro){
   
   FILE *file;
   file = fopen(ficheiro,"r");

   int aux1, aux2 = 0, i, erro = 0, inferior, superior, tam = musica_tamanho(m), duracao, instrumento, bateria2,contador=0;
   char nota1, nota2;
   instante *aux;
   

    if(m == NULL || file == NULL || ficheiro == NULL)
    {
    return -1;
    }
    

    while(fscanf(file,"%d",&aux1) != EOF) //le o primeiro 
    {   
        m->cursor=m->inicio;//sempre no inicio de um novo ciclo, as variaveis sao resetadas
        switch (aux1)
        {
            case 1:
              fscanf(file," %d %d",&inferior,&superior);
              if(inferior >= tam || superior >= tam || inferior < 0 || superior < 0 || superior < inferior)// teste de erro
              {
                  break;
              }
              
                for(aux2=inferior; aux2<=superior; aux2++)//remove do instante inferio ate o superior
                {
                    aux = instante_remove(m, aux2);
                    erro = instante_apaga(aux);
                    if(erro == -1) return -1;
                }
                contador++;//incrementa o contador de correcoes
              
               break;

             case 2:
               fscanf(file," %d %c %c",&instrumento ,&nota1,&nota2);
               if((instrumento != 0 && instrumento != 1 )|| nota1 <'A'|| nota1 > 'G' || nota2< 'A' || nota2 > 'G')//teste de erros
               {
                   break;
               }

               if(instrumento == 0)//se for guitarra
               {
                while(i<tam)
                {
                    if(m->cursor->notasGuitarra[0] == nota1)//se a notaGuitarra for igual a nota1, substitui
                        {
                            m->cursor->notasGuitarra[0] = nota2;
                        }
                    if(m->cursor->notasGuitarra[1] == nota1)
                        {
                             m->cursor->notasGuitarra[1] = nota2;
                        }
                            m->cursor=m->cursor->proximo;
                            i++;
                }
                        contador++;
                        
                }
                else if(instrumento == 1)// igual o de guitarra
                {
                 while(i<tam)
                    {
                        if(m->cursor->notasPiano[0] == nota1)
                        {
                                m->cursor->notasPiano[0] = nota2;
                        }
                        if(m->cursor->notasPiano[1] == nota1)
                        {
                            m->cursor->notasPiano[1] = nota2;
                        }
                        m->cursor=m->cursor->proximo;
                        i++;
                    }
                        contador++;
                }
                    break;

            case 3:
              fscanf(file," %d %d %d %d", &instrumento, &bateria2 ,&inferior , &superior);
              if(inferior > tam || inferior < 0 || (bateria2 !=0 && bateria2 !=1) || superior >tam || superior < 0 || superior < inferior)//teste de erro
              {
                  break;
              }
              
                for(i=0;i<=inferior;i++)
                {
                    m->cursor=m->cursor->proximo;
                }
                for(aux2=inferior; aux2<=superior; aux2++)
                {
                    m->cursor->bateria[instrumento] = bateria2; //atualiza a bateria n
                    m->cursor=m->cursor->proximo;
                }
                contador++;
             
             break;
            
            case 4:
              fscanf(file," %d %d %d", &duracao, &inferior, &superior);
              if(inferior > tam || inferior < 0 || superior < 0 || superior <= inferior ||superior>tam )
              {
                  break;
              }

                for(i=0;i<=inferior;i++)//mesmo raciocinio do anterior
                {
                    m->cursor = m->cursor->proximo;      
                }
                for(aux2=inferior; aux2<=superior; aux2++)
                {
                    m->cursor->duracao = duracao;    //atualiza  a duracao entre os instantes inferior e superior        
                    m->cursor = m->cursor->proximo;           
                }
                contador++;
                 
                break;
        }
    }

    fclose(file);
    return contador;
}