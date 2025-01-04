/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sishige <sishige@student.42tokyo.j>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 23:26:00 by sishige           #+#    #+#             */
/*   Updated: 2024/09/29 18:56:53 by sishige          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	die(const char *cause)
{
	perror(cause);
	exit(EXIT_FAILURE);
}

void	panic(const char *cause)
{
	int		err_num;
	char	*err_msg;

	err_num = errno;
	err_msg = strerror(err_num);
	ft_fprintf(stderr, "pipex: %s: %s\n", cause, err_msg);
	exit(EXIT_FAILURE);
}

int	warn(const char *cause)
{
	int		err_num;
	char	*err_msg;

	err_num = errno;
	err_msg = strerror(err_num);
	fprintf(stderr, "pipex: %s: %s\n", cause, err_msg);
	return (err_num);
}
