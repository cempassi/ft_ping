/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 10:24:11 by cempassi          #+#    #+#             */
/*   Updated: 2020/11/02 03:01:13 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <sys/time.h>
#include <sysexits.h>

unsigned int g_sign = 0;

static struct addrinfo *resolve_host(t_ping *ping)
{
	struct addrinfo *host;
	struct addrinfo	 hints;
	int				 error;

	host = NULL;
	ft_bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = AI_ADDRCONFIG;
	if ((error = getaddrinfo(ping->host, NULL, &hints, &host)))
	{
		ping->exit = EX_NOHOST;
		if (error == EAI_NONAME)
			ft_dprintf(STDERR_FILENO, "%s: cannot resolve %s: host unknown\n",
					   ping->name, ping->host);
		else
			ft_dprintf(STDERR_FILENO, "%s: failed to reach host\n", ping->name);
		return (NULL);
	}
	return (host);
}

static int validate_ping(t_ping *ping, uint16_t seq)
{
	if (g_sign & PING_INTERUPT)
		return (-1);
	if (ping->options & OPT_O && seq == 1)
		return (-1);
	if (ping->options & OPT_C && seq == ping->count)
		return (-1);
	if (ping->exit >= EX__BASE)
		return (-1);
	return (0);
}

static int ping_loop(t_ping *ping, t_addrinfo *host, t_packet *packet)
{
	t_time	 *time;
	size_t packet_size;

	packet_size = ping->payload_size + ICMP_HEADER_LEN;
	time = (t_time *)packet->payload;
	while (validate_ping(ping, ping->seq) == 0)
	{
		ft_bzero(time, sizeof(t_time));
		packet->header.checksum = 0;
		packet->header.echo.seq = ping->seq;
		if (get_time(ping, &time->sent))
			return (-1);
		packet->header.checksum = checksum(packet, packet_size);
		if (send_packet(ping, host, packet) || recv_packet(ping) || delay(ping))
		{
			if (ping->exit > EX__BASE)
				return (-1);
		}
		++ping->seq;
	}
	return (0);
}

static int run_ping(t_ping *ping)
{
	struct addrinfo *host;
	t_packet 		 *packet;

	if ((packet = generate_packet(ping)) == NULL)
	{
		return (-1);
	}
	if ((host = resolve_host(ping)) == NULL)
	{
		return (-1);
	}
	display_start(ping, host);
	ping_loop(ping, host, packet);
	display_stats(ping);
	ft_memdel((void **)&packet);
	freeaddrinfo(host);
	ft_lstdel(&ping->sent, NULL);
	ft_lstdel(&ping->recv, NULL);
	ft_lstdel(&ping->delays, NULL);
	return (0);
}

int 	main(int ac, char **av)
{
	t_ping ping;

	if (ac == 1)
	{
		display_help(av[0]);
		return (EX_USAGE);
	}
	if (init_prgm(&ping, ac, av))
	{
		if (ping.exit == EX_USAGE)
			display_help(av[0]);
		return (ping.exit);
	}
	if (ping.options & OPT_H)
	{
		display_help(av[0]);
		return(0);
	}
	run_ping(&ping);
	return (ping.exit);
}
