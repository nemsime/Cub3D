#include "../../include/cub3d.h"

/*
static char	*MAP = "\
1111111\
1001001\
1010001\
1100N01\
1111111";

#define MAP_STR_COLS	7
#define MAP_STR_ROWS	5
*/

#define MINVAL(x, min)			if(x < min) x = min
#define MAXVAL(x, max)			if(x > max) x = max


void	init_map_raycast(t_map* m, t_coord* c)
{
	int		col;
	int		row;
	char	ch;
	
	// c->pos.x = m->height / 2.0;
	// c->pos.y = m->width / 2.0;
	// c->dir.x = -1.0;
	// c->dir.y = 0.0;
	// c->plane.x = 0.0;
	// c->plane.y = 0.66;
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
				c->plane.x = -c->dir.y * 0.66;
				c->plane.y = c->dir.x * 0.66;
			}
			col++;
		}
		row++;
	}
}

int	rc_hit_wall(t_map* m, int col, int row)
{
	char	ch;

	/* m->grid is indexed as grid[row][col] */
	if (col < 0 || row < 0 )
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

static void	draw_column_3d(t_map *m, t_img *img, t_coord *c, int x)
{
	double		camera_x;
	t_dpoint	ray;
	t_dpoint 	delta;
	t_dpoint 	sideDist;
	double		perp_wall_dist;
	t_point 	step;
	t_point 	map;
	int			hit;
	int			side;
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			y;
	int			color;

	camera_x = 2.0 * x / (double)WIN_W - 1.0;
	ray.x = c->dir.x + c->plane.x * camera_x;
	ray.y = c->dir.y + c->plane.y * camera_x;
	map.x = (int)c->pos.x;
	map.y = (int)c->pos.y;
	delta.x = fabs(1.0 / ray.x);
	delta.y = fabs(1.0 / ray.y);
	hit = 0;
	if (ray.x < 0)
	{
		step.x = -1;
		sideDist.x = (c->pos.x - map.x) * delta.x;
	}
	else
	{
		step.x = 1;
		sideDist.x = (map.x + 1.0 - c->pos.x) * delta.x;
	}
	if (ray.y < 0)
	{
		step.y = -1;
		sideDist.y = (c->pos.y - map.y) * delta.y;
	}
	else
	{
		step.y = 1;
		sideDist.y = (map.y + 1.0 - c->pos.y) * delta.y;
	}
	while (hit == 0)
	{
		if (sideDist.x < sideDist.y)
		{
			sideDist.x += delta.x;
			map.x += step.x;
			side = 0;
		}
		else
		{
			sideDist.y += delta.y;
			map.y += step.y;
			side = 1;
		}
		if (rc_hit_wall(m, map.x, map.y))
			hit = 1;
	}
	if (side == 0) perp_wall_dist = (sideDist.x - delta.x);
	else perp_wall_dist = (sideDist.y - delta.y);
	MINVAL(perp_wall_dist, 0.001);
	line_height = (int)(WIN_H / perp_wall_dist);
	draw_start = -line_height / 2 + WIN_H / 2;
	MINVAL(draw_start, 0);
	draw_end = line_height / 2 + WIN_H / 2;
	MAXVAL(draw_end, WIN_H - 1);
	color = side ? COLOR_N : COLOR_E;
	y = 0;
	while (y < draw_start) put_color(img, x, y, COLOR_C), y++;
	y = draw_start;
	while (y <= draw_end) put_color(img, x, y, color), y++;
	y = draw_end + 1;
	while (y < WIN_H) put_color(img, x, y, COLOR_F), y++;
}


void	draw(t_game *g)
{
	t_img	*img;
	int x;

	if ((int)g->img_n == 1)
		img = &g->img1;
	else
		img = &g->img;
	g->img_n *= -1;
	x = 0;
	while (x < WIN_W)
	{
		draw_column_3d(&g->map, img, &g->coord, x);
		x++;
	}
	add_minimap(&g->map, img, &g->coord);
	mlx_put_image_to_window(g->mlx, g->win, img->img, 0, 0);
}
