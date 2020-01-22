#include <stdlib.h>
#include <stdio.h>
#include "bisonProj.tab.h"

//struct
typedef struct cell{
	float val;
	struct cell *next;
}cells;

typedef struct line{
	cells *cell;
	struct line *next;
}lines;

typedef struct title{
	char *title;
	struct title *next;
}titles;


typedef struct table{
	char *nome;
	titles *titulos;
	lines *linhas;
	struct tabel *next;
}tabels;


//prototipos de funções
tabels *StartListTable();
tabels *InsertNewTable(tabels *tabela, char *nome);
tabels *InsertTitles(tabels *tabela, char *nometab, char *title);
tabels *InsertCells(tabels *tabela, char *nometab, int num_line, float value);


//Funções
int main(){
	yyparse();
	return 0;
}

//Inicia Tabela
tabels *StartListTable(){
	tabels *tabela;
	tabela=NULL;
	return tabela;
}


//cria nova tabela com o nome dado pelo user
tabels *InsertNewTable(tabels *tabela, char *nome){
	tabels *aux, *new;
	aux=tabela;
	if (tabela==NULL){
		tabela=(tabels *)malloc(sizeof(tabels));
		tabela->nome=nome;
		tabela->titulos=NULL;
		tabela->linhas=NULL;
		tabela->next=NULL;
	}else{
		while(aux->next!=NULL)
			aux=aux->next;
		new=(tabels *)malloc(sizeof(tabels));
		new->nome=nome;
		new->titulos=NULL;
		new->linhas=NULL;
		new->next=NULL;
		aux->next=new;
	}	
return tabela;
}

//Acrescenta novos titulos
tabels *InsertTitles(tabels *tabela, char *nometab, char *title){
    
    tabels *aux_tab;
    aux_tab=tabela;
    titles *new, *aux_titl;
    
    
    while (aux_tab->nome != nometab)
        aux_tab=aux_tab->next;
    
    aux_titl=aux_tab->titulos;
    
    if (aux_tab->titulos == NULL){
        new=(titles *)malloc(sizeof(titles));
        new->title= title;
        new->next==NULL;
        aux_tab->titulos=new;
    }else{
        while(aux_titl->next !=NULL)
            aux_titl=aux_titl->next;
        new=(titles *)malloc(sizeof(titles));
        new->title= title;
        new->next==NULL;
        aux_titl->next=new;
        
    }
    return tabela;
}

//inserir valores nas linhas e celulas
tabels *InsertCells(tabels *tabela, char *nometab, int num_line, float value){
    tabels *aux_tab;
    lines *aux_line;
    cells *aux_cell, *new;
    aux_tab=tabela;
    
    while(aux_tab->nome != nometab && aux_tab!=NULL)
        aux_tab=aux_tab->next;
    
    if(aux_tab==NULL){
        printf("Nao foi encontrado a tabela");
        return tabela;
    }
    aux_line=aux_tab->linhas;
    
    for (int i=1; i<num_line; i++)
        aux_line=aux_line->next;
    
    if(aux_line==NULL)
        aux_line=(lines *)malloc(sizeof(lines));
    
    aux_cell=aux_line->cell;
    
    if(aux_cell == NULL){
        new=(cells *)malloc(sizeof(cells));
        new->val=value;
        new->next=NULL;
        aux_cell = new;
    }else{
        while(aux_cell->next != NULL)
            aux_cell=aux_cell->next;
        new=(cells *)malloc(sizeof(cells));
        new->val=value;
        new->next=NULL;
        aux_cell->next=new;
    }
    
    return tabela;
}

//gravar tabela
void SaveTable(tabels *tabela, char *nameTab, char *nameFich){
    titles *auxts;
    tabels *auxt;
    lines *auxl;
    cells *auxc;
    auxt=tabela;
    FILE *f;
    
    while (auxt->nome != nameTab){
        auxt=auxt->next;
    }
    
    if(auxt == NULL){
        printf("Tabela nao encontrada.\n");
        return;
    }
    //criar novo ficheiro e abrir
    f = fopen(nameFich, "rt");
    
    if (f==NULL){
        printf("Ficheiro criado/aberto sem sucesso!\n");
        return;
    }
    
    //imprimir titulos
    auxts=auxt->titulos;
    while (auxts->next != NULL){
        fprintf(f, "%s\t", auxts->title);
        auxts=auxts->next;
    }
    fprintf(f, "%s\n", auxts->title);
    
    //imprimir restantes dados no ficheiro
    auxl=auxt->linhas;
    auxc=auxl->cell;
    
    while(auxl != NULL && auxc != NULL){
        if (auxc->next != NULL){
            fprintf(f, "%.2f\t", auxc->val);
            auxc=auxc->next;
        }else if(auxc->next==NULL){
            fprintf(f, "%.2f\n", auxc->val);
            auxc=auxc->next;
        }else if(auxc==NULL){
            auxl=auxl->next;
            auxc=auxl->cell;
        }    
        
        
    }
    
    fclose(f);
    
    printf("\n\tGravado com sucesso!");   
}

