#include "../../include/cub3d.h"

int   process_elements(char *line, t_game *game)
{
    if (is_empty(line))
        return 1;
    t_id id = get_element_id(line);
    if (id == ID_NO || id == ID_SO || id == ID_WE || id == ID_EA)
        return (parse_texture(line, id, game));
    if (id == ID_F || id == ID_C)
        return (parse_color(line, id, game));
    return 0;
}

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
    if (check_flag(game, CHECK_DUPLICATE))
        return 0;
    assets_input(game, id, line, 0);
    return 1;
}

int parse_color(char *line, t_id id, t_game *game)
{
    if (!space_move(&line,id))
        return 0;
    char **rgb = ft_split(line, ',');
    if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
        return (free_split(&rgb), 0);
    int r = get_rgb_value(rgb[0]);
    int g = get_rgb_value(rgb[1]);
    int b = get_rgb_value(rgb[2]);
    free_split(&rgb);
    if (r < 0 || g < 0 || b < 0)
        return 0;
    int color = (r << 16) | (g << 8) | b;
    set_flag(game, id);
    if (check_flag(game, CHECK_DUPLICATE))
        return 0;
    assets_input(game, id, line, color);
    return 1;
}

int	get_rgb_value(char *rgb)
{
	char	*clean;
	int		j;
	int		value;

	j = 0;
	clean = ft_strtrim(rgb, " \t\n\r");
	if (!clean || clean[0] == '\0')
		return (-1);
	while (clean[j])
	{
		if (!ft_isdigit(clean[j]))
			return (free(clean), -1);
		j++;
	}
	value = ft_atoi(clean);
	if (value < 0 || value > 255)
		return (free(clean), -1);
	return (free(clean), value);
}
