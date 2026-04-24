#include "../../include/cub3d.h"


static int	get_tex_color_img(t_img *t, int tex_x, int tex_y)
{
	int	pixel;

	if (!t || !t->addr)
		return (0);
	if (tex_x < 0 || tex_y < 0 || tex_x >= t->w || tex_y >= t->h)
		return (0);
	pixel = tex_y * t->line_length + (t->w - tex_x) * t->bytes_per_pixel;
	return ((unsigned char)t->addr[pixel] | ((unsigned char)t->addr[pixel
			+ 1] << 8) | ((unsigned char)t->addr[pixel + 2] << 16));
}

static t_img	*select_wall_texture_img(t_game *g, int side, t_dpoint ray)
{
	if (side == 0)
	{
		if (ray.x > 0)
			return (g->assets.we);
		return (g->assets.ea);
	}
	if (ray.y > 0)
		return (g->assets.no);
	return (g->assets.so);
}

static void	draw_col(t_game *g, int side, t_dpoint sideDist, t_dpoint delta,
		t_img *img, int x, t_coord *c, t_dpoint ray)
{
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		y;
	double	perp_wall_dist;
	int		color;
	t_img	*tex_img;
	int		tex_x;
	double	wall_x;
	double	tex_step;
	double	tex_pos;
	int		tex_y;

	if (side == 0)
		perp_wall_dist = (sideDist.x - delta.x);
	else
		perp_wall_dist = (sideDist.y - delta.y);
	MINVAL(perp_wall_dist, 0.001);
	line_height = (int)(WIN_H / perp_wall_dist);
	draw_start = -line_height / 2 + WIN_H / 2;
	MINVAL(draw_start, 0);
	draw_end = line_height / 2 + WIN_H / 2;
	MAXVAL(draw_end, WIN_H - 1);
	tex_img = select_wall_texture_img(g, side, ray);
	if (side == 0)
		wall_x = c->pos.y + perp_wall_dist * ray.y;
	else
		wall_x = c->pos.x + perp_wall_dist * ray.x;
	wall_x -= floor(wall_x);
	tex_x = 0;
	tex_step = 0.0;
	tex_pos = 0.0;
	tex_x = (int)(wall_x * (double)tex_img->w);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= tex_img->w)
		tex_x = tex_img->w - 1;
	if (side == 0 && ray.x > 0)
		tex_x = tex_img->w - tex_x - 1;
	if (side == 1 && ray.y < 0)
		tex_x = tex_img->w - tex_x - 1;
	tex_step = 1.0 * tex_img->h / line_height;
	tex_pos = (draw_start - WIN_H / 2.0 + line_height / 2.0) * tex_step;
	y = 0;
	while (y < draw_start)
		put_color(img, x, y, g->assets.ceiling_color), y++;
	while (y <= draw_end)
	{
		tex_y = (int)tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= tex_img->h)
			tex_y = tex_img->h - 1;
		color = get_tex_color_img(tex_img, tex_x, tex_y);
		put_color(img, x, y, color);
		tex_pos += tex_step;
		y++;
	}
	while (y < WIN_H)
		put_color(img, x, y, g->assets.floor_color), y++;
}

static void	raycast(t_game *g, t_img *img, t_coord *c, int x)
{
	double		camera_x;
	t_dpoint	ray;
	t_dpoint	delta;
	t_dpoint	sideDist;
	t_point		step;
	t_point		map;
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
		if (hit_wall(&g->map, map.x, map.y))
			hit = 1;
	}
	draw_col(g, side, sideDist, delta, img, x, c, ray);
}

void	draw(t_game *g)
{
	t_img	*img;
	int		x;

	if ((int)g->img_n == 1)
		img = &g->img1;
	else
		img = &g->img;
	g->img_n *= -1;
	x = 0;
	while (x < WIN_W)
	{
		raycast(g, img, &g->coord, x);
		x++;
	}
	add_minimap(&g->map, img, &g->coord);
	mlx_put_image_to_window(g->mlx, g->win, img->img, 0, 0);
}
