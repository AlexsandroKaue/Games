#include "jhi_window.h" // -> Esse o arquivo que possui as funções referentes a janela, inclusive
#include "jhi_timer.h" // -> Temporização para animação
#include "jhi_shapes.h"
#include "jhi_keyboard.h"
#include "jhi_text.h"
#include "time.h"
#include "jhi_sound.h"
#include "tetris.h"

//****************************************************************************
//*                                                                          *
//*  Exemplo 9: Realizando uma animação simples				     *
//*                                                                          *
//*  Year: 2014                                                              *
//*  Version:  1.0                                                           *
//*  Author(s): João Gonçalves <joao.goncalves@larces.uece.br>               *
//*             Henrique Neto <henrique.brandao@larces.uece.br>              *
//****************************************************************************/

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
	JHI_Music music;
	JHI_Effect block;
	JHI_Effect destruction;	
	JHI_Font font_text;
	JHI_Text score_t;
	JHI_Text value_t;
	char txt[20];
	char score[10];
	int s = 0;
	int speed = 25;
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
	//central_point.y = point.y + EDGE + (SIZE_BL OCK/2);
	//central_point.x = point.x + EDGE + (SIZE_BLOCK/2);
	
	
	//Essa função irá inicializar a janela e todos os outros módulos incluindo som, fonte.
	jhi_initialize_window(WIDTH_WINDOW+100, HEIGHT_WINDOW, 32, BACKGROUND_COLOR);

	//Nessa função escolhemos o nome da janela, que ficará no topo da tela
	jhi_choice_window_name("Exemplo 9");
	jhi_load_music(&music, "sounds/tetris_theme.ogg");
	jhi_load_font(&font_text, "fonts/PonyMaker.ttf", 15);
	jhi_init_text(&score_t);
	jhi_init_text(&value_t);
	score_t.pos.x = (QTD_COLS_BLOCK + 4)  * (EDGE + SIZE_BLOCK + EDGE);
	score_t.pos.y = (QTD_ROWS_BLOCK / 3)  * (EDGE + SIZE_BLOCK + EDGE);
	value_t.pos.x = (QTD_COLS_BLOCK + 4)  * (EDGE + SIZE_BLOCK + EDGE);
	value_t.pos.y = (QTD_ROWS_BLOCK / 2)  * (EDGE + SIZE_BLOCK + EDGE);

	jhi_load_effect(&block, "sounds/tetris_rotation.ogg");
	jhi_load_effect(&destruction, "sounds/tetris_destruction.ogg");
	jhi_play_music(&music, -1);
	jhi_delay_mili_seconds(20);

	//Seta a quantidade de frames por segundo na tela
	jhi_set_fps_timer(speed);

	jhi_init_keyboard(&key);
	//A função get_close_window() retorna se foi clicado no X da janela, dessa forma
	// o retorno é igual a CLOSE, então o loop principal deve ser quebrado, indicando
	//que o programa deve ser fechado
	rotation_peace(0,peace,0);
	//printf("%s\n","Vermelho ganhou!");
	memset(txt, 0, sizeof(char) * 20);
	memset(score, 0, sizeof(char) * 20);
	sprintf(txt, "%s", "SCORE");
	sprintf(score, "%d", s);
	jhi_set_text(&font_text, &score_t, WHITE, txt);
	jhi_set_text(&font_text, &value_t, WHITE, score);
	jhi_draw_text(&score_t, score_t.pos);
	jhi_draw_text(&value_t, value_t.pos); 
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
		

		jhi_draw_text(&score_t, score_t.pos);
		jhi_draw_text(&value_t, value_t.pos); 
		if(colision_bottom){
			srand( (unsigned)time(NULL));
			op = rand() % 5;
			rotation = 0;
			rotation_peace(op,peace,rotation);
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
			point.y-=(1+ 2*EDGE + turbo);
			colision = 0;
		}
		turbo = 0;
		for (i = 0; i < jhi_get_number_of_events(); i++){
			
			key = jhi_get_keyboard_status(i);
			//jhi_delay_mili_seconds(500);
			printf("%d\n",i );

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
			
		}

		if (move)
		{

			if(key.key == KEY_RIGHT/* && point.x + SIZE_BLOCK + 2*EDGE < WIDTH_WINDOW && 
				tab[central_point.y / (SIZE_BLOCK + 2*EDGE)][(central_point.x + SIZE_BLOCK + 2*EDGE) / (SIZE_BLOCK + 2*EDGE)].fill == 0*/)
			{	
				//jhi_delay_mili_seconds(150);
				jhi_play_effect(&block, 0);
				point.x+=(SIZE_BLOCK+EDGE);
				//count_time_right_pressed++;
			}
			//Se teclado LEFT
			else if(key.key == KEY_LEFT/* && point.x > 0 && 
				tab[central_point.y / (SIZE_BLOCK + 2*EDGE)][(central_point.x - SIZE_BLOCK - 2*EDGE) / (SIZE_BLOCK + 2*EDGE)].fill == 0*/) 
			{
				//jhi_delay_mili_seconds(150);
				jhi_play_effect(&block, 0);
				point.x-=(SIZE_BLOCK+EDGE);
				//count_time_left_pressed++;
			}
			else if(key.key == KEY_UP){
				rotation = (rotation + 1) % 4;
				jhi_play_effect(&block, 0);
				rotation_peace(op,peace,rotation);
				move = 0;
				//cur_key = NO_KEY;
			}else if(key.key == KEY_DOWN){
				turbo = SIZE_BLOCK - 1;
			}
			
			central_point.x = point.x + EDGE + (SIZE_BLOCK/2);
			b = central_point.x / (SIZE_BLOCK + 2*EDGE);
			colision = verify_colision(tab,peace,a,b);
	 	}


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
			}else if(key.key == KEY_UP){
				printf("%s\n","Colision ROTATION" );
				rotation_peace(op,peace,rotation-1);
			}
		}
	 	
	 		

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
			for(j=0;j<QTD_COLS_BLOCK+2;j++){
				if(tab[i][j].fill == 1){
					rect.y = i * (SIZE_BLOCK+2*EDGE);
					rect.x = j * (SIZE_BLOCK+2*EDGE);
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

			for(i=QTD_ROWS_BLOCK-1;i>0;i--){
				count = 0;
				for(j=1;j<QTD_COLS_BLOCK+1;j++){
					if(tab[i][j].fill){
						count++;
					}
				}
				if(count == QTD_COLS_BLOCK){
					printf("%s %d\n","Destruindo linha",i);
					//getchar();
					jump++;
					line = i;
				}
			}
			if(jump!=0){
				s += jump * 100;
				sprintf(score, "%d", s);
				jhi_set_text(&font_text, &value_t, WHITE, score);
				jhi_play_effect(&destruction, 0);
				line_destruction_effect(tab,line,jump);
				for(i=line-1;i>0;i--){
					for(j=1;j<QTD_COLS_BLOCK+1;j++){
						tab[i+jump][j] = tab[i][j];  
					}
				}
			}

			if(a == 0){
				printf("%s\n","Game Over" );
				getchar();
			}
		}else{
			for(i=0;i<TAM;i++){
				for(j=0;j<TAM;j++){
					if(peace[i][j].fill){
						tab[a + i + offset_up][b + j + offset_side].fill = 0;
					}
				}
			}
			point.y += (1 + 2*EDGE + turbo);
		}
		jhi_wait_time();
		//jhi_delay_mili_seconds(100);
	}

	//Limpar alocações feitas pela biblioteca
	jhi_quit_and_free();

	return 0;
}