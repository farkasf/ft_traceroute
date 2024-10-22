/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 00:21:54 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/22 02:57:58 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

static void	print_troute_header(t_troute *troute)
{
	dprintf(STDOUT_FILENO, "traceroute to %s (%s), %d hops max, %d byte packets\n", \
		troute->args.target, troute->network.host_ip, troute->args.max_ttl, UDP_DATALEN);
}

void	troute_routine(t_troute *troute)
{
	unsigned int	ttl;
	unsigned int	i;

	ttl = 1;
	print_troute_header(troute);
	while (ttl <= troute->args.max_ttl)
	{
		i = 0;
		set_packet_lifetime(troute->network.udp_socket_fd, ttl, troute);
		ft_memset(&troute->network.previous_addr, 0, sizeof(troute->network.previous_addr));

		dprintf(STDOUT_FILENO, "%2u:  ", ttl);
		
		while (i < troute->args.n_queries)
		{
			troute->network.target_addr.sin_port = htons(troute->args.probe_port + ttl + i);
			send_udp_probe(troute);
			receive_reply(troute);
			dprintf(STDOUT_FILENO, " %c", i == troute->args.n_queries - 1 ? '\n' : ' ');
			i++;
		}
		ttl++;
	}
}