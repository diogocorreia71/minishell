/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintpointer_fd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 10:04:16 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/27 09:38:54 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_fprintf.h"

static int	len(unsigned long num)
{
	int	i;

	i = 2;
	while (num != 0)
	{
		num = num / 16;
		i++;
	}
	return (i);
}

int	ft_printpointer_start_fd(unsigned long nbr, int fd)
{
	if (nbr == 0)
		return (write(fd, "(nil)", 5));
	write(fd, "0x", 2);
	return (ft_printpointer_fd(nbr, fd));
}

int	ft_fprintpointer_fd(unsigned long nbr, int fd)
{
	char				*hexa;

	hexa = "0123456789abcdef";
	if (nbr < 16)
		ft_fputchar_fd(*(hexa + nbr), fd);
	else
	{
		ft_fprintpointer_fd(nbr / 16, fd);
		ft_fprintpointer_fd(nbr % 16, fd);
	}
	return (len(nbr));
}
