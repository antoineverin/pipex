/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averin <averin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:08:07 by antoine           #+#    #+#             */
/*   Updated: 2023/12/27 16:24:24 by averin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

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
	if (!filename || strncmp(filename, "", 1) == 0)
		return (free(filename), ft_dprintf(2, "Empty command\n"), NULL);
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
	ft_dprintf(2, "%s: Unknow command\n", filename);
	return (free(filename), NULL);
}

/**
 * @param file_path Path to file to execute needs to have x rights
 * @param args String containing the args to pass to executable
 * @param in_fd Fd with input to pass
 * @param out_fd Fd int wich output
 * @param 1 if ended correctly, else 0
*/
static int	execute(char *file_path, char *args, int in_fd, int out_fd)
{
	int		pid;
	char	**args_tab;
	int		child_status;

	args_tab = ft_split(args, ' ');
	if (!args_tab)
		return (FALSE);
	pid = fork();
	if (pid == 0)
	{
		if (dup2(in_fd, 0) == -1)
			return (ft_free_split(args_tab), close(in_fd), close(out_fd),
				perror("infd"), FALSE);
		if (dup2(out_fd, 1) == -1)
			return (ft_free_split(args_tab), close(in_fd), close(out_fd),
				perror("outfd"), FALSE);
		if (execve(file_path, args_tab, NULL) == -1)
			return (ft_free_split(args_tab), close(in_fd), close(out_fd),
				perror("execve"), FALSE);
	}
	waitpid(pid, &child_status, 0);
	ft_free_split(args_tab);
	return (close(in_fd), close(out_fd), TRUE);
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

static int	pipex(char **commands, int size, char **path, int *fds)
{
	int		i;
	char	*file;
	int		pipes[4];

	if (pipe(pipes) != 0 || pipe(pipes + 2) != 0)
		return (perror("pipex"), FALSE);
	fd_copy(fds[0], pipes[1]);
	i = -1;
	while (++i < size)
	{
		file = get_file_path(commands[i], path);
		if (!file)
			return (close(pipes[0]), close(pipes[1]), close(pipes[2]),
				close(pipes[3]), close(fds[1]), free(file), FALSE);
		if (!execute(file, commands[i], pipes[0], pipes[3]))
			return (free(file), FALSE);
		pipes[0] = pipes[2];
		pipes[1] = pipes[3];
		if (pipe(pipes + 2) != 0)
			return (free(file), close(pipes[0]),
				perror("pipex"), FALSE);
		free(file);
	}
	fd_copy(pipes[0], fds[1]);
	return (close(pipes[0]), close(pipes[2]), close(pipes[3]), TRUE);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	**path;
	int		fds[2];

	if ((ft_strncmp(argv[1], "here_doc", 9) == 0 && argc <= 5) || argc <= 4)
		return (ft_dprintf(2, ERROR_USAGE, argv[0], argv[0]), 1);
	if (ft_strncmp(argv[1], "here_doc", 9) == 0 && argv++ && argc--)
		fds[0] = here_doc(argv[1]);
	else
		fds[0] = open(argv[1], O_RDONLY);
	if (fds[0] < 0)
		return (perror(argv[1]), 3);
	if (ft_strncmp(argv[0], "here_doc", 9) == 0)
		fds[1] = open(argv[argc - 1], O_WRONLY | O_APPEND | O_CREAT, 0666);
	else
		fds[1] = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fds[1] < 0)
		return (close(fds[0]), perror(argv[argc - 1]), 4);
	path = find_path(envp);
	if (!path)
		return (ft_dprintf(2, "Memory Error\n"), ft_free_split(path), 2);
	if (!pipex(argv + 2, argc - 3, path, fds))
		return (ft_free_split(path), 5);
	ft_free_split(path);
	return (0);
}
