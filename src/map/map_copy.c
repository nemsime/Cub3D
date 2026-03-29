#include "../../include/cub3d.h"

char *create_map_row(const char *line, int width)
{
    char *row = malloc(width + 3);
    if (!row)
        return NULL;
    ft_memset(row, ' ', width + 2);
    row[width + 2] = '\0';
    if (line)
        ft_memcpy(&row[1], line, ft_strlen(line));
    return row;
}

int map_copy(t_map_node *head, t_map *map)
{
    t_map_node *node;
    int i;
    
    map->grid = (char **)malloc(sizeof(char *) * (map->height + 3));
    if (!map->grid)
        return (0);
    i = 0;
    map->grid[i++] = create_map_row(NULL, map->width);
    if (!map->grid[i-1])
        return 0;
    node = head;
    while (node)
    {
        map->grid[i++] = create_map_row(node->line, map->width);
        if (!map->grid[i-1])
            return 0;
        node = node->next;
    }
    map->grid[i++] = create_map_row(NULL, map->width);
    if (!map->grid[i-1])
        return 0;
    map->grid[i] = NULL;
    return (1);
}
