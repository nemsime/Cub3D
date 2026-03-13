#include "../include/cub3d.h"

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

void	free_game_content(t_game *game)
{
	if (!game)
		return;

	free_assets(game);
	free_map(&game->map);

}