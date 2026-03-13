/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtumanya <mtumanya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 12:08:33 by mtumanya          #+#    #+#             */
/*   Updated: 2026/03/03 16:43:24 by mtumanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	len;
	char	*dup;

	len = ft_strlen (s1);
	dup = (char *) malloc (len + 1);
	if (dup == NULL)
		return (NULL);
	ft_memset (dup, 0, len + 1);
	ft_memcpy (dup, s1, len + 1);
	return (dup);
}
