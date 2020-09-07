/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/22 05:03:49 by cempassi          #+#    #+#             */
/*   Updated: 2020/09/07 02:10:44 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include "memory.h"
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

static int init_packet(t_ping *ping, t_packet *packet)
{
	ft_bzero(packet, sizeof(t_packet));
	packet->header.icmp_type = ICMP_ECHO;
	packet->message = malloc(ping->packet_size - sizeof(struct icmp));
	return 0;
}

int main(int ac, char **av)
{
	t_ping	 ping;
	t_packet packet;

	if (ac == 1 || init_prgm(&ping, ac, av))
	{
		ft_dprintf(2, "usage: %s [-%s] host\n", av[0], OPTSTR);
		return (-1);
	}
	init_packet(&ping, &packet);
	ft_printf("Socket fd: %d\n", ping.socket);
	return 0;
}
