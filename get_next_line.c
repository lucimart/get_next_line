/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucimart <lucimart@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/07 03:07:56 by lucimart          #+#    #+#             */
/*   Updated: 2019/12/02 21:48:59 by lucimart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static ssize_t	nl_index(char *str)
{
	size_t i;

	i = -1;
	while (str && str[++i])
		if (str[i] == '\n')
			return (i);
	return (-1);
}

static int		free_and_ret(char **str, int ret)
{
	if (*str)
	{
		free(*str);
		*str = 0;
	}
	return (ret);
}

int				gnl_aux(ssize_t len_rd, char **line, char **cache, ssize_t nl_i)
{
	char	*aux;
	int		to_ret;

	if (len_rd == 0 && (!*cache || !**cache) && (*line = ft_strnew(0)))
		return (free_and_ret(cache, 0));
	if (nl_i == -1)
	{
		if (!(*line = ft_substr(*cache, 0, ft_strlen(*cache))))
			return (free_and_ret(cache, -1));
		to_ret = 0;
		aux = 0;
	}
	else
	{
		if (!(*line = ft_substr(*cache, 0, nl_i)) ||
			!(aux = ft_substr(*cache, nl_i + 1, ft_strlen(*cache) - 1)))
			return (free_and_ret(cache, -1));
		to_ret = 1;
	}
	free_and_ret(cache, 0);
	*cache = aux;
	return (to_ret);
}

int				get_next_line(int fd, char **line)
{
	char		buf[BUFFER_SIZE + 1];
	ssize_t		len_read;
	static char	*cache;
	char		*aux;

	if (!line || fd < 0 || BUFFER_SIZE < 1)
		return (free_and_ret(&cache, -1));
	while ((len_read = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[len_read] = '\0';
		if (!(aux = ft_strjoin(cache, buf)))
			return (free_and_ret(&cache, -1));
		free_and_ret(&cache, 0);
		if (nl_index(cache = aux) != -1)
			break ;
	}
	if (len_read < 0)
		return (free_and_ret(&cache, -1));
	return (gnl_aux(len_read, line, &cache, nl_index(cache)));
}
