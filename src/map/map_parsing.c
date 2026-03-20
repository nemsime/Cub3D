#include "../../include/cub3d.h"

int map_parsing(int fd, char *top_line, t_game *game)
{
    char *cur_line;
	int i;
    while (top_line)
    {
        cur_line = get_next_line(fd);
        trim_right(top_line);
        if (is_empty(top_line) && cur_line)
            return (set_error(game, "ERR: empty line in map\n"),free(cur_line), free(top_line), 0);
        i = 0;
        while (top_line[i])
        {
            if (!ft_strchr(" 01NSEW", top_line[i]))
                return (set_error(game, "ERROR: invalid character in map\n"), free(cur_line), free(top_line), 0);
            if (is_player(top_line, i))
                game->assets.state.player_count++;
            i++;
        }
        if (!add_map_line(&game->map, top_line))
            return (set_error(game, "ERR: malloc failed while parsing map\n"),free(cur_line), free(top_line), 0);
        free(top_line);
        top_line = cur_line;
    }
    return (1);
}

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

int is_player(char *top_line,int i)
{
    if (top_line[i] == 'N' || top_line[i] == 'S' || 
        top_line[i] == 'E' || top_line[i] == 'W') 
       return 1;
    return 0;
}
