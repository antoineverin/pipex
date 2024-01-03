/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averin <averin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 16:08:50 by averin            #+#    #+#             */
/*   Updated: 2024/01/03 11:46:00 by averin           ###   ########.fr       */
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

char	*get_file_path(char *cmd, char **path)
{
	size_t	i;
	char	*file_path;

	cmd = ft_strcut(cmd, ' ');
	if (ft_strchr(cmd, '/') != NULL)
		return (cmd);
	if (!cmd)
		return (free(cmd), ft_dprintf(2, ERROR_MEM), NULL);
	if (ft_strncmp(cmd, "", 1) == 0)
		return (ft_dprintf(2, "Command '%s' not found\n", cmd), \
		free(cmd), NULL);
	i = -1;
	while (path != NULL && path[++i])
	{
		file_path = ft_strjoin(path[i], cmd);
		if (!file_path)
			return (free(cmd), NULL);
		if (access(file_path, F_OK | X_OK) == 0)
			return (free(cmd), file_path);
		free(file_path);
	}
	ft_dprintf(2, "Command '%s' not found\n", cmd);
	return (free(cmd), NULL);
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
