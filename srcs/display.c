/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 13:19:10 by cempassi          #+#    #+#             */
/*   Updated: 2020/10/28 01:43:14 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>

void display_recv(t_ping *ping, t_iphdr *iph, t_packet *packet)
{
	char src[INET_ADDRSTRLEN];
	t_time *time;

	inet_ntop(AF_INET, &iph->saddr, src, INET_ADDRSTRLEN);
	time = (t_time *)packet->payload;
	printf("%lu bytes from %s: icmp_seq=%u ttl=%d time=%4.3lf ms\n",
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

void get_sum(void *acc, void *data)
{
	double *sum;
	double *value;

	sum = acc;
	value = data;
	*sum += *value;
}

void get_variance(void *acc, void *data)
{
	t_stats *stats;
	double *value;

	stats = acc;
	value = data;
	stats->sum += pow(*value - stats->avg, 2.0);
}

void display_stats(t_ping *ping)
{
	t_stats *stats;
	size_t loss;
	double variance;

	variance = 0;
	stats = &ping->stats;
	loss = ft_lstlen(ping->sent);
	ft_printf("--- %s ping statistics ---\n", ping->host);
	ft_printf("%d packets transmitted, ", stats->sent);
	ft_printf("%d packets received, ", stats->recv);
	if (ping->stats.duplicate)
	{
		ft_printf("+%d duplicates, ", stats->duplicate);
	}
	printf("%.1lf%% packet loss\n", (double)100 * loss / stats->sent);
	ft_lstfold(ping->delays, &stats->sum, get_sum);
	stats->avg = stats->sum / (stats->recv + stats->duplicate);
	stats->sum = 0;
	ft_lstfold(ping->delays, stats, get_variance);
	variance = stats->sum / (stats->recv + stats->duplicate);
	printf("round-trip min/avg/max/stddev = %.3lf/%.3lf/%.3lf/%.3lf ms\n",\
			stats->min, stats->avg, stats->max, sqrt(variance));
}
