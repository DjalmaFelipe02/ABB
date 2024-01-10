#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

//Desenvolvedores:
//Djalma Felipe de Sousa Neto - RGM: 30262585
//Lucas Bezerra dos Santos - RGM: 29469970
//Davi Brito Machado - RGM: 30116104

// Definindo a estrtura do nó (arvore)
typedef struct no{
	struct no * esq;
	int			rgm;
	char 		nome[55];
	struct no * dir;
} t_no;

typedef t_no * t_arvore;

// Cria um no vazio
t_no * criar ()
{
    t_no * no = (t_no *) malloc(sizeof(t_no));

    if (no)
        no->esq = no->dir = NULL;
        no->rgm = -1;

    return no;
}

// Verifica se árvore está vazia
int isVazia (t_arvore tree){
	return (tree == NULL);
}

void exibirPreOrdem(t_arvore tree) {
    if (tree!=NULL) {        
        printf("%d %s | ", tree->rgm,tree->nome);
        exibirPreOrdem(tree->esq);
        exibirPreOrdem(tree->dir);
    }
}

void exibirInOrdem(t_arvore tree) {
    if (tree!=NULL) {
        exibirInOrdem(tree->esq);
        printf("%d %s | ", tree->rgm,tree->nome);
        exibirInOrdem(tree->dir);
    }
}

void exibirPosOrdem(t_arvore tree) {
    if (tree!=NULL) {
        exibirPosOrdem(tree->esq);
        exibirPosOrdem(tree->dir);
        printf("%d %s | ", tree->rgm,tree->nome);
    }
}

void gotoxy(int coluna, int linha)
{ 
   COORD point;
   point.X = coluna;
   point.Y = linha;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

// Exibir a arvore - Procedimento recursivo, usando um percurso pre-ordem.
// sugestao de uso: exibirGraficamente(arvore, 10, 10, 3);

void exibirGraficamente(t_arvore tree, int col, int lin, int desloc)
{
    // col e lin sao as coordenadas da tela onde a arvore ira iniciar,
    // ou seja, a posicao da raiz, e desloc representa o deslocamento na tela
    // (em colunas) de um no em relacao ao no anterior.
    if (tree == NULL)
        return; // condicao de parada do procedimento recursivo
        
    gotoxy(col,lin);
    
    printf("%d %s", tree->rgm,tree->nome);
    if (tree->esq != NULL)
        exibirGraficamente(tree->esq,col-desloc,lin+2,desloc/2+1);
        
    if (tree->dir != NULL)
        exibirGraficamente(tree->dir,col+desloc,lin+2,desloc/2+1);
}


t_no * busca(t_arvore tree, int idd)
{
    if (tree == NULL){	
    	printf ("Nao encontrado");
        return NULL;
	}
	char nomo[55]; 
    if (tree->rgm == idd){  
		strcpy(nomo, tree -> nome);  	
    	printf("RGM: %d \nNome: %s \n", idd, nomo);
        return tree;
	}
    if (idd < tree->rgm)
        return busca(tree->esq, idd);
    else
        return busca(tree->dir, idd);
}

int inserir (t_arvore * tree, int idd, char nick[55])
{
    int ok;
    
    if (*tree == NULL) {
    	
        *tree = criar();
        if (*tree == NULL)
            return 0;
            
        (*tree)->rgm = idd;
        strcpy((*tree)->nome, nick);

        
        return 1;
    }
       
    if (idd > (*tree)->rgm)
        ok = inserir (&((*tree)->dir), idd, nick);
    else
        if (idd < (*tree)->rgm)
            ok = inserir (&((*tree)->esq), idd, nick);
        else
            ok = 0;
    
	return ok;
}
t_no * buscaSetPai(t_arvore tree, int idd, t_no ** pai)
{
    if (tree == NULL) {
        *pai = NULL;
        return NULL;
    }
    if (idd == tree->rgm)
        return tree;

    if (idd < tree->rgm) {
        *pai = tree;
        return buscaSetPai(tree->esq, idd, pai);
    }
    else {
        *pai = tree;
        return buscaSetPai(tree->dir, idd, pai);
    }
}

int remover (t_arvore * tree, int idd) {
    t_no 	* no, // no aponta para o no a ser removido
    		* pai, // pai aponta para o pai do no
    		* sub, // sub aponta que ira substituir o no no
    		* paiSuce, // pai do no sucessor
    		* suce; // sucessor do no no
        
    no = *tree; pai=NULL;
    
    // procura o no a ser removido, e seta o seu pai.
    no = buscaSetPai(*tree, idd, &pai);
    if (no == NULL){
    	printf("Nao encontrado");
        return 0; // a chave nao existe na arvore, nao conseguiu remover
	}

    // agora iremos ver os dois primeiros casos, o no tem um filho no maximo
    if (no->esq == NULL)
        sub = no->dir;
    else {
        if (no->dir == NULL)
           sub = no->esq;
        else { // caso em que o no tem dois filhos
	        paiSuce = no;
	        sub = no->dir;
	        suce = sub->esq;   // suce eh sempre o filho esq de sub
	        while (suce != NULL) {
	              paiSuce = sub;
	              sub = suce;
	              suce = sub->esq;
	        }
	        // neste ponto, sub eh o sucessor em ordem de no
	        if (paiSuce != no) {
	           //  no nao e o pai de sub, e sub == paiSuce->esq
	           paiSuce->esq = sub->dir;
	           // remove o no sub de sua atual posicao e o
	           // substitui pelo filho direito de sub
	           // sub ocupa o lugar de no
	           sub->dir = no->dir;
	        }
	        // define o filho esquerdo de sub de modo que sub
	        // ocupe o lugar de no
	        sub->esq = no->esq;
		}		
	}
    // insere sub na posicao ocupada anteriormente por no
    if (pai == NULL) // no eh a raiz, nao tem pai
       *tree = sub;
    else
        // verifica se o no eh o filho da esquerda ou da direita
        if (no == pai->dir)
            pai->dir = sub;
        else
            pai->esq = sub;
            
    free(no); // libera o no
    
    return 1; // verdadeiro, conseguiu remover
}

int removerTodos(t_arvore *tree){
	t_no *no = *tree;
    if (*tree == NULL)
        return;
    
    removerTodos(&(*tree)->esq);
    removerTodos(&(*tree)->dir);
    
    free(*tree);
    *tree = NULL;

}

void lerArquivo(t_arvore * tree, char * nomeArquivo)
{
    FILE * arq;
    int rgm;
    char nome[55];

    arq = fopen(nomeArquivo, "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while (fscanf(arq, "%d %s\n", &rgm, nome) != EOF) {
        inserir(tree, rgm, nome);
    }

    fclose(arq);
}
	
int main (){
	
	setlocale(LC_ALL, "Portuguese");
	t_arvore arvore = NULL;
	lerArquivo(&arvore, "nomes.txt");

	int op = 1;
	int op2 = 0;
	int rgm;
	char nome[55];
	
	
//	system("cls");
	while(op>0){
		printf("MENU\n");
		printf("1 - inserir nome e RGM\n");
		printf("2 - remover um RGM\n");
		printf("3 - buscar por RGM\n");
		printf("4 - exibir arvore\n");
		printf("5 - esvaziar arvore\n");
		printf("0 - fechar programa\n");
		scanf("%d", &op);
		
			switch(op){
			
			case 1:{	
				system("cls");		
				printf("Inserir nome: ");
				scanf("%s",&nome);
				
				printf("Inserir RGM: ");
				scanf("%d",&rgm);
				system("pause");
				system("cls");
				
				inserir(&arvore, rgm,nome);
				
				break;
			}
			
			case 2:{
				system("cls");
				printf("Remover um RGM: ");
				scanf("%d",&rgm);
				remover(&arvore, rgm);
				system("pause");
				system("cls");
				
				break;
			}			
			
			case 3:{
				system("cls");
				printf("Buscar por RGM: ");
				scanf("%d",&rgm);
				busca(arvore, rgm);
				system("pause");
				system("cls");
				
				
				break;
			}			
			
			case 4:{
				system("cls");
				printf("Exibir Arvoré:\nDigite 1 - PRE\nDigite 2 - IN\nDigite 3 - PÓS\nDigite 4 - Graficamente\n");
				scanf("%d", &op2);
				switch(op2){
								
					case 1:{
						system("cls");
						exibirPreOrdem(arvore);
						printf("\n\n\n\n");
						system("pause");
						system("cls");						
						break;
					}			
					case 2:{
						system("cls");
						exibirInOrdem(arvore);
						printf("\n\n\n\n");
						system("pause");
						system("cls");
						break;
					}	
					case 3:{
						system("cls");
						exibirPosOrdem(arvore);
						printf("\n\n\n\n");
						system("pause");
						system("cls");						
						break;
					}
					
					case 4:{
						system("cls");
						exibirGraficamente(arvore, 60, 1, 25);
						printf("\n\n\n\n");
						system("pause");
						system("cls");						
						break;
					}
					
				}
				
				break;
			}		
		
			case 5:{
				system("cls");
				removerTodos(&arvore);	
			    printf("Arvore apagada.");
				printf("\n\n\n\n");
				system("pause");
				system("cls");							
				break;
			}
			case 0:{
				printf("Fechar programa");
				break;
			}			
		}			
	}

}
