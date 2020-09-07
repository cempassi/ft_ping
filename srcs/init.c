/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 02:05:55 by cempassi          #+#    #+#             */
/*   Updated: 2020/09/07 02:14:07 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static int parse_opt(t_ping *ping, t_opt *option, int ac, char **av)
{
	char opt;
	char *optarg;

	while ((opt = ft_getopt(ac, av, option, &optarg)) != -1)
	{
		if (opt == 'v')
			ping->option |= OPT_V;
		else if (opt == 'h')
			ping->option |= OPT_H;
		else if (opt == 'c')
		{
			if (!ft_strcheck(optarg, ft_isdigit))
			{
				ft_dprintf(2, "%s: illegal option arg -- %s\n", av[0], OPT_C_ERROR);
				return (-1);
			}
			ping->option |= OPT_C;
		}
	}
	return (0);
}

int 	init_socket(t_ping *ping, char **av)
{
	ping->socket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ping->socket == -1)
	{
		ft_dprintf(STDERR_FILENO, "%s: Socket allocation failed\n", av[0]);
		return (-1);
	}
	return (0);
}

int 	init_prgm(t_ping *ping, int ac, char **av)
{
	t_opt option;
	char *optarg;
	int	  error;

	ft_bzero(ping, sizeof(struct s_ping));
	ft_bzero(&option, sizeof(t_opt));
	option.optstr = OPTSTR;
	optarg = NULL;
	ping->packet_size = 42;
	if ((error = ft_getopt(ac, av, &option, &optarg)) != 0)
	{
		if (option.error)
			ft_dprintf(2, "%s: illegal option -- %s\n",av[0],  option.error);
		else
			ft_dprintf(2, "%s: illegal option -- %c\n",av[0],  error);
		return (error);
	}
	if(parse_opt(ping, &option, ac, av) || init_socket(ping, av))
		return (-1);
	return (0);
}
