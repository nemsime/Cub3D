#include "../../include/cub3d.h"

int parse_texture(char *line, t_id id, t_game *game)
{
    if (!space_move(&line,id)) 
        return 0;
    trim_right(line);
    int len = ft_strlen(line);
    if (len < 4 || ft_strncmp(line + len - 4, ".xpm", 4) != 0)
        return 0;
    int fd = open(line, O_RDONLY);
    if (fd < 0)
        return 0;
    close(fd);
    set_flag(game, id);
    if (check_flag(game, 'D'))
        return 0;
    assets_input(&game, id, line, 0);
    return 1;
}