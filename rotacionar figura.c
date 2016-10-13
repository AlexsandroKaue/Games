#include "jhi_window.h" // -> Esse o arquivo que possui as funções referentes a janela, inclusive
#include "jhi_timer.h" // -> Temporização para animação
#include "jhi_shapes.h"
#include "jhi_keyboard.h"
#include "time.h"
#include "math.h"

#define SIZE_BLOCK 50
#define WIDTH_WINDOW 500
#define HEIGHT_WINDOW 600 
#define OFFSET_HORIZON WIDTH_WINDOW / SIZE_BLOCK
#define MATRIX_ROW_INDEX HEIGHT_WINDOW / SIZE_BLOCK
#define MATRIX_COL_INDEX WIDTH_WINDOW / SIZE_BLOCK
#define SPEED 2

void rotation_matrix(JHI_Point2d poly[], int num, int orientation);
void translation_matrix(JHI_Point2d poly[], int num, int x, int y, int way);
JHI_Point2d** initialize_rotation(JHI_Point2d poly[], int num);
void free_rotation_matrix(JHI_Point2d** rotation);

int main()
{

	JHI_Point2d poly[6];
	JHI_Point2d origin;
	JHI_KeyboardSt key;

	origin.x = 250;
	origin.y = 250;

	poly[0].x = origin.x;
	poly[0].y = origin.y;
	//col_points[0] = pol_points[0]; 

	poly[1].x = origin.x + SIZE_BLOCK;
	poly[1].y = origin.y;
	
	poly[2].x = origin.x + SIZE_BLOCK;
	poly[2].y = origin.y + 4*SIZE_BLOCK;
	
	poly[3].x = origin.x;
	poly[3].y = origin.y + 4*SIZE_BLOCK;
	
	//poly[4].x = origin.x + SIZE_BLOCK;
	//poly[4].y = origin.y + 2*SIZE_BLOCK;
	
	//poly[5].x = origin.x;
	//poly[5].y = origin.y + 2*SIZE_BLOCK;

	int i = 4;
	int num = 4;
	//circle.x = 400;
	//circle.y = 400;
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

	//JHI_Point2d** rotation = initialize_rotation(poly,num);
	while (jhi_get_close_window() != JHI_CLOSE)
	{
		//Essas duas funções devem sempre esta no começo do loop
		//update() -> função de atualização das ações feitas dentro do loop
		//clean() -> função para limpar a tela, para desenhar novamente na próxima iteração
		jhi_update();
		jhi_clean();

		jhi_draw_polygon(poly, num, WHITE);
		//jhi_draw_fill_circle(circle, 50, WHITE);
		key = jhi_get_keyboard_status(0);
		if (key.key_event == KEYBOARD_DOWN)
		{
			if(key.key == KEY_UP)
			{
				i=(i+1)%4;
				
				rotation_matrix(poly,num,1);
				translation_matrix(poly,num,origin.x,origin.y,1);
			}
			if(key.key == KEY_DOWN){
				rotation_matrix(poly,num,-1);
				translation_matrix(poly,num,origin.x,origin.y,-1);
			}

			
		}
			
	}

	//Limpar alocações feitas pela biblioteca
	jhi_quit_and_free();

	return 0;
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

/*JHI_Point2d** initialize_rotation(JHI_Point2d poly[], int num){
	JHI_Point2d** rotation;
	rotation = (JHI_Point2d**)malloc(4*sizeof(JHI_Point2d*));
	int i,j;
	for (i = 0; i < 4; i++ )
		*(rotation + i) = (JHI_Point2d*)malloc(num*sizeof(JHI_Point2d));

	
	for(i=0;i<4;i++){
		for(j=0;j<num;j++){
			rotation[i][j] = poly[j];
		}
	}

	printf("Ponto 0 %d %d\n",rotation[0][0].x,rotation[0][0].y);
	printf("Ponto 1 %d %d\n",rotation[0][1].x,rotation[0][1].y);
	rotation_matrix(rotation[1],num,1);
	printf("Ponto 0 %d %d\n",rotation[1][0].x,rotation[1][0].y);
	printf("Ponto 1 %d %d\n",rotation[1][1].x,rotation[1][1].y);
	rotation_matrix(rotation[2],num,1);
	rotation_matrix(rotation[2],num,1);
	printf("Ponto 0 %d %d\n",rotation[2][0].x,rotation[2][0].y);
	printf("Ponto 1 %d %d\n",rotation[2][1].x,rotation[2][1].y);
	rotation_matrix(rotation[3],num,1);
	rotation_matrix(rotation[3],num,1);
	rotation_matrix(rotation[3],num,1);
	printf("Ponto 0 %d %d\n",rotation[3][0].x,rotation[3][0].y);
	printf("Ponto 1 %d %d\n",rotation[3][1].x,rotation[3][1].y);

	return rotation;
}

void free_rotation_matrix(JHI_Point2d** rotation){
	int i;
	for(i=0;i<4;i++)
		free(rotation[i]);
	free(rotation);
}*/




