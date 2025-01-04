/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sishige <sishige@student.42tokyo.j>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 23:19:02 by sishige           #+#    #+#             */
/*   Updated: 2024/10/01 19:33:32 by sishige          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	child_process(t_pipex pipex, char **cmd_args, int i)
{
	if (i == 0)
	{
		close(pipex.pipes[i].fds[R]);
		if (set_input(pipex.infile))
		{
			fprintf(stderr, "%s: %s: %s\n", cmd_args[0], pipex.infile, strerror(errno));
			close(pipex.pipes[i].fds[W]);
			exit(EXIT_FAILURE);
		}
	}
	if (i == pipex.n_cmds - 1)
		set_output(pipex.outfile);
	else
	{
		if (dup2(pipex.pipes[i].fds[W], STDOUT_FILENO) == -1)
			die("dup2");
		close(pipex.pipes[i].fds[W]);
	}
	ft_execvpe(cmd_args[0], cmd_args, pipex.envp);
	fprintf(stderr, "pipex: %s: command not found\n", cmd_args[0]);
	exit(127);
}

static void	parent_process(int pipefds[2])
{
	close(pipefds[W]);
	if (dup2(pipefds[R], STDIN_FILENO) == -1)
		die("dup2");
	close(pipefds[R]);
}

static int	wait_process(pid_t *pids, int n_cmds)
{
	int	i;
	int	wait_stat;
	int	exit_stat;

	i = 0;
	while (i < n_cmds)
	{
		waitpid(pids[i], &wait_stat, 0);
		i++;
	}
	exit_stat = 0;
	if (WIFEXITED(wait_stat))
		exit_stat = WEXITSTATUS(wait_stat);
	else if (WIFSIGNALED(wait_stat))
		exit_stat = WTERMSIG(wait_stat);
	return (exit_stat);
}

char	**make_args(char *str)
{
	char	**args;

	args = ft_split(str, ' ');
	if (args == NULL)
		die("ft_split");
	if (*args != NULL && *str != ' ')
		return (args);
	args = (char **)malloc(sizeof(char *) * 2);
	if (args == NULL)
		die("malloc");
	args[0] = ft_strdup(str);
	args[1] = NULL;
	return (args);
}

int	create_process(t_pipex pipex)
{
	pid_t	*pids;
	char	**cmd_args;
	int		i;

	pids = malloc(sizeof(pid_t) * pipex.n_cmds);
	if (pids == NULL)
		die("create_process(): malloc");
	i = pipex.n_cmds;
	while (i--)
	{
		cmd_args = make_args(pipex.cmds[i]);
		pids[i] = fork();
		if (pids[i] == -1)
			die("create_process(): fork");
		if (pids[i] == 0)
			child_process(pipex, cmd_args, i);
		else if (i - 1 < 0)
			parent_process(pipex.pipes[i].fds);
		cleanup(cmd_args);
	}
	return (wait_process(pids, pipex.n_cmds));
}

//int	create_process(t_pipex pipex)
//{
//	pid_t	*pids;
//	char	**cmd_args;
//	int		i;
//
//	pids = (pid_t *)malloc(sizeof(pid_t) * pipex.n_cmds);
//	if (pids == NULL)
//		die("malloc");
//	i = 0;
//	while (i < pipex.n_cmds)
//	{
//		cmd_args = make_args(pipex.cmds[i]);
//		pids[i] = fork();
//		if (pids[i] == -1)
//			die("fork");
//		if (pids[i] == 0)
//			child_process(pipex, cmd_args, i);
//		else if (i != pipex.n_cmds - 1)
//			parent_process(pipex.pipes[i].fds);
//		cleanup(cmd_args);
//		i++;
//	}
//	return (wait_process(pids, pipex.n_cmds));
//}
