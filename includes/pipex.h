/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoine <antoine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:06:37 by antoine           #+#    #+#             */
/*   Updated: 2023/12/18 13:09:16 by antoine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdio.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "libft.h"

# define ERROR_USAGE "Usage: %s file1 cmd1 cmd2 file2\n"
# define ERROR_MEM "Memory Error\n"

typedef struct s_command {
	char	*file;
	char	**args;
}	t_command;

char	**find_path(char **envp);
char	*get_file_path(char *cmd, char **path);

#endif
