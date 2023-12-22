/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antoine <antoine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 13:06:37 by antoine           #+#    #+#             */
/*   Updated: 2023/12/22 19:10:11 by antoine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include "libft.h"

# define ERROR_USAGE "Usage:\t%s file1 cmd ... cmdn file2\nor\t%s here_doc \
delimiter cmd ... cmdn file2\n"

int	fd_copy(int in, int out);
int	here_doc(char *delimiter);

#endif
