CC = cc
SRCS = \
src/main.c \
src/parsing/parse_elements.c \
src/parsing/parse_elements_utils.c \
src/parsing/parse_assets.c \
src/parsing/parse_file.c \
src/map/map_parsing.c \
src/map/map_validation.c \
src/map/map_copy.c \
src/map/flood_fill.c \
src/utils/utils.c \
src/utils/free.c \
src/utils/error.c \

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