#include "../../include/cub3d.h"


void put_color(t_img *img, int x, int y, int color)
{
	int pixel;
	
	pixel = y * img->line_length + x * img->bytes_per_pixel;
	img->addr[pixel] = color & 0xFF;
	img->addr[pixel + 1] = (color >> 8) & 0xFF;
	img->addr[pixel + 2] = (color >> 16) & 0xFF; 
}

static int	close_window(t_game *f)
{
	mlx_destroy_image(f->mlx, f->img1.img);
	mlx_destroy_image(f->mlx, f->img.img);
	mlx_destroy_window(f->mlx, f->win);
	mlx_destroy_display(f->mlx);
	free(f->mlx);
	exit(0);
	return (0);
}

static int	key_hook(int key, t_game *g)
{
	t_coord *c = &g->coord;

	if (key == 65307)
		return (close_window(g), 0);
	else if (key == 97 && c->pos.x < g->map.width - 0.1)
		c->pos.x += 0.1;
	else if (key == 100 && c->pos.x > 0.1)
		c->pos.x -= 0.1;
	else if (key == 119 && c->pos.y < g->map.height - 0.1)
		c->pos.y += 0.1;
	else if (key == 115 && c->pos.y > 0.1)
		c->pos.y -= 0.1;
	else if (key == 65361)
		c->dir.x += 0.1;
	else if (key == 65363)
		c->dir.x -= 0.1;
	
	// check_pos()?
	printf("c->pos.x: %f, c->pos.y: %f\n%d", c->pos.x, c->pos.y, key);
	draw(g);
	return (0);
}

static void	init_image(t_img *i, void *mlx)
{
	i->img = mlx_new_image(mlx, WIN_W, WIN_H);
	i->addr = mlx_get_data_addr(i->img, &i->bits_per_pixel, &i->line_length,
			&i->endian);
	i->bytes_per_pixel = i->bits_per_pixel / 8;
}

void	start_gui(t_game *g)
{
	g->mlx = mlx_init();
	g->win = mlx_new_window(g->mlx, WIN_W, WIN_H, "cub3D");
	init_image(&g->img, g->mlx);
	init_image(&g->img1, g->mlx);
	g->img_n = 1;
	init_map_raycast(&g->map, &g->coord);
	draw(g);
	mlx_hook(g->win, 17, 0, close_window, g);
	mlx_hook(g->win, 2, 1L << 0, key_hook, g);
	mlx_loop(g->mlx);
}

