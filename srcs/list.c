/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cedricmpassi <cempassi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 18:47:56 by cedricmpa         #+#    #+#             */
/*   Updated: 2020/10/28 02:00:15 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <stdio.h>
#include <math.h>

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

	stats = acc;
	value = data;
	stats->sum += pow(*value - stats->avg, 2.0);
}
