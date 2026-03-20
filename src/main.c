#include "../include/cub3d.h"

int	main(int argc, char **argv)
{
	t_game game;
	// validation_stage(argc, argv, &game);
	(void)argc; (void)argv;

	/* Dummy mode: start_gui uses map dims for sizing; free_game_content
	** needs pointers to be NULL-safe. */
	ft_memset(&game, 0, sizeof(t_game));

	// init_game(&game);
	start_gui(&game);
	free_game_content(&game);
}
