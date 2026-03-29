#include "../../include/cub3d.h"

char	*process_assets(int fd, t_game *game)
{
	char	*line;

	line = get_next_line(fd);
	if (!line || line[0] == '\0')
	{
		if (line)
			free(line);
		set_error(game, "ERR: empty or corrupted file\n");
		return (NULL);
	}
	while (line)
	{
		if (is_wall(line))
			return (line);
		if (!process_elements(line, game))
			return (free(line), NULL);
		free(line);
		line = get_next_line(fd);
	}
	set_error(game, "ERR: missing assets or no map\n");
	return (NULL);
}

int	validate_map(int fd, char *first_line, t_game *game)
{
	t_map_node	*map_head;

	if (check_flag(game, CHECK_MISSING))
		return (set_error(game, "ERR: missing assets or no map\n"), free(first_line), 0);
	map_head = map_parsing(fd, first_line, game);
	if (!map_head)
		return (0);
	if (!map_copy(map_head, &game->map))
	{
		free_map(map_head);
		return (set_error(game, "ERR: memory allocation failed while copying map\n"), 0);
	}
	free_map(map_head);
	return (map_validation(game));
}

int	validate_file(int fd, t_game *game)
{
	char	*first_line;

	first_line = process_assets(fd, game);
	if (!first_line)
		return (0);
	return (validate_map(fd, first_line, game));
}

void	validation_stage(int argc, char **argv, t_game *game)
{
	size_t	len;
	int		fd;

	if (argc != 2)
		end_error(0, "ERROR: usage: ./cub3D <*.cub>\n",NULL);
	len = ft_strlen(argv[1]);
	if (len < 4 || (ft_strncmp(argv[1] + len - 4, ".cub", 4) != 0))
		end_error(0, "ERROR: map must be .cub file\n",NULL);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		end_error(0, "ERROR: open failed\n",NULL);
	ft_memset(game, 0, sizeof(t_game));
	if (!validate_file(fd, game))
	{
		get_next_line(-1);
		end_error(fd, game->error_msg,game);
	}
	close(fd);
}
