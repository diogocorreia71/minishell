/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 15:45:39 by rumachad          #+#    #+#             */
/*   Updated: 2023/04/20 13:03:02 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return (i);
} */

char	*ft_strdup(const char *s)
{
	char			*duplicate;
	unsigned int	i;
	unsigned int	strlen;

	strlen = ft_strlen(&s[0]);
	i = 0;
	duplicate = (char *) malloc((strlen + 1) * sizeof(char));
	if (duplicate == NULL)
		return (NULL);
	while (i < strlen)
	{
		duplicate[i] = s[i];
		i++;
	}
	duplicate[i] = '\0';
	return (duplicate);
}

/* int main()
{
	char str[] = "Hello";
	char *a;

	a = ft_strdup(&str[0]);
	printf("%s\n", a);
	free(a);
} */