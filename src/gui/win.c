#include "../../include/cub3d.h"

#define MINVAL(x, min)			if(x < min) x = min
#define MAXVAL(x, max)			if(x > max) x = max

int	hit_wall(t_map* m, int col, int row)
{
	char	ch;

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

static void draw_col(int side, t_dpoint sideDist, t_dpoint delta, t_img *img, int x)
{

	int			line_height;
	int			draw_point;
	int			y;
	double		walldist;
	int			color;

	if (side == 0) walldist = (sideDist.x - delta.x);
	else walldist = (sideDist.y - delta.y);
	MINVAL(walldist, 0.001);
	line_height = (int)(WIN_H / walldist);
	draw_point = -line_height / 2 + WIN_H / 2;
	MINVAL(draw_point, 0);
	y = 0;
	while (y < draw_point) put_color(img, x, y, COLOR_C), y++;
	draw_point = line_height / 2 + WIN_H / 2;
	MAXVAL(draw_point, WIN_H - 1);
	color = side ? COLOR_N : COLOR_E;
	while (y <= draw_point) put_color(img, x, y, color), y++;
	while (y < WIN_H) put_color(img, x, y, COLOR_F), y++;
};

static void	raycast(t_map *m, t_img *img, t_coord *c, int x)
{
	double		camera_x;
	t_dpoint	ray;
	t_dpoint 	delta;
	t_dpoint 	sideDist;
	t_point 	step;
	t_point 	map;
	int			hit;
	int			side;

	camera_x = 2.0 * x / (double)WIN_W - 1.0;
	ray.x = c->dir.x + c->plane.x * camera_x;
	ray.y = c->dir.y + c->plane.y * camera_x;
	map.x = (int)c->pos.x;
	map.y = (int)c->pos.y;
	delta.x = fabs(1.0 / ray.x);
	delta.y = fabs(1.0 / ray.y);
	step.x = 1 + (-2 * (ray.x < 0));
	step.y = 1 + (-2 * (ray.y < 0));
	sideDist.x = ((1 && (ray.x < 0)) + c->pos.x - map.x) * delta.x;
	if (ray.x < 0)
		sideDist.x = (c->pos.x - map.x) * delta.x;
	else
		sideDist.x = (map.x + 1.0 - c->pos.x) * delta.x;
	if (ray.y < 0)
		sideDist.y = (c->pos.y - map.y) * delta.y;
	else
		sideDist.y = (map.y + 1.0 - c->pos.y) * delta.y;
	hit = 0;
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
		if (hit_wall(m, map.x, map.y))
			hit = 1;
	}
	draw_col(side, sideDist, delta, img, x);
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
		raycast(&g->map, img, &g->coord, x);
		x++;
	}
	add_minimap(&g->map, img, &g->coord);
	mlx_put_image_to_window(g->mlx, g->win, img->img, 0, 0);
}
