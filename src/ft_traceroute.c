/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 02:16:43 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/23 01:13:32 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

static void	setup_struct(t_troute *troute)
{
	fetch_ip_addr(troute);
	init_sockets(troute);
}

int	main(int ac, char **av)
{
	t_troute	troute;

	check_uid();
	ft_memset(&troute, 0, sizeof(troute));

	parse_args(&troute.args, ac, av);
	setup_struct(&troute);

	troute_routine(&troute);

	free_struct(&troute);
	return (0);
}
