#include "../../include/cub3d.h"

void flood_fill(t_map *map, int x, int y)
{
    if (x < 0 || y < 0 || y >= map->height + 2 || x >= map->width + 2)
        return;
    char c = map->grid[y][x];
    if (c == '1' || c == '/')
        return;
    if (c == '0' || c == 'N' || c == 'S' || c == 'W' || c == 'E')
    {
        map->error = 1;
        return;
    }
    if (c == ' ')
    {
        map->grid[y][x] = '/';
        flood_fill(map, x + 1, y);
        flood_fill(map, x - 1, y);
        flood_fill(map, x, y + 1);
        flood_fill(map, x, y - 1);
    }
}

