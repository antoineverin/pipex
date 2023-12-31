/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averin <averin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:08:07 by antoine           #+#    #+#             */
/*   Updated: 2023/12/31 23:02:40 by averin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	exec_child(char *file, char **args, int infd, int outfd)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		if (dup2(infd, 0) == -1 || dup2(outfd, 1) == -1)
			return (perror("dup2"), -1);
		if (execve(file, args, NULL) == -1)
			return (perror("execve"), -1);
	}
	return (pid);
}

static int	exec_commands(char **cmds, char **path, int count, int *fds)
{
	char	*file;
	char	**args;
	int		pipedes[2];
	int		pid;

	file = get_file_path(cmds[0], path);
	if (!file)
		return (-1);
	args = ft_split(cmds[0], ' ');
	if (!args)
		return (ft_dprintf(2, ERROR_MEM), free(file), -1);
	if (pipe(pipedes) == -1 && count != 0)
		return (perror("pipe"), ft_fsplit(args), free(file), -1);
	if (count == 0)
		pipedes[1] = fds[1];
	pid = exec_child(file, args, fds[0], pipedes[1]);
	(close(fds[0]), close(pipedes[1]), fds[0] = pipedes[0]);
	if (count == 0)
		return (pid);
	return (exec_commands(cmds + 1, path, count - 1, fds));
}

int	main(int argc, char *argv[], char *envp[])
{
	char	**path;
	int		fds[2];

	if (argc <= 4)
		return (ft_dprintf(2, ERROR_USAGE, argv[0]), 1);
	fds[0] = open(argv[1], O_RDONLY);
	if (fds[0] < 0)
		return (perror(argv[1]), 3);
	fds[1] = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
	if (fds[1] < 0)
		return (close(fds[0]), perror(argv[argc - 1]), 4);
	path = find_path(envp);
	if (!path)
		return (ft_dprintf(2, "Memory Error\n"), ft_fsplit(path), 2);
	exec_commands(argv + 2, path, argc - 4, fds);
	wait(NULL);
	return (0);
}
