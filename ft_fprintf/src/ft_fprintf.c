/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 11:37:12 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/27 09:38:31 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_fprintf.h"

int	formats(const char format, va_list args, int nw, int fd)
{
	if (format == 'c')
		nw = ft_fputchar_fd(va_arg(args, int), fd);
	else if (format == 's')
		nw = ft_fputstr_fd(va_arg(args, char *), fd);
	else if (format == 'p')
		nw = ft_fprintpointer_start_fd(va_arg(args, unsigned long), fd);
	else if (format == 'd')
		nw = ft_fputnbr_fd(va_arg(args, int), fd);
	else if (format == 'i')
		nw = ft_fputnbr_fd(va_arg(args, int), fd);
	else if (format == 'u')
		nw = ft_fput_unsi_nbr_fd(va_arg(args, unsigned int), fd);
	else if (format == 'x')
		nw = ft_fprinthexa_fd(va_arg(args, unsigned int), fd);
	else if (format == 'X')
		nw = ft_fprinthexa_upp_fd(va_arg(args, unsigned int), fd);
	else if (format == '%')
		nw = ft_fputchar_fd(fd, '%');
	return (nw);
}

int	ft_fprintf(int fd, const	char *format, ...)
{
	int		i;
	int		nw;
	int		k;
	va_list	args;

	i = 0;
	nw = 0;
	va_start(args, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			k = formats(format[i], args, k, fd);
			nw = k + nw;
		}
		else
		{
			nw++;
			ft_fputchar_fd(format[i], fd);
		}
		i++;
	}
	va_end(args);
	return (nw);
}
