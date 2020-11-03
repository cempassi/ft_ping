/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/22 05:03:31 by cempassi          #+#    #+#             */
/*   Updated: 2020/11/03 01:26:07 by cedricmpa        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
#define FT_PING_H
#include "libft.h"
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>

#define OPTSTR "c:i:m:s:qvho"

#define PING_INTERUPT 	0x0001
#define PING_ALARM 		0x0002

#define OPT_H 0x0001
#define OPT_V 0x0002
#define OPT_C 0x0004
#define OPT_O 0x0008
#define OPT_I 0x0010
#define OPT_M 0x0020
#define OPT_S 0x0040
#define OPT_Q 0x0080

#define OPT_C_ERROR "-c [NUMBER]"
#define OPT_C_E_STR "invalid count of packets to transmit: "
#define OPT_I_ERROR "-i [NUMBER]"
#define OPT_I_E_STR "Invalid Timing interval: "
#define OPT_M_ERROR "-m [NUMBER]"
#define OPT_M_E_STR "Invalid TTL: "
#define OPT_S_ERROR "-s [NUMBER]"

#define ICMP_ECHO_CODE 0
#define ICMP_HEADER_LEN 8
#define IP_HEADER_LEN 20
#define TIME_DATA sizeof(t_time)

#define DEFAULT_PAYLOAD "42"
#define DEFAULT_PAYLOAD_SIZE 54
#define DEFAULT_TTL 64
#define DEFAULT_INTERVAL 1
#define ICMP_TOS 0
#define MTU 1500
#define HOST_LEN 256

#define MAX_DELAY 1000
#define MAX_TTL 255

#define DESC_C "-c count: Stop after sending (and recieving) count packets."
#define DESC_I "-i wait: Wait ait seconds between sending each packet."
#define DESC_M "-m ttl: Set the IP Time To Live for outgoing packets."
#define DESC_S "-s packetsize: Specify the number of daya bytes to be sent."
#define DESC_H "-h: Display help."
#define DESC_O "-o: Exit successfully after recieving one reply packet."
#define DESC_Q "-q: Quiet output. Nothing is displayed except the summury lines."
#define DESC_V "-v: Verbose output. ICMP Packets other than ECHO_RESPONSE that are recieved are listed."

extern unsigned int g_sign;

typedef struct addrinfo t_addrinfo;
typedef struct msghdr t_msghdr;

typedef struct s_socket
{
	int		fd;
	int 	ttl;
	int 	tos;
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

typedef struct s_timexceed
{
	uint8_t 		type;
	uint8_t 		code;
	uint16_t 		checksum;
	uint32_t 		unused;
	t_iphdr 		iphdr;
	t_icmp_v4_hdr 	icmp_hdr;

} t_timexceed;

typedef struct s_packet
{
	t_icmp_v4_hdr header;
	char 		  payload[];
} t_packet;

typedef struct 	s_stats
{
	uint16_t 	sent;
	uint16_t 	recv;
	uint16_t 	duplicate;
	double 		sum;
	double 		min;
	double 		max;
	double 		avg;
	double 		stddev;
} 				t_stats;

typedef struct s_ping
{
	t_socket		 socket;
	int16_t 		 exit;
	uint16_t 		 seq;
	uint32_t		 options;
	uint32_t 		 interval;
	size_t			 count;
	size_t			 payload_size;
	t_stats 		 stats;
	char *			 name;
	char *			 host;
	char *			 payload;
	t_list 			*sent;
	t_list 			*recv;
	t_list 			*delays;
} t_ping;

int 		init_prgm(t_ping *ping, int ac, char **av);
int 		init_socket(t_ping *ping);

int 		send_packet(t_ping *ping, t_addrinfo *host, t_packet *packet);
int 		recv_packet(t_ping *ping);

t_packet 	*generate_packet(t_ping *ping);

double  	duration(t_time *time);
int 		calculate_stats(t_ping *ping, t_packet *packet);
int 		get_time(t_ping *ping, struct timeval *current);
int 		delay(t_ping *ping);

uint16_t 	checksum(void *addr, int count);
double 		square_root(double number);
void 		sig_handler(int signo);
char 		*reverse_dns(char *str);

int 		validate_checksum(t_packet *packet, uint32_t packet_size);
int 		validate_recv(t_ping *ping, char *buffer, int recieved);
int 		validate_send(t_ping *ping, int16_t sent);

void 		display_start(t_ping *ping, struct addrinfo *host);
void 		display_help(char *name);
void 		display_recv(t_ping *ping, t_iphdr *iph, t_packet *packet);
void 		display_stats(t_ping *ping);
void 		display_lists(t_ping *ping);

int 		packet_cmp(void *data, void *to_find);
void 		get_variance(void *acc, void *data);
void 		get_sum(void *acc, void *data);

#endif
