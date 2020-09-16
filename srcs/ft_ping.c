/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/22 05:03:49 by cempassi          #+#    #+#             */
/*   Updated: 2020/09/16 07:13:42 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include "memory.h"
#include "str.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

char *generate_payload(t_ping *ping)
{
	char * payload;
	size_t size;
	size_t to_copy;
	size_t payload_len;

	size = ping->packet_size - sizeof(t_icmp_v4_hdr);
	if ((payload = ft_memalloc(size)) == NULL)
	{
		ft_dprintf(2, "%s: payload generation failed\n", ping->name);
		return NULL;
	}
	payload_len = ft_strlen(ping->payload);
	to_copy = size / payload_len;
	while (to_copy)
	{
		ft_strlcat(payload, ping->payload, size);
		--to_copy;
	}
	if (size % payload_len)
	{
		ft_strlcat(payload, ping->payload, size);
	}
	return (payload);
}

static int init_packet(t_ping *ping, t_packet *packet)
{
	ft_bzero(packet, sizeof(t_packet));
	packet->header.type = ICMP_ECHO;
	packet->header.code = ICMP_ECHO_CODE;
	packet->header.echo.id = getpid();
	if ((packet->payload = generate_payload(ping)) == NULL)
	{
		return (-1);
	}
	return 0;
}

int main(int ac, char **av)
{
	t_ping	 ping;
	t_packet packet;

	if (ac == 1)
	{
		ft_dprintf(2, "usage: %s [-%s] host\n", av[0], OPTSTR);
		return (-1);
	}
	if (init_prgm(&ping, ac, av))
	{
		return (-1);
	}
	init_packet(&ping, &packet);
	return 0;
}
