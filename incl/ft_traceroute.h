/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 09:33:52 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/20 12:10:48 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

# define DEF_HOPS 30
# define MAX_HOPS 255
# define DEF_PROBES 3
# define MAX_PROBES 10

typedef struct s_args
{
	char			*target;
	unsigned int	n_queries;
	unsigned int	max_ttl;
	unsigned int	send_wait;
	unsigned int	first_ttl;
	bool			resolve;
}	t_args;

typedef struct s_troute
{
	t_args	args;
}	t_troute;

#endif
