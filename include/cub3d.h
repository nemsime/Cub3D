#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include "../mlx_linux/mlx.h"
# include <fcntl.h>
# include <stdlib.h>
# include <math.h>

# define WIN_W 1000
# define WIN_H 1000

/* ENUMS */

typedef enum e_id
{
    ID_NO,
    ID_SO,
    ID_WE,
    ID_EA,
    ID_F,
    ID_C,
    ID_UNKNOWN
} t_id;

typedef enum e_flag_mode
{
    CHECK_DUPLICATE,
    CHECK_MISSING
} t_flag_mode;

/* STRUCTS */

typedef struct s_map_node
{
    char                *line;
    struct s_map_node   *next;
}   t_map_node;

typedef struct s_map
{
	t_map_node  *head;
	t_map_node *tail;
	char **grid;
	int width;
	int height;
	int error;
} t_map;

typedef struct s_parse_state
{
	int	no;
	int	so;
	int	we;
	int	ea;
	int	f;
	int	c;
	int player_count;
} t_parse_state;

typedef struct s_assets
{
	char			*no;
	char			*so;
	char			*we;
	char			*ea;

	int				floor_color;
	int				ceiling_color;
	t_parse_state	state;
} t_assets;


typedef struct s_point
{
	int				x;
	int				y;
}					t_point;

typedef struct s_dpoint
{
	double			x;
	double			y;
}					t_dpoint;


typedef struct s_img
{
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				bytes_per_pixel;
	int				line_length;
	int				endian;
}					t_img;

typedef struct s_coord
{
	t_dpoint		pos;
	t_dpoint		dir;
	t_dpoint		plane;
}					t_coord;

typedef struct s_game
{
	t_map			map;
	t_assets		assets;
	t_coord			coord;

	t_img			img;
	t_img			img1;
	char 			img_n;

	void			*mlx;
	void			*win;
	char			*error_msg;
} t_game;

/* ==================== GUI ==================== */
void	start_gui(t_game *game);
void	add_minimap(t_img*, t_coord*);
void 	put_color(t_img *img, int x, int y, int color);
void	init_map_raycast(t_coord* c); // temp funciton
void	draw(t_game *g);

// void	init_game(t_game *game);

/* ==================== PARSING ==================== */

t_id	get_element_id(char *line);
int		process_elements(char *line, t_game *game);

int		parse_texture(char *line, t_id id, t_game *game);
int		parse_color(char *line, t_id id, t_game *game);

int		validate_file(int fd, t_game *game);
void	validation_stage(int argc, char **argv, t_game *game);

int	map_parsing(int fd, char *top_line, t_game *game);
int	map_validation(t_game *game);

/* ==================== MAP ==================== */

int		add_map_line(t_map *map, char *line);
int		is_player(char *line, int i);

void	flood_fill(t_map *map, int x, int y);

char	*create_map_row(const char *line, int width);
int		map_copy(t_map *map);

int		is_wall(char *line);

/* ==================== ASSETS ==================== */

void	set_flag(t_game *game, t_id id);
int		check_flag(t_game *game, t_flag_mode mode);
void	assets_input(t_game *game, t_id id, char *texture, int color);

/* ==================== UTILS ==================== */

int		is_empty(char *line);
int		starts_with(char *line, const char *element);
void	trim_right(char *str);
char	*space_move(char **line, t_id id);

int		ft_isspace(int c);
int		get_rgb_value(char *rgb);

/* ==================== MEMORY ==================== */

void	free_split(char ***arr);
void	free_game_content(t_game *game);
void	free_map(t_map *map);
void	free_grid(t_map *map);
void	free_assets(t_game *game);

/* ==================== ERROR ==================== */

void	end_error(int fd, char *str, t_game *game);
void	set_error(t_game *game, const char *str);

# endif