#include "../../include/cub3d.h"

static int	inrange(int x, float px, int s)
{
	return (x >= px - s && x <= px + s);
}

static void	adds(t_coord *c, t_img *i, int x, int y)
{
	float	px;
	float	py;
	int		s;

	px = c->pos.x * MMSCALE;
	py = c->pos.y * MMSCALE;
	s = 5;
	if (inrange(x, px, s) && inrange(y, py, s) && (x - px) * (x - px) + (y - py)
		* (y - py) < s * s)
		put_color(i, x + MM_PADING, y + MM_PADING, MMCOLP);
	else if (y % MMSCALE == 0 || x % MMSCALE == 0)
		put_color(i, x + MM_PADING, y + MM_PADING, MMCOL);
}

void	add_minimap(t_map *m, t_img *i, t_coord *c)
{
	char	res;
	int		x;
	int		y;

	x = -1;
	while (++x < (m->width + 2) * MMSCALE)
	{
		y = -1;
		while (++y < (m->height + 2) * MMSCALE)
		{
			res = hit_wall(m, x / MMSCALE, y / MMSCALE);
			if (res == 1)
				put_color(i, x + MM_PADING, y + MM_PADING, MMCOLW);
			else if (res == 0)
				put_color(i, x + MM_PADING, y + MM_PADING, MMCOL);
			else
				continue ;
			adds(c, i, x, y);
		}
	}
}
