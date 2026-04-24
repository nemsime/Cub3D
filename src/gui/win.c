#include "../../include/cub3d.h"

static void	setup_ray(t_coord *c, int x)
{
	double	camera_x;

	camera_x = 2.0 * x / (double)WIN_W - 1.0;
	c->ray.x = c->dir.x + c->plane.x * camera_x;
	c->ray.y = c->dir.y + c->plane.y * camera_x;
	c->map.x = (int)c->pos.x;
	c->map.y = (int)c->pos.y;
	c->delta.x = fabs(1.0 / c->ray.x);
	c->delta.y = fabs(1.0 / c->ray.y);
	if (c->ray.x < 0)
		c->step.x = -1;
	else
		c->step.x = 1;
	if (c->ray.y < 0)
		c->step.y = -1;
	else
		c->step.y = 1;
	if (c->ray.x < 0)
		c->sideDist.x = (c->pos.x - c->map.x) * c->delta.x;
	else
		c->sideDist.x = (c->map.x + 1.0 - c->pos.x) * c->delta.x;
	if (c->ray.y < 0)
		c->sideDist.y = (c->pos.y - c->map.y) * c->delta.y;
	else
		c->sideDist.y = (c->map.y + 1.0 - c->pos.y) * c->delta.y;
}

static void	dda(t_game *g)
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		if (g->coord.sideDist.x < g->coord.sideDist.y)
		{
			g->coord.sideDist.x += g->coord.delta.x;
			g->coord.map.x += g->coord.step.x;
			g->coord.side = 0;
		}
		else
		{
			g->coord.sideDist.y += g->coord.delta.y;
			g->coord.map.y += g->coord.step.y;
			g->coord.side = 1;
		}
		if (hit_wall(&g->map, g->coord.map.x, g->coord.map.y))
			hit = 1;
	}
}

void	draw(t_game *g)
{
	t_img	*img;
	int		x;

	img = &g->img;
	x = 0;
	while (x < WIN_W)
	{
		setup_ray(&g->coord, x);
		dda(g);
		draw_col(g, x, img);
		x++;
	}
	add_minimap(&g->map, img, &g->coord);
	mlx_put_image_to_window(g->mlx, g->win, img->img, 0, 0);
}

int	get_tex_color_img(t_img *t, int tex_x, int tex_y)
{
	int	pixel;

	if (!t || !t->addr)
		return (0);
	if (tex_x < 0 || tex_y < 0 || tex_x >= t->w || tex_y >= t->h)
		return (0);
	pixel = tex_y * t->line_length + tex_x * t->bytes_per_pixel;
	return (((unsigned char)t->addr[pixel]) | (((unsigned char)t->addr[pixel
					+ 1]) << 8) | (((unsigned char)t->addr[pixel + 2]) << 16));
}
