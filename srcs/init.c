/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 02:05:55 by cempassi          #+#    #+#             */
/*   Updated: 2020/10/28 19:27:15 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <signal.h>
#include <sys/time.h>
#include <sysexits.h>
#include <stdio.h>

static int get_argument(t_ping *ping, char *optarg, uint8_t opt, char *errmsg)
{
	if (!ft_strcheck(optarg, ft_isdigit))
	{
		ping->exit = EX_USAGE;
		ft_dprintf(2, "%s: illegal option arg -- %s\n", ping->name, errmsg);
		return (-1);
	}
	ping->options |= opt;
	return (ft_atoi(optarg));
}

static int parse_opt(t_ping *ping, t_opt *option, int ac, char **av)
{
	char  opt;
	char *optarg;

	while ((opt = ft_getopt(ac, av, option, &optarg)) > 0 && ping->exit == 0)
	{
		if (opt == 'v')
			ping->options |= OPT_V;
		else if (opt == 'h')
			ping->options |= OPT_H;
		else if (opt == 'o')
			ping->options |= OPT_O;
		else if (opt == 'q')
			ping->options |= OPT_Q;
		else if (opt == 'c')
			ping->count = get_argument(ping, optarg, OPT_C, OPT_C_ERROR);
		else if (opt == 'i')
			ping->interval = get_argument(ping, optarg, OPT_I, OPT_I_ERROR);
		else if (opt == 'm')
			ping->socket.ttl = get_argument(ping, optarg, OPT_M, OPT_M_ERROR);
		else if (opt == 's')
			ping->payload_size = get_argument(ping, optarg, OPT_S, OPT_M_ERROR);
	}
	return (0);
}

static void init_ping(t_ping *ping, char **av)
{
	ft_bzero(ping, sizeof(struct s_ping));
	ping->name = av[0];
	ping->payload_size = DEFAULT_PAYLOAD_SIZE;
	ping->payload = DEFAULT_PAYLOAD;
	ping->interval = DEFAULT_INTERVAL;
	ping->socket.ttl = DEFAULT_TTL;
	ping->socket.tos = ICMP_TOS;
}

int init_prgm(t_ping *ping, int ac, char **av)
{
	t_opt option;
	char *optarg;
	int	  error;

	setbuf(stdout, NULL);
	ft_bzero(&option, sizeof(t_opt));
	init_ping(ping, av);
	signal(SIGINT, &sig_interupt);
	option.optstr = OPTSTR;
	optarg = NULL;
	if ((error = ft_getopt(ac, av, &option, &optarg)) != 0)
	{
		ping->exit = EX_USAGE;
		if (option.error)
			ft_dprintf(2, "%s: illegal option -- %s\n", av[0], option.error);
		else
			ft_dprintf(2, "%s: illegal option -- %c\n", av[0], error);
		return (-1);
	}
	ping->host = av[ac - 1];
	if (parse_opt(ping, &option, ac, av))
		return (-1);
	return (init_socket(ping));
}
