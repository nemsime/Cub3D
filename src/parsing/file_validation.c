#include "../../include/cub3d.h"

int	validate_file(int fd, t_game *game)
{
	char	*line;
	line = get_next_line(fd);
	if (!line ||  line[0] == '\0')
	{
		if (line)
			free(line);
		return (0);
	}
	while (line)
	{
		if(is_wall(line))
			break;
		if (!process_elements(line, game))
			return (free(line), 0);
		free(line);
		line = get_next_line(fd);
	}
	if (check_flag(game, 'Z') || (line[0] == '\0' || line[0] == '\n'))
		return (free(line),0);
	if (!map_parsing(fd, line, &game->map, &game->assets.state.player_count))
    	return (0);
	return (1);
}

void	validation_stage(int argc, char **argv, t_game *game)
{
	size_t	len;
	int		fd;

	if (argc != 2)
		end_error(0, "ERROR: usage: ./cub3D <*.cub>\n");
	len = ft_strlen(argv[1]);
	if (len < 4 || (ft_strncmp(argv[1] + len - 4, ".cub", 4) != 0))
		end_error(0, "ERROR: map must be .cub file\n");
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		end_error(0, "ERROR: open failed\n");
	ft_memset(game, 0, sizeof(t_game));
	if (!validate_file(fd, game))
	{
		free_game_content(game);
		get_next_line(-1);
		end_error(fd, "ERROR: elements are not valid\n");
	}
	close(fd);
}
