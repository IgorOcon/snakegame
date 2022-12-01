#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "queue.h"

#ifndef _BOOLEAN_H_
#define _BOOLEAN_H_

#define ALT 25
#define LARG 35

typedef enum{
	false,
	true
}boolean;

#endif

#define TAM_INI 3
#define ALT_LARG_MIN 7


typedef struct snake{
	struct snake* prev, *next;
	int x, y;
}snake;

typedef enum{
	ESQ, DIR, CIM, BAI
}Direcao;

typedef struct replay{
	int dif;
	int dir;
	int tempo;
}replay;

boolean game_over;
boolean precisa_de_comida;
snake* cobra = NULL;
snake comida;
replay* rep;
int altura, largura;
char ** tela;
int pontos = 0;
int dificuldade;

/*	FUNCOES DE TELA	 */

char** tela_ini(){
	int i;
	char** t = (char**) malloc(sizeof(char*)*altura);
	for(i=0; i<altura; i++)
		t[i] = (char*) malloc(sizeof(char)*largura);

	return t;
}

//faz a moldura com #
void plot_moldura(char** t){
	int i, j;
	for(i=0; i<altura; i++){
		for(j=0; j<largura; j++){
			if(i==0 || j==0 || i==(altura-1) || j==(largura-1))
				t[i][j] = '#';
			else
				t[i][j] = ' ';
		}
	}
}

//libera tela
void tela_libera(char** t){
	int i;
	for(i=0; i<altura; i++)
		free(t[i]);

	free(t);
}

//imprime tela
void tela_imprime(char** t){
	int i, j;
	for(i=0; i<altura; i++){
		for(j=0; j<largura; j++){
			printf("%c ", t[i][j]);	
		}
		printf("\n");
	}
}

void plot_snake(char** t, snake* s){
	snake* aux = s;

	do{
		t[aux->y][aux->x] = '*';
		aux = aux->next;
	}while(aux != s);
}


/*		FUNCOES DA COBRA	*/
void snake_inicia(int tam){

	int i, count;
	int l = largura/2, a = altura/2;
	for(i=0; i<tam; i++){
		snake* s = malloc(sizeof(snake));
		s->prev = s->next = NULL;
		s->x = l;
		s->y = a;
		l++;

		queue_append((fila_t **)&cobra, (fila_t *)s);
	}
}

void snake_libera(){
	snake *aux;
	snake *s = cobra;

	do{
		aux = s->next;
		free(s);
		s = aux;
	}while(aux != cobra);
}

//funcoes para movimentar a cobra
void mov_cobra(Direcao d){

	snake* i = cobra->prev;

	while(i!=cobra){
		i->x = i->prev->x;
		i->y = i->prev->y;

		i = i->prev;
	}

	switch(d){
		case CIM:
			cobra->y--;
			break;
		case ESQ:
			cobra->x--;
			break;
		case BAI:
			cobra->y++;
			break;
		case DIR:
			cobra->x++;
			break;
	}
}

/*JOGO*/

void gera_comida(){
	int n_livre = 0; //armazena quantos espacos livres estao disponiveis
	int i, j;

	int *ys = (int*)malloc(sizeof(int)*altura*largura);
	int *xs = (int*)malloc(sizeof(int)*largura*altura);

	for(i=1; i<altura-1; i++){
		for(j=1; j<largura-1; j++){
			if(tela[i][j] == ' '){
				ys[n_livre++] = i;
				xs[n_livre] = j;
			}
		}
	}

	if(n_livre == 0){
		printf("Parabens, voce venceu!!!!!!!!\nNem sabia que era possivel...\n");
		exit(0);
	}else{
		int n = rand()%n_livre;
		comida.x = xs[n];
		comida.y = ys[n];
	}	


	free(ys);
	free(xs);
	precisa_de_comida = false;
}

void game_conditions(){
	//A cobra bateu em alguma das paredes?
	if(cobra->x <= 0 || cobra->x >= (largura-1) || cobra->y <= 0 || cobra->y >= (altura-1)){
		game_over = true;
		return;
	}

	//se a cobra bateu nela mesma
	snake* i = cobra->next;

	do{
		if(cobra->x == i->x && cobra->y == i->y){
			game_over = true;
			return;
		}
		i = i->next;
	}while(i != cobra);

	//gera comida
	if(precisa_de_comida){
		gera_comida();
	}else{
		if(cobra->x == comida.x && cobra->y == comida.y){
			pontos++;
			precisa_de_comida = true;
			snake* new = malloc(sizeof(snake));
			new->next = new->prev = NULL;
			new->x = cobra->prev->x;
			new->y = cobra->prev->y;
			queue_append((fila_t**)&cobra, (fila_t*)new);
		}
	}


}

void plot_comida(char** t){
	t[comida.y][comida.x] = '@';
}

void menu_pause(){
	
	char tcl;
	system("CLS");
				
	printf("***************************************");
	printf("\n*          JOGO PAUSADO!              *");
	printf("\n***************************************\n\n\n");

	printf("***************************************");
	printf("\n*       ESCOLHA A DIFICULDADE         *");
	printf("\n*                                     *");
	printf("\n* 1 - Facil                           *");
	printf("\n* 2 - Normal                          *");
	printf("\n* 3 - Dificil                         *");
	printf("\n* 4 - Insano                          *");
	printf("\n***************************************\n\n\n");	
	tcl = getch();
	
	switch(tcl){
		case '1':
			dificuldade = 100000;
		break;
       	case '2':
       		dificuldade = 80000;
       	break;
		case '3':
			dificuldade = 25000;
		break;
		case '4':
			dificuldade = 1000;
		break;
		
	}	
}


int main(int argc, char *argv[]){
	dificuldade = 80000;
	srand(time(NULL));
	game_over = false;
	precisa_de_comida = true;

	
	//definição da altura e largura da tela
	altura = ALT;
	largura = LARG;

	tela = tela_ini();
	plot_moldura(tela);

	snake_inicia(TAM_INI);

	char tecla;
	int col, lin;
	col = (largura-54)/2;
	lin = altura/2;
	
	Direcao dir = ESQ;
	do{

		//alguma tecla foi digitada?
		if(kbhit()){
			
			tecla=getch();
			
			if ((int)tecla == 27){
				menu_pause();
			}
			switch(tecla){
				case 'w':
        		case 'W':  
					if(dir != BAI)
						dir = CIM;
					break;
				case 'a':
        		case 'A':  
					if(dir!=DIR)
						dir = ESQ;
					break;
				case 's':
        		case 'S':  
					if(dir != CIM)
						dir = BAI;
					break;
				case 'd':
        		case 'D':  
					if(dir != ESQ)
						dir = DIR;
					break;
			}
		}

		//movimenta a cobra
		//rep->dir = dir;
		mov_cobra(dir);

		//condicoes de "morte"
		game_conditions();

		//desenha a moldura
		plot_moldura(tela);

		//desenha a cobra
		plot_snake(tela, cobra);

		//desenha comida
		plot_comida(tela);

		system("cls");
		tela_imprime(tela);	

		
		usleep(dificuldade);		
		//rep->tempo = temp;
		//rep->dif = dificuldade;
		
	}while(!game_over);

	printf("Game Over\n");
	printf("Pontos: %d/%d\n", pontos, largura*altura);
	printf("%.2lf%% do jogo concluido\n", ((double)pontos/(double)(largura*altura))*20.0);

	snake_libera(cobra);
	tela_libera(tela);
	return 0;
}
