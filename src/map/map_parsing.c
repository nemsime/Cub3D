#include "../../include/cub3d.h"

int	validate_map_line(char *line, t_game *game)
{
	int i = 0;

	while (line[i])
	{
		if (!ft_strchr(" 01NSEW", line[i]))
			return (set_error(game, "ERROR: invalid character in map\n"), 0);
		if (is_player(line, i))
			game->assets.state.player_count++;
		i++;
	}
	return (1);
}

t_map_node *map_parsing(int fd, char *top_line, t_game *game)
{
	char *cur_line;
	t_map_list list;

	ft_memset(&list, 0, sizeof(t_map_list));
	while (top_line)
	{
		cur_line = get_next_line(fd);
		trim_right(top_line);
		if (is_empty(top_line) && cur_line)
			return (set_error(game, "ERR: empty line in map\n"),free(cur_line), free(top_line), NULL);
		if (!validate_map_line(top_line, game))
			return (free(cur_line), free(top_line), NULL);
		if (!add_map_line(&list, top_line))
			return (set_error(game, "ERR: malloc failed while parsing map\n"),free(cur_line), free(top_line), NULL);
		free(top_line);
		top_line = cur_line;
	}
	game->map.width = list.width;
	game->map.height = list.height;
	return (list.head);
}

int add_map_line(t_map_list *list, char *line)
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
    if (!list->head)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    len = ft_strlen(line);
    if (len > list->width)
        list->width = len;
    list->height++;
    return (1);
}

int is_player(char *top_line,int i)
{
    if (top_line[i] == 'N' || top_line[i] == 'S' || 
        top_line[i] == 'E' || top_line[i] == 'W') 
       return 1;
    return 0;
}
