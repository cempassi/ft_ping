/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_io.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 10:20:21 by cempassi          #+#    #+#             */
/*   Updated: 2020/10/27 17:41:43 by cedricmpa        ###   ########.fr       */
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
	return (0);
}

static int setup_message(t_msghdr *message, struct iovec *vector, char *buffer)
{
	message->msg_iov = vector;
	message->msg_iovlen = 1;
	message->msg_name = NULL;
	message->msg_namelen = 0;
	message->msg_flags = 0;
	vector[0].iov_base = buffer;
	vector[0].iov_len = MTU;
	return (0);
}

static int validate_recv(t_ping *ping, t_packet *packet)
{
	uint16_t sum;
	t_time	 *time;

	time = (t_time *)packet->payload;
	if (packet->header.type != ICMP_ECHOREPLY)
		return (-1);
	sum = packet->header.checksum;
	packet->header.checksum = 0;
	packet->header.checksum = checksum(packet, ping->packet_size);
	if (sum == packet->header.checksum)
	{
		gettimeofday(&time->recv, NULL);
		return (0);
	}
	ft_dprintf(STDERR_FILENO, "Packet validation failed\n");
	return (-1);
}

int send_packet(t_ping *ping, t_addrinfo *host, t_packet *packet)
{
	int16_t sent;
	t_time *time;

	time = (t_time *)packet->payload;
	printf("Packent send time=%.3lf\n", (double)time->sent.tv_usec);
	sent = sendto(ping->socket.fd, packet, ping->packet_size, 0, host->ai_addr,
			host->ai_addrlen);
	return (validate_send(ping, sent));
}

int recv_packet(t_ping *ping)
{
	struct msghdr message;
	struct iovec  vector[1];
	char		  buffer[MTU];
	int			  recieved;
	t_packet 	  *packet;

	ft_bzero(&message, sizeof(struct msghdr));
	ft_bzero(&buffer, MTU);
	ft_bzero(&vector, sizeof(struct iovec));
	setup_message(&message, vector, buffer);
	recieved = recvmsg(ping->socket.fd, &message, 0);
	if (recieved > 0)
	{
		packet = (t_packet *)(buffer + 20);
		if (validate_recv(ping, packet) == 0)
		{
			display_recv(ping, (t_iphdr *)buffer, packet);
		}
	}
	return (0);
}
