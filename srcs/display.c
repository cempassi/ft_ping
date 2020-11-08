/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 13:19:10 by cempassi          #+#    #+#             */
/*   Updated: 2020/11/08 10:23:22 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <sysexits.h>

void 			display_recv(t_ping *ping, t_iphdr *iph, t_packet *packet)
{
	char 	src[INET_ADDRSTRLEN];
	t_time 	*time;
	size_t 	packet_size;
	char 	*host;

	inet_ntop(AF_INET, &iph->saddr, src, INET_ADDRSTRLEN);
	time = (t_time *)packet->payload;
	packet_size = get_packet_size(ping);
	if((host = reverse_dns(src)) == NULL)
	{
		ping->exit = EX_OSERR;
		return;
	}
	ft_printf("%lu bytes from %s (%s): icmp_seq=%u ttl=%d ",
			packet_size, host, src, packet->header.echo.seq, iph->ttl);
	ft_strdel(&host);
	if (ping->payload_size >= TIME_DATA)
		printf("time=%.3lf ms", duration(time));
	if (ping->options & OPT_V && ping->payload_size > MAX_PAYLOAD_SIZE)
		ft_dprintf(2, "\n%s %d", OPT_S_E_STR, ping->payload_size);
	ft_putchar('\n');
}

void 			display_help(char *name)
{
	ft_dprintf(STDERR_FILENO, "usage: %s [-hoqv] [-c count] ", name);
	ft_dprintf(STDERR_FILENO, "[-i wait] [-m ttl] [-s packetsize] host\n");
	ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_C);
	ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_I);
	ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_M);
	ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_S);
	ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_H);
	ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_O);
	ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_Q);
	ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_V);
}

void 			display_start(t_ping *ping, struct addrinfo *host)
{
	char				host_ip[INET_ADDRSTRLEN];
	int					data;
	struct sockaddr_in *address;

	address = (struct sockaddr_in *)host->ai_addr;
	data = ping->payload_size;
	inet_ntop(AF_INET, &address->sin_addr, host_ip, INET_ADDRSTRLEN);
	ft_printf("PING %s (%s): %d(%d) bytes of data\n", ping->host
			, host_ip, data, ping->payload_size + IP_HEADER_LEN);
}

static void 	display_rtt(t_ping *ping)
{
	t_stats *stats;
	double variance;

	stats = &ping->stats;
	variance = 0;
	ft_lstfold(ping->delays, &stats->sum, get_sum);
	stats->avg = stats->sum / (stats->recv + stats->duplicate);
	stats->sum = 0;
	ft_lstfold(ping->delays, stats, get_variance);
	variance = stats->sum / (stats->recv + stats->duplicate);
	printf("round-trip min/avg/max/stddev = %.3lf/%.3lf/%.3lf/%.3lf ms\n",\
			stats->min, stats->avg, stats->max, square_root(variance));
}

void 			display_stats(t_ping *ping)
{
	t_stats *stats;
	size_t loss;

	stats = &ping->stats;
	loss = ft_lstlen(ping->sent);
	ft_printf("--- %s ping statistics ---\n", ping->host);
	ft_printf("%d packets transmitted, ", stats->sent);
	ft_printf("%d packets received, ", stats->recv);
	if (ping->stats.duplicate)
		ft_printf("+%d duplicates, ", stats->duplicate);
	printf("%.1lf%% packet loss\n", (double)100 * loss / stats->sent);
	if (ping->payload_size >= TIME_DATA && ping->stats.recv > 0)
		display_rtt(ping);
}
