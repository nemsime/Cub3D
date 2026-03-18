#include "../include/cub3d.h"

int	main(int argc, char **argv)
{
	t_game game;
	validation_stage(argc, argv, &game);
	free_game_content(&game);
}
