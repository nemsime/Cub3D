#include "../include/cub3d.h"


char	**convert_map(t_map_node *map_list,int height)
{
	char	**map;
	int		i;

	map = malloc(sizeof(char *) * (height + 3));
	if (!map)
		return (NULL);

	i = 0;
	while (map_list)
	{
		map[i] = ft_strdup(map_list->line);
		map_list = map_list->next;
		i++;
	}

	map[i] = NULL;
	return (map);
}

int	main(int argc, char **argv)
{
	t_game game;
	validation_stage(argc, argv, &game);
	//convert_map(game.map.head,game.map.height);
	free_game_content(&game);
}