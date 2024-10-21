/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:56:36 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/22 00:37:03 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

void	set_packet_lifetime(int socket_fd, unsigned int ttl, t_troute *troute)
{
	if (setsockopt(socket_fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1)
	{
		dprintf(STDERR_FILENO, "ft_traceroute: failed to set TTL: %s\n", strerror(errno));
		free_struct(troute);
		exit(EXIT_FAILURE);
	}
}

static int	resolve_host(char *hostname, struct addrinfo **res)
{
	struct addrinfo	hints;

	*res = NULL;
	ft_memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	if (getaddrinfo(hostname, NULL, &hints, res) == -1 || *res == NULL)
		return (-1);
	return (0);
}

void	fetch_ip_addr(t_troute *troute)
{
	struct addrinfo	*res;

	if (resolve_host(troute->args.target, &res) == -1)
	{
		dprintf(STDERR_FILENO, "%s: Name or service not known\n", troute->args.target);
		print_args_error("Cannot handle \"host\" cmdline arg `%s' on position 1 (argc %d)\n", \
			&troute->args, troute->args.target, troute->args.target_pos);
	}
	if (res->ai_addrlen != sizeof(troute->network.remote_addr))
	{
		dprintf(STDERR_FILENO, "ft_traceroute: address length mismatch: %s\n", strerror(errno));
		free_struct(troute);
		exit(EXIT_FAILURE);
	}
	ft_memcpy(&troute->network.remote_addr, res->ai_addr, res->ai_addrlen);

	if (inet_ntop(AF_INET, &troute->network.remote_addr.sin_addr, troute->network.host_ip, INET_ADDRSTRLEN) == NULL)
	{
		dprintf(STDERR_FILENO, "ft_traceroute: failed to convert IP address to string format: %s\n", strerror(errno));
		freeaddrinfo(res);
		free_struct(troute);
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(res);
}

void	setup_upd_socket(t_troute *troute)
{
	troute->network.socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (troute->network.socket_fd == -1)
	{
		dprintf(STDERR_FILENO, "ft_traceroute: failed to create UDP socket: %s\n", strerror(errno));
		free_struct(troute);
		exit(EXIT_FAILURE);
	}
}
