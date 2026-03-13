#include "../../include/cub3d.h"

int add_map_line(t_map *map, char *line)
{
    t_map_node *new_node;
    int len;

    new_node = malloc(sizeof(t_map_node));
    if (!new_node)
        return (0);
    new_node->line = ft_strdup(line);
    if (!new_node->line)
        return (free(new_node), 0);
    new_node->next = NULL;
    if (!map->head)
    {
        map->head = new_node;
        map->tail = new_node;
    }
    else
    {
        map->tail->next = new_node;
        map->tail = new_node;
    }
    len = ft_strlen(line);
    if (len > map->width)
        map->width = len;
    map->height++;
    return (1);
}