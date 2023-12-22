/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoine <antoine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 15:40:04 by antoine           #+#    #+#             */
/*   Updated: 2023/12/22 18:50:17 by antoine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

/**
 * Copies the content of an fd into an another
 * @param in The in file descriptor
 * @param out The out file descriptor
*/
int	fd_copy(int in, int out)
{
	char	*line;

	while (oget_next_line(in, &line))
	{
		ft_dprintf(out, "%s", line);
		free(line);
	}
	return (close(in), close(out), TRUE);
}

static char	*get_next_filename(char *name)
{
	size_t	len;
	if (!name)
	{
		name = ft_calloc(2, sizeof(char));
		len = 1;
	}
	else
	{
		len = ft_strlen(name) + 1;
		free(name);
		name = ft_calloc(len + 1, sizeof(char));
	}
	if (!name)
		return (NULL);
	ft_memset(name, 'a', len);
	name[len] = '\0';
	return (name);
}

static char	*find_file_name()
{
	int		fd;
	char	*filename;
	
	filename = get_next_filename(NULL);
	if (!filename)
		return (NULL);
	fd = access(filename, F_OK);
	while (fd == 0)
	{
		filename = get_next_filename(filename);
		if (!filename)
			return (NULL);
		fd = access(filename, F_OK);
	}
	return (filename);
}

int	here_doc(char *delimiter)
{
	int		fd;
	char	*filename;
	char	*line;

	delimiter = ft_strjoin(delimiter, "\n");
	filename = find_file_name();
	if (!filename || !delimiter)
		return (free(filename), free(delimiter), -1);
	fd = open(filename, O_WRONLY | O_CREAT, 0666);
	if (fd < 0)
		return (free(filename), free(delimiter), -1);
	while (ft_printf("here_doc > ") && oget_next_line(0, &line))
	{
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			break ;
		ft_dprintf(fd, "%s", line);
		free(line);
	}
	free(line);
	close(fd);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (unlink(filename), free(filename), free(delimiter), -1);
	return (unlink(filename), free(filename), free(delimiter), fd);
}
