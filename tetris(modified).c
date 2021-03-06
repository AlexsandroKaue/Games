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
#define SIZE_BLOCK 50
#define WIDTH_WINDOW 500
#define HEIGHT_WINDOW 600 
#define OFFSET_HORIZON WIDTH_WINDOW / SIZE_BLOCK
#define MATRIX_ROW_INDEX HEIGHT_WINDOW / SIZE_BLOCK
#define MATRIX_COL_INDEX WIDTH_WINDOW / SIZE_BLOCK
#define SPEED 2

typedef struct{
	int x;
	int y;
	int fill;
	int color;
}Coordenate;


	


void inicializa_matriz(Coordenate m[][MATRIX_COL_INDEX]);

void start_points(JHI_Point2d pol_points[], int num);

int main()
{

	JHI_Point2d pol;
	JHI_Keys cur_key;
	Coordenate tab[MATRIX_ROW_INDEX][MATRIX_COL_INDEX];
	inicializa_matriz(tab);
	JHI_Point2d rect;
	JHI_Point2d pol_points[6];
	JHI_Point2d col_points[3];
	int turbo = 0;
	int move = 0;
	int color;
	int linha = 0;
	int game_over=0;
	int a0, a1, a2;
	int b0, b1, b2;
	int prox_y_0, prox_y_1, prox_y_2;
	int colision = 0;
	JHI_KeyboardSt key;
	JHI_MouseSt mouse;
	char rotation0[] = {'r','r','r','d','l','l','d','l','u','u'};

	pol.y = 0;
	srand( (unsigned)time(NULL) );
	pol.x = (rand() % OFFSET_HORIZON) * SIZE_BLOCK;

	//Essa função irá inicializar a janela e todos os outros módulos incluindo som, fonte.
	jhi_initialize_window(WIDTH_WINDOW, HEIGHT_WINDOW, 32, BLACK);

	//Nessa função escolhemos o nome da janela, que ficará no topo da tela
	jhi_choice_window_name("Exemplo 9");


	//Seta a quantidade de frames por segundo na tela
	jhi_set_fps_timer(20);

	jhi_init_keyboard(&key);
	//A função get_close_window() retorna se foi clicado no X da janela, dessa forma
	// o retorno é igual a CLOSE, então o loop principal deve ser quebrado, indicando
	//que o programa deve ser fechado
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
		if(game_over){
			mouse = jhi_get_mouse_status(0);
			if (mouse.mouse_event == ON_CLICK_LEFT)
			{
				inicializa_matriz(tab);
				game_over=0;
			}

		}else{
			start_points(pol_points,6);
			start_points(col_points,3);
			pol_points[0].x = pol.x;
			pol_points[0].y = pol.y;
			//col_points[0] = pol_points[0]; 

			pol_points[1].x = pol.x + 3*SIZE_BLOCK;
			pol_points[1].y = pol.y;
			
			pol_points[2].x = pol.x + 3*SIZE_BLOCK;
			pol_points[2].y = pol.y + SIZE_BLOCK;
			col_points[2] = pol_points[2];
			
			pol_points[3].x = pol.x + SIZE_BLOCK;
			pol_points[3].y = pol.y + SIZE_BLOCK;
			col_points[1] = pol_points[3];
			
			pol_points[4].x = pol.x + SIZE_BLOCK;
			pol_points[4].y = pol.y + 2*SIZE_BLOCK;
			
			pol_points[5].x = pol.x;
			pol_points[5].y = pol.y + 2*SIZE_BLOCK;
			col_points[0] = pol_points[5];	
			jhi_draw_polygon(pol_points, 6, WHITE);
			int i,j;
			for(i=0;i<MATRIX_ROW_INDEX;i++){
				for(j=0;j<MATRIX_COL_INDEX;j++){
					if(tab[i][j].fill == 1){
						rect.y = i * SIZE_BLOCK;
						rect.x = j * SIZE_BLOCK;
						jhi_draw_fill_rect(rect, SIZE_BLOCK, SIZE_BLOCK, tab[i][j].color);
					}
				}
			}

			for (i = 0; i < jhi_get_number_of_events(); i++)
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
					if(key.key == KEY_RIGHT && pol.x + SIZE_BLOCK < WIDTH_WINDOW && 
						tab[(pol.y + SIZE_BLOCK) / SIZE_BLOCK][(pol.x + SIZE_BLOCK) / SIZE_BLOCK].fill == 0)
					{
						pol.x+=SIZE_BLOCK;
					}
					//Se teclado LEFT
					else if(key.key == KEY_LEFT && pol.x > 0 && 
						tab[(pol.y + SIZE_BLOCK) / SIZE_BLOCK][(pol.x / SIZE_BLOCK)-1].fill == 0) 
					{
						pol.x-=SIZE_BLOCK;
					}
				}

			}
			if (move)
			{
				if(key.key == KEY_DOWN) turbo = 20;
				if(key.key == KEY_UP){ 
					pol_points[0].x = pol.x;
					pol_points[0].y = pol.y;
					//col_points[0] = pol_points[0]; 

					pol_points[1].x = pol.x + 2*SIZE_BLOCK;
					pol_points[1].y = pol.y;
					
					pol_points[2].x = pol.x + 2*SIZE_BLOCK;
					pol_points[2].y = pol.y + 3*SIZE_BLOCK;
					col_points[2] = pol_points[2];
					
					pol_points[3].x = pol.x + SIZE_BLOCK;
					pol_points[3].y = pol.y + 3*SIZE_BLOCK;
					col_points[1] = pol_points[3];
					
					pol_points[4].x = pol.x + SIZE_BLOCK;
					pol_points[4].y = pol.y + SIZE_BLOCK;
					
					pol_points[5].x = pol.x;
					pol_points[5].y = pol.y + SIZE_BLOCK;
					col_points[0] = pol_points[5]; 
				}

			}
			prox_y_0 = col_points[0].y + SPEED + turbo;
			prox_y_1 = col_points[1].y + SPEED + turbo;
			prox_y_2 = col_points[2].y + SPEED + turbo;
			a0 = prox_y_0 / SIZE_BLOCK;
			a1 = prox_y_1 / SIZE_BLOCK;
			a2 = prox_y_2 / SIZE_BLOCK;
			b0 = pol.x / SIZE_BLOCK;
			b1 = (pol.x / SIZE_BLOCK) + 1;
			b2 = (pol.x / SIZE_BLOCK) + 2;
			int a;
			int b;
			if(prox_y_0 < HEIGHT_WINDOW)
			{	
				if(tab[a0][b0].fill == 1){
					colision=1;
					tab[a0 - 1][b0].fill = 1;
					tab[a0 - 2][b0].fill = 1;
					tab[a0 - 2][b0 + 1].fill = 1;
					tab[a0 - 2][b0 + 2].fill = 1;
					//pol.y = prox_y_0;
					int i,j;
					linha = 0;
					for(j=0;j<MATRIX_COL_INDEX;j++){
						linha += tab[a0 - 1][j].fill;
					}
					if(a0 - 2 == 0){
						game_over = 1;
					}
				}else if(tab[a1][b1].fill == 1){
					colision=1;
					tab[a1 - 1][b1].fill = 1;
					tab[a1 - 1][b1 + 1].fill = 1;
					tab[a1 - 1][b1 - 1].fill = 1;
					tab[a1][b1 - 1].fill = 1;
					int i,j;
					linha = 0;
					for(j=0;j<MATRIX_COL_INDEX;j++){
						linha += tab[a1 - 1][j].fill;
					}
					if(a1 - 1 == 0){
						game_over = 1;
					}
				}else if(tab[a2][b2].fill == 1){
					colision=1;
					tab[a2 - 1][b2].fill = 1;
					tab[a2 - 1][b2 - 1].fill = 1;
					tab[a2 - 1][b2 - 2].fill = 1;
					tab[a2][b2 - 2].fill = 1;
					int i,j;
					linha = 0;
					for(j=0;j<MATRIX_COL_INDEX;j++){
						linha += tab[a2 - 1][j].fill;
					}
					if(a2 - 1 == 0){
						game_over = 1;
					}
				}else{
					pol.y += SPEED + turbo;
					turbo = 0; 
					colision=0;	
				}
			}else{
				colision=1;
				tab[MATRIX_ROW_INDEX - 1][b0].fill = 1;
				tab[MATRIX_ROW_INDEX - 2][b0].fill = 1;
				tab[MATRIX_ROW_INDEX - 2][b0 + 1].fill = 1;
				tab[MATRIX_ROW_INDEX - 2][b0 + 2].fill = 1;
				turbo =0;
				int i,j;
				linha = 0;
				for(j=0;j<MATRIX_COL_INDEX;j++){
					linha += tab[MATRIX_ROW_INDEX-1][j].fill;
				}
				
			}

			if(colision){
				if(linha == MATRIX_COL_INDEX){
					for(i=MATRIX_ROW_INDEX-2;i>=0;i--){
						for(j=MATRIX_COL_INDEX-1;j>=0;j--){
							tab[i+1][j] = tab[i][j];
							tab[i][j].fill = 0;
						}
					}
				}
				pol.y = 0;
				srand( (unsigned)time(NULL) );
				pol.x = (rand() % OFFSET_HORIZON) * SIZE_BLOCK;
			}

		}
		//Espera tempo necessário para controlar a quantidade de frames por segundo
		jhi_wait_time();
	}

	//Limpar alocações feitas pela biblioteca
	jhi_quit_and_free();

	return 0;
}

void inicializa_matriz(Coordenate m[][MATRIX_COL_INDEX])
{
	int i,j;
	Coordenate coordenate;

	for(i=0;i<MATRIX_ROW_INDEX;i++){
		for(j=0;j<MATRIX_COL_INDEX;j++){
			coordenate.y = i;
			coordenate.x = j;
			coordenate.fill = 0;
			coordenate.color = WHITE;
			m[i][j] = coordenate;
		}
	}
}

void start_points(JHI_Point2d pol_points[], int num){
	int i;
	JHI_Point2d p;
	for(i=0;i<num;i++){
		p.x = 0;
		p.y = 0;
		pol_points[i] = p;
	}
}

void rotate_L_0(JHI_Point2d polygon[], JHI_Point2d origin){
	pol_points[0].x = origin.x;
	pol_points[0].y = origin.y;
	//col_points[0] = pol_points[0]; 

	pol_points[1].x = origin.x + 3*SIZE_BLOCK;
	pol_points[1].y = origin.y;
			
	pol_points[2].x = origin.x + 3*SIZE_BLOCK;
	pol_points[2].y = origin.y + SIZE_BLOCK;
	col_points[2] = pol_points[2];
			
	pol_points[3].x = origin.x + SIZE_BLOCK;
	pol_points[3].y = origin.y + SIZE_BLOCK;
	col_points[1] = pol_points[3];
			
	pol_points[4].x = origin.x + SIZE_BLOCK;
	pol_points[4].y = origin.y + 2*SIZE_BLOCK;
			
	pol_points[5].x = origin.x;
	pol_points[5].y = origin.y + 2*SIZE_BLOCK;
	col_points[0] = pol_points[5];	
}





