#include "../../include/cub3d.h"

int map_validation(t_game *game)
{
    if (game->map.height < 3 || game->map.width < 3)
    {
        return (set_error(game, "ERR: smallest possible valid map is 3x3\n"), 0);
    }
	if (game->assets.state.player_count != 1)
        return (set_error(game, "ERR: map must contain exactly one player\n"), 0);
	flood_fill(&game->map, 0, 0);
	if (game->map.error)
		return (set_error(game, "ERR: map is not enclosed by walls\n"), 0);
	return (1);
}

int is_wall(char *line)
{
    int i = 0;
    if (!line)
        return 0;
    while (line[i] && ft_isspace(line[i]))
        i++;
    if (!line[i])
        return 0;
    while (line[i] && line[i] != '\n')
    {
        if (line[i] != '1' && !ft_isspace(line[i]))
            return 0;
        i++;
    }
    return 1;
}
