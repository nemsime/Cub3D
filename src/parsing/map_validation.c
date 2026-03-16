#include "../../include/cub3d.h"

int is_player(char *top_line,int i)
{
    if (top_line[i] == 'N' || top_line[i] == 'S' || 
        top_line[i] == 'E' || top_line[i] == 'W') 
    {
       return 1;
    }
    return 0;
}

void flood_fill(t_map *map, int x, int y)
{
    if (x < 0 || y < 0 || y >= map->height + 1 || x >= map->width + 1)
        return;
    char c = map->grid[y][x];
    if (c == '1' || c == 'X')
        return;
    if (c == '0')
    {
        map->error = 1;
        return;
    }
    map->grid[y][x] = 'X';
    flood_fill(map, x + 1, y);
    flood_fill(map, x - 1, y);
    flood_fill(map, x, y + 1);
    flood_fill(map, x, y - 1);
}

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

int map_copy(t_map *map)
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
    node = map->head;
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


int map_parsing(int fd,char *top_line,t_map *map,int *player_count)
{
    char *cur_line;
	int i;
    while (top_line)
    {
        cur_line = get_next_line(fd);
        trim_right(top_line);
        if (is_empty(top_line) && cur_line) // between map rows we have empty line
            return (free(cur_line),free(top_line),0);
        i = 0;
        while (top_line[i])
        {
            if (!ft_strchr(" 01NSEW", top_line[i])) // acceptable elements
                return (free(cur_line),free(top_line),0);
            if (is_player(top_line,i)) // player count and position in future(?)
                   (*player_count)++;
            i++;
        }
        if (!add_map_line(map, top_line))
            return (free(cur_line),free(top_line),0);
        free(top_line);
        top_line = cur_line;
    }
    if(*player_count != 1)
              return (free(cur_line),free(top_line),0);
    if(!map_copy(map)) 
         return (free(cur_line),free(top_line),0);
    flood_fill(map, 0, 0);
    if (map->error)
        return (free(cur_line),free(top_line),0);
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
