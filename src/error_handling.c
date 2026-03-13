#include "../include/cub3d.h"

void	end_error(int fd, char *str)
{
	write(2, str, ft_strlen(str));
	if (fd)
		close(fd);
	exit(EXIT_FAILURE);
}

void	free_split(char ***arr)
{
	int i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i])
	{
		free((*arr)[i]);
		i++;
	}
	free(*arr);
	*arr = NULL;
}
