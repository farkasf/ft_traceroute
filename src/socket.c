/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 21:56:36 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/21 22:29:52 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

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
		dprintf(STDERR_FILENO, "ft_traceroute: address length mismatch\n");
		free_struct(troute);
		exit(EXIT_FAILURE);
	}
	ft_memcpy(&troute->network.remote_addr, res->ai_addr, res->ai_addrlen);

	if (inet_ntop(AF_INET, &troute->network.remote_addr.sin_addr, troute->network.host_ip, INET_ADDRSTRLEN) == NULL)
	{
		dprintf(STDERR_FILENO, "ft_traceroute: failed to convert IP address to string format\n");
		freeaddrinfo(res);
		free_struct(troute);
		exit(EXIT_FAILURE);
	}

	freeaddrinfo(res);
}
