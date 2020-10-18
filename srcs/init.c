/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 02:05:55 by cempassi          #+#    #+#             */
/*   Updated: 2020/10/18 11:44:08 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include "stdio.h"
#include <sys/_types/_timeval.h>
#include <signal.h>

static int parse_opt(t_ping *ping, t_opt *option, int ac, char **av)
{
	char  opt;
	char *optarg;

	while ((opt = ft_getopt(ac, av, option, &optarg)) > 0)
	{
		if (opt == 'v')
			ping->options |= OPT_V;
		else if (opt == 'h')
			ping->options |= OPT_H;
		else if (opt == 'c')
		{
			if (!ft_strcheck(optarg, ft_isdigit))
			{
				ft_dprintf(2, "%s: illegal option arg -- %s\n", av[0],
						   OPT_C_ERROR);
				return (-1);
			}
			ping->options |= OPT_C;
			ping->count = ft_atoi(optarg);
		}
	}
	return (0);
}

int init_debug_timeout(t_ping *ping)
{
	t_socket *	   sock;
	struct timeval timeout;
	int			   debug;

	sock = &ping->socket;
	debug = 1;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	if (ping->options & OPT_SO_DEBUG)
	{
		if (setsockopt(sock->fd, IPPROTO_IP, SO_DEBUG, &debug, sizeof(int)))
		{
			ft_dprintf(2, "%s: SO_DEBUG configuration failed\n", ping->name);
			return (-1);
		}
	}
	if (setsockopt(sock->fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout,
				   sizeof(timeout)))
	{
		ft_dprintf(2, "%s: SO_DEBUG configuration failed\n", ping->name);
		return (-1);
	}
	return (0);
}

int init_socket(t_ping *ping)
{
	t_socket *sock;

	sock = &ping->socket;
	if ((sock->fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: Socket allocation failed\n", ping->name);
		return (-1);
	}
	if (setsockopt(sock->fd, IPPROTO_IP, IP_TTL, &(sock->ttl),
				   sizeof(uint8_t *))
		< 0)
	{
		ft_dprintf(STDERR_FILENO, "%s: ttl configuration failed\n", ping->name);
		return (-1);
	}
	if (setsockopt(sock->fd, IPPROTO_IP, IP_TOS, &(sock->tos),
				   sizeof(uint8_t *)))
	{
		ft_dprintf(STDERR_FILENO, "%s: Tos configuration failed\n", ping->name);
		return (-1);
	}
	return (init_debug_timeout(ping));
}

void init_ping(t_ping *ping, char **av)
{
	ping->name = av[0];
	ping->packet_size = DEFAULT_PACKET_SIZE;
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

	ft_bzero(ping, sizeof(struct s_ping));
	ft_bzero(&option, sizeof(t_opt));
	init_ping(ping, av);
	signal(SIGINT, &sig_interupt);
	option.optstr = OPTSTR;
	optarg = NULL;
	if ((error = ft_getopt(ac, av, &option, &optarg)) != 0)
	{
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