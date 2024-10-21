/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 03:04:35 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/21 22:30:31 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

void	free_struct(t_troute *troute)
{
	if (troute->args.target)
	{
		free(troute->args.target);
		troute->args.target = NULL;
	}
}

void	print_args_error(const char *format, t_args *args, ...)
{
	va_list	args_list;

	va_start(args_list, args);
	vdprintf(STDERR_FILENO, format, args_list);
	va_end(args_list);
	if (args->target)
	{
		free(args->target);
		args->target = NULL;
	}
	exit(EXIT_FAILURE);
}

void	print_usage(void)
{
	dprintf(STDOUT_FILENO,
		"\nUsage:\n"
		"  ./ft_traceroute [ -n ] [ -f first_ttl ] [ -m max_ttl ] [ -p port ] [ -q nqueries ] host\n"
		"\nOptions:\n"
		"  -f first_ttl                Start from the first_ttl hop (instead from 1).\n"
		"  -m max_ttl                  Set the max number of hops (max TTL to be reached).\n"
		"                              Default is 30.\n"
		"  -n                          Do not resolve IP addresses to their domain names.\n"
		"  -p port                     Set the destination port to use. Default is 33434.\n"
		"  -q nqueries                 Set the number of probes per each hop. Default is 3.\n"
		"  --help                      Read this help and exit.\n"
		"\nArguments:\n"
		"+     host          The host to traceroute to.\n\n");
	exit(EXIT_SUCCESS);
}
