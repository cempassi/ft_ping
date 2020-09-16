/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 07:26:33 by cempassi          #+#    #+#             */
/*   Updated: 2020/09/16 07:26:35 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

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
