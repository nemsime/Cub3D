#include "../../include/cub3d.h"
#include <math.h>

int	hit_wall(t_map *m, int col, int row)
{
	char	ch;

	if (col < 0 || row < 0)
		return (1);
	if ((int)ft_strlen(m->grid[row]) <= col)
		return (1);
	ch = m->grid[row][col];
	if (ch == '1')
		return (1);
	if (ch == '0')
		return (0);
	if (ch == '/')
		return (-1);
	if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
		return (0);
	return (1);
}

void	put_color(t_img *img, int x, int y, int color)
{
	int	pixel;

	pixel = y * img->line_length + x * img->bytes_per_pixel;
	img->addr[pixel] = color & 0xFF;
	img->addr[pixel + 1] = (color >> 8) & 0xFF;
	img->addr[pixel + 2] = (color >> 16) & 0xFF;
}

void	init_image(t_img *i, void *mlx, char *xpm)
{
	char	*new_path;

	if (xpm)
	{
		new_path = ft_strdup(xpm);
		if (!new_path)
			return ;
		if (i->path)
			free(i->path);
		i->path = new_path;
		xpm = i->path;
	}
	if (!mlx)
		return ;
	i->w = WIN_W;
	i->h = WIN_H;
	if (!xpm)
		i->img = mlx_new_image(mlx, WIN_W, WIN_H);
	else
		i->img = mlx_xpm_file_to_image(mlx, xpm, &i->w, &i->h);
	if (!i->img)
		return ;
	i->addr = mlx_get_data_addr(i->img, &i->bits_per_pixel, &i->line_length,
			&i->endian);
	i->bytes_per_pixel = i->bits_per_pixel / 8;
}
