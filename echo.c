/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:28:35 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/26 03:11:23 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	echo_options(char *split_args, int *flag)
{
	char	*tmp;
	int		i;
	int		k;

	i = 0;
	while (split_args[i] != ' ' && split_args[i])
		i++;
	tmp = (char *)malloc(sizeof(tmp) * (i + 1));
	if (tmp == NULL)
		return ;
	k = 0;
	while (k < i)
	{
		tmp[k] = split_args[k];
		k++;
	}
	tmp[k] = '\0';
	if (!ft_strncmp(tmp, "-n", 3) || !ft_strncmp(tmp, "\"-n\"", 5)
		|| !ft_strncmp(tmp, "\'-n\'", 5))
		*flag = 1;
	else
		*flag = 0;
	free(tmp);
}

void	echo(t_minishell *shell)
{
	int	i;
	int	flag;

	flag = 0;
	/* echo_options(shell->split_args, &flag); */
	i = 1;
	if (flag == 1)
		i = 2;
	while (shell->cmd_split[i])
	{
		printf("%s", shell->cmd_split[i]);
		if (shell->cmd_split[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (flag == 0)
		printf("\n");
}
