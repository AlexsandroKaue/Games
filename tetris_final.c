#ifndef TETRIS_C
#define TETRIS_C

#include "tetris.h"

/**********************************************************************************/
void inicializa_matriz(Coordenate m[][QTD_COLS_BLOCK+2])
{
	int i,j;
	Coordenate coordenate;
	coordenate.color = WHITE;
	for(i=0;i<QTD_ROWS_BLOCK;i++){
		for(j=1;j<QTD_COLS_BLOCK+1;j++){
			coordenate.fill = 0;
			m[i][j] = coordenate;
		}
	}

	for(j=0;j<QTD_COLS_BLOCK+2;j++){
		coordenate.fill = 1;
		m[QTD_ROWS_BLOCK][j] = coordenate;
	}


	for(i=0;i<QTD_ROWS_BLOCK;i++){
		coordenate.fill = 1;
		m[i][0] = coordenate;
		m[i][QTD_COLS_BLOCK+1] = coordenate;
	}
	
}
/**********************************************************************************/

void rotation_matrix(JHI_Point2d poly[], int num, int orientation)
{
	int aux_x, aux_y;
	int i;
	for(i=0;i<num;i++){
		aux_x = poly[i].x * 0 - poly[i].y * orientation;
		aux_y = poly[i].x * orientation + poly[i].y * 0;
		poly[i].x = aux_x;
		poly[i].y = aux_y;
	}
}
/**********************************************************************************/

void translation_matrix(JHI_Point2d poly[], int num, int x, int y, int way)
{
	int i;
	if(way == 1){
		for(i=0;i<num;i++){
			poly[i].x += 2*x; 
		}	
	}else if(way == -1){
		for(i=0;i<num;i++){
			poly[i].y += 2*y;
		}
	}
	
}
/**********************************************************************************/

void initialize_peace(Coordenate m[][TAM], int linha, int coluna){
	Coordenate coordenate;
	coordenate.fill = 0;
	int i,j;
	for(i=0;i<linha;i++){
		for(j=0;j<coluna;j++){
			m[i][j] = coordenate;
		}
	}
}
/**********************************************************************************/

void rotation_peace(int i, Coordenate m[][TAM] , int s){
	if(i == 0){
		status_L(m,s);
	}else if(i == 1){
		status_I(m,s);
	}else if(i == 2){
		status_Q(m,s);
	}else if(i == 3){
		status_Z(m,s);
	}else if(i == 4){
		status_T(m,s);
	}

}
/**********************************************************************************/

void status_L(Coordenate m[][TAM] , int s){
	Coordenate coordenate;
	coordenate.fill = 1;
	initialize_peace(m,TAM,TAM);
	switch(s){
		case 0: m[0][0] = coordenate;
				m[1][0] = coordenate;
				m[0][1] = coordenate;
				m[0][2] = coordenate;
				break;
		case 1: m[0][0] = coordenate;
				m[0][1] = coordenate;
				m[1][1] = coordenate;
				m[2][1] = coordenate;
				break;	
		case 2: m[0][2] = coordenate;
				m[1][2] = coordenate;
				m[1][1] = coordenate;
				m[1][0] = coordenate;
				break;
		case 3:
				m[0][1] = coordenate;
				m[1][1] = coordenate;
				m[2][1] = coordenate;
				m[2][2] = coordenate;
				break;
	}
}
/**********************************************************************************/

void status_I(Coordenate m[][TAM] , int s){
	Coordenate coordenate;
	coordenate.fill = 1;
	initialize_peace(m,TAM,TAM);
	switch(s){
		case 0:	
		case 2: m[0][1] = coordenate;
				m[1][1] = coordenate;
				m[2][1] = coordenate;
				m[3][1] = coordenate;
				break;
		case 1:
		case 3:
				m[1][0] = coordenate;
				m[1][1] = coordenate;
				m[1][2] = coordenate;
				m[1][3] = coordenate;
				break;
	}
}
/**********************************************************************************/

void status_Q(Coordenate m[][TAM] , int s){
	Coordenate coordenate;
	coordenate.fill = 1;
	initialize_peace(m,TAM,TAM);
	switch(s){
		case 0:	
		case 2:
		case 1:
		case 3:
				m[0][0] = coordenate;
				m[0][1] = coordenate;
				m[1][0] = coordenate;
				m[1][1] = coordenate;
	}
}
/**********************************************************************************/

void status_Z(Coordenate m[][TAM] , int s){
	Coordenate coordenate;
	coordenate.fill = 1;
	initialize_peace(m,TAM,TAM);
	switch(s){
		case 0:	
		case 2: m[0][1] = coordenate;
				m[0][2] = coordenate;
				m[1][1] = coordenate;
				m[1][0] = coordenate;
				break;
		case 1:
		case 3:
				m[0][0] = coordenate;
				m[1][0] = coordenate;
				m[1][1] = coordenate;
				m[2][1] = coordenate;
				break;
	}
}
/**********************************************************************************/

void status_T(Coordenate m[][TAM] , int s){
	Coordenate coordenate;
	coordenate.fill = 1;
	initialize_peace(m,TAM,TAM);
	switch(s){
		case 0:	m[0][0] = coordenate;
				m[0][1] = coordenate;
				m[0][2] = coordenate;
				m[1][1] = coordenate;
				break;
		case 1: m[0][2] = coordenate;
				m[1][2] = coordenate;
				m[1][1] = coordenate;
				m[2][2] = coordenate;
				break;
		case 2: m[2][2] = coordenate;
				m[2][1] = coordenate;
				m[1][1] = coordenate;
				m[2][0] = coordenate;
				break;
		case 3:
				m[2][0] = coordenate;
				m[1][0] = coordenate;
				m[1][1] = coordenate;
				m[0][0] = coordenate;
				break;
	}
}
/*************************************************************************************************/

int verify_colision(Coordenate tab[][QTD_COLS_BLOCK+2], Coordenate peace[][TAM], int a, int b){
	int i,j,colision;
	colision = 0;
	for(i=0;i<TAM;i++){
		for(j=0;j<TAM;j++){
			if(peace[i][j].fill){
				if(tab[a+i][b+j].fill){
					colision = 1;
					printf("%s %d, %d\n","Colision in position: ",a+i,b+j);
					goto exit;
				}
			}
		}
	}
	exit:
	return colision;
}
/*************************************************************************************************/

#endif