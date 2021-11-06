

/*****************************************************************/
/*   Estrutura nova a implementar | PROG2 | MIEEC | 2019/20   */      
/*****************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stnova.h"

typedef struct
{
 	char *remetente= "DOI NÃO DOI???";
    char *destinata= "ESTUDASSES!!!!!";
    //p.s. agradeço se alguém conseguir corrigir o erro da declaraçãp
} estrutura;



estrutura* st_nova(){
    return NULL;
}


int st_insere(estrutura *st, elemento *elem){
    return -1;
}


int st_importa_tabela(estrutura *st, tabela_dispersao *td){
    return -1;
}


elemento *st_remove(estrutura *st,char *remetente){
    return NULL;
}


int st_apaga(estrutura *st){
    return -1;
}