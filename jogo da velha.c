#include "jhi_window.h" // Esse header está no diretório da biblioteca
#include "jhi_image.h" //-> Esse arquivo é header da estrutura Image
#include "jhi_shapes.h"
#include "jhi_font.h"  //-> Esse arquivo é header da estrutura Font
#include "jhi_text.h"  //-> Esse arquivo é header da estrutura Text
#include <string.h>
void inicializa_matriz(int m[][3]);

int main()
{
	JHI_Image img;

	JHI_MouseSt mouse;

	JHI_Font f1;

	JHI_Text t1;
	
	jhi_initialize_window(400, 400, 32, WHITE);

	jhi_choice_window_name("Tabuleiro");

	jhi_load_image(&img, "images/tabuleiro.png");

	jhi_load_font(&f1, "fonts/PonyMaker.ttf", 30);

	jhi_init_text(&t1);

	t1.pos.x = 80;
	t1.pos.y = 100;

	img.pos.x = 0;
	img.pos.y = 0;
	int i;
	char txt[20];
	int tab[3][3];
	int velha = 9;
	int turn = 0;
	int coluna;
	int linha;
	int diag_principal;
	int diag_secundaria;
	int game_over=0;
	jhi_draw_image(&img, img.pos);
	inicializa_matriz(tab);
	while (jhi_get_close_window() != JHI_CLOSE)
	{

		jhi_update();

		if(game_over){
			mouse = jhi_get_mouse_status(0);
			if (mouse.mouse_event == ON_CLICK_LEFT)
			{
				jhi_clean();
				jhi_draw_image(&img, img.pos);
				inicializa_matriz(tab);
				coluna=0;
				linha=0;
				diag_principal=0;
				diag_secundaria=0;
				velha=9;
				game_over=0;
			}

		}else{
			mouse = jhi_get_mouse_status(0);

			//se foi clicado botão esquerdo do mouse

			if (mouse.mouse_event == ON_CLICK_LEFT && turn == 0 && !game_over)
			{

				int a = mouse.x / 133;
				int b = mouse.y / 133;

				if(tab[b][a] == 0){
					jhi_draw_fill_circle(jhi_get_point(66.5+a*133,66.5+b*133), 50, RED);
					tab[b][a] = 1;
					velha--;
					coluna = tab[0][a] + tab[1][a] + tab[2][a];
					linha = tab[b][0] + tab[b][1] + tab[b][2];
					if(b==a){
						diag_principal = tab[0][0] + tab[1][1] + tab[2][2];
					}
					if(b+a==2){
						diag_secundaria = tab[0][2] + tab[1][1] + tab[2][0];
					}
					
					if(linha == 3 || coluna == 3 || diag_principal == 3 || diag_secundaria == 3){
						printf("%s\n","Vermelho ganhou!");
						memset(txt, 0, sizeof(char) * 20);
						sprintf(txt, "%s", "Vermelho ganhou!");
						jhi_set_text(&f1, &t1,BLACK, txt);
						jhi_draw_text(&t1, t1.pos);
						game_over =1;
					}  
					else if(velha == 0)
					{
						printf("%s\n", "Deu velha!");
						memset(txt, 0, sizeof(char) * 20);
						sprintf(txt, "%s", "Deu velha!");
						jhi_set_text(&f1, &t1,BLACK, txt);
						jhi_draw_text(&t1, t1.pos);
						game_over =1;
					}

					turn = 1;
				}

			}
			else if(mouse.mouse_event == ON_CLICK_LEFT && turn == 1 && !game_over){
				int a = mouse.x / 133;
				int b = mouse.y / 133;
				
				if(tab[b][a] == 0){
					jhi_draw_fill_circle(jhi_get_point(66.5+a*133,66.5+b*133), 50, YELLOW);
					tab[b][a] = -1;
					velha--;
					coluna = tab[0][a] + tab[1][a] + tab[2][a];
					linha = tab[b][0] + tab[b][1] + tab[b][2];
					diag_principal;
					if(b==a){
						diag_principal = tab[0][0] + tab[1][1] + tab[2][2];
					}
					if(b+a==2){
						diag_secundaria = tab[0][2] + tab[1][1] + tab[2][0];
					}
					
					if(linha == -3 || coluna == -3 || diag_principal == -3 || diag_secundaria == -3){
						printf("%s\n","Amarelo ganhou!");
						memset(txt, 0, sizeof(char) * 20);
						sprintf(txt, "%s", "Amarelo ganhou!");
						jhi_set_text(&f1, &t1, BLACK, txt);
						jhi_draw_text(&t1, t1.pos);
						game_over =1;
					} 				
					else if(velha == 0){
						printf("%s\n", "Deu velha!");
						memset(txt, 0, sizeof(char) * 20);
						sprintf(txt, "%s", "Deu velha!");
						jhi_set_text(&f1, &t1,BLACK, txt);
						jhi_draw_text(&t1, t1.pos);
						game_over =1;	
					}

					turn = 0;
				}				
			}
		}

	}

	jhi_free_text(&t1);

	jhi_free_font(&f1);

	jhi_free_image(&img);

	jhi_quit_and_free();

	return 0;
}

void inicializa_matriz(int m[][3])
{
	int i,j;
	for(i=0;i<3;i++){
		for(j=0;j<3;j++){
			m[i][j] = 0;
		}
	}
}



