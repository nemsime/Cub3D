#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdlib.h>

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
} t_map;

typedef struct s_parse_state
{
	int	no;
	int	so;
	int	we;
	int	ea;
	int	f;
	int	c;
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

typedef struct s_game
{
	t_map			map;
	t_assets		assets;
	// t_player		player;

	void			*mlx;
	void			*win;
	void			*img;
} t_game;

/* PARSING */

t_id	get_element_id(char *line);
int		process_elements(char *line, t_game *game);

int		parse_texture(char *line, t_id id, t_game *game);
int		parse_color(char *line, t_id id, t_game *game);

int		validate_file(int fd, t_game *game);
int		map_parsing(int fd, char *top_line, t_map *map);

int		add_map_line(t_map *map, char *line);

/* ASSETS HELPERS */

void    set_flag(t_game *game, t_id id);
int     check_flag(t_game *game, char flag);
void    assets_input(t_game **game, t_id id, char *texture, int color);

/* VALIDATION */

void	validation_stage(int argc, char **argv, t_game *game);

/* UTILS */

int		is_empty(char *line);
int		starts_with(char *line, const char *element);
void	trim_right(char *str);
char	*space_move(char **line, t_id id);

int		ft_isspace(int c);
int		is_first_wall(char *line);
int		is_line_closed(char *line);

/* MEMORY MANAGEMENT */

void	free_split(char ***arr);
void	free_game_content(t_game *game);
void	free_map(t_map *map);

/* ERROR HANDLING */

void	end_error(int fd, char *str);

#endif