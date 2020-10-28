/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 10:24:11 by cempassi          #+#    #+#             */
/*   Updated: 2020/10/28 00:59:57 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <sys/time.h>

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
		if (error == EAI_NONAME)
			ft_dprintf(STDERR_FILENO, "%s: cannot resolve %s: host unknown\n",
					   ping->name, ping->host);
		else
			ft_dprintf(STDERR_FILENO, "%s: failed to reach host\n", ping->name);
		return (NULL);
	}
	return (host);
}

static int ping_loop(t_ping *ping, t_addrinfo *host, t_packet *packet)
{
	uint16_t seq;
	t_time	 *time;

	seq = 0;
	time = (t_time *)packet->payload;
	while (validate_ping(ping, seq) == 0)
	{
		ft_bzero(time, sizeof(t_time));
		packet->header.checksum = 0;
		packet->header.echo.seq = seq;
		gettimeofday(&time->sent, NULL);
		packet->header.checksum = checksum(packet, ping->packet_size);
		if (send_packet(ping, host, packet))
		{
			return (-1);
		}
		if (recv_packet(ping) || waiter(ping))
		{
			return (-1);
		}
		++seq;
	}
	return (0);
}

static void clean_ping(t_ping *ping)
{
	ft_lstdel(&ping->sent, NULL);
	ft_lstdel(&ping->recv, NULL);
	ft_lstdel(&ping->delays, NULL);
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
	return (0);
}

int main(int ac, char **av)
{
	t_ping ping;

	if (ac == 1)
	{
		ft_dprintf(2, "usage: %s [-%s] host\n", av[0], OPTSTR);
		return (-1);
	}
	if (init_prgm(&ping, ac, av))
	{
		return (-1);
	}
	run_ping(&ping);
	clean_ping(&ping);
	return (0);
}
