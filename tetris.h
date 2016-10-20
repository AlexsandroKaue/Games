#ifndef TETRIS_H
#define TETRIS_H

#include "tetris_defines.h"
#include "jhi_shapes.h"

typedef struct{
	int fill;
	int color;
}Coordenate;

void inicializa_matriz(Coordenate m[][QTD_COLS_BLOCK+2]);

void rotation_matrix(JHI_Point2d poly[], int num, int orientation);

void translation_matrix(JHI_Point2d poly[], int num, int x, int y, int way);

void initialize_peace(Coordenate m[][TAM], int linha, int coluna);

void status_L(Coordenate m[][TAM] , int s);

void status_I(Coordenate m[][TAM] , int s);

void status_Q(Coordenate m[][TAM] , int s);

void status_Z(Coordenate m[][TAM] , int s);

void status_T(Coordenate m[][TAM] , int s);

void rotation_peace(int i, Coordenate m[][TAM] , int s);

int verify_colision(Coordenate tab[][QTD_COLS_BLOCK+2], Coordenate peace[][TAM], int a, int b);

#endif