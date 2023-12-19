/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoine <antoine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:08:07 by antoine           #+#    #+#             */
/*   Updated: 2023/12/19 11:25:38 by antoine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char const *argv[])
{
	(void) argc;
	(void) argv;

	char *av[] = { "cat", NULL };
	int in = open("in", O_RDONLY);
	int pipes[2];
	if (pipe(pipes) != 0)
		perror("pipe");
	int pid = fork();
	if (pid == 0)
	{
		if (dup2(in, 0) == -1)
			perror("dup2 in");
		if (dup2(pipes[1], 1) == -1)
			perror("dup2 out");
		if (execve("/usr/bin/cat", (char * const *) av, NULL) != 0)
			perror("Execve");
	}
	waitpid(pid, NULL, 0);
	close(pipes[1]);

	char *av2[] = { "grep", "Hello", NULL };
	int out = open("out", O_RDWR | O_CREAT, 777);
	if (out == -1)
		perror("open out");
	pid = fork();
	if (pid == 0)
	{
		if (dup2(pipes[0], 0) == -1)
			perror("2 dup2 in");
		if (dup2(out, 1) == -1)
			perror("2 dup2 out");
		if (execve("/usr/bin/grep", (char * const *) av2, NULL) != 0)
			perror("Execve");
	}
	waitpid(pid, NULL, 0);
	close(pipes[0]);
	close(in);
	close(out);
	return (0);
}
