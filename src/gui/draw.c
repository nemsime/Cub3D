#include "../../include/cub3d.h"

static t_img	*select_wall_texture_img(t_game *g)
{
	t_coord	*c;

	c = &g->coord;
	if (c->side == 0)
	{
		if (c->ray.x > 0)
			return (g->assets.we);
		return (g->assets.ea);
	}
	if (c->ray.y > 0)
		return (g->assets.no);
	return (g->assets.so);
}

static int	compute_line(t_coord *c, double *perp)
{
	int	line_h;

	if (c->side == 0)
		*perp = c->sideDist.x - c->delta.x;
	else
		*perp = c->sideDist.y - c->delta.y;
	MINVAL(*perp, 0.001);
	line_h = (int)(WIN_H / *perp);
	c->draw_start = -(line_h) / 2 + WIN_H / 2;
	MINVAL(c->draw_start, 0);
	c->draw_end = (line_h) / 2 + WIN_H / 2;
	MAXVAL(c->draw_end, WIN_H - 1);
	return (line_h);
}

static void	calc_tex_params(t_coord *c, t_tex *tex, double wall_x, int line_h)
{
	if (!tex || !tex->img)
		return ;
	tex->x = (int)(wall_x * (double)tex->img->w);
	if (tex->x < 0)
		tex->x = 0;
	if (tex->x >= tex->img->w)
		tex->x = tex->img->w - 1;
	if (c->side == 0 && c->ray.x > 0)
		tex->x = tex->img->w - tex->x - 1;
	if (c->side == 1 && c->ray.y < 0)
		tex->x = tex->img->w - tex->x - 1;
	tex->step = 1.0 * tex->img->h / line_h;
	tex->pos = (c->draw_start - WIN_H / 2.0 + line_h / 2.0) * tex->step;
}

static void	draw_strip(t_game *g, t_img *img, t_tex *tex, int x)
{
	int	y;
	int	ty;
	int	color;

	y = 0;
	while (y < g->coord.draw_start)
		put_color(img, x, y++, g->assets.ceiling_color);
	while (y <= g->coord.draw_end)
	{
		ty = (int)tex->pos;
		if (ty < 0)
			ty = 0;
		if (ty >= tex->img->h)
			ty = tex->img->h - 1;
		color = get_tex_color_img(tex->img, tex->x, ty);
		put_color(img, x, y++, color);
		tex->pos += tex->step;
	}
	while (y < WIN_H)
		put_color(img, x, y++, g->assets.floor_color);
}

void	draw_col(t_game *g, int x, t_img *img)
{
	double	perp;
	int		line_h;
	t_tex	tex;
	double	wall_x;

	line_h = compute_line(&g->coord, &perp);
	tex.img = select_wall_texture_img(g);
	if (g->coord.side == 0)
		wall_x = g->coord.pos.y + perp * g->coord.ray.y;
	else
		wall_x = g->coord.pos.x + perp * g->coord.ray.x;
	wall_x -= floor(wall_x);
	tex.x = 0;
	tex.step = 0.0;
	tex.pos = 0.0;
	calc_tex_params(&g->coord, &tex, wall_x, line_h);
	draw_strip(g, img, &tex, x);
}
