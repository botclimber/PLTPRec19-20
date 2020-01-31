#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "bisonProj.tab.h"

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

//prototipos de funções ->LOAD "ficheiro.tsv" AS notas;
tabels *LoadTable(tabels *tabela, char *nome_fich, char *nome_tab);
//<faz parte do load>
titles *InsertTitles(FILE *f, int tam, int now);
lines *InsertLines(FILE *f, int tam, int now);
cells *InsertCells(FILE *f, int tam, int now);
int charcount( FILE *const fin );
//</faz parte do load>
//imprimir 2 colunas ->PRINT nr, nota1 FROM notas;
void Print2Columns(tabels *tabela, char *name_tab, char *column1, char *column2);

//Funções
int main(){
	tabels *t;
	t=NULL;
	t=LoadTable(t, "teste.txt", "hoje");
	printf("Com sucesso!\n\n");
        Print2Columns(t, "hoje", "nota3", "nota1");
        system("pause");
	/*yyparse();
	return 0;*/
}


//cria nova tabela com o nome dado pelo user
tabels *LoadTable(tabels *tabela, char *nome_fich, char *nome_tab){
	tabels *aux_t, *new;
	lines *aux_l;
	cells *aux_c;
	FILE *f;
	int i=0;

        
	aux_t=tabela;

	f=fopen(nome_fich, "rt");
	if(f==NULL){
		printf("Erro ao abrir o ficheiro %s!\n", nome_fich);
		return tabela;
	}

        i= charcount(f);
        fseek(f, 0, SEEK_SET);
        
	if (tabela==NULL){
		tabela=(tabels *)malloc(sizeof(tabels));
		tabela->next=NULL;
		tabela->nome=nome_tab;
		tabela->titulos=InsertTitles(f, i, 0);
		tabela->linhas=InsertLines(f, i, 0);
		return tabela;
	}else{
		while(aux_t->next != NULL)
			aux_t=aux_t->next;
		aux_t->next=(tabels *)malloc(sizeof(tabels));
		aux_t=aux_t->next;
		aux_t->next=NULL;
		aux_t->nome=nome_tab;
		aux_t->titulos=InsertTitles(f, i, 0);
		aux_t->linhas=InsertLines(f, i, 0);
	}


	fclose(f);
	
	return tabela;
}

//tamanho da 1 linha
int charcount( FILE *fin ){
    int count;
    char c;
    count = 0;
    c=fgetc(fin);
    for( ;; )
    {
        c = fgetc( fin );
        if( c == '\n' ){
            ++count;
            return count;
        }
        if (c=='\t')
            ++count;
    }
}


//Acrescenta novos titulos
titles *InsertTitles(FILE *f, int tam, int now){
    titles *aux;

    aux=(titles *)malloc(sizeof(titles));
    aux->title = strdup("a");
    //now+=(fscanf(f, "%s", aux->title));
    now= now + fscanf(f, "%s", aux->title);
    if (now<tam){
    	aux->next=InsertTitles(f, tam, now);
    }else{
	aux->next=NULL;
    }

    return aux;
}

//inserir valores nas linhas e celulas
lines *InsertLines(FILE *f, int tam, int now){
    lines *aux_line;
    if(now<tam){
        aux_line=(lines *)malloc(sizeof(lines));
        aux_line->cell=InsertCells(f, tam, 0);
        now++;
        aux_line->next=InsertLines(f, tam, now);        
    }else{
        return NULL;     
        }
    
    return aux_line;
}

cells *InsertCells(FILE *f, int tam, int now){
        float i;
        cells *new;
	new=(cells *)malloc(sizeof(cells));
        new->val=0;
        //now= now + fscanf(f, "%f", new->val);
        now+=(fscanf(f, "%f", &i));
        new->val=i;
	if (now < tam)
		new->next=InsertCells(f, tam, now);
	else{
		new->next=NULL;
	}
	return new;
}

void Print2Columns(tabels *tabela, char *name_tab, char *column1, char *column2) {
    tabels *aux_t = tabela;
    titles *aux_tit;
    lines *aux_l;
    cells *aux_c;
    int col1 = 0, col2 = 0;

    while ((strcmp(aux_t->nome, name_tab)) != 0)
        aux_t = aux_t->next;

    aux_l = aux_t->linhas;
    aux_tit = aux_t->titulos;



    while (aux_tit != NULL && (strcmp(aux_tit->title, column1)) != 0) {
        aux_tit = aux_tit->next;
        col1++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", column1);
        return;
    }

    aux_tit = aux_t->titulos;

    while (aux_tit != NULL && (strcmp(aux_tit->title, column2)) != 0) {
        aux_tit = aux_tit->next;
        col2++;
    }
    if (aux_tit == NULL) {
        printf("Nao existe a coluna %s\n", column2);
        return;
    }

    aux_tit = aux_t->titulos;

    if (col1 < col2) {
        for (int i = 0; i < col1; i++)
            aux_tit = aux_tit->next;

        printf("%s\t", aux_tit->title);

        for (int i = col1; i < col2; i++)
            aux_tit = aux_tit->next;

        printf("%s\n", aux_tit->title);

        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            for (int i = 0; i < col1; i++)
                aux_c = aux_c->next;
            printf("%.1f\t", aux_c->val);
            for (int i = col1; i < col2; i++)
                aux_c = aux_c->next;
            printf("%.1f\n", aux_c->val);
            aux_l = aux_l->next;
        }
    } else {
        for (int i = 0; i < col2; i++)
            aux_tit = aux_tit->next;

        printf("%s\t", aux_tit->title);

        for (int i = col2; i < col1; i++)
            aux_tit = aux_tit->next;

        printf("%s\n", aux_tit->title);

        while (aux_l != NULL) {
            aux_c = aux_l->cell;
            for (int i = 0; i < col2; i++)
                aux_c = aux_c->next;
            printf("%.1f\t", aux_c->val);
            for (int i = col2; i < col1; i++)
                aux_c = aux_c->next;
            printf("%.1f\n", aux_c->val);
            aux_l = aux_l->next;
        }
    }
}    






//gravar tabela
/*void SaveTable(tabels *tabela, char *nameTab, char *nameFich){
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

*/