/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cedricmpassi <cempassi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 00:53:27 by cedricmpa         #+#    #+#             */
/*   Updated: 2020/10/28 19:26:57 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <sysexits.h>

static void generate_payload(t_ping *ping, t_packet *packet)
{
	size_t size;
	size_t to_copy;
	size_t payload_len;
	size_t time;

	time = 0;
	size = ping->payload_size;
	if (size >= TIME_DATA)
	{
		time = TIME_DATA;
		ft_bzero(packet->payload, time);
		size -= time;
	}
	payload_len = ft_strlen(ping->payload);
	to_copy = size / payload_len;
	while (to_copy)
	{
		ft_strlcat(packet->payload + time, ping->payload, size);
		--to_copy;
	}
	if (size % payload_len)
		ft_strlcat(packet->payload + time, ping->payload, size);
	return;
}

t_packet *generate_packet(t_ping *ping)
{
	t_packet *packet;
	size_t packet_size;

	packet_size = ping->payload_size + ICMP_HEADER_LEN;
	if ((packet = ft_memalloc(packet_size)) == NULL)
	{
		ping->exit = EX_USAGE;
		ft_dprintf(2, "%s: packet allocation failed\n", ping->name);
		return (NULL);
	}
	packet->header.type = ICMP_ECHO;
	packet->header.code = ICMP_ECHO_CODE;
	packet->header.echo.id = getpid();
	generate_payload(ping, packet);
	return (packet);
}
