/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 02:05:55 by cempassi          #+#    #+#             */
/*   Updated: 2020/11/03 02:55:04 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <signal.h>
#include <sys/time.h>
#include <sysexits.h>
#include <stdio.h>

static int 	check_arg(t_ping *ping, uint8_t opt, int64_t number)
{
	int e;

	e = 0;
	if (opt == OPT_C && (number < 0 || number > INT32_MAX))
		e = ft_dprintf(2, "%s: %s `%d`\n", ping->name, OPT_C_E_STR, number);
	else if (opt == OPT_M && (number < 0 || number > MAX_TTL))
		e = ft_dprintf(2, "%s: %s `%d`\n", ping->name, OPT_M_E_STR, number);
	else if (opt == OPT_I && (number < 0 || number > MAX_DELAY))
		e = ft_dprintf(2, "%s: %s `%d`\n", ping->name, OPT_I_E_STR, number);
	if (e)
		ping->exit = EX_USAGE;
	return (ping->exit == EX_USAGE ? -1 : number);
}

static int 	get_argument(t_ping *ping, char *optarg, uint8_t opt, char o)
{
	uint32_t convert;

	if (!ft_strcheck(optarg, ft_isdigit))
	{
		ping->exit = EX_USAGE;
		ft_dprintf(2, "%s: Argument is not a number -- %c\n", ping->name, o);
		return (-1);
	}
	convert = ft_atoi(optarg);
	ping->options |= opt;
	return (check_arg(ping, opt, convert));
}

static int 	parse_opt(t_ping *ping, t_opt *option, int ac, char **av)
{
	char  o;
	char *optarg;

	while ((o = ft_getopt(ac, av, option, &optarg)) > 0 && ping->exit == 0)
	{
		if (o == 'v')
			ping->options |= OPT_V;
		else if (o == 'h')
			ping->options |= OPT_H;
		else if (o == 'o')
			ping->options |= OPT_O;
		else if (o == 'q')
			ping->options |= OPT_Q;
		else if (o == 'c')
			ping->count = get_argument(ping, optarg, OPT_C, o);
		else if (o == 'i')
			ping->interval = get_argument(ping, optarg, OPT_I, o);
		else if (o == 'm')
			ping->socket.ttl = get_argument(ping, optarg, OPT_M, o);
		else if (o == 's')
			ping->payload_size = get_argument(ping, optarg, OPT_S, o);
	}
	return (ping->exit);
}

static void init_ping(t_ping *ping, t_opt *option, char **av)
{
	ft_bzero(ping, sizeof(struct s_ping));
	ft_bzero(option, sizeof(t_opt));
	ping->name = av[0];
	ping->payload_size = DEFAULT_PAYLOAD_SIZE;
	ping->payload = DEFAULT_PAYLOAD;
	ping->interval = DEFAULT_INTERVAL;
	ping->socket.ttl = DEFAULT_TTL;
	ping->socket.tos = ICMP_TOS;
	option->optstr = OPTSTR;
	option->first_arg = 1;
	signal(SIGINT, sig_handler);
	signal(SIGALRM, sig_handler);
}

int 		init_prgm(t_ping *ping, int ac, char **av)
{
	t_opt option;
	int	  error;

	setbuf(stdout, NULL);
	init_ping(ping, &option, av);
	if ((error = ft_getopt(ac, av, &option, NULL)))
	{
		ping->exit = EX_USAGE;
		if (ft_strchr(OPTSTR, error))
			ft_dprintf(2, "%s: Requires an argument -- -%c\n", av[0], error);
		else
			ft_dprintf(2, "%s: illegal option -- -%c\n", av[0], error);
		return (-1);
	}
	if (option.first_arg != (size_t)ac - 1)
	{
		ping->exit = EX_USAGE;
		return (-1);
	}
	ping->host = av[ac - 1];
	if (parse_opt(ping, &option, ac, av))
		return (-1);
	return (init_socket(ping));
}
