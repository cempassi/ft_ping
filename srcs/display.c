/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 13:19:10 by cempassi          #+#    #+#             */
/*   Updated: 2020/10/18 12:24:40 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <arpa/inet.h>

void display_recv(t_ping *ping, t_iphdr *iph, t_packet *packet, t_time *diff)
{

	char src[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &iph->saddr, src, INET_ADDRSTRLEN);
	ft_printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%f\n",
			  ping->packet_size, src, packet->header.echo.seq, iph->ttl,
			  diff->sent.tv_usec - diff->recieved.tv_usec);
}

void display_start(t_ping *ping, struct addrinfo *host)
{
	char				host_ip[INET_ADDRSTRLEN];
	int					data;
	struct sockaddr_in *address;

	address = (struct sockaddr_in *)host->ai_addr;
	data = ping->packet_size - ICMP_HEADER_LEN;
	inet_ntop(AF_INET, &address->sin_addr, host_ip, INET_ADDRSTRLEN);
	ft_printf("PING %s (%s): %d bytes of data\n", ping->host, host_ip, data);
}
