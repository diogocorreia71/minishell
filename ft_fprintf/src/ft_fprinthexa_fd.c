/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprinthexa_fd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/28 12:29:03 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/27 09:38:41 by rumachad         ###   ########.fr       */
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
		num = num / 16;
		i++;
	}
	return (i);
}

int	ft_fprinthexa_fd(unsigned int nbr, int fd)
{
	char	*hexa;

	hexa = "0123456789abcdef";
	if (nbr < 16)
		ft_fputchar_fd(*(hexa + nbr), fd);
	else
	{
		ft_fprinthexa_fd(nbr / 16, fd);
		ft_fprinthexa_fd(nbr % 16, fd);
	}
	return (len(nbr));
}
