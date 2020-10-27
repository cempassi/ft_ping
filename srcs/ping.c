/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 10:24:11 by cempassi          #+#    #+#             */
/*   Updated: 2020/10/27 17:17:59 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include "memory.h"
#include <sys/time.h>

static void generate_payload(t_ping *ping, t_packet *packet)
{
	size_t size;
	size_t to_copy;
	size_t payload_len;
	size_t time;

	time = sizeof(t_time);
	size = ping->packet_size - sizeof(t_icmp_v4_hdr) - time;
	payload_len = ft_strlen(ping->payload);
	to_copy = size / payload_len;
	ft_bzero(packet->payload, time);
	while (to_copy)
	{
		ft_strlcat(packet->payload + time, ping->payload, size);
		--to_copy;
	}
	if (size % payload_len)
	{
		ft_strlcat(packet->payload + time, ping->payload, size);
	}
	return;
}

static t_packet *generate_packet(t_ping *ping)
{
	t_packet *packet;

	if ((packet = ft_memalloc(ping->packet_size)) == NULL)
	{
		ft_dprintf(2, "%s: packet allocation failed\n", ping->name, OPTSTR);
		return (NULL);
	}
	packet->header.type = ICMP_ECHO;
	packet->header.code = ICMP_ECHO_CODE;
	packet->header.echo.id = getpid();
	generate_payload(ping, packet);
	return (packet);
}

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

int run_ping(t_ping *ping)
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
	ft_memdel((void **)&packet);
	freeaddrinfo(host);
	return (0);
}
