#include "../../include/cub3d.h"

void	free_map(t_map *map)
{
	t_map_node	*tmp;
	t_map_node	*next;

	if (!map)
		return;
	tmp = map->head;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->line);
		free(tmp);
		tmp = next;
	}
	map->head = NULL;
	map->width = 0;
	map->height = 0;
}

void	free_assets(t_game *game)
{
	if (game->assets.no)
		free(game->assets.no);
	if (game->assets.so)
		free(game->assets.so);
	if (game->assets.we)
		free(game->assets.we);
	if (game->assets.ea)
		free(game->assets.ea);
}

void	free_grid(t_map *map)
{
	if (!map || !map->grid)
		return;
	for (int i = 0; map->grid[i]; i++)
		free(map->grid[i]);
	free(map->grid);
	map->grid = NULL;
}

int	free_win(t_game *g)
{
	mlx_destroy_image(g->mlx, g->img.img);
	mlx_destroy_image(g->mlx, g->img1.img);
	mlx_destroy_window(g->mlx, g->win);
	mlx_destroy_display(g->mlx);
	free(g->mlx);
	exit(0);
	return (0);
}


void	free_game_content(t_game *game)
{
	if (!game)
		return;

	free_assets(game);
	free_map(&game->map);
	free_grid(&game->map);
	if (game->error_msg)
	{
		free(game->error_msg);
		game->error_msg = NULL;
	}
}
