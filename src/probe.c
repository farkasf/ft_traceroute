/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   probe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:36:25 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/22 03:02:58 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

void	receive_reply(t_troute *troute)
{
	t_probe	probe;

	probe.addr_size = sizeof(probe.recv_addr);
	probe.timeout.tv_sec = 1;
	probe.timeout.tv_usec = 0;

	setsockopt(troute->network.icmp_socket_fd, SOL_SOCKET, SO_RCVTIMEO, &probe.timeout, sizeof(probe.timeout));
	probe.recv_bytes = recvfrom(troute->network.icmp_socket_fd, probe.recv_packet, sizeof(probe.recv_packet), 0, \
		(struct sockaddr *)&probe.recv_addr, &probe.addr_size);

	if (probe.recv_bytes == -1)
	{
		if (errno == EHOSTUNREACH || errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
		{
			dprintf(STDERR_FILENO, " * ");
			return ;
		}
		dprintf(STDERR_FILENO, "ft_traceroute: recvfrom failed: %s\n", strerror(errno));
		free_struct(troute);
		exit(EXIT_FAILURE);
	}
	dprintf(STDOUT_FILENO, "OK");
}

void	send_udp_probe(t_troute *troute)
{
	char		packet[UDP_HDRLEN + UDP_DATALEN];
	ssize_t		sent_bytes;

	ft_memset(&packet, 0, sizeof(packet));
	sent_bytes = sendto(troute->network.udp_socket_fd, packet, sizeof(packet), 0, \
		(struct sockaddr *)&troute->network.target_addr, sizeof(troute->network.target_addr));
	if (sent_bytes == -1)
	{
		dprintf(STDERR_FILENO, "ft_traceroute: sendto failed: %s\n", strerror(errno));
		free_struct(troute);
		exit(EXIT_FAILURE);
	}
}
