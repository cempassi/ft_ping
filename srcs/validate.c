/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cedricmpassi <cempassi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 16:33:11 by cedricmpa         #+#    #+#             */
/*   Updated: 2020/10/28 19:43:20 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <errno.h>
#include <sysexits.h>

static char  *dst_unreachable(int code)
{
	char *msg[16];

	msg[0] = "Destination network unreachable";
	msg[1] = "Destination host unreachable";
	msg[2] = "Destination protocol unreachable";
	msg[3] = "Destination port unreachable";
	msg[4] = "Fragmentation required, and DF flag set";
	msg[5] = "Source route failed";
	msg[6] = "Destination network unknown";
	msg[7] = "Destination host unknown";
	msg[8] = "Source host isolated";
	msg[9] = "Network administratively prohibited";
	msg[10] = "Host administratively prohibited";
	msg[11] = "Network unreachable for ToS";
	msg[12] = "Host unreachable for ToS";
	msg[13] = "Communication administratively prohibited";
	msg[14] = "Host Precedence Violation";
	msg[15] = "Precedence cutoff in effect ";
	return (msg[code]);
}

static char *time_exceeded(int code)
{
	char *msg[2];

	msg[0] = "TTL expired in transit";
	msg[1] = "Fragment reassembly time exceeded ";
	return(msg[code]);
}

int validate_recv(t_ping *ping, t_packet *packet, int recieved)
{
	if (recieved < 0)
	{
		if (errno == EAGAIN)
			ft_dprintf(STDERR_FILENO, "Timeout\n");
		return (-1);
	}
	if (packet->header.type != ICMP_ECHOREPLY)
	{
		ft_dprintf(2, "%s: ", ping->name);
		if (packet->header.type == ICMP_UNREACH)
			ft_dprintf(2, "%s\n", dst_unreachable(packet->header.code));
		else if (packet->header.type == ICMP_TIMXCEED)
			ft_dprintf(2, "%s\n", time_exceeded(packet->header.code));
		return (-1);
	}
	if (validate_checksum(packet, ping->payload_size + ICMP_HEADER_LEN))
	{
		ft_dprintf(STDERR_FILENO, "Packet validation failed\n");
		return (-1);
	}
	return (0);
}

int validate_send(t_ping *ping, int16_t sent)
{
	if (sent < 0)
	{
		ping->exit = EX_IOERR;
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

int validate_checksum(t_packet *packet, uint32_t packet_size)
{
	uint32_t expected_sum;
	uint32_t sum;

	expected_sum = packet->header.checksum;
	packet->header.checksum = 0;
	sum = checksum(packet, packet_size);
	packet->header.checksum = expected_sum;
	return (sum == expected_sum ? 0 : -1);
}
