/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoine <antoine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:08:07 by antoine           #+#    #+#             */
/*   Updated: 2023/12/20 15:48:44 by antoine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * @param filename The name of the file to search from start to first space
 * @param path An array to string wich represent the path env variable
 * @result The file path or NULL if file not exist
*/
static char	*get_file_path(char *filename, char **path)
{
	size_t	i;
	char	*file_path;

	filename = ft_strcut(filename, ' ');
	i = -1;
	while (path[++i])
	{
		file_path = ft_strjoin(path[i], filename);
		if (!file_path)
			return (free(filename), NULL);
		if (access(file_path, F_OK | X_OK) == 0)
			return (free(filename), file_path);
		free(file_path);
	}
	return (free(filename), NULL);
}

/**
 * @param file_path Path to file to execute needs to have x rights
 * @param args String containing the args to pass to executable
 * @param in_fd Fd with input to pass
 * @param out_fd Fd int wich output
 * @param 1 if ended correctly, else 0
 * TODO: Check if exit exit only child
 * TODO: Send envp ?
*/
static int	execute(char *file_path, char *args, int in_fd, int out_fd)
{
	int		pid;
	char	**args_tab;

	args_tab = ft_split(args, ' ');
	if (!args_tab)
		return (FALSE);
	pid = fork();
	if (pid == 0)
	{
		if (dup2(in_fd, 1) == -1)
			return (perror(file_path), exit(1), FALSE);
		if (dup2(out_fd, 0) == -1)
			return (perror(file_path), exit(1), FALSE);
		if (execve(file_path, args_tab, NULL) == -1)
			return (perror(file_path), exit(1), FALSE);
	}
	waitpid(pid, NULL, 0);
	ft_free_split(args_tab);
	return (TRUE);
}

/**
 * @param envp An array containing env variables
 * @return The line that contains the path env variable terminated with /
*/
static char	**find_path(char **envp)
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
				return (ft_free_split(path), NULL);
			free(path[i]);
			path[i] = temp;
		}
	}
	return (path);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		i;
	char	**path;
	char	*file;

	if (argc <= 4)
		return (ft_dprintf(2, "Usage: %s input_file cmd1 cmd2 \
			... cmdn output_file\n", argv[0]), 1);
	i = -1;
	path = find_path(envp);
	if (!path)
		return (ft_dprintf(2, "Memory Error\n"), ft_free_split(path), 2);
	i = 1;
	while (++i < argc - 1)
	{
		file = get_file_path(argv[i], path);
		if (!execute(file, argv[i], 0, 1))
			return (3);
		free(file);
	}
	ft_free_split(path);
	return (0);
}
