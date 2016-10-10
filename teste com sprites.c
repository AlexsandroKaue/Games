#include "jhi_window.h" // -> Esse o arquivo que possui as funções referentes a janela,
#include "jhi_image.h" //-> Esse arquivo é header da estrutura Image
#include "jhi_sound.h"
#include "jhi_keyboard.h"
#include "jhi_timer.h"


//****************************************************************************
//*                                                                          *
//*  Exemplo 11: Pac Man com teclado					     			 	 *
//*                                                                          *
//*  Year: 2014                                                              *
//*  Version:  1.0                                                           *
//*  Author(s): João Gonçalves <joao.goncalves@larces.uece.br>               *
//*             Henrique Neto <henrique.brandao@larces.uece.br>              *
//****************************************************************************/

//largura e altura de cada pac_man na figura
#define WIDHT_JUMP_MAN 119.75
#define HEIGHT_JUMP_MAN 182
#define JUMP_MAN_NUMBER_CLIPS 8
#define WIDHT_WALK_MAN 94.6
#define HEIGHT_WALK_MAN 173
#define WALK_MAN_NUMBER_CLIPS 8

int main()
{
	JHI_Point2d vet_pos_jump[]  = {
	{0,0}, {WIDHT_JUMP_MAN,0}, {2*WIDHT_JUMP_MAN,0}, {3*WIDHT_JUMP_MAN,0},
	{4*WIDHT_JUMP_MAN,0}, {5*WIDHT_JUMP_MAN,0}, {6*WIDHT_JUMP_MAN,0},{7*WIDHT_JUMP_MAN,0}};

	JHI_Point2d vet_pos_walk[]  = {
	{0,0}, {WIDHT_WALK_MAN,0}, {2*WIDHT_WALK_MAN,0}, {3*WIDHT_WALK_MAN,0},
	{4*WIDHT_WALK_MAN,0}, {5*WIDHT_WALK_MAN,0}, {6*WIDHT_WALK_MAN,0},{7*WIDHT_WALK_MAN,0}};

	//imagem com todos as posições do pac_man
	JHI_Image toy_jump;
	JHI_Image toy_walk;

	int pos = 0;
	JHI_Point2d point_toy_jump;
	JHI_Point2d point_toy_walk;

	//Essa função irá inicializar a janela e todos os outros módulos incluindo som, fonte.
	jhi_initialize_window(800, 600, 32, WHITE);

	//Nessa função escolhemos o nome da janela, que ficará no topo da tela
	jhi_choice_window_name("Exemplo 11");

	//A cor de fundo do pac_man é o azul de valores (0, 0, 255) do modelo RGB
	// a cor BLUE contém esses valores, logo esse cor de fundo fica transparente
	jhi_load_image_with_transparent_color(&toy_jump, "images/salto.png", BLUE);
	jhi_load_image_with_transparent_color(&toy_walk, "images/walk.png", BLUE);

	jhi_set_fps_timer(10);

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
		
		point_toy_jump.x = 0;
		point_toy_jump.y = 0;
		point_toy_walk.x = 0;
		point_toy_walk.y = HEIGHT_JUMP_MAN;
		//Desenha o sprite correto do pacman
		jhi_draw_image_with_clip(&toy_jump, point_toy_jump, vet_pos_jump[pos%8], WIDHT_JUMP_MAN,  HEIGHT_JUMP_MAN);
		jhi_draw_image_with_clip(&toy_walk, point_toy_walk, vet_pos_walk[pos%8], WIDHT_WALK_MAN,  HEIGHT_WALK_MAN);
		pos++;
		//Espera tempo necessário para controlar a quantidade de frames por segundo
		jhi_wait_time();
	}

	//desalocar a imagem que foi carregada
	jhi_free_image(&toy_jump);
	jhi_free_image(&toy_walk);

	//Limpar alocações feitas pela biblioteca
	jhi_quit_and_free();

	return 0;
}
