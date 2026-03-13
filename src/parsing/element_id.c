#include "../../include/cub3d.h"

t_id get_element_id(char *line)
{
    while (*line == ' ' || *line == '\t')
        line++;
    if (!ft_strncmp(line, "NO ", 3))
        return ID_NO;
    if (!ft_strncmp(line, "SO ", 3))
        return ID_SO;
    if (!ft_strncmp(line, "WE ", 3))
        return ID_WE;
    if (!ft_strncmp(line, "EA ", 3))
        return ID_EA;
    if (!ft_strncmp(line, "F ", 2))
        return ID_F;
    if (!ft_strncmp(line, "C ", 2))
        return ID_C;
    return ID_UNKNOWN;
}