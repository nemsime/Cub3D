#include "../../include/cub3d.h"


int	is_empty(char *line)
{
	int	i;

	if (!line)
		return (1);
	i = 0;
	while (line[i])
	{
		if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n'
			&& line[i] != '\r')
			return (0);
		i++;
	}
	return (1);
}

int	starts_with(char *line, const char *element)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	while (element[j] && line[i + j])
	{
		if (line[i + j] != element[j])
			return (0);
		j++;
	}
	if (element[j] == '\0')
		return (1);
	return (0);
}

void	trim_right(char *str)
{
	int	len;

	if (!str)
		return ;
	len = ft_strlen(str);
	while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len
			- 1] == '\n' || str[len - 1] == '\r'))
	{
		str[len - 1] = '\0';
		len--;
	}
}

char *space_move(char **line,t_id id)
{
    if (!line || !*line)
        return NULL;

    while (**line == ' ' || **line == '\t')
        {(*line)++;}
	
	if (id == ID_NO || id == ID_SO || id == ID_WE || id == ID_EA)
       { *line += 3;}
    else if (id == ID_F || id == ID_C)
        {*line += 2;} 

	while (**line == ' ' || **line == '\t')
        {(*line)++;}

    if (**line == '\0' || **line == '\n')
       { return NULL;}

    return *line;
}

int ft_isspace(int c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}

