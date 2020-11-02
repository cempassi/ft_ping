/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_io.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 10:20:21 by cempassi          #+#    #+#             */
/*   Updated: 2020/11/02 04:20:35 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/time.h>
#include <sysexits.h>
#include <unistd.h>

static int 	setup_message(t_msghdr *message, struct iovec *vector, char *buffer)
{
	ft_bzero(message, sizeof(struct msghdr));
	ft_bzero(buffer, MTU);
	ft_bzero(vector, sizeof(struct iovec));
	message->msg_iov = vector;
	message->msg_iovlen = 1;
	message->msg_name = NULL;
	message->msg_namelen = 0;
	message->msg_flags = 0;
	vector[0].iov_base = buffer;
	vector[0].iov_len = MTU;
	return (0);
}


static int 	process_packet(t_ping *ping, t_packet *packet)
{
	ping->exit = 0;
	if(!ft_lstmove_if(&ping->sent, &ping->recv, packet, packet_cmp))
	{
		++ping->stats.duplicate;
		calculate_stats(ping, packet);
		return (recv_packet(ping));
	}
	else
	{
		++ping->stats.recv;
		calculate_stats(ping, packet);
		return (0);
	}
}

static int 	await_packet(t_ping *ping, struct msghdr *message)
{
	int recieved;

	alarm(1);
	recieved = -1;
	while ((g_sign & (PING_ALARM + PING_INTERUPT)) == 0 && recieved <= 0)
		recieved = recvmsg(ping->socket.fd, message, MSG_DONTWAIT);
	if (g_sign & PING_ALARM)
	{
		ft_dprintf(2, "Request timeout for icmp_seq %d\n", ping->seq);
		g_sign &= ~(PING_ALARM);
		return (-1);
	}
	alarm(0);
	return (recieved);
}

int 		recv_packet(t_ping *ping)
{
	struct msghdr message;
	struct iovec  vector[1];
	char		  buffer[MTU];
	int			  recieved;
	t_packet 	  *packet;

	setup_message(&message, vector, buffer);
	packet = (t_packet *)(buffer + 20);
	recieved = await_packet(ping, &message);
	if (validate_recv(ping, buffer, recieved) == 0)
	{
		if (get_time(ping, &((t_time *)packet->payload)->recv))
			return (-1);
		if ((ping->options & OPT_Q) == 0)
			display_recv(ping, (t_iphdr *)buffer, packet);
		return(process_packet(ping, packet));
	}
	return (recieved < 0 ? -1 : 0);
}

int 		send_packet(t_ping *ping, t_addrinfo *host, t_packet *packet)
{
	int16_t sent;
	t_list *node;
	size_t packet_size;

	packet_size = ping->payload_size + ICMP_HEADER_LEN;
	sent = sendto(ping->socket.fd, packet, packet_size, 0, host->ai_addr,
			host->ai_addrlen);
	ping->stats.sent += 1;
	if (validate_send(ping, sent) > 0)
	{
		if ((node = ft_lstnew(packet, sizeof(t_packet))) == NULL)
		{
			ping->exit = EX_OSERR;
			ft_dprintf(2, "%s: packet allocation failed\n", ping->name);
			return (-1);
		}
		ft_lstadd(&ping->sent, node);
		return (0);
	}
	return (0);
}
