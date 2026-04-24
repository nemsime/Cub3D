/*
** Clean, small-function raycasting and textured wall drawing.
** This file replaces a corrupted version. Functions are small to ease 42 norm
** compliance and to keep warnings under control (initialized variables,
** explicit checks, parenthesized bit ops).
*/

#include "../../include/cub3d.h"

static int	get_tex_color_img(t_img *t, int tex_x, int tex_y)
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

static void	compute_line_params(int side, t_dpoint sideDist, t_dpoint delta,
		double *perp, int *line_h, int *draw_start, int *draw_end)
{
	if (side == 0)
		*perp = sideDist.x - delta.x;
	else
		*perp = sideDist.y - delta.y;
	MINVAL(*perp, 0.001);
	*line_h = (int)(WIN_H / *perp);
	*draw_start = -(*line_h) / 2 + WIN_H / 2;
	MINVAL(*draw_start, 0);
	*draw_end = (*line_h) / 2 + WIN_H / 2;
	MAXVAL(*draw_end, WIN_H - 1);
}

static double	compute_wall_x(int side, t_coord *c, t_dpoint ray, double perp)
{
	double	v;

	if (side == 0)
		v = c->pos.y + perp * ray.y;
	else
		v = c->pos.x + perp * ray.x;
	return (v - floor(v));
}

static void	calc_tex_params(t_img *tex, double wall_x, int side, int line_h,
		int *tex_x, double *tex_step, double *tex_pos, int draw_start,
		t_dpoint ray)
{
	if (!tex || !tex->img)
		return ;
	*tex_x = (int)(wall_x * (double)tex->w);
	if (*tex_x < 0)
		*tex_x = 0;
	if (*tex_x >= tex->w)
		*tex_x = tex->w - 1;
	if (side == 0 && ray.x > 0)
		*tex_x = tex->w - *tex_x - 1;
	if (side == 1 && ray.y < 0)
		*tex_x = tex->w - *tex_x - 1;
	*tex_step = 1.0 * tex->h / line_h;
	*tex_pos = (draw_start - WIN_H / 2.0 + line_h / 2.0) * *tex_step;
}

static void	draw_strip(t_game *g, t_img *img, t_img *tex, int tx, double tpos,
		double tstep, int ds, int de, int x)
{
	int	y;
	int	ty;
	int	color;

	y = 0;
	while (y < ds)
		put_color(img, x, y++, g->assets.ceiling_color);
	while (y <= de)
	{
		ty = (int)tpos;
		if (ty < 0)
			ty = 0;
		if (ty >= tex->h)
			ty = tex->h - 1;
		color = get_tex_color_img(tex, tx, ty);
		put_color(img, x, y++, color);
		tpos += tstep;
	}
	while (y < WIN_H)
		put_color(img, x, y++, g->assets.floor_color);
}

static void	draw_col(t_game *g, int side, t_dpoint sideDist, t_dpoint delta,
		t_img *img, int x, t_coord *c, t_dpoint ray)
{
	double	perp;
	int		line_h;
	int		draw_start;
	int		draw_end;
	t_img	*tex;
	int		tx;
	double	tstep;
	double	tpos;
	double	wall_x;
		int y;

	compute_line_params(side, sideDist, delta, &perp, &line_h, &draw_start,
		&draw_end);
	tex = select_wall_texture_img(g, side, ray);
	wall_x = compute_wall_x(side, c, ray, perp);
	tx = 0;
	tstep = 0.0;
	tpos = 0.0;
	calc_tex_params(tex, wall_x, side, line_h, &tx, &tstep, &tpos, draw_start,
		ray);
	if (tex && tex->img)
		draw_strip(g, img, tex, tx, tpos, tstep, draw_start, draw_end, x);
	else
	{
		y = 0;
		while (y < draw_start)
			put_color(img, x, y++, g->assets.ceiling_color);
		while (y <= draw_end)
			put_color(img, x, y++, 0x00808080);
		while (y < WIN_H)
			put_color(img, x, y++, g->assets.floor_color);
	}
}

static void	setup_ray(t_coord *c, int x, t_dpoint *ray, t_dpoint *delta,
		t_dpoint *sideDist, t_point *step, t_point *map)
{
	double	camera_x;

	camera_x = 2.0 * x / (double)WIN_W - 1.0;
	ray->x = c->dir.x + c->plane.x * camera_x;
	ray->y = c->dir.y + c->plane.y * camera_x;
	map->x = (int)c->pos.x;
	map->y = (int)c->pos.y;
	delta->x = fabs(1.0 / ray->x);
	delta->y = fabs(1.0 / ray->y);
	if (ray->x < 0)
		step->x = -1;
	else
		step->x = 1;
	if (ray->y < 0)
		step->y = -1;
	else
		step->y = 1;
	if (ray->x < 0)
		sideDist->x = (c->pos.x - map->x) * delta->x;
	else
		sideDist->x = (map->x + 1.0 - c->pos.x) * delta->x;
	if (ray->y < 0)
		sideDist->y = (c->pos.y - map->y) * delta->y;
	else
		sideDist->y = (map->y + 1.0 - c->pos.y) * delta->y;
}

static void	perform_dda(t_game *g, t_dpoint *sideDist, t_dpoint *delta,
		t_point *step, t_point *map, int *side)
{
	int	hit;

	hit = 0;
	while (hit == 0)
	{
		if (sideDist->x < sideDist->y)
		{
			sideDist->x += delta->x;
			map->x += step->x;
			*side = 0;
		}
		else
		{
			sideDist->y += delta->y;
			map->y += step->y;
			*side = 1;
		}
		if (hit_wall(&g->map, map->x, map->y))
			hit = 1;
	}
}

static void	raycast(t_game *g, t_img *img, t_coord *c, int x)
{
	t_dpoint	ray;
	t_dpoint	delta;
	t_dpoint	sideDist;
	t_point		step;
	t_point		map;
	int			side;

	setup_ray(c, x, &ray, &delta, &sideDist, &step, &map);
	perform_dda(g, &sideDist, &delta, &step, &map, &side);
	draw_col(g, side, sideDist, delta, img, x, c, ray);
}

void	draw(t_game *g)
{
	t_img	*img;
	int		x;

	img = &g->img;
	x = 0;
	while (x < WIN_W)
	{
		raycast(g, img, &g->coord, x);
		x++;
	}
	add_minimap(&g->map, img, &g->coord);
	mlx_put_image_to_window(g->mlx, g->win, img->img, 0, 0);
}
