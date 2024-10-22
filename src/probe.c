/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   probe.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:36:25 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/22 06:11:57 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

void	calculate_rtt(t_timer *timer)
{
	double	seconds;
	double	useconds;

	seconds = timer->rtt_finish.tv_sec - timer->rtt_start.tv_sec;
	useconds = timer->rtt_finish.tv_usec - timer->rtt_start.tv_usec;
	timer->rtt = seconds * 1000 + useconds / 1000;
}

void	print_trace_result(t_probe *probe, t_troute *troute)
{
	struct hostent	*hop;
	char			*domain;

	hop = gethostbyaddr(&(probe->recv_addr.sin_addr), sizeof(struct in_addr), AF_INET);
	if (hop)
		domain = hop->h_name;
	else
	{
		domain = inet_ntoa(probe->recv_addr.sin_addr);
		if (ft_strlen(domain) == 0)
		{
			dprintf(STDERR_FILENO, "ft_traceroute: failed to convert IP address to string format\n");
			free_struct(troute);
			exit(EXIT_FAILURE);
		}
	}
	calculate_rtt(&troute->timer);
	if (troute->network.previous_addr.sin_addr.s_addr == probe->recv_addr.sin_addr.s_addr)
		dprintf(STDOUT_FILENO, " %.3f ms", troute->timer.rtt);
	else
		dprintf(STDOUT_FILENO, "%s (%s)  %.3f ms", domain, inet_ntoa(probe->recv_addr.sin_addr), troute->timer.rtt);
}

static int	analyze_reply(t_probe *probe)
{
	struct ip		*ip_hdr;
	struct icmp		*icmp_hdr;
	
	ip_hdr = (struct ip *)probe->recv_packet;
	icmp_hdr = (struct icmp *)(probe->recv_packet + (ip_hdr->ip_hl << 2));
	if (icmp_hdr->icmp_type == 0 || icmp_hdr->icmp_type == 3 || icmp_hdr->icmp_type == 11 || icmp_hdr->icmp_type == 5)
		return (0);
	dprintf(STDERR_FILENO, " * (no response)");
	return (-1);
}

void	handle_reply(t_troute *troute)
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
			dprintf(STDERR_FILENO, "*");
			return ;
		}
		dprintf(STDERR_FILENO, "ft_traceroute: recvfrom failed: %s\n", strerror(errno));
		free_struct(troute);
		exit(EXIT_FAILURE);
	}
	gettimeofday(&(troute->timer.rtt_finish), NULL);
	if (analyze_reply(&probe) == -1)
		return ;
	print_trace_result(&probe, troute);
	troute->network.previous_addr = probe.recv_addr;
}

void	send_udp_probe(t_troute *troute)
{
	char		packet[UDP_HDRLEN + UDP_DATALEN];
	ssize_t		sent_bytes;

	ft_memset(&packet, 0, sizeof(packet));
	sent_bytes = sendto(troute->network.udp_socket_fd, packet, sizeof(packet), 0, \
		(struct sockaddr *)&troute->network.target_addr, sizeof(troute->network.target_addr));
	gettimeofday(&(troute->timer.rtt_start), NULL);
	if (sent_bytes == -1)
	{
		dprintf(STDERR_FILENO, "ft_traceroute: sendto failed: %s\n", strerror(errno));
		free_struct(troute);
		exit(EXIT_FAILURE);
	}
}
