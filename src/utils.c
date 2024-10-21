/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffarkas <ffarkas@student.42prague.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 03:04:35 by ffarkas           #+#    #+#             */
/*   Updated: 2024/10/21 03:13:28 by ffarkas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/ft_traceroute.h"

void	print_args_error(const char *format, t_args *args, ...)
{
	va_list	args_list;

	va_start(args_list, args);
	vdprintf(STDERR_FILENO, format, args_list);
	va_end(args_list);
	if (args->target)
		free(args->target);
	exit(EXIT_FAILURE);
}

void	print_usage(void)
{
	dprintf(STDOUT_FILENO, "this will be usage text\n");
	exit(EXIT_SUCCESS);
}
