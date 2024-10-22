/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   probe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:36:25 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/22 02:04:43 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

void	send_udp_probe(t_troute *troute)
{
	char		packet[UPD_HDRLEN + UPD_DATALEN];
	ssize_t		sent_bytes;

	ft_memset(&packet, 0, sizeof(packet));
	sent_bytes = sendto(troute->network.socket_fd, packet, sizeof(packet), 0, \
		(struct sockaddr *)&troute->network.target_addr, sizeof(troute->network.target_addr));
	if (sent_bytes == -1)
	{
		dprintf(STDERR_FILENO, "ft_traceroute: sendto failed\n");
		free_struct(troute);
		exit(EXIT_FAILURE);
	}
	dprintf(STDOUT_FILENO, "sent %zu bytes", sent_bytes);
}
