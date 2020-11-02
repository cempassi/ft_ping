/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cedricmpassi <cempassi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 16:33:11 by cedricmpa         #+#    #+#             */
/*   Updated: 2020/11/02 03:02:12 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <errno.h>
#include <arpa/inet.h>
#include <sysexits.h>

static char 	*dst_unreachable(int code)
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

static void 	time_exceeded(t_iphdr *iphdr, t_timexceed *packet, int recieved)
{
	char src[INET_ADDRSTRLEN];
	char pack_src[INET_ADDRSTRLEN];
	char pack_dst[INET_ADDRSTRLEN];

	inet_ntop(AF_INET, &iphdr->saddr, src, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &packet->iphdr.saddr, pack_src, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &packet->iphdr.daddr, pack_dst, INET_ADDRSTRLEN);
	ft_dprintf(2, "%d bytes from %s: Time to live exceeded\n", recieved, src);
	ft_dprintf(2, "Vr HL TOS  Len   ID Flg  off TTL Pro  cks %*s %*s %*s\n"
			, ft_strlen(pack_src) / 2, "Src"
			, ft_strlen(pack_src)/ 2, ""
			, ft_strlen(pack_dst) / 2, "Dst");
	ft_dprintf(2, "%2d %2hhd %3d %4x %4x %3d %04d %3d %3d %4x %*s %*s\n\n",
			packet->iphdr.version, packet->iphdr.ihl, packet->iphdr.tos
			, packet->iphdr.len, packet->iphdr.id, packet->iphdr.flags
			, packet->iphdr.frag_offset, packet->iphdr.proto
			, packet->iphdr.ttl, packet->iphdr.csum
			, ft_strlen(pack_src) / 2, pack_src
			, ft_strlen(pack_dst) / 2, pack_dst);
	return;
}

int 			validate_recv(t_ping *ping, char *buffer, int recieved)
{
	t_packet *packet;

	if (g_sign & PING_INTERUPT || recieved <= 0)
		return (-1);
	packet = (t_packet *)(buffer + 20);
	if (packet->header.type != ICMP_ECHOREPLY)
	{
		if (packet->header.type == ICMP_UNREACH)
			ft_dprintf(2, "%s\n", dst_unreachable(packet->header.code));
		else if (packet->header.type == ICMP_TIMXCEED)
		{
			time_exceeded((t_iphdr *)buffer, (t_timexceed *)packet, recieved);
			recv_packet(ping);
		}
		return (-1);
	}
	return (0);
}

int 			validate_send(t_ping *ping, int16_t sent)
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
			ft_dprintf(2, "%s: sendto: Socket is not connected\n", ping->name);
		return (-1);
	}
	return (sent);
}

int 			validate_checksum(t_packet *packet, uint32_t packet_size)
{
	uint32_t expected_sum;
	uint32_t sum;

	expected_sum = packet->header.checksum;
	packet->header.checksum = 0;
	sum = checksum(packet, packet_size);
	packet->header.checksum = expected_sum;
	return (sum == expected_sum ? 0 : -1);
}
