#include "../include/cub3d.h"

// static void print_map(t_game *g)
// {
// 	// t_map_node *n = g->map.head;
// 	// for (n; n++; n != g->map.tail)
// 	// 	{
// 	// 		for (char c = )
// 	// 		printf("\n");
// 	// 	}
// 	t_map m = g->map;
// 	for (int i = 0; i < m.height; i++){
// 	for (int j = 0; j < m.width; j++)

// 		printf("%c ", m.grid[i][j]);
// 	printf("\n");
// 	}
// }

int	main(int argc, char **argv)
{
	t_game game;
	ft_memset(&game, 0, sizeof(t_game));

	validation_stage(argc, argv, &game);

	// print_map(&game);
	start_gui(&game);
	free_game_content(&game);
}
