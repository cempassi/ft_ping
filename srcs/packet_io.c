/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet_io.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/20 10:20:21 by cempassi          #+#    #+#             */
/*   Updated: 2020/09/20 13:37:53 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <sys/time.h>
#include <errno.h>
#include <arpa/inet.h>

int send_packet(t_ping *ping, t_addrinfo *host, t_packet *packet, t_time *diff)
{
	int16_t sent;

	sent = sendto(ping->socket.fd, packet, ping->packet_size, 0, host->ai_addr,
				  host->ai_addrlen);
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
	gettimeofday(&diff->sent, NULL);
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

int recv_packet(t_ping *ping, t_time *diff)
{
	struct msghdr message;
	struct iovec  vector[1];
	char		  buffer[MTU];
	int			  recieved;

	ft_bzero(&message, sizeof(struct msghdr));
	ft_bzero(&buffer, MTU);
	ft_bzero(&vector, sizeof(struct iovec));
	setup_message(&message, vector, buffer);
	recieved = recvmsg(ping->socket.fd, &message, 0);
	if (((t_packet *)buffer + 20)->header.type == ICMP_ECHOREPLY)
	{
		gettimeofday(&diff->recieved, NULL);
		display_recv(ping, (t_iphdr *)buffer,(t_packet *)buffer + 20, diff);
	}
	return (0);
}
