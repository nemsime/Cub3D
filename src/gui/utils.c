#include "../../include/cub3d.h"
#include <math.h>


void put_color(t_img *img, int x, int y, int color)
{
	int pixel;
	
	pixel = y * img->line_length + x * img->bytes_per_pixel;
	img->addr[pixel] = color & 0xFF;
	img->addr[pixel + 1] = (color >> 8) & 0xFF;
	img->addr[pixel + 2] = (color >> 16) & 0xFF; 
}

static int	close_window(t_game *g)
{
	free_game_content(g);
	mlx_destroy_image(g->mlx, g->img1.img);
	mlx_destroy_image(g->mlx, g->img.img);
	mlx_destroy_window(g->mlx, g->win);
	mlx_destroy_display(g->mlx);
	free(g->mlx);
	g->mlx = NULL;
	exit(0);
}

static int	key_hook(int key, t_game *g)
{
	t_coord *c = &g->coord;
	t_dpoint res = {c->pos.x, c->pos.y};
	double	move;
	double	rot;
	double	old_dir_x;
	double	old_plane_x;

	move = 0.1;
	if (key == 65307)
		return (close_window(g), 0);
	else if (key == 119) // w
	{
		res.x = c->pos.x + c->dir.x * move;
		res.y = c->pos.y + c->dir.y * move;
	}
	else if (key == 115) // s
	{
		res.x = c->pos.x - c->dir.x * move;
		res.y = c->pos.y - c->dir.y * move;
	}
	else if (key == 97) // a
	{
		res.x = c->pos.x + c->dir.y * move;
		res.y = c->pos.y - c->dir.x * move;
	}
	else if (key == 100) // d
	{
		res.x = c->pos.x - c->dir.y * move;
		res.y = c->pos.y + c->dir.x * move;
	}
	else if (key == 65361 || key == 65363) // left/right smooth rotation
	{
		rot = 0.1;
		if (key == 65361)
			rot = -rot;
		old_dir_x = c->dir.x;
		c->dir.x = c->dir.x * cos(rot) - c->dir.y * sin(rot);
		c->dir.y = old_dir_x * sin(rot) + c->dir.y * cos(rot);
		old_plane_x = c->plane.x;
		c->plane.x = c->plane.x * cos(rot) - c->plane.y * sin(rot);
		c->plane.y = old_plane_x * sin(rot) + c->plane.y * cos(rot);
	}

	if (key != 65361 && key != 65363)
	{
		c->plane.x = -c->dir.y * FOV;
		c->plane.y = c->dir.x * FOV;
	}
	if (!hit_wall(&g->map, res.x, res.y))
		c->pos.x = res.x, c->pos.y = res.y;
	
	// check_pos()?
	// printf("c->pos.x: %f, c->pos.y: %f\tkey:%d\tdirX:%f\tdirY:%f\n", 
	// 	c->pos.x, c->pos.y, key, c->dir.x, c->dir.y);
	draw(g);
	return (0);
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

void	start_gui(t_game *g)
{
	g->mlx = mlx_init();
	g->win = mlx_new_window(g->mlx, WIN_W, WIN_H, "cub3D");
	init_image(&g->img, g->mlx, 0);
	init_image(&g->img1, g->mlx, 0);
	if (g->assets.no && g->assets.no->path)
		init_image(g->assets.no, g->mlx, g->assets.no->path);
	if (g->assets.so && g->assets.so->path)
		init_image(g->assets.so, g->mlx, g->assets.so->path);
	if (g->assets.we && g->assets.we->path)
		init_image(g->assets.we, g->mlx, g->assets.we->path);
	if (g->assets.ea && g->assets.ea->path)
		init_image(g->assets.ea, g->mlx, g->assets.ea->path);
	g->img_n = 1;
	init_map(&g->map, &g->coord);
	draw(g);
	mlx_hook(g->win, 17, 0, close_window, g);
	mlx_hook(g->win, 2, 1L << 0, key_hook, g);
	mlx_loop(g->mlx);
}

