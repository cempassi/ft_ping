/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cedricmpassi <cempassi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 18:47:56 by cedricmpa         #+#    #+#             */
/*   Updated: 2020/10/28 17:49:07 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>

int packet_cmp(void *data, void *to_find)
{
	t_packet *packet;
	t_packet *chr;

	packet = data;
	chr = to_find;
	return (packet->header.echo.seq == chr->header.echo.seq ? 1 : 0);
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
	double difference;

	stats = acc;
	value = data;
	difference = *value - stats->avg;
	stats->sum += difference * difference;
}
