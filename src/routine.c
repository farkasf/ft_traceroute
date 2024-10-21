/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 00:21:54 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/22 00:38:25 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

static void	print_troute_header(t_troute *troute)
{
	dprintf(STDOUT_FILENO, "traceroute to %s (%s), %d hops max, %d byte packets\n", \
		troute->args.target, troute->network.host_ip, troute->args.max_ttl, UPD_DATALEN);
}

void	troute_routine(t_troute *troute)
{
	unsigned int	ttl;

	ttl = 1;
	print_troute_header(troute);
	while (ttl <= troute->args.max_ttl)
	{
		set_packet_lifetime(troute->network.socket_fd, ttl, troute);
		dprintf(STDOUT_FILENO, "%c%u:  ", ttl <= 9 ? ' ' : '\0', ttl);
		
		unsigned int current_ttl;
		socklen_t len = sizeof(current_ttl);
		getsockopt(troute->network.socket_fd, IPPROTO_IP, IP_TTL, &current_ttl, &len);
		dprintf(STDOUT_FILENO, "%u\n", current_ttl);
		//send probes
		ttl++;
	}
}
