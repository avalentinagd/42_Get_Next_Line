/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angalleg <angalleg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 17:24:56 by angalleg          #+#    #+#             */
/*   Updated: 2026/11/03 17:24:56 by angalleg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/* unificamos de forma segura la variable estática (stash) 
con el nuevo texto leído (buffer), gestionando la memoria para evitar fugas (leaks).

static (solo visible dentro de este archivo C) recibe el acumulado actual (stash)
y lo recién leído (buffer), devolviendo un nuevo string (temp). */
static char	*ft_join_and_free(char *stash, char *buffer)
{
	char	*temp;// puntero temporal para almacenar el resultado de la unión

	if (!stash) // ¿Es stash = NULL? (es decir, ¿no apunta a ninguna dir. de memoria?).
    /*  Esto ocurre en dos situaciones muy concretas:
        1. En la primera llamada que se le hace a get_next_line para ese archivo (porque la variable estática nace valiendo NULL por defecto).
        2. Cuando ya se leyó todo el archivo en llamadas anteriores y la estática quedó totalmente limpia y liberada. */
	{
		stash = malloc(1);// Si está vacía, le asigna 1 byte de memoria inicial para convertirla en una cadena válida (evitando problemas de punteros nulos en ft_strjoin).
		if (!stash)// Comprueba si el malloc falló
			return (NULL);
		stash[0] = '\0';// Convierte ese único byte en una cadena vacía pero válida (\0 finalizador)
	}
	temp = ft_strjoin(stash, buffer);// Une el contenido actual del stash con el buffer recién leído y guarda la nueva cadena en temp.
	free(stash);// Libera la memoria antigua del stash (ya no la necesitamos porque ahora está ampliada en temp)
	return (temp);//
}

/* Función principal de lectura, cumpliendo el límite de líneas */
/* Esta función gestiona el bucle de lectura utilizando read() hasta 
conseguir un salto de línea o llegar al final del archivo.

Recibe el descriptor de archivo (fd) y la estática actual, devolviendo el stash actualizado*/
char	*ft_fill_stash(int fd, char *stash)
{
	char	*buffer;// Declara el buffer temporal de lectura
	int		bytes_read;// variable entera para registrar cuántos bytes lee el sistema operativo

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));// Reserva memoria dinámica para el buffer basada en el BUFFER_SIZE (más 1 espacio extra para el terminador nulo '\0').
	if (!buffer)// Protege la reserva de memoria por si el sistema se queda sin recursos.
		return (NULL);
	bytes_read = 1;// Inicializa bytes_read en un valor positivo para obligar a que el bucle while comience a ejecutarse
	while (!ft_strchr(stash, '\n') && bytes_read > 0)
    /*  El bucle se repite mientras en nuestro stash NO exista un salto de línea (\n) 
        Y el último read haya leído datos (es decir, bytes_read mayor que 0). Si encuentra un \n, 
        el bucle se detiene porque ya tiene suficiente para sacar una línea.*/
	{
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        /*  Ejecuta la función read(), pidiendo leer tantos bytes como dicte el BUFFER_SIZE, 
            guardándolos en buffer y devolviendo cuántos bytes leyó realmente.*/
		if (bytes_read == -1)// Si read devuelve -1, significa que hubo un error al leer el origen de datos al que apunta el descriptor de archivo (fd).
		{// cuando evaluamos if (bytes_read == -1), estamos protegiendo nuestro programa por si ocurre algun fallo en el canal de lectura, limpiemos la memoria correctamente y evitemos un segfault.
			free(buffer);// Libera el buffer temporal para evitar fugas
			return (NULL);// retorna NULL debido al error.
		}
		buffer[bytes_read] = '\0';// Coloca un terminador nulo al final de los bytes leídos para que C sepa dónde termina la cadena de texto de forma segura.
		stash = ft_join_and_free(stash, buffer);// Llama a nuestra función auxiliar para anexar lo recién leído al acumulado.
	}
	free(buffer);// Una vez que el bucle termina (porque ya hay un \n o se acabó el archivo), libera el buffer temporal ya que no se usará más.
	return (stash);// Devuelve el stash completo y listo para el siguiente paso.
}

/*  Esta función recorre el stash buscando el salto de línea, reserva memoria 
    justa para la línea exacta, la copia y le añade el terminador nulo (\0).*/
char	*ft_extract_line(char *stash)
{
	int		i;
	char	*line;

	if (!stash[0])
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		line = malloc(sizeof(char) * (i + 2));
	else
		line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
        line[i] = stash[i];
		i++;
    }
	if (stash[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

/*  Esta función se encarga de eliminar la línea que ya devolvimos 
    y dejar en la variable estática únicamente el residuo sobrante.*/
char	*ft_clean_stash(char *stash)
{
	int		i;
	int		j;
	char	*new_stash;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	new_stash = malloc(sizeof(char) * (ft_strlen(stash) - i));
	if (!new_stash)
	{
		free(stash);
		return (NULL);
	}
	i++;
	j = 0;
	while (stash[i])
		new_stash[j++] = stash[i++];
	new_stash[j] = '\0';
	free(stash);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = ft_fill_stash(fd, stash);
	if (!stash)
	{
		free(stash);
		return (NULL);
	}
	line = ft_extract_line(stash);
	if (!line)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = ft_clean_stash(stash);
	return (line);
}
