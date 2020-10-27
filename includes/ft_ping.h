/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/22 05:03:31 by cempassi          #+#    #+#             */
/*   Updated: 2020/10/27 17:37:07 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
#define FT_PING_H
#include "libft.h"
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <sys/_types/_timeval.h>
#include <sys/socket.h>
#include <sys/types.h>

#define OPTSTR "c:vho"

#define PING_INTERUPT 0x0001

#define OPT_H 0x0001
#define OPT_V 0x0002
#define OPT_C 0x0004
#define OPT_O 0x0008
#define OPT_SO_DEBUG 0x0010

#define OPT_C_ERROR "-c [NUMBER]"

#define ICMP_V4_REPLY 0x00
#define ICMP_V4_DST_UNREACHABLE 0x03
#define ICMP_V4_SRC_QUENCH 0x04
#define ICMP_V4_REDIRECT 0x05
#define ICMP_V4_ECHO 0x08
#define ICMP_V4_ROUTER_ADV 0x09
#define ICMP_V4_ROUTER_SOL 0x0a
#define ICMP_V4_TIMEOUT 0x0b
#define ICMP_V4_MALFORMED 0x0c

#define ICMP_ECHO_CODE 0
#define ICMP_HEADER_LEN 8
#define IP_HEADER_LEN 20

#define DEFAULT_PAYLOAD "42"
#define DEFAULT_PACKET_SIZE 64
#define DEFAULT_TTL 64
#define DEFAULT_INTERVAL 1
#define ICMP_TOS 0
#define MTU 1500


extern unsigned int g_sign;

typedef struct addrinfo t_addrinfo;
typedef struct msghdr t_msghdr;

typedef struct s_socket
{
	int		fd;
	uint8_t ttl;
	uint8_t tos;
} t_socket;

typedef struct s_time
{
	struct timeval sent;
	struct timeval recv;
} t_time;

typedef struct s_iphdr
{
    uint8_t version : 4;
    uint8_t ihl : 4;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t flags : 3;
    uint16_t frag_offset : 13;
    uint8_t ttl;
    uint8_t proto;
    uint16_t csum;
    uint32_t saddr;
    uint32_t daddr;
} t_iphdr;

typedef struct s_icmp_v4_echo
{
	uint16_t id;
	uint16_t seq;
} t_icmp_v4_echo;

typedef struct s_icmp_v4_hdr
{
	uint8_t	 type;
	uint8_t	 code;
	uint16_t checksum;
	t_icmp_v4_echo echo;
} t_icmp_v4_hdr;

typedef struct s_packet
{
	t_icmp_v4_hdr header;
	char 		  payload[];
} t_packet;

typedef struct s_ping
{
	t_socket		 socket;
	uint32_t		 options;
	uint32_t 		 interval;
	size_t			 count;
	size_t			 packet_size;
	char *			 name;
	char *			 host;
	char *			 payload;
	t_list 			*sent;
	t_list 			*recieved;
} t_ping;

int		 init_prgm(t_ping *ping, int ac, char **av);
uint16_t checksum(void *addr, int count);

int send_packet(t_ping *ping, t_addrinfo *host, t_packet *packet);
int recv_packet(t_ping *ping);
int run_ping(t_ping *ping);

void sig_interupt(int signo);
int waiter(t_ping *ping);
int validate_ping(t_ping *ping, uint16_t seq);

void display_start(t_ping *ping, struct addrinfo *host);
void display_recv(t_ping *ping, t_iphdr *iph, t_packet *packet);
#endif
