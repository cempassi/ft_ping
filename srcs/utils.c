/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 07:26:33 by cempassi          #+#    #+#             */
/*   Updated: 2020/11/02 03:04:37 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sysexits.h>

uint16_t 	checksum(void *addr, int count)
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

double 		square_root(double number)
{
	double tmp;
	double result;

	result = number / 2;
	tmp = 0;
	while(result != tmp)
	{
		tmp = result;
		result = (number / tmp + tmp) / 2;
	}
	return (result);
}

void 		sig_handler(int signo)
{
	if (signo == SIGINT)
		g_sign |= PING_INTERUPT;
	else if (signo == SIGALRM)
		g_sign |= PING_ALARM;
}
