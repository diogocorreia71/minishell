/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 12:28:35 by rumachad          #+#    #+#             */
/*   Updated: 2023/11/27 14:52:36 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void	echo_options(char **cmd_split, int *flag)
{
	char	*tmp;
	int		i;
	int		k;

	i = 0;
	
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
} */

void	echo(t_minishell *shell)
{
	int	i;
	int	flag;

	flag = 0;
	if (!shell->cmd_split[1])
	{
		printf("\n");
		return ;
	}
	if (!ft_strncmp(shell->cmd_split[1], "-n", 3))
		flag = 1;
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
