/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cedricmpassi <cempassi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 18:47:56 by cedricmpa         #+#    #+#             */
/*   Updated: 2020/10/27 19:14:46 by cedricmpa        ###   ########.fr       */
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

void display_packet(void *data)
{
	t_packet *packet;

	packet = data;
	ft_printf("id: %d, seq: %d, type: %d, code: %d, checksum: %d\n",
			packet->header.echo.id,
			packet->header.echo.seq,
			packet->header.type,
			packet->header.code,
			packet->header.checksum);
}
