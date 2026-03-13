#include "../../include/cub3d.h"

int map_parsing(int fd,char *top_line,t_map *map)
{
    char *cur_line;
	int i;
    while (top_line)
    {
		cur_line = get_next_line(fd);
		trim_right(top_line);
        if (is_empty(top_line) && cur_line)
            return (free(cur_line),free(top_line),0);
		if(!is_line_closed(top_line))
			return (free(cur_line),free(top_line),0);
		i = 0;
        while (top_line[i])
        {
            if (top_line[i] != ' ' && top_line[i] != '0' && top_line[i] != '1' &&
                top_line[i] != 'N' && top_line[i] != 'S' &&
                top_line[i] != 'E' && top_line[i] != 'W')
                return (free(cur_line),free(top_line),0);
			i++;
        }
		if(!is_first_wall(top_line) && cur_line == NULL)
			return (free(cur_line),free(top_line),0);
        if (!add_map_line(map, top_line))
            return (free(cur_line),free(top_line),0);
        free(top_line);
        top_line = cur_line;
    }
    return (1); 
}


int is_first_wall(char *line)
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

int is_line_closed(char *line)
{
    char *start;
    char *end;

    if (!line)
        return 0;
    start = line;
    while (*start && ft_isspace(*start))
        start++;
    if (*start != '1')
        return 0;
    end = line + ft_strlen(line) - 1;
    while (end > start && ft_isspace(*end))
        end--;
    if (*end != '1')
        return 0;
    return 1;
}