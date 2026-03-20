#include "../../include/cub3d.h"

static int	close_window(t_game *f);
static int	key_hook(int key, t_game *f);

static void put_pixel(char *data, int x, int y, int color, int size_line, int bpp) {
    int pixel = y * size_line + x * (bpp / 8);
    data[pixel] = color & 0xFF; 			// blue
    data[pixel + 1] = (color >> 8) & 0xFF; 	// green
    data[pixel + 2] = (color >> 16) & 0xFF; // red
}


static void	draw(t_game *d)
{
	t_img		*img;
	int			x;
	int			y;
	int			color;

	if ((int)d->img_n == 1)
		img = &d->img1;
	else
		img = &d->img;
	d->img_n *= -1;

	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			color = (x & 255) << 16 | (y & 255) << 8; //| ((x + y) & 255);
			put_pixel(img->addr, x, y, color, img->line_length, img->bits_per_pixel);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(d->mlx, d->win, img->img, 0, 0);
}

static int	key_hook(int key, t_game *f)
{
	if (key == 65307)
		close_window(f);
	// else if (key >= CODE_LEFT && key < CODE_LEFT + 5)
	// 	move_view(f, key - CODE_LEFT);
	// else if (key == 61)
	// 	zoom(f, 0.8, WIN_H / 2, WIN_W / 2);
	// else if (key == 45)
	// 	zoom(f, 1.2, WIN_H / 2, WIN_W / 2);
	// draw_f(f);
	return (0);
}

int	close_window(t_game *f)
{
	mlx_destroy_image(f->mlx, f->img1.img);
	mlx_destroy_image(f->mlx, f->img.img);
	mlx_destroy_window(f->mlx, f->win);
	mlx_destroy_display(f->mlx);
	free(f->mlx);
	exit(0);
	return (0);
}



static void	init_image(t_img *i, void *mlx)
{
	i->img = mlx_new_image(mlx, WIN_W, WIN_H);
	i->addr = mlx_get_data_addr(i->img, &i->bits_per_pixel, &i->line_length,
			&i->endian);
	i->bytes_per_pixel = i->bits_per_pixel / 8;
}

static void	init_game(t_game *g)
{
	g->mlx = mlx_init();
	g->win = mlx_new_window(g->mlx, WIN_W, WIN_H, "cub3D");
	init_image(&g->img, g->mlx);
    init_image(&g->img1, g->mlx);
    g->img_n = 1; // switches between image 1 and -1

    // g->error_msg = NULL;
    // g->map.head = NULL;
    // g->map.tail = NULL;
    // g->map.grid = NULL;
    // g->map.width = 0;
    // g->map.height = 0;
    // g->map.error = 0;
    // g->assets.no = NULL;
    // g->assets.so = NULL;
    // g->assets.we = NULL;
    // g->assets.ea = NULL;
}

void start_gui(t_game *g)
{
	init_game(g);

	draw(g);
	mlx_hook(g->win, 17, 0, close_window, g);
	mlx_hook(g->win, 2, 1L << 0, key_hook, g);
	mlx_loop(g->mlx);
}