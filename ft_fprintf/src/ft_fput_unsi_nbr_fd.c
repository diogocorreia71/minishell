/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fput_unsi_nbr_fd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 13:16:06 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/27 09:39:01 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_fprintf.h"

static int	len(unsigned int num)
{
	int	i;

	i = 0;
	if (num == 0)
		i++;
	while (num != 0)
	{
		num = num / 10;
		i++;
	}
	return (i);
}

int	ft_fput_unsi_nbr_fd(unsigned int n, int fd)
{
	int		k;
	char	c;

	k = len(n);
	if (n < 10)
	{
		c = n + '0';
		ft_fputchar_fd(c, fd);
	}
	else
	{
		ft_fput_unsi_nbr_fd(n / 10, fd);
		ft_fput_unsi_nbr_fd(n % 10, fd);
	}
	return (k);
}
