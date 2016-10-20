#include "jhi_window.h" // -> Esse o arquivo que possui as funções referentes a janela, inclusive
#include "jhi_timer.h" // -> Temporização para animação
#include "jhi_shapes.h"
#include "jhi_keyboard.h"
#include "time.h"

//****************************************************************************
//*                                                                          *
//*  Exemplo 9: Realizando uma animação simples				     *
//*                                                                          *
//*  Year: 2014                                                              *
//*  Version:  1.0                                                           *
//*  Author(s): João Gonçalves <joao.goncalves@larces.uece.br>               *
//*             Henrique Neto <henrique.brandao@larces.uece.br>              *
//****************************************************************************/
#define SIZE_BLOCK 20
#define EDGE 1
#define QTD_ROWS_BLOCK 20
#define QTD_COLS_BLOCK 10
#define WIDTH_WINDOW QTD_COLS_BLOCK * (EDGE + SIZE_BLOCK + EDGE)
#define HEIGHT_WINDOW QTD_ROWS_BLOCK * (EDGE + SIZE_BLOCK + EDGE)
#define TAM 4
typedef struct{
	int fill;
	int color;
}Coordenate;

typedef struct{
	int l[3][3];
}L;

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


int main()
{
	char teste;
	JHI_Point2d point;
	JHI_Keys cur_key;
	Coordenate tab[QTD_ROWS_BLOCK + 1][QTD_COLS_BLOCK+2];
	inicializa_matriz(tab);
	Coordenate peace[TAM][TAM];
	JHI_Point2d rect;
	int turbo = 0;
	int move = 0;
	int line = 0;
	int jump = 0;
	int game_over=0;
	int a;
	int b;
	int dl;
	int dv;
	int prox_y;
	int colision;
	int colision_left;
	int colision_right;
	int colision_bottom;
	int offset_up;
	int offset_side;
	int count_time_right_pressed = 0;
	int count_time_left_pressed = 0;
	JHI_KeyboardSt key;
	JHI_MouseSt mouse;
	JHI_Point2d central_point;
	int speed = 15;
	int rotation = 0;
	int turn = 1;
	int op = 0;
	int color = 0;
	int change_color = 0;
	int count;
	int i,j;
	point.y = 0;
	srand( (unsigned)time(NULL) );
	point.x = 5*(EDGE + SIZE_BLOCK + EDGE);
	//central_point.y = point.y + EDGE + (SIZE_BLOCK/2);
	//central_point.x = point.x + EDGE + (SIZE_BLOCK/2);
	
	
	//Essa função irá inicializar a janela e todos os outros módulos incluindo som, fonte.
	jhi_initialize_window(WIDTH_WINDOW, HEIGHT_WINDOW, 32, BLACK);

	//Nessa função escolhemos o nome da janela, que ficará no topo da tela
	jhi_choice_window_name("Exemplo 9");


	//Seta a quantidade de frames por segundo na tela
	jhi_set_fps_timer(speed);

	jhi_init_keyboard(&key);
	//A função get_close_window() retorna se foi clicado no X da janela, dessa forma
	// o retorno é igual a CLOSE, então o loop principal deve ser quebrado, indicando
	//que o programa deve ser fechado
	rotation_peace(0,peace,0);
	while (jhi_get_close_window() != JHI_CLOSE)
	{
		//Essas duas funções devem sempre esta no começo do loop
		//update() -> função de atualização das ações feitas dentro do loop
		//clean() -> função para limpar a tela, para desenhar novamente na próxima iteração
		//Antes de qualquer ação dentro deve ser inicializado o temporizador para ser calculado a quantidade
		//de tempo necessária para executar cada iteração do loop
		jhi_timer_start();
		jhi_update();
		jhi_clean();

		if(colision_bottom){
			srand( (unsigned)time(NULL));
			op = rand() % 5;
			rotation_peace(op,peace,0);
		}

		if(change_color){
			srand( (unsigned)time(NULL));
			color = rand() % 4;
			change_color = 0;
		}

		offset_side = 0;
		offset_up = 0;
		colision_right = 0;
		colision_left = 0;
		colision_bottom = 0;
		//getchar();
		central_point.y = point.y + EDGE + (SIZE_BLOCK/2);
		central_point.x = point.x + EDGE + (SIZE_BLOCK/2);
		a = central_point.y / (SIZE_BLOCK + 2*EDGE);
		b = central_point.x / (SIZE_BLOCK + 2*EDGE);
		//getchar();

		colision = verify_colision(tab,peace,a,b);
		//printf("%d\n",colision );
		//getchar();

		if(colision){
			printf("%s\n","Colision BOTTOM" );
			offset_up = -1;
			colision_bottom = 1;
			change_color = 1;
			point.y-=(5 + 2*EDGE + turbo);
			turbo = 0;
		}

		for (i = 0; i < jhi_get_number_of_events(); i++){
			key = jhi_get_keyboard_status(i);
			printf("%d\n",i );
			if (key.key_event == KEYBOARD_DOWN)
			{
				if(key.key == KEY_RIGHT/* && point.x + SIZE_BLOCK + 2*EDGE < WIDTH_WINDOW && 
					tab[central_point.y / (SIZE_BLOCK + 2*EDGE)][(central_point.x + SIZE_BLOCK + 2*EDGE) / (SIZE_BLOCK + 2*EDGE)].fill == 0*/)
				{
					point.x+=(SIZE_BLOCK+EDGE);
					//count_time_right_pressed++;
				}
				//Se teclado LEFT
				else if(key.key == KEY_LEFT/* && point.x > 0 && 
					tab[central_point.y / (SIZE_BLOCK + 2*EDGE)][(central_point.x - SIZE_BLOCK - 2*EDGE) / (SIZE_BLOCK + 2*EDGE)].fill == 0*/) 
				{
					point.x-=(SIZE_BLOCK+EDGE);
					//count_time_left_pressed++;
				}
				else if(key.key == KEY_UP){
					rotation = (rotation + 1) % 4;
					rotation_peace(op,peace,rotation);
				}else if(key.key == KEY_DOWN){
					turbo = 8;
				}
			}
		}
		
		central_point.x = point.x + EDGE + (SIZE_BLOCK/2);
		b = central_point.x / (SIZE_BLOCK + 2*EDGE);
		
		colision = verify_colision(tab,peace,a,b);

		if(colision){
			if(key.key == KEY_RIGHT){
				printf("%s\n","Colision RIGHT" );
				offset_side = -1;
				colision_right = 1;
				point.x-=(SIZE_BLOCK+EDGE);
			}
			else if(key.key == KEY_LEFT){
				printf("%s\n","Colision LEFT" );
				offset_side = 1;
				colision_left = 1;
				point.x+=(SIZE_BLOCK+EDGE);
			}else if(key.key == KEY_DOWN){
				rotation_peace(op,peace,rotation-1);
			}
		}
		key.key = NO_KEY;

			

		for(i=0;i<TAM;i++){
			for(j=0;j<TAM;j++){
				if(peace[i][j].fill){
					tab[a + i + offset_up][b + j + offset_side].fill = 1;
					tab[a + i + offset_up][b + j + offset_side].color = color;
					printf("%d, %d\n",a,b);
					//getchar();
				}
			}
		}
		
		for(i=0;i<QTD_ROWS_BLOCK;i++){
			for(j=1;j<QTD_COLS_BLOCK+1;j++){
				if(tab[i][j].fill == 1){
					rect.y = i * (SIZE_BLOCK+2*EDGE);
					rect.x = (j-1) * (SIZE_BLOCK+2*EDGE);
					rect.y += EDGE;
					rect.x += EDGE;
					jhi_draw_fill_rect(rect, SIZE_BLOCK, SIZE_BLOCK, tab[i][j].color);
				}
			}
		}

		
		if(colision_bottom){
			point.y = 0;
			srand( (unsigned)time(NULL) );
			point.x = 5*(EDGE + SIZE_BLOCK + EDGE);
			jump = 0;
			count = 0;
			for(i=QTD_ROWS_BLOCK-1;i>0;i--){
				for(j=1;j<QTD_COLS_BLOCK+1;j++){
					if(tab[i][j].fill){
						count++;
					}
				}
				if(count == QTD_COLS_BLOCK){
					printf("%s\n","Destruindo linha");
					//getchar();
					jump++;
					line = i;
					count = 0;
				}else{
					break;
				}
			}

			for(i=line+1;i>0;i--){
				for(j=1;j<QTD_COLS_BLOCK+1;j++){
					tab[i+jump][j] = tab[i][j];  
				}
			}

		}else{
			for(i=0;i<TAM;i++){
				for(j=0;j<TAM;j++){
					if(peace[i][j].fill){
						tab[a + i + offset_up][b + j + offset_side].fill = 0;
					}
				}
			}
			point.y += (5 + 2*EDGE + turbo);
			turbo = 0;
		}
		
		jhi_wait_time();
	}

	//Limpar alocações feitas pela biblioteca
	jhi_quit_and_free();

	return 0;
}

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


