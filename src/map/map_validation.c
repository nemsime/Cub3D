#include "../../include/cub3d.h"

int map_validation(t_map *map, int p_count)
{
	if (p_count != 1)
		return (0);
	if (!map_copy(map))
		return (0);
	flood_fill(map, 0, 0);
	if (map->error)
		return (0);
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
