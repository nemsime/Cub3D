#include "../include/cub3d.h"

static void print_map(t_game *g)
{
	t_map m = g->map;
	for (int i = 0; i < m.height + 2; i++){
	for (int j = 0; j < m.width + 2; j++)

		printf("%c ", m.grid[i][j]);
	printf("\n");
	}
}
 
int	main(int argc, char **argv)
{
	t_game game;
	ft_memset(&game, 0, sizeof(t_game));

	validation_stage(argc, argv, &game);

	print_map(&game);
	start_gui(&game);
}
