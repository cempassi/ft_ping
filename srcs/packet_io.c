/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_io.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 10:20:21 by cempassi          #+#    #+#             */
/*   Updated: 2020/10/28 01:47:47 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>

static int validate_send(t_ping *ping, int16_t sent)
{
	if (sent < 0)
	{
		if (sent == EPERM)
			ft_dprintf(STDERR_FILENO, "%s: sendto: Operation not permitted\n",
					ping->name);
		else if (sent == EMSGSIZE)
			ft_dprintf(STDERR_FILENO, "%s: sendto: Message too long\n",
					ping->name);
		else if (sent == EACCES)
			ft_dprintf(STDERR_FILENO, "%s: sendto: Permission denied\n",
					ping->name);
		else
			ft_dprintf(2, "%s: sendto: Error code %d\n", ping->name, sent);
		return (-1);
	}
	return (sent);
}

int send_packet(t_ping *ping, t_addrinfo *host, t_packet *packet)
{
	int16_t sent;
	t_list *node;

	sent = sendto(ping->socket.fd, packet, ping->packet_size, 0, host->ai_addr,
			host->ai_addrlen);
	if (validate_send(ping, sent) > 0)
	{
		if ((node = ft_lstnew(packet, sizeof(t_packet))) == NULL)
		{
			ft_dprintf(2, "%s: packet allocation failed\n", ping->name);
			return (-1);
		}
		ft_lstadd(&ping->sent, node);
		ping->stats.sent += 1;
		return (0);
	}
	return (-1);
}

static int setup_message(t_msghdr *message, struct iovec *vector, char *buffer)
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

static int validate_recv(t_ping *ping, t_packet *packet, int recieved)
{
	if (recieved < 0)
	{
		if (errno == EAGAIN)
			ft_dprintf(STDERR_FILENO, "Timeout\n");
		return (-1);
	}
	if (packet->header.type != ICMP_ECHOREPLY)
	{
		ft_dprintf(STDERR_FILENO, "Invalid ICMP Header type\n");
		return (-1);
	}
	if (validate_checksum(packet, ping->packet_size))
	{
		ft_dprintf(STDERR_FILENO, "Packet validation failed\n");
		return (-1);
	}
	return (0);
}

int recv_packet(t_ping *ping)
{
	struct msghdr message;
	struct iovec  vector[1];
	char		  buffer[MTU];
	int			  recieved;
	t_packet 	  *packet;

	setup_message(&message, vector, buffer);
	packet = (t_packet *)(buffer + 20);
	recieved = recvmsg(ping->socket.fd, &message, 0);
	if (validate_recv(ping, packet, recieved) == 0)
	{
		gettimeofday(&((t_time *)packet->payload)->recv, NULL);
		display_recv(ping, (t_iphdr *)buffer, packet);
		if(!ft_lstmove_if(&ping->sent, &ping->recv, packet, packet_cmp))
		{
			++ping->stats.duplicate;
			calculate_stats(ping, packet);
			return (recv_packet(ping));
		}
	}
	++ping->stats.recv;
	calculate_stats(ping, packet);
	return (0);
}
