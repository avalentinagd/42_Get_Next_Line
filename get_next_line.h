/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angalleg <angalleg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 17:24:56 by angalleg          #+#    #+#             */
/*   Updated: 2026/11/03 17:24:56 by angalleg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

/* Definición del BUFFER_SIZE por defecto si no se pasa al compilar */
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

/* Prototipo principal */
char	*get_next_line(int fd);

/* Auxiliares de la función principal */
char	*ft_fill_stash(int fd, char *stash);
char	*ft_extract_line(char *stash);
char	*ft_clean_stash(char *stash);

/* Prototipos de tus funciones de ayuda (puedes añadir las que necesites) */
size_t	ft_strlen(const char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s);

#endif