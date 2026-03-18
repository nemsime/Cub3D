#include "../../include/cub3d.h"

void assets_input(t_game *game, t_id id, char *texture, int color)
{
    if (id == ID_NO)
        game->assets.no = ft_strdup(texture);
    else if (id == ID_SO)
        game->assets.so = ft_strdup(texture);
    else if (id == ID_WE)
        game->assets.we = ft_strdup(texture);
    else if (id == ID_EA)
        game->assets.ea = ft_strdup(texture);
    else if (id == ID_F)
        game->assets.floor_color = color;
    else if (id == ID_C)
        game->assets.ceiling_color = color;
}

void set_flag(t_game *game, t_id id)
{
    if (id == ID_NO)
        game->assets.state.no++;
    else if (id == ID_SO)
        game->assets.state.so++;
    else if (id == ID_WE)
        game->assets.state.we++;
    else if (id == ID_EA)
        game->assets.state.ea++;
    else if (id == ID_F)
        game->assets.state.f++;
    else if (id == ID_C)
        game->assets.state.c++;
}

int	check_flag(t_game *game, t_flag_mode mode)
{
	if (mode == CHECK_DUPLICATE)
	{
		if (game->assets.state.we > 1 || game->assets.state.ea > 1
			|| game->assets.state.so > 1 || game->assets.state.no > 1
			|| game->assets.state.c > 1 || game->assets.state.f > 1)
			return (1);
	}
	else if (mode == CHECK_MISSING)
	{
		if (game->assets.state.we == 0 || game->assets.state.ea == 0
			|| game->assets.state.so == 0 || game->assets.state.no == 0
			|| game->assets.state.c == 0 || game->assets.state.f == 0)
			return (1);
	}
	return (0);
}
