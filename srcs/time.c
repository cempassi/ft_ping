/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cedricmpassi <cempassi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 18:18:55 by cedricmpa         #+#    #+#             */
/*   Updated: 2020/10/30 18:21:55 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <sys/time.h>
#include <sysexits.h>

double duration(t_time *time)
{
	double sent;
	double recv;

	sent = time->sent.tv_sec * 1000000.0 + time->sent.tv_usec;
	recv = time->recv.tv_sec * 1000000.0 + time->recv.tv_usec;
	return ((recv - sent) / 1000.0);
}

int calculate_stats(t_ping *ping, t_packet *packet)
{
	t_stats *stats;
	double delay;
	t_list *node;

	if (ping->payload_size < TIME_DATA)
		return (0);
	stats = &ping->stats;
	delay = duration((t_time *)packet->payload);
	if (!stats->min || delay < stats->min)
		stats->min = delay;
	if (stats->max < delay)
		stats->max = delay;
	if ((node = ft_lstnew(&delay, sizeof(double))) == NULL)
	{
		ping->exit = EX_OSERR;
		ft_dprintf(2, "%s: delay allocation failed\n", ping->name);
		return (-1);
	}
	ft_lstaddback(&ping->delays, node);
	return (0);
}

int get_time(t_ping *ping, struct timeval *current)
{
	size_t packet_size;

	packet_size = ping->payload_size + ICMP_HEADER_LEN;
	if (packet_size < TIME_DATA + ICMP_HEADER_LEN)
		return (0);
	if (gettimeofday(current, NULL))
	{
		ping->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: time retrieval failed\n", ping->name);
		return (-1);
	}
	return (0);
}

int delay(t_ping *ping)
{
	struct timeval current;
	struct timeval to_wait;

	if (gettimeofday(&current, NULL))
	{
		ping->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: time retrieval failed\n", ping->name);
		return (-1);
	}
	to_wait = current;
	to_wait.tv_sec += ping->interval;
	while (current.tv_sec < to_wait.tv_sec)
	{
		if (gettimeofday(&current, NULL))
		{
			ping->exit = EX_OSERR;
			ft_dprintf(STDERR_FILENO, "%s: time retrieval failed\n", ping->name);
			return (-1);
		}
	}
	return (0);
}

