/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: averin <averin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:08:07 by antoine           #+#    #+#             */
/*   Updated: 2024/01/02 15:49:41 by averin           ###   ########.fr       */
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
	(close(infd), close(outfd));
	return (pid);
}

static int	exec_commands(char **cmds, char **path, int count, int *fds)
{
	char	*file;
	char	**args;
	int		pipedes[2];
	int		pid;

	file = get_file_path(cmds[0], path);
	if (!file && count == 0)
		return (close(fds[0]), close(fds[1]), -1);
	args = ft_split(cmds[0], ' ');
	if (!args)
		return (ft_dprintf(2, ERROR_MEM), free(file), -1);
	if (count != 0 && pipe(pipedes) == -1)
		return (perror("pipe"), ft_fsplit(args), free(file), -1);
	if (count == 0)
		pipedes[1] = fds[1];
	if (file)
		pid = exec_child(file, args, fds[0], pipedes[1]);
	(free(file), ft_fsplit(args), fds[0] = pipedes[0]);
	if (count == 0)
		return (pid);
	return (exec_commands(cmds + 1, path, count - 1, fds));
}

static int	wait_childs(int pid)
{
	int	wstatus;
	int	code;

	while (errno != ECHILD)
		if (wait(&wstatus) == pid && WIFEXITED(wstatus))
			code = WEXITSTATUS(wstatus);
	if (pid == -1)
		return (127);
	return (code);
}

static int	get_heredoc(char *del)
{
	char	*file;
	char	*line;
	int		fds[2];

	del = ft_strjoin(del, "\n");
	file = find_heredoc_file();
	if (!file || !del)
		return (free(del), free(file), -1);
	fds[0] = open(file, O_WRONLY | O_CREAT);
	fds[1] = open(file, O_RDONLY);
	(unlink(file), free(file));
	if (fds[0] < 0 || fds[1] < 0)
		return (free(del), close(fds[0]), close(fds[1]), -1);
	while (ft_printf("here_doc > ") && oget_next_line(0, &line))
	{
		if (ft_strncmp(line, del, ft_strlen(del) + 1) == 0)
			break ;
		(ft_putstr_fd(line, fds[0]), free(line));
	}
	return (free(del), free(line), close(fds[0]), fds[1]);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	**path;
	int		fds[2];
	int		pid;
	int		is_heredoc;

	is_heredoc = ft_strncmp(argv[1], "here_doc", 9) == 0;
	if ((is_heredoc && argc <= 5) || argc <= 4)
		return (ft_dprintf(2, ERROR_USAGE, argv[0], argv[0]), 1);
	if (is_heredoc && argv++ && argc--)
		fds[0] = get_heredoc(argv[1]);
	else
		fds[0] = open(argv[1], O_RDONLY);
	if (fds[0] < 0)
		return (perror(argv[1]), 3);
	if (is_heredoc)
		fds[1] = open(argv[argc - 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
	else
		fds[1] = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fds[1] < 0)
		return (close(fds[0]), perror(argv[argc - 1]), 4);
	path = find_path(envp);
	pid = exec_commands(argv + 2, path, argc - 4, fds);
	return (ft_fsplit(path), wait_childs(pid));
}
