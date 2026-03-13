CC = cc
SRCS = \
src/game.c \
src/error_handling.c \
src/parsing/element_id.c \
src/parsing/utils_lines.c \
src/parsing/assets_state.c \
src/parsing/parse_texture.c \
src/parsing/parse_color.c \
src/parsing/process_elements.c \
src/parsing/map_list.c \
src/parsing/map_validation.c \
src/parsing/file_validation.c \
src/free_game.c
CFLGS = -Wall -Wextra -Werror
#MLXFLAGS = -lmlx -Lmlx -Imlx -lXext -lX11 -lm -lz
LIBFT = libft/libft.a
NAME = cub3D
RM = rm -rf
HEADER = include/cub3d.h

OBJ = $(SRCS:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(MAKE) -C libft
	$(CC) -o $(NAME) $(OBJ) $(LIBFT) 
#$(MLXFLAGS)

%.o: %.c Makefile 
	$(CC) $(CFLGS) -c $< -o $@

clean:
	$(MAKE) clean -C libft
	$(RM) $(OBJ)

fclean: clean
	$(MAKE) fclean -C libft
	$(RM) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re