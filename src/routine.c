/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 00:21:54 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/23 01:29:20 by ffarkas          ###   ########.fr       */
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
	unsigned int	i;

	print_troute_header(troute);
	while (troute->args.first_ttl <= troute->args.max_ttl)
	{
		i = 0;
		set_packet_lifetime(troute->network.udp_socket_fd, troute->args.first_ttl, troute);
		ft_memset(&troute->network.previous_addr, 0, sizeof(troute->network.previous_addr));

		dprintf(STDOUT_FILENO, "%2u  ", troute->args.first_ttl);
		
		while (i < troute->args.n_queries)
		{
			troute->network.target_addr.sin_port = htons(troute->args.probe_port + troute->args.first_ttl + i - 1);
			send_udp_probe(troute);
			handle_reply(troute);
			if (troute->network.target_addr.sin_addr.s_addr == troute->network.previous_addr.sin_addr.s_addr)
				troute->network.target_reached = 1;
			if (i != troute->args.n_queries - 1)
				dprintf(STDOUT_FILENO, " ");
			i++;
		}
		dprintf(STDOUT_FILENO, "\n");
		if (troute->network.target_reached)
		{
			free_struct(troute);
			exit(EXIT_SUCCESS);
		}
		troute->args.first_ttl++;
	}
}
