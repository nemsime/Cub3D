#include "../../include/cub3d.h"

int   process_elements(char *line, t_game *game)
{
    if (is_empty(line))
        return 1;
    t_id id = get_element_id(line);
    if (id == ID_NO || id == ID_SO || id == ID_WE || id == ID_EA)
        return (parse_texture(line, id, game));
    if (id == ID_F || id == ID_C)
        return (parse_color(line, id, game));
    return 0;
}