#include "../../include/cub3d.h"

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

static int	handle_wasd(int key, t_dpoint *res, t_coord *c, double move)
{
	if (key == 119)
	{
		res->x = c->pos.x + c->dir.x * move;
		res->y = c->pos.y + c->dir.y * move;
	}
	else if (key == 115)
	{
		res->x = c->pos.x - c->dir.x * move;
		res->y = c->pos.y - c->dir.y * move;
	}
	else if (key == 97)
	{
		res->x = c->pos.x + c->dir.y * move;
		res->y = c->pos.y - c->dir.x * move;
	}
	else if (key == 100)
	{
		res->x = c->pos.x - c->dir.y * move;
		res->y = c->pos.y + c->dir.x * move;
	}
	else
		return (0);
	return (1);
}

static void	handle_arrows(int key, t_coord *c, double rot)
{
	double	old_dir_x;
	double	old_plane_x;

	if (key == 65361 || key == 65363)
	{
		if (key == 65361)
			rot = -rot;
		old_dir_x = c->dir.x;
		c->dir.x = c->dir.x * cos(rot) - c->dir.y * sin(rot);
		c->dir.y = old_dir_x * sin(rot) + c->dir.y * cos(rot);
		old_plane_x = c->plane.x;
		c->plane.x = c->plane.x * cos(rot) - c->plane.y * sin(rot);
		c->plane.y = old_plane_x * sin(rot) + c->plane.y * cos(rot);
	}
}

static int	key_hook(int key, t_game *g)
{
	t_coord		*c;
	t_dpoint	res;

	c = &g->coord;
	res = c->pos;
	if (key == 65307)
		return (close_window(g), 0);
	else if (handle_wasd(key, &res, c, 0.1))
	{
		c->plane.x = -c->dir.y * FOV;
		c->plane.y = c->dir.x * FOV;
	}
	else
		handle_arrows(key, c, 0.1);
	if (!hit_wall(&g->map, res.x, res.y))
	{
		c->pos.x = res.x;
		c->pos.y = res.y;
	}
	draw(g);
	return (0);
}

void	start_gui(t_game *g)
{
	g->mlx = mlx_init();
	g->win = mlx_new_window(g->mlx, WIN_W, WIN_H, "cub3D");
	init_image(&g->img, g->mlx, 0);
	init_image(&g->img1, g->mlx, 0);
	init_image(g->assets.no, g->mlx, g->assets.no->path);
	init_image(g->assets.so, g->mlx, g->assets.so->path);
	init_image(g->assets.we, g->mlx, g->assets.we->path);
	init_image(g->assets.ea, g->mlx, g->assets.ea->path);
	g->img_n = 1;
	init_map(&g->map, &g->coord);
	draw(g);
	mlx_hook(g->win, 17, 0, close_window, g);
	mlx_hook(g->win, 2, 1L << 0, key_hook, g);
	mlx_loop(g->mlx);
}
