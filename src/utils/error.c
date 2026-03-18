#include "../../include/cub3d.h"

void	end_error(int fd, char *str)
{
	write(2, str, ft_strlen(str));
	if (fd)
		close(fd);
	exit(EXIT_FAILURE);
}
