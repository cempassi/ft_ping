/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 07:26:33 by cempassi          #+#    #+#             */
/*   Updated: 2020/09/20 13:36:47 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <sys/select.h>
#include <sys/time.h>

uint16_t checksum(void *addr, int count)
{

	uint32_t  sum;
	uint16_t *ptr;

	sum = 0;
	ptr = addr;
	while (count > 1)
	{
		sum += *ptr++;
		count -= 2;
	}

	if (count > 0)
		sum += *(uint8_t *)ptr;

	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	return ~sum;
}

int valid_checksum(t_packet *packet, uint32_t packet_size)
{
	uint32_t expected_sum;
	uint32_t sum;

	expected_sum = packet->header.checksum;
	packet->header.checksum = 0;
	sum = checksum(packet, packet_size);
	packet->header.checksum = expected_sum;
	return (sum == expected_sum ? 0 : 1);
}

int waiter(t_ping *ping)
{
	struct timeval current;
	struct timeval to_wait;

	if (gettimeofday(&current, NULL))
	{
		ft_dprintf(STDERR_FILENO, "%s: time retrieval failed\n", ping->name);
		return (-1);
	}
	to_wait = current;
	to_wait.tv_sec += ping->interval;
	while (current.tv_sec < to_wait.tv_sec)
	{
		if (gettimeofday(&current, NULL))
		{
			ft_dprintf(STDERR_FILENO, "%s: time retrieval failed\n",
					   ping->name);
			return (-1);
		}
	}
	return (0);
}

int validate_ping(t_ping *ping, uint16_t seq)
{
	if (g_sign & PING_INTERUPT)
		return (-1);
	if (ping->options & OPT_O && seq == 1)
		return (-1);
	if (ping->options & OPT_C && seq == ping->count)
		return (-1);
	return (0);
}
