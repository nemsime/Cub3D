#include "../../include/cub3d.h"
#include <math.h>


static char	*MAP = "111111\
100101\
101001\
1100N1\
111111";

#define MAP_STR_COLS	6
#define MAP_STR_ROWS	5
#define COLOR_F 		0x00606060
#define COLOR_C			0x00404040
#define COLOR_N			0x00804040
#define COLOR_S			0x00802040

#define MINVAL(x, min)	if(x < min) x = min
#define MAXVAL(x, max)	if(x > max) x = max

static double	g_pos_x;
static double	g_pos_y;
static double	g_dir_x;
static double	g_dir_y;
static double	g_plane_x;
static double	g_plane_y;

static int		close_window(t_game *f);
static int		key_hook(int key, t_game *f);

// static void	put_pixel(char *data, int x, int y, int color, int size_line, int bpp)
// {
// 	int	pixel;

// 	pixel = y * size_line + x * (bpp / 8);
// 	data[pixel] = color & 0xFF;
// 	data[pixel + 1] = (color >> 8) & 0xFF;
// 	data[pixel + 2] = (color >> 16) & 0xFF;
// }

static void put_color(t_img *img, int x, int y, int color)
{
	int pixel;
	
	pixel = y * img->line_length + x * img->bytes_per_pixel;
	img->addr[pixel] = color & 0xFF;
	img->addr[pixel + 1] = (color >> 8) & 0xFF;
	img->addr[pixel + 2] = (color >> 16) & 0xFF; 
}

static int	map_cell(int col, int row)
{
	return (MAP[row * MAP_STR_COLS + col]);
}


static void	init_map_raycast(void)
{
	int		col;
	int		row;
	char	c;
	
	g_pos_x = MAP_STR_COLS / 2.0;
	g_pos_y = MAP_STR_ROWS / 2.0;
	g_dir_x = -1.0;
	g_dir_y = 0.0;
	g_plane_x = 0.0;
	g_plane_y = 0.66;
	row = 0;
	while (row < MAP_STR_ROWS)
	{
		col = 0;
		while (col < MAP_STR_COLS)
		{
			c = map_cell(col, row);
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
			{
				g_pos_x = col + 0.5;
				g_pos_y = row + 0.5;
				if (c == 'N')
				{
					g_dir_x = 0.0;
					g_dir_y = -1.0;
				}
				else if (c == 'S')
				{
					g_dir_x = 0.0;
					g_dir_y = 1.0;
				}
				else if (c == 'E')
				{
					g_dir_x = 1.0;
					g_dir_y = 0.0;
				}
				else
				{
					g_dir_x = -1.0;
					g_dir_y = 0.0;
				}
				g_plane_x = -g_dir_y * 0.66;
				g_plane_y = g_dir_x * 0.66;
			}
			col++;
		}
		row++;
	}
}

static int	rc_hit_wall(int col, int row)
{
	char	ch;

	if (col < 0 || row < 0 || col >= MAP_STR_COLS || row >= MAP_STR_ROWS)
		return (1);
	ch = map_cell(col, row);
	if (ch == '1')
		return (1);
	if (ch == '0')
		return (0);
	if (ch == 'N' || ch == 'S' || ch == 'E' || ch == 'W')
		return (0);
	return (1);
}


static void	draw_column_3d(t_img *img, int x)
{
	double		camera_x;
	double		ray_dir_x;
	double		ray_dir_y;
	int			map_x;
	int			map_y;
	double		side_dist_x;
	double		side_dist_y;
	double		delta_dist_x;
	double		delta_dist_y;
	double		perp_wall_dist;
	int			step_x;
	int			step_y;
	int			hit;
	int			side;
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			y;
	int			color;

	camera_x = 2.0 * x / (double)WIN_W - 1.0;
	ray_dir_x = g_dir_x + g_plane_x * camera_x;
	ray_dir_y = g_dir_y + g_plane_y * camera_x;
	map_x = (int)g_pos_x;
	map_y = (int)g_pos_y;
	delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1.0 / ray_dir_x);
	delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1.0 / ray_dir_y);
	hit = 0;
	if (ray_dir_x < 0)
	{
		step_x = -1;
		side_dist_x = (g_pos_x - map_x) * delta_dist_x;
	}
	else
	{
		step_x = 1;
		side_dist_x = (map_x + 1.0 - g_pos_x) * delta_dist_x;
	}
	if (ray_dir_y < 0)
	{
		step_y = -1;
		side_dist_y = (g_pos_y - map_y) * delta_dist_y;
	}
	else
	{
		step_y = 1;
		side_dist_y = (map_y + 1.0 - g_pos_y) * delta_dist_y;
	}
	while (hit == 0)
	{
		if (side_dist_x < side_dist_y)
		{
			side_dist_x += delta_dist_x;
			map_x += step_x;
			side = 0;
		}
		else
		{
			side_dist_y += delta_dist_y;
			map_y += step_y;
			side = 1;
		}
		if (rc_hit_wall(map_x, map_y))
			hit = 1;
	}
	if (side == 0) perp_wall_dist = (side_dist_x - delta_dist_x);
	else perp_wall_dist = (side_dist_y - delta_dist_y);
	MINVAL(perp_wall_dist, 0.001);
	line_height = (int)(WIN_H / perp_wall_dist);
	draw_start = -line_height / 2 + WIN_H / 2;
	MINVAL(draw_start, 0);
	draw_end = line_height / 2 + WIN_H / 2;
	MAXVAL(draw_end, WIN_H - 1);
	color = side ? COLOR_N : COLOR_S;
	y = 0;
	while (y < draw_start) put_color(img, x, y, COLOR_C), y++;
	y = draw_start;
	while (y <= draw_end) put_color(img, x, y, color), y++;
	y = draw_end + 1;
	while (y < WIN_H) put_color(img, x, y, COLOR_F), y++;
}


static void	draw(t_game *d)
{
	t_img	*img;
	int x;

	if ((int)d->img_n == 1)
		img = &d->img1;
	else
		img = &d->img;
	d->img_n *= -1;
	x = 0;
	while (x < WIN_W)
	{
		draw_column_3d(img, x);
		x++;
	}
	mlx_put_image_to_window(d->mlx, d->win, img->img, 0, 0);
}

//d 100 s 115 a 97 w 119
static int	key_hook(int key, t_game *f)
{
	if (key == 65307)
		return (close_window(f), 0);
	else if (key == 97)
		g_pos_x -= 0.1;
	else if (key == 100)
		g_pos_x += 0.1;
	else if (key == 119)
		g_pos_y -= 0.1;
	else if (key == 115)
		g_pos_y += 0.1;
	else if (key == 65361)
		g_dir_x += 0.1;
	else if (key == 65363)
		g_dir_x -= 0.1;
	
	// if (key == 65361)
	// 	g_pos_x -= 0.1;
	// if (key == 65363)
	// 	g_pos_x += 0.1;
	// if (key == 65362)
	// 	g_pos_y -= 0.1;
	// if (key == 65364)
	// 	g_pos_y += 0.1;
	// check_pos()?
	printf("g_pos_x: %f, g_pos_y: %f\n%d", g_pos_x, g_pos_y, key);
	draw(f);
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
	g->img_n = 1;
	init_map_raycast();
}

void	start_gui(t_game *g)
{
	init_game(g);
	draw(g);
	mlx_hook(g->win, 17, 0, close_window, g);
	mlx_hook(g->win, 2, 1L << 0, key_hook, g);
	mlx_loop(g->mlx);
}
