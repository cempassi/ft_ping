/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/16 07:26:33 by cempassi          #+#    #+#             */
/*   Updated: 2020/11/02 04:09:31 by cedricmpa        ###   ########.fr       */
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

char 		*reverse_dns(char *str)
{
	struct sockaddr_in 	address;
	char 				buffer[HOST_LEN];

	ft_bzero(buffer, HOST_LEN);
	ft_bzero(&address, sizeof(struct sockaddr_in));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, str, &address.sin_addr);
	if (getnameinfo((struct sockaddr *)&address,sizeof(struct sockaddr)
				, buffer, HOST_LEN, NULL, 0, 0))
	{
		return (NULL);
	}
	return (ft_strdup(buffer));
}
