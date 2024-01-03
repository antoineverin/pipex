/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averin <averin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 16:08:50 by averin            #+#    #+#             */
/*   Updated: 2024/01/03 13:53:59 by averin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**find_path(char **envp)
{
	size_t	i;
	char	**path;
	char	*temp;

	i = -1;
	while (envp[++i] && ft_strncmp("PATH=", envp[i], 5) != 0)
		continue ;
	if (!envp[i])
		return (NULL);
	path = ft_split(ft_strchr(envp[i], '=') + 1, ':');
	if (!path)
		return (NULL);
	i = -1;
	while (path[++i])
	{
		if (path[i][ft_strlen(path[i]) - 1] != '/')
		{
			temp = ft_strjoin(path[i], "/");
			if (!temp)
				return (ft_fsplit(path), NULL);
			free(path[i]);
			path[i] = temp;
		}
	}
	return (path);
}

char	*get_file_path(char *s, char **path)
{
	size_t	i;
	char	*file_path;

	s = ft_strcut(s, ' ');
	if (ft_strchr(s, '/') != NULL && access(s, F_OK | X_OK) == 0)
		return (s);
	else if (ft_strchr(s, '/') != NULL && access(s, F_OK | X_OK) != 0)
		return (ft_dprintf(2, "Command '%s' not found\n", s), free(s), NULL);
	if (!s)
		return (free(s), ft_dprintf(2, ERROR_MEM), NULL);
	if (ft_strncmp(s, "", 1) == 0)
		return (ft_dprintf(2, "Command '%s' not found\n", s), free(s), NULL);
	i = -1;
	while (path != NULL && path[++i])
	{
		file_path = ft_strjoin(path[i], s);
		if (!file_path)
			return (free(s), NULL);
		if (access(file_path, F_OK | X_OK) == 0)
			return (free(s), file_path);
		free(file_path);
	}
	ft_dprintf(2, "Command '%s' not found\n", s);
	return (free(s), NULL);
}

char	*find_heredoc_file(void)
{
	char	*file;
	int		size;

	size = 2;
	file = NULL;
	file = ft_calloc(size, sizeof(char));
	if (!file)
		return (NULL);
	ft_memset(file, 'a', size - 1);
	while (access(file, F_OK) != 0 && errno != ENOENT)
	{
		free(file);
		if (++size < 0)
			return (NULL);
		file = ft_calloc(size, sizeof(char));
		if (!file)
			return (NULL);
		ft_memset(file, 'a', size - 1);
	}
	return (file);
}
