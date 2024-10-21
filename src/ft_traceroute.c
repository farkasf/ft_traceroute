/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 02:16:43 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/21 23:50:26 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

static void	traceroute_setup(t_troute *troute)
{
	fetch_ip_addr(troute);
	setup_upd_socket(troute);
}

int	main(int ac, char **av)
{
	t_troute	troute;

	check_uid();
	if (ac < 2)
		print_usage();

	ft_memset(&troute, 0, sizeof(troute));
	parse_args(&troute.args, ac, av);
	traceroute_setup(&troute);

	dprintf(STDERR_FILENO, "traceroute to %s (%s), %d hops max, %d byte packets\n", \
		troute.args.target, troute.network.host_ip, troute.args.max_ttl, UPD_DATALEN);

	free_struct(&troute);
	return (0);
}


/*
	printf("\nTarget: %s (resolved as IP %s)\n", troute.args.target, troute.network.host_ip);
	printf("Number of queries (-q): %u\n", troute.args.n_queries);
	printf("Max TTL (-m): %u\n", troute.args.max_ttl);
	printf("Port (-p): %u\n", troute.args.probe_port);
	printf("First TTL (-f): %u\n", troute.args.first_ttl);
	printf("Resolve DNS (-n): %s\n", troute.args.resolve ? "enabled" : "disabled");
	printf("Socket: %d\n\n", troute.network.socket_fd);
*/
