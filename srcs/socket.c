/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cedricmpassi <cempassi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/27 22:20:09 by cedricmpa         #+#    #+#             */
/*   Updated: 2020/10/28 17:46:55 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <sys/socket.h>
#include <sys/time.h>
#include <sysexits.h>

static int set_ip_opt(t_ping *ping)
{
	t_socket *sock;

	sock = &ping->socket;
	if (setsockopt(sock->fd, IPPROTO_IP, IP_TTL, &(sock->ttl),
				   sizeof(uint8_t *)) < 0)
	{
		ping->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: ttl configuration failed\n", ping->name);
		return (-1);
	}
	if (setsockopt(sock->fd, IPPROTO_IP, IP_TOS, &(sock->tos),
				   sizeof(uint8_t *)) < 0)
	{
		ping->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: Tos configuration failed\n", ping->name);
		return (-1);
	}
	return (0);
}

static int set_socket_opt(t_ping *ping)
{
	t_socket 	   	*sock;
	struct timeval 	time;
	int 			broadcast;

	sock = &ping->socket;
	time.tv_sec = 10;
	time.tv_usec = 0;
	broadcast = 1;
	if (setsockopt(sock->fd, SOL_SOCKET, SO_RCVTIMEO, &time, sizeof(time)))
	{
		ping->exit = EX_OSERR;
		ft_dprintf(2, "%s: SO_RCVTIMEO configuration failed\n", ping->name);
		return (-1);
	}
	if (setsockopt(sock->fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(int)))
	{
		ping->exit = EX_OSERR;
		ft_dprintf(2, "%s: SO_DEBUG configuration failed\n", ping->name);
		return (-1);
	}
	return (0);
}

int init_socket(t_ping *ping)
{
	t_socket *sock;

	sock = &ping->socket;
	if ((sock->fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		ping->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: Socket allocation failed\n", ping->name);
		return (-1);
	}
	return (set_socket_opt(ping) < 0 || set_ip_opt(ping) < 0 ? -1 : 0);
}
