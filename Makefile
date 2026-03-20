CC = cc
SRCS = \
src/main.c \
src/gui/win.c \
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
src/utils/error.c

CFLGS = -Wall -Wextra -Werror
#MLXFLAGS = -lmlx -Lmlx -Imlx -lXext -lX11 -lm -lz
LIBFT = libft/libft.a
NAME = cub3D
HEADER = include/cub3d.h

OBJ = $(SRCS:%.c=$(OBJDIR)/%.o)
OBJDIR = obj

MLX_C =  -I/usr/include -Imlx_linux -O3 
MLX_F = -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz

all: $(NAME)

$(NAME): $(OBJ)
	$(MAKE) -C libft
	$(CC) -o $(NAME) $(OBJ) $(LIBFT) $(MLX_F)

# %.o: %.c Makefile 
# 	$(CC) $(CFLGS) -c $< -o $@
$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLGS) $(MLX_C) -c $< -o $@


clean:
	$(MAKE) clean -C libft
	rm -rf $(OBJDIR)

fclean: clean
	$(MAKE) fclean -C libft
	rm -rf $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re