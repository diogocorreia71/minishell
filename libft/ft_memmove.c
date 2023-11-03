/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 12:22:56 by rumachad          #+#    #+#             */
/*   Updated: 2023/04/26 11:54:50 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned int		i;
	unsigned char		*src1;
	unsigned char		*dest1;

	src1 = (unsigned char *)src;
	dest1 = (unsigned char *)dest;
	if (!src1 && !dest1)
		return (NULL);
	if (src1 < dest1)
	{
		while (n-- > 0)
			dest1[n] = src1[n];
	}
	else
	{
		i = 0;
		while (i < n)
		{
			dest1[i] = src1[i];
			i++;
		}
	}
	return (dest);
}

/* int main()
{
	char src[] = "12345";
	char dest[] = "45678";

	printf("%s\n", ft_memmove(&dest[0], &src[0], 3));
} */