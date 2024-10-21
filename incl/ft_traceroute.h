/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 09:33:52 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/21 22:27:56 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <string.h>
# include <stdbool.h>

# include <arpa/inet.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <netinet/ip_icmp.h>

# define DEF_PORT 33434
# define DEF_HOPS 30
# define MAX_HOPS 255
# define DEF_PROBES 3
# define MAX_PROBES 10

typedef struct s_network
{
	struct sockaddr_in	remote_addr;
	char				host_ip[INET_ADDRSTRLEN];
}	t_network;

typedef struct s_args
{
	char			*target;
	int				target_pos;
	unsigned int	n_queries;
	unsigned int	max_ttl;
	unsigned int	probe_port;
	unsigned int	first_ttl;
	bool			resolve;
}	t_args;

typedef struct s_troute
{
	t_args		args;
	t_network	network;
}	t_troute;

void	parse_args(t_args *args, int ac, char **av);

void	free_struct(t_troute *troute);
void	print_usage(void);
void	print_args_error(const char *format, t_args *args, ...);

void	fetch_ip_addr(t_troute *troute);

#endif
