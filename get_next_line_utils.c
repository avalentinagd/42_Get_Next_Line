/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angalleg <angalleg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 17:24:56 by angalleg          #+#    #+#             */
/*   Updated: 2026/11/03 17:24:56 by angalleg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	char	c_char;

	if (!s) // <--- protección añadida
        return (NULL);
	c_char = (char)c;
	while (*s)
	{
		if (*s == c_char)
			return ((char *)s);
		s++;
	}
	if (*s == c_char)
		return ((char *)s);
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_mem;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	new_mem = (char *)malloc(sizeof(char)
			* (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!new_mem)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new_mem[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		new_mem[i + j] = s2[j];
		j++;
	}
	new_mem[i + j] = '\0';
	return (new_mem);
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	size_t	i;
	char	*s2;

	len = 0;
	while (s[len])
		len++;
	s2 = malloc(len + 1);
	if (!s2)
		return (NULL);
	i = 0;
	while (s[i])
	{
		s2[i] = s[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
