#include "../../include/cub3d.h"

static void	init_side(t_coord *c, char ch)
{
	if (ch == 'N')
	{
		c->dir.x = 0.0;
		c->dir.y = -1.0;
	}
	else if (ch == 'S')
	{
		c->dir.x = 0.0;
		c->dir.y = 1.0;
	}
	else if (ch == 'E')
	{
		c->dir.x = 1.0;
		c->dir.y = 0.0;
	}
	else
	{
		c->dir.x = -1.0;
		c->dir.y = 0.0;
	}
}

void	init_map(t_map *m, t_coord *c)
{
	int		col;
	int		row;
	char	ch;

	row = 0;
	while (row < m->height)
	{
		col = 0;
		while (col < m->width)
		{
			ch = m->grid[row][col];
			if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
			{
				c->pos.x = col + 0.5;
				c->pos.y = row + 0.5;
				init_side(c, ch);
				c->plane.x = -c->dir.y * FOV;
				c->plane.y = c->dir.x * FOV;
			}
			col++;
		}
		row++;
	}
}
