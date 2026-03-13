#include "../include/cub3d.h"

int	main(int argc, char **argv)
{
	t_game game;
	validation_stage(argc, argv, &game);
	printf("%d\n",game.map.width);
	printf("%d\n",game.map.height);
	free_game_content(&game);
}