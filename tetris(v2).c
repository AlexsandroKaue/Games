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
#define QTD_ROWS_BLOCK 21
#define QTD_COLS_BLOCK 20
#define WIDTH_WINDOW QTD_COLS_BLOCK * (EDGE + SIZE_BLOCK + EDGE)
#define HEIGHT_WINDOW QTD_ROWS_BLOCK * (EDGE + SIZE_BLOCK + EDGE)
 
typedef struct{
	int fill;
	int color;
}Coordenate;

typedef struct{
	int l[3][3];
}L;

void inicializa_matriz(Coordenate m[][QTD_COLS_BLOCK]);
void rotation_matrix(JHI_Point2d poly[], int num, int orientation);
void translation_matrix(JHI_Point2d poly[], int num, int x, int y, int way);
void vetify_region(Coordenate m[][3], int linha, int coluna);
void status_L(Coordenate m[][3] , int s, int i, int j);

int main()
{

	JHI_Point2d point;
	JHI_Keys cur_key;
	Coordenate tab[QTD_ROWS_BLOCK + 1][QTD_COLS_BLOCK];
	inicializa_matriz(tab);
	Coordenate peace[3][3];
	JHI_Point2d rect;
	int turbo = 0;
	int move = 0;
	int color;
	int linha = 0;
	int game_over=0;
	int a;
	int b;
	int prox_y;
	int colision = 0;
	JHI_KeyboardSt key;
	JHI_MouseSt mouse;
	JHI_Point2d central_point;
	int speed = 2;
	int rotation = 3;

	point.y = 0;
	srand( (unsigned)time(NULL) );
	point.x = 0;
	central_point.y = point.y + EDGE + (SIZE_BLOCK/2);
	central_point.x = point.x + EDGE + (SIZE_BLOCK/2);
	
	
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
	status_L(peace,0,3,3);
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

		key = jhi_get_keyboard_status(0);
		if (key.key_event == KEYBOARD_DOWN)
		{
			if(key.key == KEY_RIGHT && point.x + SIZE_BLOCK + 2*EDGE < WIDTH_WINDOW && 
				tab[central_point.y / (SIZE_BLOCK + 2*EDGE)][(central_point.x + SIZE_BLOCK + 2*EDGE) / (SIZE_BLOCK + 2*EDGE)].fill == 0)
			{
				point.x+=(SIZE_BLOCK+EDGE);
			}
			//Se teclado LEFT
			else if(key.key == KEY_LEFT && point.x > 0 && 
				tab[central_point.y / (SIZE_BLOCK + 2*EDGE)][(central_point.x - SIZE_BLOCK - 2*EDGE) / (SIZE_BLOCK + 2*EDGE)].fill == 0) 
			{
				point.x-=(SIZE_BLOCK+EDGE);
			}else if(key.key == KEY_UP){
				rotation = (rotation + 1) % 4;
				status_L(peace,rotation,3,3);
			}else if(key.key == KEY_DOWN){
				turbo = 2*(SIZE_BLOCK + 2*EDGE);
			}
		}

		central_point.y = point.y + EDGE + (SIZE_BLOCK/2);
		central_point.x = point.x + EDGE + (SIZE_BLOCK/2);
		a = central_point.y / (SIZE_BLOCK + 2*EDGE);
		b = central_point.x / (SIZE_BLOCK + 2*EDGE);
		int i,j;
		for(i=0;i<3;i++){
			for(j=0;j<3;j++){
				if(peace[i][j].fill){
					if(tab[a+i][b+j].fill){
						colision = 1;
						printf("%s %d, %d\n","Colision in position: ",a+i,b+j );
						goto saida;
					}else{
						colision = 0;
					}

				}
			}
		}
		saida:

		if(colision){
			printf("%s\n","Colision" );
			for(i=0;i<3;i++){
				for(j=0;j<3;j++){
					if(peace[i][j].fill){
						tab[(a+i)-1][b+j].fill = 1;
						printf("%d, %d\n",a,b);
						//getchar();
					}
				}
			}
			point.y = 0;
			point.x = 0;
		}else{
			printf("%s\n","Not Colision" );
			for(i=0;i<3;i++){
				for(j=0;j<3;j++){
					if(peace[i][j].fill){
						tab[a+i][b+j].fill = 1;
					}
				}
			}
			//colision = 0;
			point.y += (SIZE_BLOCK + 2*EDGE + turbo);
			turbo = 0;
			//srand( (unsigned)time(NULL) );
			//point.x = (rand() % OFFSET_HORIZON) * SIZE_BLOCK;
			
		}
		
		for(i=0;i<QTD_ROWS_BLOCK;i++){
			for(j=0;j<QTD_COLS_BLOCK;j++){
				if(tab[i][j].fill == 1){
					rect.y = i * (SIZE_BLOCK+2*EDGE);
					rect.x = j * (SIZE_BLOCK+2*EDGE);
					rect.y += EDGE;
					rect.x += EDGE;
					jhi_draw_rect(rect, SIZE_BLOCK, SIZE_BLOCK, WHITE);
				}
			}
		}

		if(!colision){
			for(i=0;i<3;i++){
				for(j=0;j<3;j++){
					if(peace[i][j].fill){
						tab[a+i][b+j].fill = 0;
					}
				}
			}
		}


		//jhi_delay_mili_seconds(1000);
		/*for (i = 0; i < jhi_get_number_of_events(); i++)
		{

			//Pega o status do teclado no evento i
			key = jhi_get_keyboard_status(i);

			if (jhi_is_key_arrow(key.key))
			{
				//se o teclado foi pressionado com a direção não atual
				// ative o movimento e atualize a direção atual
				if (key.key_event == KEYBOARD_DOWN && key.key != cur_key)
				{
					move = 1;
					cur_key = key.key;

				}
				//se o teclado foi soltado com a direção atual
				//então desative o movimento
				else if (key.key_event == KEYBOARD_UP && key.key == cur_key)
				{
					move = 0;
					cur_key = NO_KEY;
				}
			}

			//Se o teclado foi pressionado
			if (key.key_event == KEYBOARD_DOWN)
			{
				if(key.key == KEY_RIGHT && point.x + SIZE_BLOCK < WIDTH_WINDOW && 
					tab[(point.y + SIZE_BLOCK) / SIZE_BLOCK][(point.x + SIZE_BLOCK) / SIZE_BLOCK].fill == 0)
				{
					point.x+=SIZE_BLOCK;
				}
				//Se teclado LEFT
				else if(key.key == KEY_LEFT && point.x > 0 && 
					tab[(point.y + SIZE_BLOCK) / SIZE_BLOCK][(point.x / SIZE_BLOCK)-1].fill == 0) 
				{
					point.x-=SIZE_BLOCK;
				}
			}

		}
		if (move)
		{
			if(key.key == KEY_DOWN) turbo = 20;
		}
		prox_y = point.y + SIZE_BLOCK + SPEED + turbo;
		a = prox_y / SIZE_BLOCK;
		b = point.x / SIZE_BLOCK;
		if(prox_y < HEIGHT_WINDOW)
		{
			if(tab[a][b].fill == 1){
				colision=1;
				tab[a-1][b].fill = 1;
				point.y = prox_y;
				int i,j;
				linha = 0;
				for(j=0;j<QTD_COLS_BLOCK;j++){
					linha += tab[a][j].fill;
				}
				if(a-1==0){
					game_over = 1;
				}
			}else{
				point.y += SPEED + turbo;
				turbo = 0; 
				colision=0;	
			}
		}else{
			colision=1;
			tab[QTD_ROWS_BLOCK-1][b].fill = 1;
			turbo =0;
			int i,j;
			linha = 0;
			for(j=0;j<QTD_COLS_BLOCK;j++){
				linha += tab[QTD_ROWS_BLOCK-1][j].fill;
			}
			
		}

		if(colision){
			if(linha == QTD_COLS_BLOCK){
				for(i=QTD_ROWS_BLOCK-2;i>=0;i--){
					for(j=QTD_COLS_BLOCK-1;j>=0;j--){
						tab[i+1][j] = tab[i][j];
						tab[i][j].fill = 0;
					}
				}
			}
			point.y = 0;
			srand( (unsigned)time(NULL) );
			point.x = (rand() % OFFSET_HORIZON) * SIZE_BLOCK;
		}*/

		
		//Espera tempo necessário para controlar a quantidade de frames por segundo
		jhi_wait_time();
	}

	//Limpar alocações feitas pela biblioteca
	jhi_quit_and_free();

	return 0;
}

void inicializa_matriz(Coordenate m[][QTD_COLS_BLOCK])
{
	int i,j;
	Coordenate coordenate;

	for(i=0;i<QTD_ROWS_BLOCK;i++){
		for(j=0;j<QTD_COLS_BLOCK;j++){
			coordenate.fill = 0;
			m[i][j] = coordenate;
		}
	}
	for(j=0;j<QTD_COLS_BLOCK;j++){
		coordenate.fill = 1;
		m[QTD_ROWS_BLOCK][j] = coordenate;
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

void vetify_region(Coordenate m[][3], int linha, int coluna){
	Coordenate coordenate;
	coordenate.fill = 0;
	int i,j;
	for(i=0;i<linha;i++){
		for(j=0;j<coluna;j++){
			m[i][j] = coordenate;
		}
	}
}

void status_L(Coordenate m[][3] , int s, int i, int j){
	Coordenate coordenate;
	coordenate.fill = 1;
	vetify_region(m,i,j);
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