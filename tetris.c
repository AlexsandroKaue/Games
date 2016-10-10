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
#define SPEED 10

typedef struct{
	int x;
	int y;
	int fill;
	int color;
}Coordenate;

void inicializa_matriz(Coordenate m[][MATRIX_COL_INDEX]);

int main()
{

	JHI_Point2d point;
	JHI_Keys cur_key;
	Coordenate tab[MATRIX_ROW_INDEX][MATRIX_COL_INDEX];
	inicializa_matriz(tab);
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

	point.y = 0;
	srand( (unsigned)time(NULL) );
	point.x = (rand() % OFFSET_HORIZON) * SIZE_BLOCK;

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
			jhi_draw_fill_rect(point, SIZE_BLOCK, SIZE_BLOCK, WHITE);
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
					for(j=0;j<MATRIX_COL_INDEX;j++){
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
				tab[MATRIX_ROW_INDEX-1][b].fill = 1;
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
				point.y = 0;
				srand( (unsigned)time(NULL) );
				point.x = (rand() % OFFSET_HORIZON) * SIZE_BLOCK;
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
