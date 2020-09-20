/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/22 05:03:49 by cempassi          #+#    #+#             */
/*   Updated: 2020/09/20 11:02:08 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_iovec_t.h>
#include <sys/socket.h>
#include <unistd.h>

unsigned int g_sign = 0;

int main(int ac, char **av)
{
	t_ping ping;

	if (ac == 1)
	{
		ft_dprintf(2, "usage: %s [-%s] host\n", av[0], OPTSTR);
		return (-1);
	}
	if (init_prgm(&ping, ac, av))
	{
		return (-1);
	}
	run_ping(&ping);
	return (0);
}
