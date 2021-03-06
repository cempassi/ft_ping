/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ullitoa.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 03:12:18 by cempassi          #+#    #+#             */
/*   Updated: 2020/07/23 03:12:18 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdlib.h>

static int		numlen(unsigned long long n)
{
	int		i;

	i = 0;
	while (n)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static	void	converter(char *buffer, unsigned long long n, int i)
{
	if (n > 0)
	{
		converter(buffer, n / 10, --i);
		buffer[i] = n % 10 + '0';
	}
}

char			*ft_ullitoa(unsigned long long int n)
{
	char	*buffer;
	size_t	len;

	if (n == 0)
		return (ft_strdup("0"));
	len = numlen(n);
	buffer = ft_strnew(len);
	converter(buffer, n, len);
	return (buffer);
}
