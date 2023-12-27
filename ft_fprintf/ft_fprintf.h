/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:17:58 by rumachad          #+#    #+#             */
/*   Updated: 2023/12/27 09:38:18 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>

int		ft_fprintf(int fd, const char *format, ...);
int		ft_fputstr_fd(char *s, int fd);
int		ft_fputchar_fd(char c, int fd);
int		ft_fprintpointer_fd(unsigned long nbr, int fd);
int		ft_fprintpointer_start_fd(unsigned long nbr, int fd);
int		ft_fprinthexa_fd(unsigned int nbr, int fd);
int		ft_fprinthexa_upp_fd(unsigned int nbr, int fd);
int		ft_fputnbr_fd(int n, int fd);
int		ft_fput_unsi_nbr_fd(unsigned int n, int fd);

#endif