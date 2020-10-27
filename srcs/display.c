/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 13:19:10 by cempassi          #+#    #+#             */
/*   Updated: 2020/10/27 17:40:46 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <arpa/inet.h>
#include <stdio.h>

double duration(t_time *time)
{
	double sent;
	double recv;

	sent = time->sent.tv_sec * 1000000.0 + time->sent.tv_usec;
	recv = time->recv.tv_sec * 1000000.0 + time->recv.tv_usec;
	return ((recv - sent) / 1000);
}

void display_recv(t_ping *ping, t_iphdr *iph, t_packet *packet)
{
	char src[INET_ADDRSTRLEN];
	t_time *time;

	inet_ntop(AF_INET, &iph->saddr, src, INET_ADDRSTRLEN);
	time = (t_time *)packet->payload;
	printf("%lu bytes from %s: icmp_seq=%u ttl=%d time=%.3lf ms\n",
			  ping->packet_size, src, packet->header.echo.seq, iph->ttl,
		  	  duration(time));
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
