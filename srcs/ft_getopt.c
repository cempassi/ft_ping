/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 15:45:30 by cempassi          #+#    #+#             */
/*   Updated: 2020/09/18 03:02:29 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"
#include "memory.h"
#include "str.h"
#include "vector.h"


int opt_init(int ac, char **av)
{
	t_opt option;
	char *optlong[3];
	char *optarg;
	int	  error;

	optarg = NULL;
	ft_bzero(&option, sizeof(t_opt));
	option.optstr = "lsq:c:";
	optlong[0] = "count=c:";
	optlong[1] = "non=s";
	optlong[2] = NULL;
	option.optlong = &optlong[0];
	if ((error = ft_getopt(ac, av, &option, &optarg)) != 0)
	{
		if (option.error)
			ft_printf("Invalid argument %s", option.error);
		else
			ft_printf("Invalid argument -%c", error);
		return error;
	}
	while ((error = ft_getopt(ac, av, &option, &optarg)) != -1)
	{
		if (optarg)
			ft_printf("Passed option: -%c, argument : %s\n", error, optarg);
		else
			ft_printf("Passed option: -%c\n", error);
	}
	return (0);
}

int main(int ac, char **av)
{
	int error;
	int i;

	error = 0;
	i = 0;
	if (ac == 1)
	{
		ft_printf("Pass arguments options to test");
		return (0);
	}
	if ((error = opt_init(ac, av)) != 0)
	{
		return (-1);
	}
	while (i < ac)
	{
		ft_printf("av[%d] : %s\n", i, av[i]);
		++i;
	}
}
