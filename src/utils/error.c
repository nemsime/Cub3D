#include "../../include/cub3d.h"

void	end_error(int fd, char *str,t_game *game)
{
	write(2, str, ft_strlen(str));
	if (fd)
		close(fd);
	free_game_content(game);
	exit(EXIT_FAILURE);
}

void	set_error(t_game *game, const char *str)
{
	if (!game)
		return;
	game->error_msg = ft_strdup(str);
}
